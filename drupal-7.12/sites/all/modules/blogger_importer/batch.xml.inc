<?php

/**
 * batch for processing a Blogger XML export file
 * code based on Batch API example @ http://drupal.org/node/180528
 */

/**
 * The $batch can include the following values. Only 'operations'
 * and 'finished' are required, all others will be set to default values.
 *
 * @param operations
 *   An array of callbacks and arguments for the callbacks.
 *   There can be one callback called one time, one callback
 *   called repeatedly with different arguments, different
 *   callbacks with the same arguments, one callback with no
 *   arguments, etc.
 *
 * @param finished
 *   A callback to be used when the batch finishes.
 *
 * @param title
 *   A title to be displayed to the end user when the batch starts.
 *
 * @param init_message
 *   An initial message to be displayed to the end user when the batch starts.
 *
 * @param progress_message
 *   A progress message for the end user. Placeholders are available.
 *   Placeholders note the progression by operation, i.e. if there are
 *   2 operations, the message will look like:
 *    'Processed 1 out of 2.'
 *    'Processed 2 out of 2.'
 *   Placeholders include:
 *     @current, @remaining, @total and @percentage
 *
 * @param error_message
 *   The error message that will be displayed to the end user if the batch
 *   fails.
 *
 * @param file
 *   Path to file containing the callbacks declared above. Always needed when
 *   the callbacks are not in a .module file.
 *
 */
function blogger_importer_batch_xml($xml_path, $limit = 10) {
  $batch = array(
    'operations' => array(
      array('blogger_importer_batch_xml_process', array($xml_path, $limit)),    // should run until it's finished
      // @todo merge the other batch in here -- add an operation -- but how to stop 2nd op if 1st op fails?
      ),
    'finished' => 'blogger_importer_batch_xml_finished',
    'title' => t('Processing Blogger Export XML'),
    'init_message' => t('Beginning Blogger import from XML file @file.', array('@file' => $xml_path)),
    'progress_message' => t('Importing Blogger posts...'),
    'error_message' => t('Blogger import has encountered an error.'),
    'file' => drupal_get_path('module', 'blogger_importer') . '/batch.xml.inc',
  );
  batch_set($batch);

  // override FAPI's normal batch_process() (back to form) w/ different output page
  batch_process('admin/content/blogger_importer/results');
}


/**
 * Batch Operation Callback
 *
 * Each batch operation callback will iterate over and over until
 * $context['finished'] is set to 1. After each pass, batch.inc will
 * check its timer and see if it is time for a new http request,
 * i.e. when more than 1 minute has elapsed since the last request.
 *
 * An entire batch that processes very quickly might only need a single
 * http request even if it iterates through the callback several times,
 * while slower processes might initiate a new http request on every
 * iteration of the callback.
 *
 * This means you should set your processing up to do in each iteration
 * only as much as you can do without a php timeout, then let batch.inc
 * decide if it needs to make a fresh http request.
 *
 * @param options1, options2
 *   If any arguments were sent to the operations callback, they
 *   will be the first arguments available to the callback.
 *
 * @param context
 *   $context is an array that will contain information about the
 *   status of the batch. The values in $context will retain their
 *   values as the batch progresses.
 *
 * @param $context['sandbox']
 *   Use the $context['sandbox'] rather than $_SESSION to store the
 *   information needed to track information between successive calls.
 *   The values in the sandbox will be stored and updated in the database
 *   between http requests until the batch finishes processing. This will
 *   avoid problems if the user navigates away from the page before the
 *   batch finishes.
 *
 * @param $context['results']
 *   The array of results gathered so far by the batch processing.
 *   The current operation can append its own.
 *
 * @param $context['message']
 *   A text message displayed in the progress page.
 *
 * @param $context['finished']
 *   A float number between 0 and 1 informing the processing engine
 *   of the completion level for the operation.
 *
 *   1 (or no value explicitly set) means the operation is finished
 *   and the batch processing can continue to the next operation.
 *
 * [ben] assuming that setting $context['success']=FALSE signals an error
 */
