<?php

/**
 * @file
 * Handles the uploading of the TEI file.
 */

/**
 * Defines a file upload form for uploading the text file.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function tei_upload(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('xml');
  $form = array();
  $form['tei_name'] = array(
    '#type' => 'textfield',
    '#description' => t('Name of TEI being uploaded'),
  );
  $form['tei_description'] = array(
    '#type' => 'textfield',
    '#description' => t('Description of TEI being uploaded'),
  );
  $form['file'] = array(
    '#title' => t('Plain Text File'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select file to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b>', array('@size' => $upload_size, '@ext' => $extensions[0])),
    '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      // Assume it's specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => "Add TEI to Collection",
  );
  return $form;
}

/**
 * Adds the uploaded file to the database.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function tei_upload_submit(array $form, array &$form_state) {
  $tei_file = file_load($form_state['values']['file']);
  $tei_path = drupal_realpath($tei_file->uri);
  $success = db_insert('tei_xslt_tei')
      ->fields(array(
        'tei_name' => $form_state['values']['tei_name'],
        'tei_description' => $form_state['values']['tei_description'],
        'tei' => file_get_contents($tei_path),
      ))
      ->execute();
  if ($success) {
    drupal_set_message(t('Successfully inserted record'));
  }
}

/**
 * @file
 * Handles the uploading of the xslt file.
 */

/**
 * Defines a file upload form for uploading the text file.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function xslt_upload(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('xsl xslt');
  $css_extensions = array('css');
  $js_extensions = array('js');

  $form = array();
  $form['xslt_name'] = array(
    '#type' => 'textfield',
    '#description' => t('Name of xslt being uploaded'),
  );
  $form['xslt_description'] = array(
    '#type' => 'textfield',
    '#description' => t('Description of xslt being uploaded'),
  );
  $form['file'] = array(
    '#title' => t('TEI File'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select file to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b>', array('@size' => $upload_size, '@ext' => $extensions[0])),
    '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      // Assume it's specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );
  $form['css_file'] = array(
    '#title' => t('CSS File'),
    '#type' => 'managed_file',
    '#required' => FALSE,
    '#description' => t('Select file to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b>', array('@size' => $upload_size, '@ext' => $css_extensions[0])),
    '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $css_extensions,
      // Assume it's specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );
  $form['js_file'] = array(
    '#title' => t('Javascript File'),
    '#type' => 'managed_file',
    '#required' => FALSE,
    '#description' => t('Select file to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b>', array('@size' => $upload_size, '@ext' => $js_extensions[0])),
    '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $js_extensions,
      // Assume it's specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => "Add xslt to Collection",
  );
  return $form;
}

/**
 * Adds the uploaded file to the database.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function xslt_upload_submit(array $form, array &$form_state) {
  $xslt_file = file_load($form_state['values']['file']);
  $xslt_path = drupal_realpath($xslt_file->uri);
  $css_contents = NULL;
  if (isset($form_state['values']['css_file'])) {
    $css_file = file_load($form_state['values']['css_file']);
    $css_path = drupal_realpath($css_file->uri);
    $css_contents = file_get_contents($css_path);
  }
  $js_contents = NULL;
  if (isset($form_state['values']['js_file'])) {
    $js_file = file_load($form_state['values']['js_file']);
    $js_path = drupal_realpath($js_file->uri);
    $js_contents = file_get_contents($js_path);
  }

  $success = db_insert('tei_xslt_xslt')
      ->fields(array(
        'xslt_name' => $form_state['values']['xslt_name'],
        'xslt_description' => $form_state['values']['xslt_description'],
        'xslt' => file_get_contents($xslt_path),
        'css' => $css_contents,
        'js' => $js_contents,
      ))
      ->execute();
  if ($success) {
    drupal_set_message(t('Successfully inserted record'));
  }
}
