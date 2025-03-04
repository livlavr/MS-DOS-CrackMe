#ifndef FUNCITON_DEFINITIONS_H_
#define FUNCITON_DEFINITIONS_H_

typedef struct {
    GtkPicture* picture;
    GdkPixbufAnimation* animation;
    GdkPixbufAnimationIter* iter;
    guint timeout_id;
  } AnimationData;

static void       loadCSS();
static void       runWorkingWindow(GtkApplication* app, gpointer* user_data);
static GtkWidget* createWorkingWindow(GtkApplication* app);
static GtkWidget* createWorkingBox(GtkWidget* working_window);
static GtkWidget* createOverlay(GtkWidget* working_box);
static GtkWidget* createCrackButton(GtkWidget* overlay);
static void       crackFile(GtkButton* crack_button, gpointer* user_data);
static gboolean   closeRequest(GtkWindow* window, gpointer* user_data);
static gboolean   updateLabel(gpointer user_data);
static GtkWidget* createMainLabel(GtkWidget* overlay);
static GtkWidget* createFilePathEntry(GtkWidget* working_box);
static gboolean   updateAnimation(gpointer user_data);
static GtkWidget* createGif(GtkWidget* overlay);

static const char* WINDOW_TITLE   = "CRACK.ME";
static const int   WINDOW_WIDTH   = 900;
static const int   WINDOW_HEIGHT  = 500;
static const char* MAIN_LABEL     = "IT'S TIME TO *ACK";
static const char* FILE_NOT_FOUND = "I CAN'T FIND SUCH FILE😡";
static const char* UNCRACKABLE    = "SORRY, I DON'T WANNA CRACK THIS";
static const char* STEP1          = "";
static const char* STEP2          = "";
static const char* STEP3          = "UHUUUUUUU\n Know you may input any password, you want to";


static const char* BACKGROUND[3] = {"0", "1", "2"};

#endif
