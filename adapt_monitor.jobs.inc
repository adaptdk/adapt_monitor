<?php
/**
 * Created by PhpStorm.
 * User: jesperbisgaard
 * Date: 20/11/15
 * Time: 16:48
 */

function adapt_monitor_get_available($refresh = FALSE) {
  $needs_refresh = FALSE;

  // Grab whatever data we currently have cached in the DB.
  $available = _adapt_monitor_get_cached_available_releases();
  $num_avail = count($available);

  $projects = adapt_monitor_update_list();
  foreach ($projects as $site => $modules) {
    foreach ($modules as $key => $project) {
      // If there's no data at all, we clearly need to fetch some.
      if (empty($available[$site . ':' . $key])) {
        adapt_monitor_create_fetch_task($site, $project);
        $needs_refresh = TRUE;
        continue;
      }

      // See if the .info file is newer than the last time we checked for data,
      // and if so, mark this project's data as needing to be re-fetched. Any
      // time an admin upgrades their local installation, the .info file will
      // be changed, so this is the only way we can be sure we're not showing
      // bogus information right after they upgrade.
      if ($project['info']['_info_file_ctime'] > $available[$key]['last_fetch']) {
        $available[$site . ':' . $key]['fetch_status'] = UPDATE_FETCH_PENDING;
      }

      // If we have project data but no release data, we need to fetch. This
      // can be triggered when we fail to contact a release history server.
      if (empty($available[$site . ':' . $key]['releases'])) {
        $available[$site . ':' . $key]['fetch_status'] = UPDATE_FETCH_PENDING;
      }

      // If we think this project needs to fetch, actually create the task now
      // and remember that we think we're missing some data.
      if (!empty($available[$site . ':' . $key]['fetch_status']) && $available[$site . ':' . $key]['fetch_status'] == UPDATE_FETCH_PENDING) {
        adapt_monitor_create_fetch_task($site, $project);
        $needs_refresh = TRUE;
      }
    }
  }
  if ($needs_refresh && $refresh) {
    // Attempt to drain the queue of fetch tasks.
    _adapt_monitor_fetch_data();
    // After processing the queue, we've (hopefully) got better data, so pull
    // the latest from the cache again and use that directly.
    $available = _adapt_monitor_get_cached_available_releases();
  }

  return $available;
}

/**
 * Returns all currently cached data about available releases for all projects.
 *
 * @return
 *   Array of data about available releases, keyed by project shortname.
 */
function _adapt_monitor_get_cached_available_releases() {
  $data = array();
  $cache_items = _update_get_cache_multiple('adapt_monitor_available_releases');
  foreach ($cache_items as $cid => $cache) {
    $cache->data['last_fetch'] = $cache->created;
    if ($cache->expire < REQUEST_TIME) {
      $cache->data['fetch_status'] = UPDATE_FETCH_PENDING;
    }
    // The project shortname is embedded in the cache ID, even if there's no
    // data for this project in the DB at all, so use that for the indexes in
    // the array.
    $parts = explode('::', $cid, 2);
    $data[$parts[1]] = $cache->data;
  }
  return $data;
}

function adapt_monitor_create_fetch_task($site, $project) {
  $fetch_tasks = &drupal_static(__FUNCTION__, array());
  if (empty($fetch_tasks)) {
    $fetch_tasks = _update_get_cache_multiple('adapt_monitor_fetch_tasks');
  }
  $cid = 'fetch_task::' . $site . '::' . $project['name'];
  if (empty($fetch_tasks[$cid])) {
    $queue = DrupalQueue::get('adapt_monitor_fetch_tasks');
    $queue->createItem($project);
    // Due to race conditions, it is possible that another process already
    // inserted a row into the {cache_update} table and the following query will
    // throw an exception.
    // @todo: Remove the need for the manual check by relying on a queue that
    // enforces unique items.
    try {
      db_insert('cache_update')
        ->fields(array(
          'cid' => $cid,
          'created' => REQUEST_TIME,
        ))
        ->execute();
    }
    catch (Exception $e) {
      // The exception can be ignored safely.
    }
    $fetch_tasks[$cid] = REQUEST_TIME;
  }
}

/**
 * Update the list of modules to fetch updates for.
 */
function adapt_monitor_update_list() {
  $results = db_select('field_data_field_module_states')
    ->fields('field_data_field_module_states', array('entity_id', 'field_module_states_value'))
    ->execute();
  $modules = cache_get('adapt_monitor:update:list');
  foreach ($results as $result) {
    $data = unserialize($result->field_module_states_value);
    foreach ($data['contrib'] as $name => $info) {
      if (!isset($modules[$name])) {
        $modules[$result->entity_id][$name] = array(
          'project_type' => 'contrib',
          'name' => $name,
          'version' => $info['version'],
          'project' => $name,
          '_info_file_ctime' => isset($info['datetime']) ? $info['datetime'] : '',
          'site' => $result->entity_id,
        );
      }
    }
  }
  cache_set('adapt_monitor:update:list', $modules, REQUEST_TIME + 86400);
  return $modules;
}

