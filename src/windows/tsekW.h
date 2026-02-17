#ifdef PLATFORM_WINDOWS

#include "../tsekI.h"
#include <windows.h>

#define WINDOWS_MAX_KEYMAP_SIZE 255

typedef struct {
  HWND handle;
  HDC deviceContext;
  int minMaxDims[4];

  bool keymap[255];
  tsekCallbacks callbacks;
  bool isCursorVisible;
} tsekWWindow;

typedef struct {
  HINSTANCE hInstance;
} tsekWContext;

void tsekW_init(tsekIContext*, tsekIWindow*, tsekIWindowInfo*, bool createGlobalContext, bool console);

void tsekW_fill_context(tsekIContext* context, bool setGlobal);
void tsekW_destroy_context(tsekIContext* context);

void tsekW_create_dummy_window(tsekIWindow* window);
void tsekW_create_window(tsekIWindow* window, tsekIWindowInfo* info);
void tsekW_destroy_window(tsekIWindow* window);

bool tsekW_get_closed_window(tsekIWindow*);
bool tsekW_update_window(tsekIWindow* window);

double tsekW_get_time();
double tsekW_get_fixed_time();

void tsekW_set_time(double time);
void tsekW_allocate_time(double framerate, double start, double end);

bool tsekW_get_cursor_visible(tsekIWindow*);
void tsekW_set_cursor_visible(tsekIWindow*, bool);

void tsekW_swap_buffers(tsekIWindow*);

// messager

void tsekW_get_window_param(tsekIWindow* window, tsekIWindowParam param, void* out);
void tsekW_set_window_param(tsekIWindow* window, tsekIWindowParam param, void* in);

#endif
