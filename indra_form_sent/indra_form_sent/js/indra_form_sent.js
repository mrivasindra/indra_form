function indra_form_sent_form_callback(event) {
    event.preventDefault();
    var baseurl = Drupal.settings.basePath;
    var url = baseurl + "admin/content/formsent/table/ajax";
    var dataSerialize = jQuery('#indra-form-sent-filter-form').serialize();
    jQuery.ajax({
        url: url,
        type: 'POST',
        dataType: 'html',
        data: dataSerialize,
        success: function (data) {
            jQuery('#form-sent-ui').empty();
            jQuery('#form-sent-ui').append(data);
        },
        error: function (xhr, ajaxOptions, trhownError) {
            alert('Error');
        }
    });
}
