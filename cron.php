<?php

/**
 * Para ejecutar con Drush el día 1 de cada mes.
 */
error_reporting(E_ALL);
ini_set("display_errors", 1);

drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
echo '===================================' . PHP_EOL;
echo 'Sending email with csv' . PHP_EOL;
echo '===================================' . PHP_EOL;
echo 'Execution result:' . PHP_EOL;
$result = indra_form_sent_email();
echo PHP_EOL;
