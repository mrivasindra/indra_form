<?php

class IndraFormSent {

  private $fid;
  private $identifier;
  private $user;
  private $language;
  private $mailto;
  private $replyto;
  private $send_date;
  private $ip_address;
  private $subject;
  private $body;

  public static function readItem($fid) {
    $query = db_select('indra_form_sent', 's')
      ->fields('s')
      ->condition('s.fid', $fid, '=');

  }

  public static function readPager($pager = 10, array $filter = array()) {
    $rows = array();
    $field = array('fid', 'identifier', 'subject', 'user', 'language', 'mailto', 'replyto', 'send_date', 'ip_address', 'body');
    $query = db_select('indra_form_sent', 'fs');
    $query->fields('fs', $field);
    if (isset($filter['identifier']) && 'all' != $filter['identifier']) {
      $query->condition('identifier', $filter['identifier'], '=');
    }
    if (isset($filter['subject']) && 'all' != $filter['subject']) {
      $query->condition('subject', $filter['subject'], '=');
    }
    if (isset($filter['mailto']) && 'all' != $filter['mailto']) {
      $query->condition('mailto', '%' . db_like($filter['mailto']) . '%', 'LIKE');
    }
    if (isset($filter['replyto']) && !empty($filter['replyto'])) {
      $query->condition('replyto', '%' . db_like($filter['replyto']) . '%', 'LIKE');
    }
    if (isset($filter['date_from']) && null !== $filter['date_from']) {
      $query->condition('send_date', $filter['date_from'], '>=');
    }
    if (isset($filter['date_to']) && null !== $filter['date_to']) {
      $query->condition('send_date', $filter['date_to'], '<=');
    }
    $result = $query
      ->extend('PagerDefault')
      ->limit($pager)
      ->orderBy('send_date', 'DESC')
      ->execute();
    while ($row = $result->fetchAssoc()) {
      $row['send_date'] = date('Y-m-d', $row['send_date']);
      $row['operations'] = l('view', 'admin/content/formsent/' . $row['fid'] . '/view') . ' | ' .
        l('delete', 'admin/content/formsent/' . $row['fid'] . '/delete');
      $rows[] = $row;
    }
    return $rows;
  }

  public static function deleteItem($fid) {
    //Fechas en estilo UNIX
  $unix_hoy = time(); //UNIX de hoy
  // 1 mes: 2629743 / 2 meses: 5259486 / 3 meses: 7889229
  $unix_past = $unix_hoy-2629743; //UNIX de hace un mes
    $num_deleted = db_delete('indra_form_sent')
      ->condition('fid', $fid)
      ->execute();
    return $num_deleted;
  }

  public static function readAllIdentifier() {
    $query = db_select('indra_form_sent', 's')
      ->distinct()
      ->fields('s', array('identifier'));
    $rows['all'] = t('All');
    foreach ($query->execute()->fetchAll() as $v) {
      $rows[$v->identifier] = $v->identifier;
    }
    return $rows;
  }

  public static function readAllSubject() {
    $query = db_select('indra_form_sent', 's')
      ->distinct()
      ->fields('s', array('subject'));
    $rows['all'] = t('All');
    foreach ($query->execute()->fetchAll() as $v) {
      $rows[$v->subject] = $v->subject;
    }
    return $rows;
  }

  public function create(IndraFormSent $forsent) {
    $nid = db_insert('indra_form_sent')
      ->fields(array(
        'identifier' => (string) $forsent->identifier,
        'user' => (string) $forsent->user,
        'language' => (string) $forsent->language,
        'mailto' => (string) $forsent->mailto,
        'replyto' => (string) $forsent->replyto,
        'send_date' => (int) $forsent->send_date,
        'ip_address' => (string) $forsent->ip_address,
        'subject' => (string) $forsent->subject,
        'body' => (string) $forsent->body,
      ))
      ->execute();
  }

// seter

  public function setIndentifier($identifier) {
    $this->identifier = substr($identifier, 0, 126);
  }

  public function setUser($user) {
    $this->user = substr($user, 0, 126);
  }

  public function setLanguage($language) {
    $this->language = is_object($language) ? $language->language : $language;
  }

  public function setMailTo($mailto) {
    $this->mailto = substr($mailto, 0, 126);
  }

  public function setReply($replyto) {
    $this->replyto = substr($replyto, 0, 126);
  }

  public function setSendDate($send_date) {
    $this->send_date = $send_date;
  }

  public function setIp($ip_address) {
    $this->ip_address = substr($ip_address, 0, 126);
  }

  public function setSubject($subject) {
    $this->subject = substr($subject, 0, 254);
  }

  public function setBody($body) {
    $this->body = substr($body, 0, 5000);
  }

}
