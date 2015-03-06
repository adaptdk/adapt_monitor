<?php
/**
 * @file
 * adapt_monitor.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function adapt_monitor_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'site-drupal_site-field_build_number'
  $field_instances['site-drupal_site-field_build_number'] = array(
    'bundle' => 'drupal_site',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the build number of this site',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'site',
    'field_name' => 'field_build_number',
    'label' => 'Build number',
    'required' => 1,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'site-drupal_site-field_core_version'
  $field_instances['site-drupal_site-field_core_version'] = array(
    'bundle' => 'drupal_site',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the core version of this site.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'site',
    'field_name' => 'field_core_version',
    'label' => 'Core version',
    'required' => 1,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'site-drupal_site-field_module_states'
  $field_instances['site-drupal_site-field_module_states'] = array(
    'bundle' => 'drupal_site',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter a serialised list of modules with their versions and any patches which might be applied.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'site',
    'field_name' => 'field_module_states',
    'label' => 'Module states',
    'required' => 0,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'site-drupal_site-field_state'
  $field_instances['site-drupal_site-field_state'] = array(
    'bundle' => 'drupal_site',
    'default_value' => array(
      0 => array(
        'value' => 2,
      ),
    ),
    'deleted' => 0,
    'description' => 'Enter the state of this site.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'site',
    'field_name' => 'field_state',
    'label' => 'State',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Build number');
  t('Core version');
  t('Enter a serialised list of modules with their versions and any patches which might be applied.');
  t('Enter the build number of this site');
  t('Enter the core version of this site.');
  t('Enter the state of this site.');
  t('Module states');
  t('State');

  return $field_instances;
}
