<?php
/**
 * Adapt monitor service file.
 *
 * @file adapt_monitor.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function adapt_monitor_default_services_endpoint() {
  $endpoints = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'adapt_monitor';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'sitereporting';
  $endpoint->authentication = array(
    'adapt_monitor' => 'adapt_monitor',
  );
  $endpoint->server_settings = array(
    'formatters' => array(
      'json' => TRUE,
      'bencode' => FALSE,
      'jsonp' => FALSE,
      'php' => FALSE,
      'xml' => FALSE,
    ),
    'parsers' => array(
      'application/json' => TRUE,
      'application/vnd.php.serialized' => FALSE,
      'application/x-www-form-urlencoded' => FALSE,
      'application/xml' => FALSE,
      'multipart/form-data' => FALSE,
      'text/xml' => FALSE,
    ),
  );
  $endpoint->resources = array(
    'adapt_monitor' => array(
      'actions' => array(
        'report' => array(
          'enabled' => '1',
        ),
      ),
    ),
  );
  $endpoint->debug = 0;

  $endpoints[] = $endpoint;

  return $endpoints;
}
