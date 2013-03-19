<?

function soshake_auto($form_id, $form_values) {
        global $theme_key, $base_url;
        
        $themes = list_themes();
        $theme_object = $themes[$theme_key];
        foreach($themes as $theme) {
                if($theme->status == 1 && $theme->name != $theme_object->name) $active_theme = $theme;
        }

        $form = array();
        $form["help"] = array(
                '#type' => 'item',
                '#title' => 'Insert easily Social Plugins on your website',
                '#description' => 'Choose simply on wich block of your website you want to display a social plugin.<br />You can see what "block" is aviaible on your theme : <a href="'.$base_url.'/admin/structure/block/demo/'.$active_theme->name.'#overlay-context=admin/config/soshake/autoinsert" target="_blank">See my theme\'s blocks</a>'
        );
        foreach($active_theme->info["regions"] as $hook=>$name) {
                $form["hook-$hook"] = array(
                        '#type' => 'item',
                        '#title' => $name,
                );
                $form["hook-$hook-like"] = array(
                        '#type' => 'checkbox',
                        '#title' => 'Facebook',
                        '#return_value' => 'like',
                        '#default_value' => variable_get("hook-$hook-like", 0),
                );
                $form["hook-$hook-tweet"] = array(
                        '#type' => 'checkbox',
                        '#title' => 'Twitter',
                        '#default_value' => variable_get("hook-$hook-tweet", 0),
                        '#return_value' => 'tweet'
                );
                $form["hook-$hook-gplus"] = array(
                        '#type' => 'checkbox',
                        '#title' => 'Google +1',
                        '#default_value' => variable_get("hook-$hook-gplus", 0),
                        '#return_value' => 'gplus'
                );
                $form["hook-$hook-linkedin"] = array(
                        '#type' => 'checkbox',
                        '#title' => 'LinkedIn Share',
                        '#default_value' => variable_get("hook-$hook-linkedin", 0),
                        '#return_value' => 'linkedin'
                );
                $form["hook-$hook-linkedin"] = array(
                        '#type' => 'checkbox',
                        '#title' => 'Pinterest Pin-it',
                        '#default_value' => variable_get("hook-$hook-pinit", 0),
                        '#return_value' => 'pinit'
                );
                $form["hook-$hook-fanbox"] = array(
                        '#type' => 'checkbox',
                        '#title' => 'Facebook Page Fan-Box',
                        '#default_value' => variable_get("hook-$hook-fanbox", 0),
                        '#return_value' => 'fanbox'
                );
                $form["hook-$hook-fbconnect"] = array(
                        '#type' => 'checkbox',
                        '#title' => 'Facebook Connect',
                        '#default_value' => variable_get("hook-$hook-fbconnect", 0),
                        '#return_value' => 'fbconnect'
                );
                $form["submit-$hook"] = array(
                        '#type' => 'submit',
                        '#value' => t('Save')
                );
        }

        $networks = array(
                "facebook" => "like",
                "twitter" => "tweet",
                "Google +1" => "gplus",
                "LinkedIn Share" => "linkedin",
                "Facebook Page Fan Box" => "fanbox",
                "Facebook Connect" => "fbconnect",
        );
        
        $db = db_delete('block') -> condition('module', "soshake") -> execute() ;
        foreach($active_theme->info["regions"] as $hook=>$name) {
                $insert = FALSE;
                foreach($networks as $network_name => $soshake_id) {
                        if(variable_get("hook-$hook-$soshake_id", 0) != "0" && !$insert) {
                                $db = db_insert('block')
                                        ->fields(array(
                                          'status' => 1,
                                          'weight' => -8,
                                          'region' => $hook,
                                          'module' => "soshake",
                                          'delta' => "soshake-$hook",
                                          'theme' => $active_theme->name,
                                          'custom' => 0,
                                          'visibility' => 0,
                                          'pages' => '',
                                          'title' => '<none>',
                                          'cache' => -1,
                                        ))
                                        -> execute() ;
                                $insert = TRUE;
                        }
                }
        }
        $db = db_insert('block')
                ->fields(array(
                  'status' => 1,
                  'weight' => -8,
                  'region' => "footer",
                  'module' => "soshake",
                  'delta' => "soshake-footer-scripts",
                  'theme' => $active_theme->name,
                  'custom' => 0,
                  'visibility' => 0,
                  'pages' => '',
                  'title' => '<none>',
                  'cache' => -1,
                ))
                -> execute() ;

        return system_settings_form($form);
}

