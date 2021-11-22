<?php

define('INDRA_FORM_SENT_PAGER', 100);

/**
 * Retorno para el menú "admin/content/formsent"
 * @return string  html
 */
function indra_form_sent_manage() {
  $table = indra_form_sent_manage_table(INDRA_FORM_SENT_PAGER);
  $filter = drupal_get_form('indra_form_sent_filter_form');
  $filter = drupal_render($filter);
  return $filter . '<div id="form-sent-ui">' . theme_table($table) . theme('pager') . '</div>';
}

/**
 * Retorno para el ajax vinculado al menú "admin/content/formsent/table"
 */
function indra_form_sent_manage_ajax() {
  $filter = indra_form_sent_filter();
  $table = indra_form_sent_manage_table(INDRA_FORM_SENT_PAGER, $filter);
  echo theme_table($table) . theme('pager');
  exit;
}

/**
 * Provee el array para construir la tabla de gestión.
 * @param type $pager
 * @param type $filter
 * @return array  renderizable para tabla.
 */
function indra_form_sent_manage_table($pager = 10, $filter = array()) {

  $data = IndraFormSent::readPager($pager, $filter);

      $currentDate = time();
      $fechaMenosTreina = $currentDate-2629743;
      $table = array();

      //if($pager['send_date']>$fechaMenosTreina && $pager['send_date']<$currentDate) {
        foreach ($data as $key => $value) {
          unset($data[$key]['body']);
        }

        $table = array(
          'header' => array('fid', 'identifier', 'subject', 'user', 'language', 'mailto', 'replyto', 'send_date', 'ip_address', 'operations', ''),
          'rows' => $data,
          'attributes' => array('id' => 'form-sent-table'),
          'caption' => '',
          'colgroups' => array(),
          'sticky' => '',
          'empty' => '',
        );
      //}
  return $table;
}

/**
 * Formulario para filtros.
 * @param array $form
 * @param array $form_state
 * @return $form
 */
function indra_form_sent_filter_form($form, &$form_state) {
  $form['identifier'] = array(
    '#type' => 'select',
    '#title' => 'Identifier',
    '#default_value' => 'all',
    '#options' => IndraFormSent::readAllIdentifier(),
    '#attributes' => [
      'class' => ['select-filter']
    ],
  );
  $form['subject'] = array(
    '#type' => 'select',
    '#title' => 'Subject',
    '#default_value' => 'all',
    '#options' => IndraFormSent::readAllSubject(),
    '#attributes' => [
      'class' => ['select-filter']
    ],
  );
/*
  $form['mailto'] = array(
    '#type' => 'select',
    '#title' => 'Mail to',
    '#default_value' => 'all',
    '#options' => IndraFormSent::readAllMailto(),
    '#attributes' => [
      'class' => ['select-filter']
    ],
  );
*/
  $form['mailto'] = array(
    '#type' => 'textfield',
    '#title' => 'Mail to',
  );
  $form['replyto'] = array(
    '#type' => 'textfield',
    '#title' => 'Reply to',
//    '#maxlength' => 30,
//    '#size' => 20,
  );
  $form['treinta'] = array(
    '#type' => 'date_popup',
    '#title' => '30 Dias',
    '#options' => IndraFormSent::readLastMonth(),
    //'#attributes' => array('class' => array('action_botton'), 'onclick' => 'indra_form_sent_form_callback(event)'),
  );
  $form['date_from'] = array(
    '#type' => 'date_popup',
    '#title' => t('From'),
    '#default_value' => null,
    '#date_format' => 'Y-m-d',
    '#size' => 10,
  );
  $form['date_to'] = array(
    '#type' => 'date_popup',
    '#title' => t('Until'),
    '#default_value' => null,
    '#date_format' => 'Y-m-d',
    '#size' => 10,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply filter'),
    '#attributes' => array('class' => array('action_botton'), 'onclick' => 'indra_form_sent_form_callback(event)'),
  );
  $form['csv'] = array(
    '#type' => 'submit',
    '#value' => t('Download csv'),
    '#submit' => array('indra_form_sent_csv'),
    '#attributes' => array('class' => array('action_botton')),
  );

  return $form;
}

/**
 * Cargar el formulario para barrar item desde la uri "admin/content/formsent/%/delete"
 * @param int $fid
 * @return array renderizable
 */
