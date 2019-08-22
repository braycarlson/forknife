#ifdef _WIN32
#include <windows.h>
#endif

#include <ctype.h>
#include <gio/gio.h>
#include <glib-2.0/glib.h>
#include <gtk/gtk.h>
#include <math.h>


static void dialog(char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        NULL,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_CLOSE,
        "%s", message
    );

    gtk_widget_set_name(dialog, "dialog");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static gboolean to_gboolean(gchar *set) {
    if (strcmp(set, "True")) {
        return FALSE;
    } else {
        return TRUE;
    }
}

static void is_numeric(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data) {
    int i;

    for (i = 0; i < length; i++) {
        if (!isdigit(text[i])) {
            g_signal_stop_emission_by_name(G_OBJECT(editable), "insert-text");
            return;
        }
    }
}

static void uncheck_read_only() {
    // Build a path to the configuration file
    gchar *path = g_build_filename(
        g_get_user_config_dir(),
        "FortniteGame\\Saved\\Config\\WindowsClient\\GameUserSettings.ini",
        NULL
    );


    // Uncheck read-only attribute on Windows
    #ifdef _WIN32
        DWORD attributes = GetFileAttributesA(path);

        if (attributes & FILE_ATTRIBUTE_READONLY) {
            attributes &= ~FILE_ATTRIBUTE_READONLY;
            SetFileAttributesA(path, attributes);
        }
    #endif

    #ifdef __APPLE__
        // Todo: File permissions on Mac OS
    #endif

    g_free(path);
}

static void check_read_only(GtkWidget *stack, GObject *object) {
    // Build a path to the configuration file
    gchar *path = g_build_filename(
        g_get_user_config_dir(),
        "FortniteGame\\Saved\\Config\\WindowsClient\\GameUserSettings.ini",
        NULL
    );

    GtkWidget *readonly = g_object_get_data(object, "readonly");
    gboolean state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(readonly));

    if (state) {
        #ifdef _WIN32
            SetFileAttributesA(path, FILE_ATTRIBUTE_READONLY);
        #endif

        #ifdef __APPLE__
            // Todo: File permissions on Mac OS
        #endif
    }

    g_free(path);
}

static void save_game_settings(GtkWidget *stack, GObject *object) {
    // Build a path to the configuration file
    gchar *path = g_build_filename(
        g_get_user_config_dir(),
        "FortniteGame\\Saved\\Config\\WindowsClient\\GameUserSettings.ini",
        NULL
    );

    GKeyFile *file = g_key_file_new();
    GError *error = NULL;

    g_key_file_load_from_file(
        file,
        path,
        G_KEY_FILE_NONE,
        &error
    );

    // Audio Quality
    GtkComboBox *audio_quality_box = g_object_get_data(object, "audio_quality_box");
    gint audio_quality = gtk_combo_box_get_active(audio_quality_box);

    g_key_file_set_integer(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "AudioQualityLevel",
        audio_quality
    );

    g_key_file_set_integer(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "LastConfirmedAudioQualityLevel",
        audio_quality
    );

    // Hide Social Name
    GtkSwitch *hide_social_name_switch = g_object_get_data(object, "hide_social_name_switch");
    gboolean hide_social_name = gtk_switch_get_active(hide_social_name_switch);

    if (hide_social_name) {
        g_key_file_set_string(
            file,
            "ChatSettings",
            "HideSocialName",
            "True"
        );
    } else {
        g_key_file_set_string(
            file,
            "ChatSettings",
            "HideSocialName",
            "False"
        );
    }

    // Auto Decline Friend Requests
    GtkSwitch *auto_decline_friend_requests_switch = g_object_get_data(object, "auto_decline_friend_requests_switch");
    gboolean auto_decline_friend_requests = gtk_switch_get_active(auto_decline_friend_requests_switch);

    if (auto_decline_friend_requests) {
        g_key_file_set_string(
            file,
            "ChatSettings",
            "AutoDeclineFriendRequests",
            "True"
        );
    } else {
        g_key_file_set_string(
            file,
            "ChatSettings",
            "AutoDeclineFriendRequests",
            "False"
        );
    }

    // Show Notifications
    GtkSwitch *show_notifications_switch = g_object_get_data(object, "show_notifications_switch");
    gboolean show_notifications = gtk_switch_get_active(show_notifications_switch);

    if (show_notifications) {
        g_key_file_set_string(
            file,
            "ChatSettings",
            "ShowNotifications",
            "True"
        );
    } else {
        g_key_file_set_string(
            file,
            "ChatSettings",
            "ShowNotifications",
            "False"
        );
    }

    // Show Global Tab
    GtkSwitch *show_global_tab_switch = g_object_get_data(object, "show_global_tab_switch");
    gboolean show_global_tab = gtk_switch_get_active(show_global_tab_switch);

    if (show_global_tab) {
        g_key_file_set_string(
            file,
            "ChatSettings",
            "ShowGlobalTab",
            "True"
        );
    } else {
        g_key_file_set_string(
            file,
            "ChatSettings",
            "ShowGlobalTab",
            "False"
        );
    }

    // Filter Mature Language
    GtkSwitch *filter_mature_language_switch = g_object_get_data(object, "filter_mature_language_switch");
    gboolean filter_mature_language = gtk_switch_get_active(filter_mature_language_switch);

    if (filter_mature_language) {
        g_key_file_set_string(
            file,
            "ChatSettings",
            "FilterMatureLanguage",
            "True"
        );
    } else {
        g_key_file_set_string(
            file,
            "ChatSettings",
            "FilterMatureLanguage",
            "False"
        );
    }

    g_key_file_save_to_file(
        file,
        path,
        &error
    );

    g_free(path);
    g_key_file_free(file);
}

