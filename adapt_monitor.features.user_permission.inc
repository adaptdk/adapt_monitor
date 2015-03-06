<?php
/**
 * @file
 * adapt_monitor.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function adapt_monitor_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'adapt site report'.
  $permissions['adapt site report'] = array(
    'name' => 'adapt site report',
    'roles' => array(
      'Developer' => 'Developer',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'adapt_monitor',
  );

  return $permissions;
}