function indra_form_sent_item_delete($fid) {
  return drupal_get_form('indra_form_sent_delete_form', $fid);
}

/**
 * Formulario para confirmar el borrado de un item
 * @param array $form
 * @param array $form_state
 * @param int $fid
 * @return array $form
 */
function indra_form_sent_delete_form($form, &$form_state, $fid) {
  $form['information'] = array(
    '#markup' => '<p>' . t('Are you sure you want to delete the record @record ?', array('@record' => $fid)) . '</p>',
  );
  $form['fid'] = array(
    '#value' => $fid,
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('indra_form_sent_delete_submit'),
    '#attributes' => array('class' => array('action_botton')),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('indra_form_sent_redirect_main_page'),
    '#attributes' => array('class' => array('action_botton')),
  );
  $form['#redirect'] = 'admin/content/formsent';
  return $form;
}

/**
 * Retorno para el ajax vinculado al menú "admin/content/formsent/table"
 */
function indra_form_sent_delete_ajax() {
  $filter = array(
    'identifier' => isset($_POST['identifier']) ? $_POST['identifier'] : null,
    'subject' => isset($_POST['subject']) ? $_POST['subject'] : null,
    'mailto' => isset($_POST['mailto']) ? $_POST['mailto'] : null,
    'replyto' => isset($_POST['replyto']) ? $_POST['replyto'] : null,
    'send_date' => isset($_POST['send_date']) ? $_POST['send_date'] : null,
  );

  $table = indra_form_sent_manage_table(INDRA_FORM_SENT_PAGER, $filter);
  echo theme_table($table) . theme('pager');
  exit;
}

/**
 * Carga la vista de un item, partes principales del correo enviado.
 * @param int $fid
 * @return string html
 */
function indra_form_sent_item_view($fid) {
  $data = IndraFormSent::readItem($fid);
  return theme('indra_form_sent_mail', $data);
}

/**
 * Retrollamada para borrar item
 */
function indra_form_sent_delete_submit($form, &$form_state) {
  $n_record = IndraFormSent::deleteItem($form['fid']['#value']);
  if ($n_record) {
    drupal_set_message(t('The record @fid has been deleted.', array('@fid' => $form['fid']['#value'])));
  }
  else {
    drupal_set_message(t('The record @fid has been deleted.', array('@fid' => $form['fid']['#value'])), 'warning');
  }
  indra_form_sent_redirect_main_page();
}

/**
 * Redirige a la tabla principal
 */
function indra_form_sent_redirect_main_page() {
  drupal_goto('admin/content/formsent');
}

/**
 * Descarga archivo csv
 */
function indra_form_sent_csv() {
  header('Content-Type: text/csv; charset=utf-8');
  header('Content-Disposition: attachment; filename=form_sent.csv');
  header('Expires: 0');
  header('Cache-Control: no-cache');

  $filter = indra_form_sent_filter();
  $output = fopen('php://output', 'w');
  foreach (indra_form_sent_scv_data($filter) as $line) {
    fputcsv($output, $line, ';');
  }
  fclose($output);

  exit;
}

function indra_form_sent_ultimos_dias($form, $formstate){

  $filter = array(
    'identifier' => isset($_POST['identifier']) ? $_POST['identifier'] : null,
    'subject' => isset($_POST['subject']) ? $_POST['subject'] : null,
    'mailto' => isset($_POST['mailto']) ? $_POST['mailto'] : null,
    'replyto' => isset($_POST['replyto']) ? $_POST['replyto'] : null,
    'date_from' => null,
    'date_to' => null,
  );

  $fechaActual = time();
  $fechaMenosTreinta = $fechaActual-2629743;

  if(isset($_POST['treinta'])){
    $filter['send_date'] = IndraFormSent::readLastMonth();
  }

  /*if($_POST['send_date']>$fechaMenosTreinta && $_POST['send_date']<$fechaActual){

    $rows['all'] = t('All');
    $query = db_select('indra_form_sent', 'as')
    ->condition('send_date', [$fechaMenosTreinta, $fechaActual], 'BETWEEN');
    foreach ($query->execute()->fetchAll() as $v){
      $filter->$v;
    }

  }*/
  return $filter;

}

/**
 * Recoge los filtros desde GUI
 * @return array
 */
