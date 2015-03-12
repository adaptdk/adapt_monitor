<?php

/**
 * @file
 *   Site module list.
 */

$plugin = array(
  'title' => t('Site module list'),
  'description' => t('Displays a table of the modules active on a site.'),
  'category' => t('Adapt widgets'),
  'render callback' => 'adapt_monitor_module_list_content_type_render',
);

/**
 * Render callback.
 */
function adapt_monitor_module_list_content_type_render($subtype, $conf, $panel_args, $context) {
  $entity_id = $panel_args[0];
  $site = entity_load('site', array($entity_id));
  if ($site) {
    $site = $site[$entity_id];

    $entity_type_name = 'site';
    $site_wrapper = entity_metadata_wrapper($entity_type_name, $site);
    $serialised_data = $site_wrapper->field_module_states->value();
    $data = unserialize($serialised_data);
    $table_header = array(
      'module' => t('Module'),
      'version' => t('Version'),
      'patches' => t('Patches'),
    );
    $rows = array();
    foreach ($data['contrib'] as $name => $info) {
      $row = array(
        $name,
        $info['version']
      );
      if(isset($info['patches'])) {
        $row[] = implode('<br/>', $info['patches']);
      } else {
        $row[] = '';
      }
      $rows[] = $row;
    }
    return theme('table', array('header' => $table_header, 'rows' => $rows));
  }
  return NULL;
}