/**
 * Attempts to drain the queue of tasks for release history data to fetch.
 */
function _adapt_monitor_fetch_data() {
  $queue = DrupalQueue::get('adapt_monitor_fetch_tasks');
  $end = time() + variable_get('update_max_fetch_time', UPDATE_MAX_FETCH_TIME);
  while (time() < $end && ($item = $queue->claimItem())) {
    adapt_monitor_fetch_status($item->data);
    $queue->deleteItem($item);
  }
}

/**
 * Check for module updates.
 */
function adapt_monitor_fetch_updates() {
  $frequency = variable_get('update_check_frequency', 1);
  $interval = 60 * 60 * 24 * $frequency;
  if ((REQUEST_TIME - variable_get('adapt_monitor_last_check', 0)) > $interval) {
    // If the configured update interval has elapsed, we want to invalidate
    // the cached data for all projects, attempt to re-fetch, and trigger any
    // configured notifications about the new status.
    _adapt_monitor_refresh();
    _adapt_monitor_fetch_data();
  }
  else {
    // Otherwise, see if any individual projects are now stale or still
    // missing data, and if so, try to fetch the data.
    adapt_monitor_get_available(TRUE);
  }

  // Clear garbage from disk.
  update_clear_update_disk_cache();
}

/**
 * Clears out all the cached available update data and initiates re-fetching.
 */
function _adapt_monitor_refresh() {
  module_load_include('inc', 'update', 'update.compare');

  // Since we're fetching new available update data, we want to clear
  // our cache of both the projects we care about, and the current update
  // status of the site. We do *not* want to clear the cache of available
  // releases just yet, since that data (even if it's stale) can be useful
  // during update_get_projects(); for example, to modules that implement
  // hook_system_info_alter() such as cvs_deploy.
  _update_cache_clear('update_project_projects');
  _update_cache_clear('update_project_data');

  $projects = adapt_monitor_update_list();

  // Now that we have the list of projects, we should also clear our cache of
  // available release data, since even if we fail to fetch new data, we need
  // to clear out the stale data at this point.
  _update_cache_clear('adapt_monitor_available_releases::', TRUE);

  foreach ($projects as $site => $modules) {
    foreach ($modules as $key => $project) {
      adapt_monitor_create_fetch_task($site, $project);
    }
  }
}

function adapt_monitor_fetch_status($data) {
  module_load_include('inc', 'update', 'update.fetch');
  global $base_url;
  $fail = &drupal_static(__FUNCTION__, array());
  // This can be in the middle of a long-running batch, so REQUEST_TIME won't
  // necessarily be valid.
  $now = time();
  if (empty($fail)) {
    // If we have valid data about release history XML servers that we have
    // failed to fetch from on previous attempts, load that from the cache.
    if (($cache = _update_cache_get('fetch_failures')) && ($cache->expire > $now)) {
      $fail = $cache->data;
    }
  }
  $max_fetch_attempts = variable_get('update_max_fetch_attempts', UPDATE_MAX_FETCH_ATTEMPTS);
  $data['includes'][$data['name']] = $data['name'];
  $success = FALSE;
  $available = array();
  $site_key = drupal_hmac_base64($base_url, drupal_get_private_key());
  $url = _update_build_fetch_url($data, $site_key);
  $fetch_url_base = _update_get_fetch_url_base($data);
  $project_name = $data['name'];
  $site = $data['site'];

  if (empty($fail[$fetch_url_base]) || $fail[$fetch_url_base] < $max_fetch_attempts) {
    $xml = drupal_http_request($url);
    if (!isset($xml->error) && isset($xml->data)) {
      $data = $xml->data;
    }
  }

  if (!empty($data)) {
    $available = update_parse_xml($data);
    // @todo: Purge release data we don't need (http://drupal.org/node/238950).
    if (!empty($available)) {
      // Only if we fetched and parsed something sane do we return success.
      $success = TRUE;
    }
  }
  else {
    $available['project_status'] = 'not-fetched';
    if (empty($fail[$fetch_url_base])) {
      $fail[$fetch_url_base] = 1;
    }
    else {
      $fail[$fetch_url_base]++;
    }
  }

  $frequency = variable_get('update_check_frequency', 1);
  $cid = 'adapt_monitor_available_releases::' . $project_name;
  _update_cache_set($cid, $available, $now + (60 * 60 * 24 * $frequency));

  // Stash the $fail data back in the DB for the next 5 minutes.
  _update_cache_set('fetch_failures', $fail, $now + (60 * 5));

  // Whether this worked or not, we did just (try to) check for updates.
  variable_set('adapt_monitor_last_check', $now);

  // Now that we processed the fetch task for this project, clear out the
  // record in {cache_update} for this task so we're willing to fetch again.
  _update_cache_clear('fetch_task::' . $site . '::' . $project_name);
  return $success;
}
