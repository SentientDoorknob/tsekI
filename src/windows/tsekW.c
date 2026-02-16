
#ifdef PLATFORM_WINDOWS

#include "tsekW.h"
#include <stdio.h>

tsekWContext* globalContext;

tsekWWindow* Wget_window(tsekIWindow* window) {
  return (tsekWWindow*)(window->inner);
}

tsekWContext* Wget_context(tsekIContext* context) {
  return (tsekWContext*)(context->inner);
}

LRESULT Wproc_window(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP) {
  return DefWindowProcW(hwnd, msg, wP, lP);
}

HINSTANCE Wget_hInstance() {
  return GetModuleHandle(NULL);
}

void Wregister_windowclass(tsekIWindowInfo* info) {

  WNDCLASSEXW windowClassInfo = {};

  windowClassInfo.cbSize = sizeof(WNDCLASSEXW);
  windowClassInfo.style = CS_HREDRAW | CS_VREDRAW;
  windowClassInfo.cbClsExtra = 0;
  windowClassInfo.cbWndExtra = 0;
  windowClassInfo.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  windowClassInfo.hCursor = LoadCursor(NULL, IDC_ARROW);
  windowClassInfo.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  windowClassInfo.lpszMenuName = NULL;
  windowClassInfo.lpszClassName = info->wndClassName;
  windowClassInfo.hInstance = globalContext->hInstance;
  windowClassInfo.lpfnWndProc = Wproc_window;

  if (!RegisterClassExW(&windowClassInfo)) {
    fprintf(stderr, "Failed to register WNDCLASS\n");
  }

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
    .style = WS_OVERLAPPEDWINDOW,
    .extendedStyle = 0,
    .pixelFormat = defaultPixelFormat,
    .minMaxDims = {0, 0, 0, 0}
  };

  if (!info) {
    info = &defaultInfo;
  }

  Wregister_windowclass(info);

  tsekW_create_window(window, info);
}

void tsekW_fill_context(tsekIContext* context, bool setGlobal) {
  tsekWContext* wcontext = Wget_context(context);
  wcontext->hInstance = Wget_hInstance();

  if (setGlobal) {
    globalContext = wcontext;
  }
}

void tsekW_destroy_context(tsekIContext* context) {
}


void tsekW_create_dummy_window(tsekIWindow* window) {
}

void tsekW_create_window(tsekIWindow* window, tsekIWindowInfo* info) {
  window->inner = malloc(sizeof(tsekWWindow));
  tsekWWindow* wwindow = Wget_window(window);

  wwindow->handle = CreateWindowExW(
      info->extendedStyle,
      info->wndClassName,
      info->title,
      info->style,
      info->x, info->y,
      info->width, info->height,
      NULL, NULL,
      globalContext->hInstance,
      window
      );

  wwindow->deviceContext = GetDC(wwindow->handle);

  memcpy(wwindow->minMaxDims, info->minMaxDims, sizeof(info->minMaxDims));

  if (!wwindow->handle) {
    DWORD err = GetLastError();
    fprintf(stderr, "Failed to create window: %lu \n", err);
  }

  ShowWindow(wwindow->handle, SW_SHOW);
}

void tsekW_destroy_window(tsekIWindow* window) {
  DestroyWindow(Wget_window(window)->handle);
  free(window->inner);
  free(window);
}

bool tsekW_get_closed_window(tsekIWindow* window) {
  return (!IsWindow(Wget_window(window)->handle));
}

bool tsekW_update_window(tsekIWindow* window) {
  MSG msg;
  while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
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