static void save_video_settings(GtkWidget *stack, GObject *object) {
    // Build a path to the configuration file
    gchar *path = g_build_filename(
        g_get_user_config_dir(),
        "FortniteGame\\Saved\\Config\\WindowsClient\\GameUserSettings.ini",
        NULL
    );

    GKeyFile *file = g_key_file_new();

    g_key_file_load_from_file(
        file,
        path,
        G_KEY_FILE_NONE,
        NULL
    );

    int error = 0;

    // Window Mode
    GtkComboBox *window_mode_box = g_object_get_data(object, "window_mode_box");
    gint window_mode = gtk_combo_box_get_active(window_mode_box);

    g_key_file_set_integer(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "LastConfirmedFullscreenMode",
        window_mode
    );

    g_key_file_set_integer(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "PreferredFullscreenMode",
        window_mode
    );

    g_key_file_set_integer(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "FullscreenMode",
        window_mode
    );

    // Width
    GtkEntry *width_entry = g_object_get_data(object, "width_entry");
    const char *width = gtk_entry_get_text(width_entry);

    if (width && *width) {
        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "ResolutionSizeX",
            width
        );

        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "DesiredScreenWidth",
            width
        );

        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "LastUserConfirmedDesiredScreenWidth",
            width
        );

        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "LastUserConfirmedResolutionSizeX",
            width
        );

        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "LastRecommendedScreenWidth",
            width
        );
    } else {
        error = 1;
    }

    // Height
    GtkEntry *height_entry = g_object_get_data(object, "height_entry");
    const char *height = gtk_entry_get_text(height_entry);

    if (height && *height) {
        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "ResolutionSizeY",
            height
        );

        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "DesiredScreenHeight",
            height
        );

        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "LastUserConfirmedDesiredScreenHeight",
            height
        );

        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "LastUserConfirmedResolutionSizeY",
            height
        );

        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "LastRecommendedScreenHeight",
            height
        );
    } else {
        error = 1;
    }

    // FPS
    GtkEntry *fps_entry = g_object_get_data(object, "fps_entry");
    const char *fps = gtk_entry_get_text(fps_entry);

    if (fps && *fps) {
        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "FrameRateLimit",
            fps
        );
    } else {
        error = 1;
    }

    // View Distance
    GtkComboBox *view_distance_box = g_object_get_data(object, "view_distance_box");
    gint view_distance = gtk_combo_box_get_active(view_distance_box);

    g_key_file_set_integer(
        file,
        "ScalabilityGroups",
        "sg.ViewDistanceQuality",
        view_distance
    );

    // Shadows
    GtkComboBox *shadows_box = g_object_get_data(object, "shadows_box");
    gint shadows = gtk_combo_box_get_active(shadows_box);

    g_key_file_set_integer(
        file,
        "ScalabilityGroups",
        "sg.ShadowQuality",
        shadows
    );

    // Anti-Aliasing
    GtkComboBox *anti_aliasing_box = g_object_get_data(object, "anti_aliasing_box");
    gint anti_aliasing = gtk_combo_box_get_active(anti_aliasing_box);

    g_key_file_set_integer(
        file,
        "ScalabilityGroups",
        "sg.AntiAliasingQuality",
        anti_aliasing
    );

    // Textures
    GtkComboBox *textures_box = g_object_get_data(object, "textures_box");
    gint textures = gtk_combo_box_get_active(textures_box);

    g_key_file_set_integer(
        file,
        "ScalabilityGroups",
        "sg.TextureQuality",
        textures
    );

    // Effects
    GtkComboBox *effects_box = g_object_get_data(object, "effects_box");
    gint effects = gtk_combo_box_get_active(effects_box);

    g_key_file_set_integer(
        file,
        "ScalabilityGroups",
        "sg.EffectsQuality",
        effects
    );

    // Post Processing
    GtkComboBox *post_processing_box = g_object_get_data(object, "post_processing_box");
    gint post_processing = gtk_combo_box_get_active(post_processing_box);

    g_key_file_set_integer(
        file,
        "ScalabilityGroups",
        "sg.PostProcessQuality",
        post_processing
    );

    // Vsync
    GtkSwitch *vsync_switch = g_object_get_data(object, "vsync_switch");
    gboolean vsync = gtk_switch_get_active(vsync_switch);

    if (vsync) {
        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "bUseVSync",
            "True"
        );
    } else {
        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "bUseVSync",
            "False"
        );
    }

    // Motion Blur
    GtkSwitch *motion_blur_switch = g_object_get_data(object, "motion_blur_switch");
    gboolean motion_blur = gtk_switch_get_active(motion_blur_switch);

    if (motion_blur) {
        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "bMotionBlur",
            "True"
        );
    } else {
        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "bMotionBlur",
            "False"
        );
    }

    // Show FPS
    GtkSwitch *show_fps_switch = g_object_get_data(object, "show_fps_switch");
    gboolean show_fps = gtk_switch_get_active(show_fps_switch);

    if (show_fps) {
        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "bShowFPS",
            "True"
        );
    } else {
        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "bShowFPS",
            "False"
        );
    }

    // Multithreaded Rendering
    GtkSwitch *multithreaded_rendering_switch = g_object_get_data(object, "multithreaded_rendering_switch");
    gboolean multithreaded_rendering = gtk_switch_get_active(multithreaded_rendering_switch);

    if (multithreaded_rendering) {
        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "bAllowMultithreadedRendering",
            "True"
        );
    } else {
        g_key_file_set_string(
            file,
            "/Script/FortniteGame.FortGameUserSettings",
            "bAllowMultithreadedRendering",
            "False"
        );
    }

    if (error != 0) {
        dialog("Please fill out every field in the Video settings.");
    } else {
        g_key_file_save_to_file(
            file,
            path,
            NULL
        );

        dialog("Your settings were successfully saved.");
    }
}

