<?php
// 
?>

<div id="header-top" >
 <div id="header-container" class="container-16 clearfix">
    <div id="branding" class="grid-16 clearfix">
    <?php if ($linked_logo_img): ?>
      <span id="logo" class="grid-3 alpha"><?php print $linked_logo_img; ?></span>
    <?php endif; ?>
    <?php if ($linked_site_name): ?>
      <h1 id="site-name" class="grid-10 omega"><?php print $linked_site_name; ?></h1>
    <?php endif; ?>
    <?php if ($site_slogan): ?>
      <div id="site-slogan" class="grid-3 omega"><?php print $site_slogan; ?></div>
    <?php endif; ?>
    <?php if($secondary_menu_links): ?>
       <div id="secondary_menu grid-3 omega">
         <?php print $secondary_menu_links; ?>
       </div>
    <?php endif; ?>
    </div>
 

 
  <?php if ($main_menu_links ): ?>
    <div id="site-menu" class="grid-16">
      <?php print $main_menu_links; ?>
    </div>
  <?php endif; ?>


 </div>
</div>

<div id="page" class="container-16 clearfix">


  <div id="site-header" class="clearfix">

  <?php if ($page['search_box']): ?>
    <div id="search-box" class="grid-6 prefix-10"><?php print render($page['search_box']); ?></div>
  <?php endif; ?>
  </div>

  <div id="site-subheader" class="prefix-1 suffix-1 clearfix">
  <?php if ($page['highlighted']): ?>
    <div id="highlighted" class="<?php print ns('grid-14', $page['header'], 7); ?>">
      <?php print render($page['highlighted']); ?>
    </div>
  <?php endif; ?>

  <?php if ($page['header']): ?>
    <div id="header-region" class="region <?php print ns('grid-14', $page['highlighted'], 7); ?> clearfix">
      <?php print render($page['header']); ?>
    </div>
  <?php endif; ?>
  </div>



  <div id="main" class="column <?php print ns('grid-16', $page['sidebar_first'], 4, $page['sidebar_second'], 4) . ' ' . ns('push-4', !$page['sidebar_first'], 4); ?>">
    <?php print $breadcrumb; ?>
    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h1 class="title" id="page-title"><?php print $title; ?></h1>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
    <?php if ($tabs): ?>
      <div class="tabs"><?php print render($tabs); ?></div>
    <?php endif; ?>
    <?php print $messages; ?>
    <?php print render($page['help']); ?>

    <div id="main-content" class="region clearfix">
      <!-- <h1>FRONT PAGE, yeah</h1> -->
      <?php 

	?>
      <div class="alpha grid-12">
	<span id="#front-cat-1" class="alpha region grid-6"><?php print render($page['front_cat_1']); ?></span>
   	<span id="#front-cat-2" class="omega region grid-6"><?php print render($page['front_cat_2']); ?></span>
      </div>
      <div class="alpha grid-12">
	<span id="#front-cat-3" class="alpha region grid-6"><?php print render($page['front_cat_3']); ?></span>
	<span id="#front-cat-4" class="omega region grid-6"><?php print render($page['front_cat_4']); ?></span>
      </div>

    </div>

    <?php print $feed_icons; ?>
  </div>

<?php if ($page['sidebar_first']): ?>
  <div id="sidebar-left" class="column sidebar region grid-4 <?php print ns('pull-12', $page['sidebar_second'], 3); ?>">
    <?php print render($page['sidebar_first']); ?>
  </div>
<?php endif; ?>

<?php if ($page['sidebar_second']): ?>
  <div id="sidebar-right" class="column sidebar region grid-4">
    <?php print render($page['sidebar_second']); ?>
  </div>
<?php endif; ?>


  <div id="footer" class="prefix-1 suffix-1">
    <?php if ($page['footer']): ?>
      <div id="footer-region" class="region grid-14 clearfix">
        <?php print render($page['footer']); ?>
      </div>
    <?php endif; ?>
  </div>

</div>