function indra_form_sent_filter() {
  $filter = array(
    'identifier' => isset($_POST['identifier']) ? $_POST['identifier'] : null,
    'subject' => isset($_POST['subject']) ? $_POST['subject'] : null,
    'mailto' => isset($_POST['mailto']) ? $_POST['mailto'] : null,
    'replyto' => isset($_POST['replyto']) ? $_POST['replyto'] : null,
    'date_from' => null,
    'date_to' => null,
    'send_date' => null,
  );

  if (isset($_POST['date_from'])) {
    $date_from = DateTime::createFromFormat('Y-m-d', $_POST['date_from']['date']);
    if ($date_from) {
      $filter['date_from'] = $date_from->modify('-1 day')->getTimestamp();
    }
    else {
      $filter['date_from'] = null;
    }
  }
  if (isset($_POST['date_to'])) {
    $date_to = DateTime::createFromFormat('Y-m-d', $_POST['date_to']['date']);
    $filter['date_to'] = $date_to ? $date_to->modify('+1 day')->getTimestamp() : null;
  }

  $currentDate = strtotime(date("d-m-Y H:i:00",time()));
  $fechaEntrada = strtotime("19-10-2021 21:00:00");

  $tiempoDeLaCajitaTreinta = $_POST['treinta']['date'];
  $tiempoDeLaCajitaTreintaEnSegundos = strtotime($_POST['treinta']['date']);
  $tiempoEnUnix = strtotime($_POST['treinta']['date']);
  $fechaMasUnMes = strtotime("-1 month");
  $dateHoy = time();
  $dateHaceUnMes = $dateHoy-2629743;

  echo "<br>Aquí imprimo el array: " . $_POST['treinta'];
  echo "<br>Aquí imprimo el strtotime variable: " . strtotime($tiempoEnUnix);
  echo "<br>Aquí imprimo las fechas: dateHoy: " . $dateHoy . ". fechaMasUnMes: " . $fechaMasUnMes . "<br>";
  echo "<br>Comparar entre hace un mes y hoy: " . strtotime($fechaMasUnMes,$currentDate);

  if (isset($_POST['treinta']) != null){

    echo "adios mundo cruel";

    $tiempoEnUnix = strtotime($_POST['treinta']['date']);
    $fechaMasUnMes = strtotime("-1 month");
    foreach($_POST['treinta'] as $c=>$v){
      echo "<br>Aquí recorro el array:" . $c . " -> " . $v;
    }

    $tiempoDeLaCajitaTreinta = $_POST['treinta']['date'];
    $tiempoDeLaCajitaTreintaEnSegundos = strtotime($_POST['treinta']['date']);

    echo "<br>Aquí imprimo el array: " . $_POST['treinta'];
    echo "<br>Aquí imprimo el strtotime variable: " . strtotime($tiempoEnUnix);
    echo "<br>Aquí imprimo las fechas: dateHoy: " . $dateHoy . ". fechaMasUnMes: " . $fechaMasUnMes . "<br>";
    echo "<br>Comparar entre hace un mes y hoy: " . strtotime($fechaMasUnMes,$currentDate);

    echo "<br>En la variable de treinta tenemos: " . $tiempoDeLaCajitaTreinta . "<br>";
    echo "<br>En la variable de treinta segundos tenemos: " . $tiempoDeLaCajitaTreintaEnSegundos . "<br>";
    echo "<br>En la variable dateHaceUnMes tengo: " . $dateHaceUnMes . "<br>";

    //Suponiendo que todo el tiempo está en unix lo que hay que comparar es:
    //Variable 1: La variable del $_POST['treinta'] = datoIntroducido:
    //Variable 2: La variable de HACE UN MES = haceUnMes
    //Variable 3: La variable de AHORA MISMO = ahoraMismo
    //datoIntroducido>haceUnMes && datoIntroducido<ahoraMismo
    echo date('d/m/Y', 1258447881);
    //$tiempoDeLaCajitaTreinta>
    if($tiempoDeLaCajitaTreintaEnSegundos>$dateHaceUnMes && $tiempoDeLaCajitaTreintaEnSegundos<$currentDate){
        echo "<br>La fecha es de hace menos de un mes<br>";
    }else{
      echo "<br>La fecha es de hace más de un mes<br>";
    }

    /*if($_POST['treinta']>$tiempoMenosTreinta && $_POST['treinta']<$currentDate){
        echo "PA";
    }else{
        echo "<br>NM";
    }*/

  }else{
    echo "Hola mundo";
  }

  return $filter;
}

