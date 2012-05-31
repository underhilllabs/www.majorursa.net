<?php
/**
 * @file
 * note_listing.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function note_listing_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'clone_of_recent_notes';
  $view->description = 'A Page Listing of the Notes';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Notes Listing';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Notes';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Category */
  $handler->display->display_options['fields']['field_category']['id'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['table'] = 'field_data_field_category';
  $handler->display->display_options['fields']['field_category']['field'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['label'] = '';
  $handler->display->display_options['fields']['field_category']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_category']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_category']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_category']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_category']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_category']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_category']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_category']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_category']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_category']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_category']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_category']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_category']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_category']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_category']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_category']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_category']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_category']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_category']['field_api_classes'] = 0;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Sort criterion: Content: Category (field_category) */
  $handler->display->display_options['sorts']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['sorts']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['sorts']['field_category_tid']['field'] = 'field_category_tid';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'notes' => 'notes',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'field_category',
      'rendered' => 1,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Category (field_category) */
  $handler->display->display_options['sorts']['field_category_tid']['id'] = 'field_category_tid';
  $handler->display->display_options['sorts']['field_category_tid']['table'] = 'field_data_field_category';
  $handler->display->display_options['sorts']['field_category_tid']['field'] = 'field_category_tid';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['granularity'] = 'day';
  $handler->display->display_options['path'] = 'notes';
  $export['clone_of_recent_notes'] = $view;

  return $export;
}
