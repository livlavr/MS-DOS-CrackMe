#include <gtk/gtk.h>
#include <gst/gst.h>
#include <time.h>
#include <string.h>

#include "FunctionDefinitions.h"
// #include "widgets.h"

int main(int argc, char** argv) {
    gst_init(&argc, &argv);

    GtkApplication* app = gtk_application_new("LIVLAVR.CRACK", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(runWorkingWindow), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    gst_deinit();

    return status;
}

static gboolean updateAnimation(gpointer user_data) {
    AnimationData* data = (AnimationData*) user_data;

    gdk_pixbuf_animation_iter_advance(data->iter, NULL);

    GdkPixbuf* frame = gdk_pixbuf_animation_iter_get_pixbuf(data->iter);

    GdkPaintable* paintable = GDK_PAINTABLE(gdk_texture_new_for_pixbuf(frame));

    gtk_picture_set_paintable(data->picture, paintable);

    return TRUE;
  }

static void runWorkingWindow(GtkApplication* app, gpointer* user_data) {
    loadCSS();

    GtkWidget* working_window       = createWorkingWindow(app);

    // GstElement* pipeline            = createAudioPipelineAndPlay();

    GtkWidget* overlay              = createOverlay(working_window);

    GtkWidget* picture              = createGif(overlay);

    GtkWidget* working_box          = createWorkingBox(overlay);

    // set gif as box child (after overlay for gif to be on the right side)

    GtkWidget* explanatory_label    =  createExplanatoryLabel(working_box);

    GtkWidget* file_path_entry      =  createFilePathEntry(working_box);

    GtkWidget* crack_button         =  createCrackButton(working_box);

    // create clicked signal to call crackFile when crack button is clicked

    g_signal_connect(crack_button, "clicked", G_CALLBACK(crackFile), overlay);

    // create close-request signal to call closeRequest when working window is closed
    // g_signal_connect(working_window, "close-request", G_CALLBACK (closeRequest), pipeline);

    gtk_window_present(GTK_WINDOW(working_window));
}

static GtkWidget* createGif(GtkWidget* overlay) {
//     GtkWidget *picture = gtk_picture_new();
//     gtk_widget_set_hexpand(picture, TRUE);
//
//     GError *error = NULL;
//
//     srand(time(NULL));
//     int background_gif_index      = rand() % 3;
//     char buffer[50] = "./img/";
//     char format[]   = ".gif";
//     strncat(buffer, BACKGROUND[background_gif_index], 10);
//     fprintf(stderr, "%s - 1\n", buffer);
//     strncat(buffer, format, 10);
//     fprintf(stderr, "%s - 1*\n", buffer);
//     // char buffer        [30]       = "img/background";
//     // char format        [30]       = ".gif";
//     // strcat(buffer, format);
//     // char buffer[30]       = "img/background0.gif\0";
//     GdkPixbufAnimation *animation = gdk_pixbuf_animation_new_from_file("img/0.gif", &error);
//     GdkPixbufAnimationIter *iter  = gdk_pixbuf_animation_get_iter(animation, NULL);
//
//     AnimationData *data = g_new0(AnimationData, 1);
//     data->picture 	    = GTK_PICTURE(picture);
//     data->animation     = animation;
//     data->iter 		    = iter;
//     data->timeout_id    = g_timeout_add(100, updateAnimation, data);
//
//     gtk_overlay_add_overlay(GTK_OVERLAY(overlay), picture);
//     gtk_picture_set_content_fit(GTK_PICTURE(picture), GTK_CONTENT_FIT_COVER);
//
//     return picture;
    GtkWidget* picture = gtk_picture_new();
    gtk_widget_set_hexpand(picture, TRUE);

    GError* error = NULL;
    GdkPixbufAnimation* animation = gdk_pixbuf_animation_new_from_file("/Users/ivanlavruschev/Education/Ded32/2Semester/MS-DOS-CrackMe/PatchApplication/img/1.gif", &error);

    GdkPixbufAnimationIter* iter = gdk_pixbuf_animation_get_iter(animation, NULL);

    AnimationData* data = g_new0(AnimationData, 1);

    data->picture   = GTK_PICTURE(picture);
    data->animation = animation;
    data->iter      = iter;

    g_timeout_add(100, updateAnimation, data);

    gtk_widget_set_halign(picture, GTK_ALIGN_END);
    gtk_widget_set_valign(picture, GTK_ALIGN_FILL);

    return picture;
}

static void loadCSS() {
    // creating css provider
    GtkCssProvider* provider = gtk_css_provider_new();

    // parse style.css
    gtk_css_provider_load_from_path(provider, "Style.css");

    // get default display
    GdkDisplay* display = gdk_display_get_default();

    // add global style provider (aka style.css) to display
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

static GtkWidget* createWorkingWindow(GtkApplication* app) {
    GtkWidget* working_window = gtk_application_window_new(app);

    gtk_window_set_title        (GTK_WINDOW (working_window), WINDOW_TITLE);
    gtk_window_set_default_size (GTK_WINDOW (working_window), WINDOW_WIDTH, WINDOW_HEIGHT);
    gtk_window_set_resizable    (GTK_WINDOW (working_window), FALSE);

    return working_window;
}

static GtkWidget* createOverlay(GtkWidget* working_window) {

    GtkWidget* overlay = gtk_overlay_new();

    gtk_widget_set_name(overlay, "overlay");

    // set overlay as working_box child
    gtk_window_set_child(GTK_WINDOW(working_window), overlay);

    return overlay;
}

static GtkWidget* createWorkingBox(GtkWidget* overlay) {
    // create working box
    GtkWidget* working_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20); //TODO

    // set name for style.css
    gtk_widget_set_name(working_box, "working_box");

    gtk_widget_set_margin_top(working_box, 120);

    // set working_box as working_window child
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), working_box);

    return working_box;
}