function soshake_manuel() {
        global $base_url;
        $form = array();
        
        $form['like'] = array(
                '#type' => 'textfield',
                '#title' => t("To add the <b>\"Like\" button</b>, copy this where you want to see it: "),
                '#description' => "",
                '#default_value' => "<div class=\"up2-like\"></div>",
        );
        $form['tweet'] = array(
                '#type' => 'textfield',
                '#title' => t("To add the <b>\"Tweet\" button</b>, copy this where you want to see it : "),
                '#description' => "",
                '#default_value' => "<div class=\"up2-tweet\"></div>",
        );
        $form['gplus'] = array(
                '#type' => 'textfield',
                '#title' => t("To add the <b>\"+1\" button from Google</b>, copy this where you want to see it : "),
                '#description' => "",
                '#default_value' => "<div class=\"up2-gplus\"></div>",
        );
        $form['linkedin'] = array(
                '#type' => 'textfield',
                '#title' => t("To add the <b>\"Share\" button from LinkedIn</b>, copy this where you want to see it : "),
                '#description' => "",
                '#default_value' => "<div class=\"up2-linkedin\"></div>",
        );
        $form['pinit'] = array(
                '#type' => 'textfield',
                '#title' => t("To add the <b>\"Pin it\" button from Pinterest</b>, copy this where you want to see it : "),
                '#description' => "",
                '#default_value' => "<div class=\"up2-pinit\"></div>",
        );
        $form['fanbox_diff'] = array(
                '#type' => 'textfield',
                '#title' => t("To display the Facebook Fanbox of your Facebook Page, copy this code where you want to see it : "),
                '#description' => "By default, the Facebook Page displayed will be the one defined in your SoShake account, if not, define the url of your Facebook Page in the \"URL\" attribute",
                '#default_value' => "<div class=\"up2-fanbox\" url=\"FACEBOOK_FAN_PAGE_URL\"></div>",
                '#size' => 100
        );
        $form['fbconnect'] = array(
                '#type' => 'textfield',
                '#title' => t("Facebook connect allow your visitors to create an account with one click, using their info from Facebook. To add this plugin, just copy this code where you want to see it : "),
                '#description' => "",
                '#default_value' => "<div class=\"up2-fbconnect\" url=\"".$base_url."/soshake\"></div>",
                '#size' => 100
        );
        return $form;
}

function soshake_layout() {
        $form = array();
        
        $form["help"] = array(
                '#type' => 'item',
                '#title' => 'SoShakes positions',
                '#description' => 'Esthetic parameters for positionning your widgets.'
        );
        $form["up2-SoShake-layout"] = array(
                '#type' => 'radios',
                '#options' => array("vertical" => "Vertical (widgets will be in a column)", "horizontal" => "Horizontal (widgets will be side by side)"),
        );
        $form["help2"] = array(
                '#type' => 'item',
                '#title' => 'SoShakes alignement',
                '#description' => ''
        );
        $form["up2-SoShake-align"] = array(
                '#type' => 'radios',
                '#options' => array("left" => "Left", "center" => "Center", "right" => "Right"),
        );
        return system_settings_form($form);
}

function soshake_account() {
        global $base_url;
        $form = array();
        
        $client = json_decode(file_get_contents("http://soshake.com/api/me.json?url=$base_url"));
        if($client->code == 200 && false) {
                $content = '
                <p>
                        Visit your SoShake account to define actions you want to display on your website when a content is shared.<br />
                        <br />
                        <a href="http://soshake.com/front" target="_blank" class="up2-button">Go to SoShake.com (link will open in a new window)</a>
                </p>
                ';
        } else {
                $content = '<div class="messages error"><p>You don\'t have a SoShake Account. This won\'t change the behavior of this module, but you won\'t benefit from every awesome feature we\'ve made !<br />
                <a href="http://soshake.com/?lang=en" target="_blank" class="up2-button">Create an account</a><p></div>';
        }
        
        $form['texte'] = array(
                '#type' => 'item',
                '#title' => t('Your SoShake Account'),
                '#description' => "
                <script type=\"text/javascript\" src=\"http://soshake.com/api/actions/cms/drupal.js\"></script>
                $content
                ",
        );
        
        return ("ttttr");
}
