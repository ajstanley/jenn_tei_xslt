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
function tei_view(array $form, array &$form_state) {

  $result = db_query('SELECT tei_id, tei_name FROM {tei_xslt_tei}');
  $tei = array();
  foreach ($result as $record) {
    $tei[$record->tei_id] = $record->tei_name;
  }
  $result = db_query('SELECT xslt_id, xslt_name FROM {tei_xslt_xslt}');
  $xslt = array();
  foreach ($result as $record) {
    $xslt[$record->xslt_id] = $record->xslt_name;
  }
  $form['tei'] = array(
    '#title' => t('Select TEI'),
    '#type' => 'select',
    '#options' => $tei,
  );
  $form['xslt'] = array(
    '#title' => t('Select XSLT'),
    '#type' => 'select',
    '#options' => $xslt,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Show TEI'),
  );
  return $form;
}

/**
 * Submit handler for tei view form.
 *
 * @param array $form
 *   Drupal form definition
 * @param array $form_state
 *   Drupla form state
 */
function tei_view_submit(array $form, array &$form_state) {
  $tei_id = $form_state['values']['tei'];
  $xslt_id = $form_state['values']['xslt'];
  drupal_goto("tei_xslt/show/tei/$tei_id/$xslt_id");
}

/**
 *
 * @param int $tei_id
 *   Unique identifier for stored TEI
 * @param int $xslt_id
 *   Unique identifier for stored XSLT
 *
 * @return html
 */
function display_transformed_tei($tei_id, $xslt_id) {
  $tei_results = db_query('SELECT tei, tei_name FROM {tei_xslt_tei} WHERE tei_id = :tei_id', array(':tei_id' => $tei_id))->fetch();
  $tei = $tei_results->tei;
  $name = $tei_results->tei_name;
  $xslt_results = db_query('SELECT xslt, css FROM {tei_xslt_xslt} WHERE xslt_id = :xslt_id', array(':xslt_id' => $xslt_id))->fetch();
  $raw_xslt = $xslt_results->xslt;
  if (isset($xslt_results->css)) {
    drupal_add_css($xslt_results->css,  array('type' => 'inline'));
  }
  if (isset($xslt_results->js)) {
    drupal_add_js($xslt_results->js,  array('type' => 'inline'));
  }
  $xml = new DOMDocument;
  $xml->loadXML($tei);
  $xsl = new DOMDocument;
  $xsl->loadXML($raw_xslt);
  $proc = new XSLTProcessor;
  $proc->importStyleSheet($xsl);
  $output = $proc->transformToXML($xml);
  $output = "<h1>$name</h1>" . $output;
  return $output;
}