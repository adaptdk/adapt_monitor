<?php
/**
 * @file
 * adapt_monitor.features.inc
 */

/**
 * Implements hook_eck_bundle_info().
 */
function adapt_monitor_eck_bundle_info() {
  $items = array(
    'site_drupal_site' => array(
      'machine_name' => 'site_drupal_site',
      'entity_type' => 'site',
      'name' => 'drupal_site',
      'label' => 'drupal site',
    ),
  );
  return $items;
}

/**
 * Implements hook_eck_entity_type_info().
 */
function adapt_monitor_eck_entity_type_info() {
  $items = array(
    'site' => array(
      'name' => 'site',
      'label' => 'site',
      'properties' => array(
        'title' => array(
          'label' => 'Title',
          'type' => 'text',
          'behavior' => 'title',
        ),
        'uid' => array(
          'label' => 'Author',
          'type' => 'integer',
          'behavior' => 'author',
        ),
        'created' => array(
          'label' => 'Created',
          'type' => 'integer',
          'behavior' => 'created',
        ),
        'changed' => array(
          'label' => 'Changed',
          'type' => 'integer',
          'behavior' => 'changed',
        ),
      ),
    ),
  );
  return $items;
}