static GObject *game_settings(GtkWidget *stack) {
    // Build a path to the configuration file
    gchar *path = g_build_filename(
        g_get_user_config_dir(),
        "FortniteGame\\Saved\\Config\\WindowsClient\\GameUserSettings.ini",
        NULL
    );

    GKeyFile *file = g_key_file_new();
    GError *error = NULL;

    g_key_file_load_from_file(
        file,
        path,
        G_KEY_FILE_NONE,
        &error
    );

    g_free(path);

    /* Get values from the configuration file */

    gint audio_quality = g_key_file_get_integer(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "AudioQualityLevel",
        &error
    );

    gchar *hide_social_name = g_key_file_get_value(
        file,
        "ChatSettings",
        "HideSocialName",
        &error
    );

    gchar *auto_decline_friend_requests = g_key_file_get_value(
        file,
        "ChatSettings",
        "AutoDeclineFriendRequests",
        &error
    );

    gchar *show_notifications = g_key_file_get_value(
        file,
        "ChatSettings",
        "ShowNotifications",
        &error
    );

    gchar *show_global_tab = g_key_file_get_value(
        file,
        "ChatSettings",
        "ShowGlobalTab",
        &error
    );

    gchar *filter_mature_language = g_key_file_get_value(
        file,
        "ChatSettings",
        "FilterMatureLanguage",
        &error
    );

    g_key_file_free(file);

    // Scrolled Window
    GtkWidget *scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_name(scroll_window, "scroll_window");
    gtk_scrolled_window_set_overlay_scrolling(GTK_SCROLLED_WINDOW(scroll_window), FALSE);
    gtk_stack_add_titled(GTK_STACK(stack), GTK_WIDGET(scroll_window), "game", "Game");

    // Viewport
    GtkWidget *viewport = gtk_viewport_new(NULL, NULL);
    gtk_widget_set_name(viewport, "viewport");
    gtk_container_add(GTK_CONTAINER(scroll_window), viewport);

    // Grid
    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_name(grid, "grid");
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 15);
    gtk_container_add(GTK_CONTAINER(viewport), grid);

    // Audio Quality Label
    GtkWidget *audio_quality_label = gtk_label_new("Audio Quality");
    gtk_widget_set_halign(audio_quality_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), audio_quality_label, 0, 0, 1, 1);

    // Audio Quality Combo Box
    GtkWidget *audio_quality_box = gtk_combo_box_text_new();
    gtk_widget_set_name(audio_quality_box, "view_distance_box");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(audio_quality_box), 0, 0, "High");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(audio_quality_box), 1, 0, "Low");
    gtk_combo_box_set_active(GTK_COMBO_BOX(audio_quality_box), audio_quality);
    gtk_grid_attach(GTK_GRID(grid), audio_quality_box, 1, 0, 1, 1);

    // Hide Social Name Label
    GtkWidget *hide_social_name_label = gtk_label_new("Hide Social Name");
    gtk_widget_set_halign(hide_social_name_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), hide_social_name_label, 0, 1, 1, 1);

    // Hide Social Name Switch
    GtkWidget *hide_social_name_switch = gtk_switch_new();
    gtk_grid_attach(GTK_GRID(grid), hide_social_name_switch, 1, 1, 1, 1);
    gboolean hide_social_name_boolean = to_gboolean(hide_social_name);
    gtk_switch_set_active(GTK_SWITCH(hide_social_name_switch), hide_social_name_boolean);

    // Auto Decline Friend Requests Label
    GtkWidget *auto_decline_friend_requests_label = gtk_label_new("Auto Decline Friend Requests");
    gtk_widget_set_halign(auto_decline_friend_requests_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), auto_decline_friend_requests_label, 0, 2, 1, 1);

    // Auto Decline Friend Requests Switch
    GtkWidget *auto_decline_friend_requests_switch = gtk_switch_new();
    gtk_grid_attach(GTK_GRID(grid), auto_decline_friend_requests_switch, 1, 2, 1, 1);
    gboolean auto_decline_friend_requests_boolean = to_gboolean(auto_decline_friend_requests);
    gtk_switch_set_active(GTK_SWITCH(auto_decline_friend_requests_switch), auto_decline_friend_requests_boolean);

    // Show Notifications Label
    GtkWidget *show_notifications_label = gtk_label_new("Show Notifications");
    gtk_widget_set_halign(show_notifications_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), show_notifications_label, 0, 3, 1, 1);

    // Show Notifications Switch
    GtkWidget *show_notifications_switch = gtk_switch_new();
    gtk_grid_attach(GTK_GRID(grid), show_notifications_switch, 1, 3, 1, 1);
    gboolean show_notifications_boolean = to_gboolean(show_notifications);
    gtk_switch_set_active(GTK_SWITCH(show_notifications_switch), show_notifications_boolean);

    // Show Global Tab Label
    GtkWidget *show_global_tab_label = gtk_label_new("Show Global Tab");
    gtk_widget_set_halign(show_global_tab_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), show_global_tab_label, 0, 4, 1, 1);

    // Show Global Tab Switch
    GtkWidget *show_global_tab_switch = gtk_switch_new();
    gtk_grid_attach(GTK_GRID(grid), show_global_tab_switch, 1, 4, 1, 1);
    gboolean show_global_tab_boolean = to_gboolean(show_global_tab);
    gtk_switch_set_active(GTK_SWITCH(show_global_tab_switch), show_global_tab_boolean);

    // Filter Mature Language Label
    GtkWidget *filter_mature_language_label = gtk_label_new("Filter Mature Language");
    gtk_widget_set_halign(filter_mature_language_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), filter_mature_language_label, 0, 5, 1, 1);

    // Filter Mature Language Switch
    GtkWidget *filter_mature_language_switch = gtk_switch_new();
    gtk_grid_attach(GTK_GRID(grid), filter_mature_language_switch, 1, 5, 1, 1);
    gboolean filter_mature_language_boolean = to_gboolean(filter_mature_language);
    gtk_switch_set_active(GTK_SWITCH(filter_mature_language_switch), filter_mature_language_boolean);

    g_object_set_data(G_OBJECT(stack), "audio_quality_box", audio_quality_box);
    g_object_set_data(G_OBJECT(stack), "hide_social_name_switch", hide_social_name_switch);
    g_object_set_data(G_OBJECT(stack), "auto_decline_friend_requests_switch", auto_decline_friend_requests_switch);
    g_object_set_data(G_OBJECT(stack), "show_notifications_switch", show_notifications_switch);
    g_object_set_data(G_OBJECT(stack), "show_global_tab_switch", show_global_tab_switch);
    g_object_set_data(G_OBJECT(stack), "filter_mature_language_switch", filter_mature_language_switch);

    return G_OBJECT(stack);
}