/**
 * Formulario de configuración
 * @return array
 */
function indra_form_sent_configuration() {
  $form = array();

  $form['indra_form_sent_id'] = array(
    '#type' => 'textarea',
    '#title' => t('Form id'),
    '#description' => t('Enter one form identifier per line."'),
    '#default_value' => variable_get('indra_form_sent_id', ''),
  );
  $form['indra_form_sent_first_layer'] = array(
    '#type' => 'textarea',
    '#title' => t('First layer'),
    '#description' => t('Initial text. Visible in the form. English language.'),
    '#default_value' => variable_get('indra_form_sent_first_layer', ''),
  );
  $form['indra_form_sent_data_policy'] = array(
    '#type' => 'textfield',
    '#title' => t('Privacy Policy Node'),
    '#description' => t('Enter the node containing the Privacy Policy. For example: 40697'),
    '#default_value' => variable_get('indra_form_sent_data_policy', ''),
    '#size' => 50,
    '#maxlength' => 50,
  );
  $form['indra_form_sent_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail to which the csv is sent'),
    '#description' => t('If you enter more than one email do it separated by commas.'),
    '#default_value' => variable_get('indra_form_sent_mail', ''),
    '#size' => 80,
    '#maxlength' => 128,
  );

  return system_settings_form($form);
}

/**
 * Retorna datos para componer csv
 * @param array $filter
 * @return array
 */
function indra_form_sent_scv_data(array $filter) {
  $rows = IndraFormSent::readPager(500, $filter);
  $data = array();
  // Cabacera
  $data[] = array('FID', 'IDENTIFIER', 'SUBJECT', 'USER', 'LANGUAGE', 'MAIL-TO', 'REPLY-TO', 'SEND-DATE', 'IP-ADDRESS', 'BODY');
  // Cuerpo
  foreach ($rows as $row) {
    $data[] = array(
      $row['fid'],
      $row['identifier'],
      $row['subject'],
      $row['user'],
      $row['language'],
      $row['mailto'],
      $row['replyto'],
      $row['send_date'],
      $row['ip_address'],
      $row['body']
    );
  }
  return $data;
}

/**
 * Envía csv por email.
 */
function indra_form_sent_email() {
  global $language;
  $language->language = 'es';

  $info = 'Informe de formularios enviados.<br />' . PHP_EOL; // para el cron
  $info .= '-------------------------------------------' . '<br />' . PHP_EOL;

  $filter = array(
    'date_from' => gmmktime(0, 0, 0, date("m") - 1, 1, date("Y")),
    'date_to' => gmmktime(0, 0, -1, date("m"), 1, date("Y")),
  );

  $body = t('Se adjunta fichero csv con los datos de los formularios enviados desde @from hasta @to', array(
    '@from' => date('m-d-Y', $filter['date_from']),
    '@to' => date('m-d-Y', $filter['date_to']),
      )
  );

  if (!$to = variable_get('indra_form_sent_mail')) {
    $info .= 'ERROR, no existe dirección a la que enviar el correo.';
    return $info;
  }

  $stream = fopen('data://text/plain,' . "", 'w+');
  foreach (indra_form_sent_scv_data($filter) as $line) {
    fputcsv($stream, $line, ';');
  }
  rewind($stream);

  $attachment = array(
    'filecontent' => stream_get_contents($stream),
    'filename' => 'form_sent.csv',
    'filemime' => 'text/csv',
  );
  fclose($stream);

  $params = array(
    'key' => 'mail_csv',
    'to' => $to,
    'from' => variable_get('site_mail', 'sistint@indra.es'),
    'body' => $body,
    'subject' => 'CSV de formularios enviados desde ' . variable_get('site_name', 'Default'),
    'attachments' => array($attachment),
  );

  $message = drupal_mail('indra_form_sent', 'mail_csv', $to, $language, $params);
  if (!$message['result']) {
    $info .= t('ERROR al enviar el e-mail (from %from to %to).', array('%from' => $message['from'], '%to' => $message['to'])) . '<br />' . PHP_EOL;
  }
  else {
    $info .= t('El e-mail se ha enviado (desde %from a %to).', array('%from' => $message['from'], '%to' => $message['to'])) . '<br />' . PHP_EOL;
    $info .= $body;
  }


  return $info;
}
