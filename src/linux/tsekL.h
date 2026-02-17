#ifdef PLATFORM_LINUX

#warning "Including Linux-Specific Headers"

#include <X11/Xutil.h>
#include "../tsekI.h"
#include <GL/glx.h>
#include "../../libs/linux/gladL.h"

#define MAX_LINUX_KEYCODE 255

typedef struct {
  Window window;
  tsekCallbacks callbacks;
  int keymap[MAX_LINUX_KEYCODE + 1];
  bool isOpen;
  bool isCursorVisible;
} tsekLWindow;

typedef struct {
  char* displayName;
  Display* display;

  XContext context;

  Atom WM_DELETE;
  Cursor invisibleCursor;

  double fixedTimeOffset;
  double timeOffset;

  GLXContext glContext;
} tsekLContext;

void tsekL_init(tsekIContext*, tsekIWindow*, tsekIWindowInfo*, bool createGlobalContext, bool console);

void tsekL_fill_context(tsekIContext* context, bool setGlobal);
void tsekL_destroy_context(tsekIContext* context);

void tsekL_create_dummy_window(tsekIWindow* window);
void tsekL_create_window(tsekIWindow* window, tsekIWindowInfo* info);
void tsekL_destroy_window(tsekIWindow* window);

bool tsekL_get_closed_window(tsekIWindow*);
bool tsekL_update_window(tsekIWindow* window);

double tsekL_get_time();
double tsekL_get_fixed_time();

void tsekL_set_time(double time);
void tsekL_allocate_time(double framerate, double start, double end);

bool tsekL_get_cursor_visible(tsekIWindow*);
void tsekL_set_cursor_visible(tsekIWindow*, bool);

void tsekL_swap_buffers(tsekIWindow*);

// messager

void tsekL_get_window_param(tsekIWindow* window, tsekIWindowParam param, void* out);
void tsekL_set_window_param(tsekIWindow* window, tsekIWindowParam param, void* in);

#endif