static GObject *video_settings(GtkWidget *stack) {
    // Build a path to the configuration file
    gchar *path = g_build_filename(
        g_get_user_config_dir(),
        "FortniteGame\\Saved\\Config\\WindowsClient\\GameUserSettings.ini",
        NULL
    );

    GKeyFile *file = g_key_file_new();
    GError *error = NULL;

    g_key_file_load_from_file(
        file,
        path,
        G_KEY_FILE_NONE,
        &error
    );

    g_free(path);

    /* Get values from the configuration file */

    gint window_mode = g_key_file_get_integer(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "FullscreenMode",
        &error
    );

    gchar *width = g_key_file_get_value(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "ResolutionSizeX",
        &error
    );

    gchar *height = g_key_file_get_value(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "ResolutionSizeY",
        &error
    );

    gdouble fps = g_key_file_get_double(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "FrameRateLimit",
        &error
    );

    gint view_distance = g_key_file_get_integer(
        file,
        "ScalabilityGroups",
        "sg.ViewDistanceQuality",
        &error
    );

    gint shadows = g_key_file_get_integer(
        file,
        "ScalabilityGroups",
        "sg.ShadowQuality",
        &error
    );

    gint anti_aliasing = g_key_file_get_integer(
        file,
        "ScalabilityGroups",
        "sg.AntiAliasingQuality",
        &error
    );

    gint textures = g_key_file_get_integer(
        file,
        "ScalabilityGroups",
        "sg.TextureQuality",
        &error
    );

    gint effects = g_key_file_get_integer(
        file,
        "ScalabilityGroups",
        "sg.EffectsQuality",
        &error
    );

    gint post_processing = g_key_file_get_integer(
        file,
        "ScalabilityGroups",
        "sg.PostProcessQuality",
        &error
    );

    gchar *vsync = g_key_file_get_value(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "bUseVSync",
        &error
    );

    gchar *motion_blur = g_key_file_get_value(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "bMotionBlur",
        &error
    );

    gchar *show_fps = g_key_file_get_value(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "bShowFPS",
        &error
    );

    gchar *multithreaded_rendering = g_key_file_get_value(
        file,
        "/Script/FortniteGame.FortGameUserSettings",
        "bAllowMultithreadedRendering",
        &error
    );

    g_key_file_free(file);

    // Scrolled Window
    GtkWidget *scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_name(scroll_window, "scroll_window");
    gtk_scrolled_window_set_overlay_scrolling(GTK_SCROLLED_WINDOW(scroll_window), FALSE);
    gtk_stack_add_titled(GTK_STACK(stack), GTK_WIDGET(scroll_window), "video", "Video");

    // Viewport
    GtkWidget *viewport = gtk_viewport_new(NULL, NULL);
    gtk_widget_set_name(viewport, "viewport");
    gtk_container_add(GTK_CONTAINER(scroll_window), viewport);

    // Grid
    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_name(grid, "grid");
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 15);
    gtk_container_add(GTK_CONTAINER(viewport), grid);

    // Window Mode
    GtkWidget *window_mode_label = gtk_label_new("Window Mode");
    gtk_widget_set_halign(window_mode_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), window_mode_label, 0, 0, 1, 1);

    // Window Mode Combo Box
    GtkWidget *window_mode_box = gtk_combo_box_text_new();
    gtk_widget_set_name(window_mode_box, "window_mode_box");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(window_mode_box), 0, 0, "Fullscreen");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(window_mode_box), 1, 0, "Windowed Fullscreen");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(window_mode_box), 2, 0, "Windowed");
    gtk_combo_box_set_active(GTK_COMBO_BOX(window_mode_box), window_mode);
    gtk_grid_attach(GTK_GRID(grid), window_mode_box, 1, 0, 1, 1);

    // Width Label
    GtkWidget *width_label = gtk_label_new("Width");
    gtk_widget_set_halign(width_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), width_label, 0, 1, 1, 1);

    // Width Text Entry
    GtkWidget *width_entry = gtk_entry_new();
    gtk_widget_set_name(width_entry, "width_entry");
    gtk_entry_set_has_frame(GTK_ENTRY(width_entry), FALSE);
    gtk_entry_set_width_chars(GTK_ENTRY(width_entry), 0);
    gtk_entry_set_max_length(GTK_ENTRY(width_entry), 4);
    gtk_entry_set_text(GTK_ENTRY(width_entry), width);
    gtk_entry_set_placeholder_text(GTK_ENTRY(width_entry), "1920");
    gtk_grid_attach(GTK_GRID(grid), width_entry, 1, 1, 1, 1);
    g_signal_connect(G_OBJECT(width_entry), "insert-text", G_CALLBACK(is_numeric), NULL);

    // Height Label
    GtkWidget *height_label = gtk_label_new("Height");
    gtk_widget_set_halign(height_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), height_label, 0, 2, 1, 1);

    // Height Text Entry
    GtkWidget *height_entry = gtk_entry_new();
    gtk_widget_set_name(height_entry, "height_entry");
    gtk_entry_set_has_frame(GTK_ENTRY(height_entry), FALSE);
    gtk_entry_set_width_chars(GTK_ENTRY(height_entry), 0);
    gtk_entry_set_max_length(GTK_ENTRY(height_entry), 4);
    gtk_entry_set_text(GTK_ENTRY(height_entry), height);
    gtk_entry_set_placeholder_text(GTK_ENTRY(height_entry), "1080");
    gtk_grid_attach(GTK_GRID(grid), height_entry, 1, 2, 1, 1);
    g_signal_connect(G_OBJECT(height_entry), "insert-text", G_CALLBACK(is_numeric), NULL);

    // FPS Label
    GtkWidget *fps_label = gtk_label_new("FPS");
    gtk_widget_set_halign(fps_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), fps_label, 0, 3, 1, 1);

    int fps_number = trunc(fps);
    char buffer[4];
    sprintf(buffer, "%d", fps_number);

    // FPS Text Entry
    GtkWidget *fps_entry = gtk_entry_new();
    gtk_widget_set_name(fps_entry, "fps_entry");
    gtk_entry_set_has_frame(GTK_ENTRY(fps_entry), FALSE);
    gtk_entry_set_width_chars(GTK_ENTRY(fps_entry), 0);
    gtk_entry_set_max_length(GTK_ENTRY(fps_entry), 3);
    gtk_entry_set_text(GTK_ENTRY(fps_entry), buffer);
    gtk_entry_set_placeholder_text(GTK_ENTRY(fps_entry), "240");
    gtk_grid_attach(GTK_GRID(grid), fps_entry, 1, 3, 1, 1);
    g_signal_connect(G_OBJECT(fps_entry), "insert-text", G_CALLBACK(is_numeric), NULL);

    // View Distance Label
    GtkWidget *view_distance_label = gtk_label_new("View Distance");
    gtk_widget_set_halign(view_distance_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), view_distance_label, 0, 4, 1, 1);

    // View Distance Combo Box
    GtkWidget *view_distance_box = gtk_combo_box_text_new();
    gtk_widget_set_name(view_distance_box, "view_distance_box");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(view_distance_box), 0, 0, "Near");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(view_distance_box), 1, 0, "Medium");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(view_distance_box), 2, 0, "Far");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(view_distance_box), 3, 0, "Epic");
    gtk_combo_box_set_active(GTK_COMBO_BOX(view_distance_box), view_distance);
    gtk_grid_attach(GTK_GRID(grid), view_distance_box, 1, 4, 1, 1);

    // Shadows Label
    GtkWidget *shadows_label = gtk_label_new("Shadows");
    gtk_widget_set_halign(shadows_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), shadows_label, 0, 5, 1, 1);

    // Shadows Combo Box
    GtkWidget *shadows_box = gtk_combo_box_text_new();
    gtk_widget_set_name(shadows_box, "shadows_box");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(shadows_box), 0, 0, "Off");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(shadows_box), 1, 0, "Medium");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(shadows_box), 2, 0, "High");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(shadows_box), 3, 0, "Epic");
    gtk_combo_box_set_active(GTK_COMBO_BOX(shadows_box), shadows);
    gtk_grid_attach(GTK_GRID(grid), shadows_box, 1, 5, 1, 1);

    // Anti-Aliasing Label
    GtkWidget *anti_aliasing_label = gtk_label_new("Anti-Aliasing");
    gtk_widget_set_halign(anti_aliasing_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), anti_aliasing_label, 0, 6, 1, 1);

    // Anti-Aliasing Combo Box
    GtkWidget *anti_aliasing_box = gtk_combo_box_text_new();
    gtk_widget_set_name(anti_aliasing_box, "anti_aliasing_box");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(anti_aliasing_box), 0, 0, "Off");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(anti_aliasing_box), 1, 0, "Medium");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(anti_aliasing_box), 2, 0, "High");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(anti_aliasing_box), 3, 0, "Epic");
    gtk_combo_box_set_active(GTK_COMBO_BOX(anti_aliasing_box), anti_aliasing);
    gtk_grid_attach(GTK_GRID(grid), anti_aliasing_box, 1, 6, 1, 1);

    // Textures Label
    GtkWidget *textures_label = gtk_label_new("Textures");
    gtk_widget_set_halign(textures_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), textures_label, 0, 7, 1, 1);

    // Textures Combo Box
    GtkWidget *textures_box = gtk_combo_box_text_new();
    gtk_widget_set_name(textures_box, "textures_box");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(textures_box), 0, 0, "Low");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(textures_box), 1, 0, "Medium");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(textures_box), 2, 0, "High");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(textures_box), 3, 0, "Epic");
    gtk_combo_box_set_active(GTK_COMBO_BOX(textures_box), textures);
    gtk_grid_attach(GTK_GRID(grid), textures_box, 1, 7, 1, 1);

    // Effects Label
    GtkWidget *effects_label = gtk_label_new("Effects");
    gtk_widget_set_halign(effects_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), effects_label, 0, 8, 1, 1);

    // Effects Combo Box
    GtkWidget *effects_box = gtk_combo_box_text_new();
    gtk_widget_set_name(effects_box, "effects_box");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(effects_box), 0, 0, "Low");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(effects_box), 1, 0, "Medium");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(effects_box), 2, 0, "High");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(effects_box), 3, 0, "Epic");
    gtk_combo_box_set_active(GTK_COMBO_BOX(effects_box), effects);
    gtk_grid_attach(GTK_GRID(grid), effects_box, 1, 8, 1, 1);

    // Post Processing Label
    GtkWidget *post_processing_label = gtk_label_new("Post Processing");
    gtk_widget_set_halign(post_processing_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), post_processing_label, 0, 9, 1, 1);

    // Post Processing Combo Box
    GtkWidget *post_processing_box = gtk_combo_box_text_new();
    gtk_widget_set_name(post_processing_box, "post_processing_box");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(post_processing_box), 0, 0, "Low");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(post_processing_box), 1, 0, "Medium");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(post_processing_box), 2, 0, "High");
    gtk_combo_box_text_insert(GTK_COMBO_BOX_TEXT(post_processing_box), 3, 0, "Epic");
    gtk_combo_box_set_active(GTK_COMBO_BOX(post_processing_box), post_processing);
    gtk_grid_attach(GTK_GRID(grid), post_processing_box, 1, 9, 1, 1);

    // Vsync Label
    GtkWidget *vsync_label = gtk_label_new("Vsync");
    gtk_widget_set_halign(vsync_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), vsync_label, 0, 10, 1, 1);

    // Vsync Switch
    GtkWidget *vsync_switch = gtk_switch_new();
    gtk_grid_attach(GTK_GRID(grid), vsync_switch, 1, 10, 1, 1);
    gboolean vsync_boolean = to_gboolean(vsync);
    gtk_switch_set_active(GTK_SWITCH(vsync_switch), vsync_boolean);

    // Motion Blur Label
    GtkWidget *motion_blur_label = gtk_label_new("Motion Blur");
    gtk_widget_set_halign(motion_blur_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), motion_blur_label, 0, 11, 1, 1);

    // Motion Blur Switch
    GtkWidget *motion_blur_switch = gtk_switch_new();
    gtk_grid_attach(GTK_GRID(grid), motion_blur_switch, 1, 11, 1, 1);
    gboolean motion_blur_boolean = to_gboolean(motion_blur);
    gtk_switch_set_active(GTK_SWITCH(motion_blur_switch), motion_blur_boolean);

    // Show FPS Label
    GtkWidget *show_fps_label = gtk_label_new("Show FPS");
    gtk_widget_set_halign(show_fps_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), show_fps_label, 0, 12, 1, 1);

    // Show FPS Switch
    GtkWidget *show_fps_switch = gtk_switch_new();
    gtk_grid_attach(GTK_GRID(grid), show_fps_switch, 1, 12, 1, 1);
    gboolean show_fps_boolean = to_gboolean(show_fps);
    gtk_switch_set_active(GTK_SWITCH(show_fps_switch), show_fps_boolean);

    // Allow Multithreaded Rendering Label
    GtkWidget *multithreaded_rendering_label = gtk_label_new("Allow Multithreaded Rendering");
    gtk_widget_set_halign(multithreaded_rendering_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), multithreaded_rendering_label, 0, 13, 1, 1);

    // Allow Multithreaded Rendering Switch
    GtkWidget *multithreaded_rendering_switch = gtk_switch_new();
    gtk_grid_attach(GTK_GRID(grid), multithreaded_rendering_switch, 1, 13, 1, 1);
    gboolean multithreaded_rendering_boolean = to_gboolean(multithreaded_rendering);
    gtk_switch_set_active(GTK_SWITCH(multithreaded_rendering_switch), multithreaded_rendering_boolean);

    g_object_set_data(G_OBJECT(stack), "window_mode_box", window_mode_box);
    g_object_set_data(G_OBJECT(stack), "width_entry", width_entry);
    g_object_set_data(G_OBJECT(stack), "height_entry", height_entry);
    g_object_set_data(G_OBJECT(stack), "fps_entry", fps_entry);
    g_object_set_data(G_OBJECT(stack), "view_distance_box", view_distance_box);
    g_object_set_data(G_OBJECT(stack), "shadows_box", shadows_box);
    g_object_set_data(G_OBJECT(stack), "anti_aliasing_box", anti_aliasing_box);
    g_object_set_data(G_OBJECT(stack), "textures_box", textures_box);
    g_object_set_data(G_OBJECT(stack), "effects_box", effects_box);
    g_object_set_data(G_OBJECT(stack), "post_processing_box", post_processing_box);
    g_object_set_data(G_OBJECT(stack), "vsync_switch", vsync_switch);
    g_object_set_data(G_OBJECT(stack), "motion_blur_switch", motion_blur_switch);
    g_object_set_data(G_OBJECT(stack), "show_fps_switch", show_fps_switch);
    g_object_set_data(G_OBJECT(stack), "multithreaded_rendering_switch", multithreaded_rendering_switch);

    return G_OBJECT(stack);
}

