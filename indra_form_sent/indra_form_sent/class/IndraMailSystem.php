<?php
/**
 * Description of IndraMailSystem
 *
 * @author jaramos
 */
class IndraMailSystem extends MimeMailSystem {

  /**
   * @Override
   * Concatena y ajusta el cuerpo para email HTML
   *
   * @param array $message
   *   Un array tal como se describre en  hook_mail_alter() con parametros
   *   opcionales descritos en mimemail_prepare_message().
   *
   * @return array
   *   El mensaje formateado.
   */
  public function format(array $message) {
    return parent::format($message);
  }

  /**
   * @Override
   * Envía el mensaje e formato html usando la variabales y configuración por defecto de Drupal.
   * Si el envío teien exito ser registra en BBDD
   *
   * @param array $message
   *   Un array tal como se describre en  hook_mail_alter() con parametros
   *   opcionales descritos en mimemail_prepare_message().
   *
   * @return boolean
   *   TRUE si el email fue enviado, de lo contrario FALSE.
   */
  public function mail(array $message) {
    $result = parent::mail($message);
    if ($result && in_array($message['key'], preg_split('/\r\n|\r|\n/', variable_get('indra_form_sent_id', '')))) {
      $sent = new IndraFormSent();
      $sent->setIndentifier($message['id']);
      $sent->setMailto($message['to']);
      $sent->setSubject($message['subject']);
      $sent->setSendDate(time());
      $sent->setIp(ip_address());
      $sent->setLanguage($message['language']);
      if (isset($message['params']['reply-to'])) {
        $sent->setReply($message['params']['reply-to']);
      }
      if (isset($message['params']['name'])) {
        $sent->setUser($message['params']['name']);
      }
      if (isset($message['params']['message'])) {
        $sent->setBody($message['params']['message']);
      }
      elseif (isset($message['params']['body'])) {
        $sent->setBody($message['params']['body']);
      }
      else {
        $sent->setBody($message['body']);
      }
      $sent->create($sent);
    }
    return $result;
  }

}