static GtkWidget* createCrackButton(GtkWidget* working_box) {
    // create crack button
    GtkWidget* crack_button = gtk_button_new_with_label("CRACK💀");

    // set name for style.css
    gtk_widget_set_name(crack_button, "crack_button");

    // set crack_button as overlay child (third child)
    gtk_box_append(GTK_BOX(working_box), crack_button);
    // gtk_overlay_add_overlay(GTK_OVERLAY(overlay), crack_button);

    return crack_button;
}

static GtkWidget* createExplanatoryLabel(GtkWidget* working_box) {
    // create explanatory label
    GtkWidget* explanatory_label = gtk_label_new(NULL);

    // set name for style.css
    gtk_widget_set_name(explanatory_label, "explanatory_label");

    // set label text
    gtk_label_set_label(GTK_LABEL(explanatory_label), MAIN_LABEL);

    gtk_label_set_justify(GTK_LABEL(explanatory_label), GTK_JUSTIFY_CENTER);

    gtk_widget_set_margin_bottom(GTK_WIDGET(explanatory_label), 20);

    // enable line break
    gtk_label_set_wrap(GTK_LABEL(explanatory_label), TRUE);

    // set explanatory_label as left_side_overlay child (first child)
    gtk_box_append(GTK_BOX(working_box), explanatory_label);

    return explanatory_label;
}
static GtkWidget* createFilePathEntry(GtkWidget* working_box) {
    // create file path entry
    GtkWidget* file_path_entry = gtk_entry_new();

    // set name for style.css
    gtk_widget_set_name(file_path_entry, "file_path_entry");

    gtk_widget_set_margin_top(GTK_WIDGET(file_path_entry), 100);

    // set placeholder text
    gtk_entry_set_placeholder_text(GTK_ENTRY(file_path_entry), "Input Path for File to Hack");

    // set file_path_entry as left_side_overlay child (second child)
    gtk_box_append(GTK_BOX(working_box), file_path_entry);

    return file_path_entry;
}

static void crackFile(GtkButton* crack_button, gpointer* user_data) {
//     // set user_data to GtkOverlay*
//     GtkOverlay* overlay = GTK_OVERLAY(user_data);
//
//     // get file_path_entry as overlay 2nd child
//     GtkWidget* explanatory_label = gtk_overlay_get_child(overlay);
//     GtkWidget* file_path_entry = gtk_widget_get_next_sibling(explanatory_label);
//
//     // get text in file_path_entry
//     GtkEntryBuffer* file_path_buffer = gtk_entry_get_buffer(GTK_ENTRY(file_path_entry));
//     const char* file_path = gtk_entry_buffer_get_text(file_path_buffer);
//
//     // check if file exists
//     FILE* file_to_crack = fopen(file_path, "r+b");
//     if (!file_to_crack){
//         gtk_label_set_label(GTK_LABEL(explanatory_label), FILE_NOT_FOUND_MSG);
//
//         fclose(file_to_crack);
//
//         return;
//     }
//
//     // crack file
//     int return_result = binaryPatch(file_to_crack, file_path);
//
//     if (return_result == NO_CRACK_TO_THIS_FILE){
//         // set NO_CRACK message in explanatory_label
//         gtk_label_set_label(GTK_LABEL(explanatory_label), NO_CRACK);
//
//         return;
//     }
//
//     // set WORK_IN_PROGRESS_MSG message in explanatory_label
//     gtk_label_set_label(GTK_LABEL(explanatory_label), WORK_IN_PROGRESS_MSG);
//
//     // call updateLabel after WORK_TIME_DELAY s
//     g_timeout_add_seconds(WORK_TIME_DELAY, updateLabel, explanatory_label);
    fprintf(stdout, "CRACKED...\n");
}

static gboolean closeRequest(GtkWindow *window, gpointer* user_data) {
    // set user data to GstElement*
    GstElement* pipeline = GST_ELEMENT(user_data);

    // stop playing audio
    gst_element_set_state(pipeline, GST_STATE_NULL);

    // clear pipeline
    gst_object_unref(pipeline);

    return FALSE;
}

static gboolean updateLabel(gpointer user_data) {
    // set user_data to GtkWidget*
    GtkWidget* label = (GtkWidget*)user_data;

    // set label's text
    gtk_label_set_label(GTK_LABEL(label), "SUCCESS! Enjoy your superiority");

    return FALSE;
}