static void activate(GtkApplication *application, gpointer data) {
    // Check for file on Windows
    #ifdef _WIN32
        // Build a path to the configuration file
        gchar *path = g_build_filename(
            g_get_user_config_dir(),
            "FortniteGame\\Saved\\Config\\WindowsClient\\GameUserSettings.ini",
            NULL
        );

        DWORD attribute = GetFileAttributes(path);
        g_free(path);

        if (attribute == INVALID_FILE_ATTRIBUTES) {
            dialog("No configuration file found.");
            gtk_main_quit();
        }
    #endif

    #ifdef __APPLE__
        // Todo: Check for file on Mac OS
    #endif

    // Window
    GtkWidget *window = gtk_application_window_new(application);
    gtk_widget_set_name(window, "window");
    gtk_window_set_title(GTK_WINDOW(window), "Forknife");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 550);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // Fixed Layout
    GtkWidget *fixed = gtk_fixed_new();
    gtk_widget_set_name(fixed, "fixed");
    gtk_container_add(GTK_CONTAINER(window), fixed);

    // Sidebar
    GtkWidget *sidebar = gtk_stack_sidebar_new();
    gtk_widget_set_name(sidebar, "sidebar");
    gtk_fixed_put(GTK_FIXED(fixed), sidebar, 0, 0);
    gtk_widget_set_size_request(sidebar, 150, 550);

    // Stack
    GtkWidget *stack = gtk_stack_new();
    gtk_widget_set_name(stack, "stack");
    gtk_widget_set_size_request(stack, 550, 500);
    gtk_fixed_put(GTK_FIXED(fixed), stack, 150, 0);

    gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), GTK_STACK(stack));

    // Switcher
    GtkWidget *switcher = gtk_stack_switcher_new();
    gtk_widget_set_name(switcher, "switcher");
    gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(switcher), GTK_STACK(stack));

    // Menu
    GObject *video = video_settings(stack);
    GObject *game = game_settings(stack);

    // Action Bar
    GtkWidget *action_bar = gtk_action_bar_new();
    gtk_fixed_put(GTK_FIXED(fixed), action_bar, 150, 500);
    gtk_widget_set_size_request(action_bar, 550, 50);

    // Checkbox
    GtkWidget *readonly = gtk_check_button_new_with_label("Read-only");
    gtk_widget_set_name(readonly, "readonly");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(readonly), TRUE);
    gtk_action_bar_pack_start(GTK_ACTION_BAR(action_bar), readonly);
    g_object_set_data(G_OBJECT(stack), "readonly", readonly);

    // Button
    GtkWidget *apply = gtk_button_new_with_label("Apply");
    gtk_widget_set_name(apply, "apply");
    gtk_action_bar_pack_end(GTK_ACTION_BAR(action_bar), apply);
    g_signal_connect(apply, "clicked", G_CALLBACK(uncheck_read_only), NULL);
    g_signal_connect(apply, "clicked", G_CALLBACK(save_video_settings), video);
    g_signal_connect(apply, "clicked", G_CALLBACK(save_game_settings), game);
    g_signal_connect(apply, "clicked", G_CALLBACK(check_read_only), stack);

    // Load and register resources
    GResource *resource = g_resource_load("resources.gresource", NULL);
    g_resources_register(resource);

    // Set icon
    GError *error = NULL;

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_resource(
        "/com.github.braycarlson.forknife/resources/fk.ico",
        &error
    );

    gtk_window_set_default_icon(pixbuf);

    // Set CSS
    GtkCssProvider *provider = gtk_css_provider_new();

    gtk_css_provider_load_from_resource(
        provider,
        "/com.github.braycarlson.forknife/resources/forknife.css"
    );

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    g_object_unref(pixbuf);
    g_object_unref(provider);
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkApplication *application = gtk_application_new("com.gtk.window", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(application, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(application), argc, argv);

    g_object_unref(application);

    return status;
}
