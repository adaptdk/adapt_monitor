<?php

/**
 * @file
 * Include file for adapt monitor module.
 */

/**
 * Authenticates a call using a shared secret to verify the request.
 *
 * @param array $settings
 *   The settings for the authentication module.
 * @param array $method
 *   The method that's being called.
 * @param array $args
 *   The arguments that are being used to call the method.
 *
 * @return void|string
 *   Returns nothing, or a error message if authentication fails.
 */
function adapt_monitor_authenticate_call($settings, $method, $args) {
  $headers = apache_request_headers();
  $shared_secret = variable_get('adapt_monitor_shared_secret', '');
  if (($shared_secret != $headers['Shared-Secret'])) {
    return t("Secret key doesn't match");
  }
  return NULL;
}
