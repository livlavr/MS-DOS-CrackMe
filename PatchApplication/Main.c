#include <stdio.h>
#include <gtk/gtk.h>
#include <gst/gst.h>
#include <time.h>
#include <string.h>

#include "FunctionDefinitions.h"
#include "Widgets.h"
#include "Patch.h"

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

    working_window  = createWorkingWindow(app);
    overlay         = createOverlay      (working_window);
    picture         = createGif          (overlay);
    working_box     = createWorkingBox   (overlay);
    main_label      = createMainLabel    (overlay);
    file_path_entry = createFilePathEntry(working_box);
    crack_button    = createCrackButton  (working_box);


    g_signal_connect(crack_button, "clicked", G_CALLBACK(crackFile), overlay);

    gtk_window_present(GTK_WINDOW(working_window));
}

static GtkWidget* createGif(GtkWidget* overlay) {
    GtkWidget* picture = gtk_picture_new();
    gtk_widget_set_hexpand(picture, TRUE);

    GError* error = NULL;

    GdkPixbufAnimation* animation = gdk_pixbuf_animation_new_from_file("img/background.gif", &error);
    GdkPixbufAnimationIter* iter  = gdk_pixbuf_animation_get_iter(animation, NULL);

    AnimationData* data = g_new0(AnimationData, 1);
    data->picture 	    = GTK_PICTURE(picture);
    data->animation     = animation;
    data->iter 		    = iter;
    data->timeout_id    = g_timeout_add(100, updateAnimation, data);

    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), picture);
    gtk_picture_set_content_fit(GTK_PICTURE(picture), GTK_CONTENT_FIT_COVER);

    return picture;
}

static void loadCSS() {
    GtkCssProvider* provider = gtk_css_provider_new();

    gtk_css_provider_load_from_path(provider, "Style.css");

    GdkDisplay* display = gdk_display_get_default();

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

    gtk_window_set_child(GTK_WINDOW(working_window), overlay);

    return overlay;
}

static GtkWidget* createWorkingBox(GtkWidget* overlay) {
    GtkWidget* working_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);

    gtk_widget_set_name(working_box, "working_box");

    gtk_widget_set_margin_top(working_box, 200);

    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), working_box);

    return working_box;
}

static GtkWidget* createCrackButton(GtkWidget* working_box) {
    GtkWidget* crack_button = gtk_button_new_with_label("CRACK💀");

    gtk_widget_set_name(crack_button, "crack_button");

    gtk_box_append(GTK_BOX(working_box), crack_button);

    return crack_button;
}

static GtkWidget* createMainLabel(GtkWidget* overlay) {
    GtkWidget* main_label = gtk_label_new(NULL);

    gtk_widget_set_name(main_label, "main_label");

    gtk_label_set_label(GTK_LABEL(main_label), MAIN_LABEL);

    gtk_label_set_justify(GTK_LABEL(main_label), GTK_JUSTIFY_CENTER);

    gtk_widget_set_margin_bottom(GTK_WIDGET(main_label), 200);

    gtk_label_set_wrap(GTK_LABEL(main_label), TRUE);

    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), main_label);

    return main_label;
}
static GtkWidget* createFilePathEntry(GtkWidget* working_box) {
    GtkWidget* file_path_entry = gtk_entry_new();

    gtk_widget_set_name(file_path_entry, "file_path_entry");

    gtk_widget_set_margin_top(GTK_WIDGET(file_path_entry), 100);

    gtk_entry_set_placeholder_text(GTK_ENTRY(file_path_entry), "Input Path for File to Hack");

    gtk_box_append(GTK_BOX(working_box), file_path_entry);

    return file_path_entry;
}

static void crackFile(GtkButton* crack_button, gpointer* user_data) {
    GtkOverlay* overlay = GTK_OVERLAY(user_data);
    const char* file_path = gtk_editable_get_text(GTK_EDITABLE(file_path_entry));

    FILE* file_to_crack = fopen(file_path, "r+b");
    if (!file_to_crack){
        gtk_label_set_label(GTK_LABEL(main_label), FILE_NOT_FOUND);
        fclose(file_to_crack);
        return;
    }

    int return_result = binaryPatch(file_to_crack, file_path);

    if (return_result == WRONG_TYPE_OF_FILE){
        gtk_label_set_label(GTK_LABEL(main_label), UNCRACKABLE);

        return;
    }

    gtk_label_set_label(GTK_LABEL(main_label), STEP3);
    // g_timeout_add_seconds(3, NULL, NULL); //TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    return;
}

static gboolean closeRequest(GtkWindow* window, gpointer* user_data) {
    GstElement* pipeline = GST_ELEMENT(user_data);

    gst_element_set_state(pipeline, GST_STATE_NULL);

    gst_object_unref(pipeline);

    return FALSE;
}

static gboolean updateLabel(gpointer user_data) {
    GtkWidget* label = (GtkWidget*)user_data;

    gtk_label_set_label(GTK_LABEL(label), "SUCCESS! Enjoy your superiority");

    return FALSE;
}