function blogger_importer_batch_xml_process($xml_path, $limit, &$context) {

  // initialize the sandbox
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;    // should persist

    if (! $context['sandbox']['xml'] = file_get_contents($xml_path)) {
      $context['success'] = FALSE;
      $context['finished'] = 1;
      return drupal_set_message(t("Unable to load XML from file @file", array('@file' => $xml_path)));
    }

    // placeholder for results
    // will contain sub-arrays, top level for categories, each category w/ entries
    // (categories include settings, comments, posts, etc)
    // set preferred order initially
    $context['results'] = array(
      'post' => array(),
      'comment' => array(),
      'settings' => array(),
      'template' => array()
    );
  }

  $results = &$context['results'];

  // load into querypath
  $qp = qp($context['sandbox']['xml']);
  $qp->top()->find('feed > entry');

  // track original # of entries (needed for progress bar)
  if (! isset($context['sandbox']['max'])) {
    $context['sandbox']['max'] = $qp->size();
  }

  // message while batch is running
  $context['message'] = t('Processed @count out of @total entries, @remaining remaining (@limit at a time).',
      array('@count' => $context['sandbox']['progress'],
            '@remaining' => $qp->size(),
            '@total' => $context['sandbox']['max'],
            '@limit' => $limit
          ));

  $entry_count = 0;     // in this run

  // loop thru all the remaining entries
  foreach($qp->top()->find('feed')->children('entry') as $entry) {

    $entry_count++;

    // NOTE: EACH TIME A QUERY IS RUN ON $qp or $entry w/o branch(), IT MODIFIES THE GLOBAL POINTER!

    // get the ID: in format tag:blogger.com,1999:blog-29350293.settings.BLOG_SEARCHABLE, get the last part
    $id = $entry->find('> id')->text();
    $id = array_pop(explode('.', $id));

    // get the 'kind' of entry
    // in format http://schemas.google.com/blogger/2008/kind#template, get the last part
    $kind = $entry->next('category')->attr('term');   // sibling
    $kind = array_pop(explode('#', $kind));

    // store entries in categories
    $results[$kind][$id] = array(
      'id' => $id,
      'title' => (string)$entry->siblings('title')->text(),
      'value' => (string)$entry->siblings('content')->text(),
      'date' => (string)$entry->siblings('published')->text(),    // this is BEFORE so next() doesn't work
      'tags' => array(),
      'time' => NULL,
    );
    if ($results[$kind][$id]['date']) {
      $results[$kind][$id]['time'] = strtotime($results[$kind][$id]['date']);
    }
    foreach ($entry->siblings('category') as $category) {
      // Tags have this scheme.
      if ($category->attr('scheme') == 'http://www.blogger.com/atom/ns#') {
        $results[$kind][$id]['tags'][] = $category->attr('term');
      }
    }

    // $entry pointer at this point is INSIDE the entry, go back up
    if ($entry->tag() != 'entry') $entry->parent('entry');

    // if it's a post, get the URL
    if ($kind == 'post') {
      // find the right <link> element
      foreach($entry->branch()->children('link') as $link) {
        if ($link->attr('rel') == 'alternate' && $link->attr('type') == 'text/html' && $link->hasAttr('href')) {
          $url = $link->attr('href');
          $results[$kind][$id]['url'] = $url;
          break;
        }
      }
      // revert the pointer (in case elem is missing)
      $entry->end();
    }

    // comment processing
    elseif ($kind == 'comment') {

      // pointer should still be on 'entry' b/c branched, just in case
      if ($entry->tag() != 'entry') $entry->parents('entry');

      // get the Post ID of the post from 'ref' attribute
      // (seems to be best comment-post connector b/c entries array is indexed by post IDs)
      // - find the <thr:in-reply-to> element
      $in_reply_to = $entry->branch()->children('thr|in-reply-to');
      $post_id = ($in_reply_to->size() == 1 && $in_reply_to->hasAttr('ref')) ? $in_reply_to->attr('ref') : NULL;
      if (!empty($post_id)) {
        $post_id = array_pop(explode('.', $post_id));
      }
      $results[$kind][$id]['post_id'] = $post_id;
    }

    if ($entry->tag() != 'entry') {
      $entry->parent('entry');
    }
    // get the author name, uri, email
    $author = $entry->branch()->children('author');

    if ($author->children()->size() > 0) {
      // pointer now on 1st child
      // structure is entry > author > author_X, pointer now at bottom level
      // (branch for each in case missing)
      $results[$kind][$id]['author_name'] = $author->branch()->siblings('name')->text();
      $results[$kind][$id]['author_uri'] = $author->branch()->siblings('uri')->text();
      $results[$kind][$id]['author_email'] = $author->branch()->siblings('email')->text();
    }

    if ($entry->tag() != 'entry') {
      // back up to entry
      $entry->parent('entry');
    }

    // back to the right element?
    if ($entry->tag() != 'entry') {
      $context['success'] = FALSE;
      $context['finished'] = 1;
      drupal_set_message(t("Parsing error: parent tag should be 'entry', instead '@tag' (kind: @kind, id: @id)",
          array('@tag' => $entry->tag(), '@kind' => $kind, '@id' => $id) ), 'error');
    }

    // remove processed entry from the xml
    $entry->remove();

    // SAVE THE REMAINING XML
    $context['sandbox']['xml'] = $qp->top()->xml();

    if ($entry_count >= $limit) {
      break;
    }

    // progress is checked outside main loop, so in case none left...
    if ($qp->size() == 0) {
      drupal_set_message( t("No more entries to process"), 'status' );
      break;
    }

  }

  // Update our progress information.

  // track progress by # of processed results
  // @todo this caused problems when parsing didn't go well, might want to revisit
  $context['sandbox']['progress'] = _blogger_importer_count_entries($results);

  // catch errors in progress math
  $qp_size = $qp->branch()->top()->find('feed')->children('entry')->size();
  $qp_remaining = $context['sandbox']['max'] - $qp_size;
  if ($qp_remaining != $context['sandbox']['progress']) {
    drupal_set_message(t("Progress count error, @amount1 vs @amount2, off by @offset",
        array(
          '@amount1' => $qp_remaining,
          '@amount2' => $context['sandbox']['progress'],
          '@offset' => abs($qp_remaining - $context['sandbox']['progress']))
        ), 'error');
  }

  $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  // just in case
  if ($context['finished'] > 1) {
    $context['finished'] = 1;
  }

  // in case progress calculations aren't right...
  if ($context['finished'] == 1 && $qp->top()->find('feed > entry')->size() > 0) {
    drupal_set_message(t("Import ended prematurely! Still @size left to go but finished.", array('@size' => $qp->size())), 'error');
    $context['success'] = FALSE;
  }
  if ($context['finished'] != 1 && $qp->top()->find('feed > entry')->size() == 0
    ||  $context['sandbox']['progress'] > $context['sandbox']['max']) {
    drupal_set_message(t("Import should have ended! 0 left to go but batch still going."), 'error');
    $context['success'] = FALSE;
    $context['finished'] = 1;
  }

}



/**
 * Batch 'finished' callback
 */
function blogger_importer_batch_xml_finished($success, $results, $operations) {

  if ($success) {

    drupal_set_message( t('@count entries processed.', array('@count' => _blogger_importer_count_entries($results) ) ) );

    // save to cache so it's available for results page & later
    blogger_importer_results($results);

    drupal_set_message('Finished', 'status');

  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $output = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));

    drupal_set_message($output, 'error');
  }

  // batch gets redirected to results page (from batch_process() above)
}
