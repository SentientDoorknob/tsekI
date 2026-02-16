
#ifdef PLATFORM_WINDOWS

#include "tsekW.h"
#include <stdio.h>
#include <windows.h>

tsekWWindow* Wget_window(tsekIWindow* window) {
  return (tsekWWindow*)(window->inner);
}

tsekWContext* Wget_context(tsekIContext* context) {
  return (tsekWContext*)(context->inner);
}

void tsekW_init(tsekIContext* context, tsekIWindow* window, tsekIWindowInfo* info, bool createGlobalContext, bool console) {

  if (console) {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    SetConsoleOutputCP(CP_UTF8);
  }

  tsekW_fill_context(context, createGlobalContext);

  tsekPixelFormat defaultPixelFormat = {
    .r_bits = 8, .g_bits = 8, .b_bits = 8, .a_bits = 8,
    .depth_bits = 24, .stencil_bits = 2, .samples = 4 };

  tsekIWindowInfo defaultInfo = {
    .title = L"Default Title",
    .width = 500, .height = 500,
    .x = 100, .y = 100,
    .borderWidth = 0,
    .classId = 0,
    .wndClassName = L"Default Class Name",
    .style = 0,
    .extendedStyle = 0,
    .pixelFormat = defaultPixelFormat,
    .minMaxDims = {0, 0, 0, 0}
  };

  if (!info) {
    info = &defaultInfo;
  }

  tsekIWindow dummyWindow;
  tsekW_create_dummy_window(&dummyWindow);

  tsekW_create_window(window, info);
}

void tsekW_fill_context(tsekIContext* context, bool setGlobal) {
}

void tsekW_destroy_context(tsekIContext* context) {
}


void tsekW_create_dummy_window(tsekIWindow* window) {
}

void tsekW_create_window(tsekIWindow* window, tsekIWindowInfo* info) {
}

void tsekW_destroy_window(tsekIWindow* window) {
}


bool tsekW_get_closed_window(tsekIWindow* window) {
  return true;
}

bool tsekW_update_window(tsekIWindow* window) {
  return true;
}


double tsekW_get_time() {
  return 0;
}

double tsekW_get_fixed_time() {
  return 0;
}


void tsekW_set_time(double time) {
}

void tsekW_allocate_time(double framerate, double start, double end) {
}


bool tsekW_get_cursor_visible(tsekIWindow* window) {
  return true;
}

void tsekW_set_cursor_visible(tsekIWindow* window, bool visible) {
}


void tsekW_swap_buffers(tsekIWindow* window) {
}


// messager

void tsekW_get_window_param(tsekIWindow* window, tsekIWindowParam param, void* out) {
}

void tsekW_set_window_param(tsekIWindow* window, tsekIWindowParam param, void* in) {
}

#endif
