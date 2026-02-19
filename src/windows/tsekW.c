
#ifdef PLATFORM_WINDOWS

#include "tsekW.h"
#include <stdio.h>
#include <GL/wglext.h>
#include <GL/wgl.h>

tsekWContext* globalContext;
int keycode_map[256];

typedef HGLRC (WINAPI *wglCreateContextAttribsARB_t)(
    HDC,
    HGLRC,
    const int*
);

typedef BOOL (WINAPI *wglChoosePixelFormatARB_t)(
    HDC, int*, float*, UINT, int*, UINT*
    );

wglCreateContextAttribsARB_t Wcreate_gl_context;
wglChoosePixelFormatARB_t Wchoose_pixel_format;

void init_windows_keycode_map() {
  for (int i = 0; i <= 255; i++) {
    keycode_map[i] = TSEK_NONE;
  }

  keycode_map['A'] = TSEK_A;
  keycode_map['B'] = TSEK_B;
  keycode_map['C'] = TSEK_C;
  keycode_map['D'] = TSEK_D;
  keycode_map['E'] = TSEK_E;
  keycode_map['F'] = TSEK_F;
  keycode_map['G'] = TSEK_G;
  keycode_map['H'] = TSEK_H;
  keycode_map['I'] = TSEK_I;
  keycode_map['J'] = TSEK_J;
  keycode_map['K'] = TSEK_K;
  keycode_map['L'] = TSEK_L;
  keycode_map['M'] = TSEK_M;
  keycode_map['N'] = TSEK_N;
  keycode_map['O'] = TSEK_O;
  keycode_map['P'] = TSEK_P;
  keycode_map['Q'] = TSEK_Q;
  keycode_map['R'] = TSEK_R;
  keycode_map['S'] = TSEK_S;
  keycode_map['T'] = TSEK_T;
  keycode_map['U'] = TSEK_U;
  keycode_map['V'] = TSEK_V;
  keycode_map['W'] = TSEK_W;
  keycode_map['X'] = TSEK_X;
  keycode_map['Y'] = TSEK_Y;
  keycode_map['Z'] = TSEK_Z; 

  // Numbers (top row)
  keycode_map['0'] = TSEK_0;
  keycode_map['1'] = TSEK_1;
  keycode_map['2'] = TSEK_2;
  keycode_map['3'] = TSEK_3;
  keycode_map['4'] = TSEK_4;
  keycode_map['5'] = TSEK_5;
  keycode_map['6'] = TSEK_6;
  keycode_map['7'] = TSEK_7;
  keycode_map['8'] = TSEK_8;
  keycode_map['9'] = TSEK_9;

  // Symbols (OEM keys – layout dependent!)
  keycode_map[VK_OEM_MINUS]      = TSEK_MINUS;
  keycode_map[VK_OEM_PLUS]       = TSEK_EQUAL;
  keycode_map[VK_OEM_4]          = TSEK_LEFTBRACKET;   // [
  keycode_map[VK_OEM_6]          = TSEK_RIGHTBRACKET;  // ]
  keycode_map[VK_OEM_5]          = TSEK_BACKSLASH;     // \
  keycode_map[VK_OEM_1]          = TSEK_SEMICOLON;     // ;
  keycode_map[VK_OEM_7]          = TSEK_APOSTROPHE;    // '
  keycode_map[VK_OEM_3]          = TSEK_GRAVE;         // `
  keycode_map[VK_OEM_COMMA]      = TSEK_COMMA;         // ,
  keycode_map[VK_OEM_PERIOD]     = TSEK_PERIOD;        // .
  keycode_map[VK_OEM_2]          = TSEK_SLASH;         // /

  // Control keys
  keycode_map[VK_RETURN]   = TSEK_ENTER;
  keycode_map[VK_ESCAPE]   = TSEK_ESCAPE;
  keycode_map[VK_BACK]     = TSEK_BACKSPACE;
  keycode_map[VK_TAB]      = TSEK_TAB;
  keycode_map[VK_SPACE]    = TSEK_SPACE;
  keycode_map[VK_CAPITAL]  = TSEK_CAPSLOCK;

  // Function keys
  keycode_map[VK_F1]  = TSEK_F1;
  keycode_map[VK_F2]  = TSEK_F2;
  keycode_map[VK_F3]  = TSEK_F3;
  keycode_map[VK_F4]  = TSEK_F4;
  keycode_map[VK_F5]  = TSEK_F5;
  keycode_map[VK_F6]  = TSEK_F6;
  keycode_map[VK_F7]  = TSEK_F7;
  keycode_map[VK_F8]  = TSEK_F8;
  keycode_map[VK_F9]  = TSEK_F9;
  keycode_map[VK_F10] = TSEK_F10;
  keycode_map[VK_F11] = TSEK_F11;
  keycode_map[VK_F12] = TSEK_F12;

  // Arrow keys
  keycode_map[VK_LEFT]  = TSEK_LEFT;
  keycode_map[VK_UP]    = TSEK_UP;
  keycode_map[VK_RIGHT] = TSEK_RIGHT;
  keycode_map[VK_DOWN]  = TSEK_DOWN;

  // Modifier keys
  keycode_map[VK_LSHIFT]   = TSEK_LEFTSHIFT;
  keycode_map[VK_RSHIFT]   = TSEK_RIGHTSHIFT;
  keycode_map[VK_LCONTROL] = TSEK_LEFTCTRL;
  keycode_map[VK_RCONTROL] = TSEK_RIGHTCTRL;
  keycode_map[VK_LMENU]    = TSEK_LEFTALT;
  keycode_map[VK_RMENU]    = TSEK_RIGHTALT;
  keycode_map[VK_LWIN]     = TSEK_LEFTMETA;
  keycode_map[VK_RWIN]     = TSEK_RIGHTMETA;

  // Numpad
  keycode_map[VK_NUMPAD0] = TSEK_NUMPAD0;
  keycode_map[VK_NUMPAD1] = TSEK_NUMPAD1;
  keycode_map[VK_NUMPAD2] = TSEK_NUMPAD2;
  keycode_map[VK_NUMPAD3] = TSEK_NUMPAD3;
  keycode_map[VK_NUMPAD4] = TSEK_NUMPAD4;
  keycode_map[VK_NUMPAD5] = TSEK_NUMPAD5;
  keycode_map[VK_NUMPAD6] = TSEK_NUMPAD6;
  keycode_map[VK_NUMPAD7] = TSEK_NUMPAD7;
  keycode_map[VK_NUMPAD8] = TSEK_NUMPAD8;
  keycode_map[VK_NUMPAD9] = TSEK_NUMPAD9;

  keycode_map[VK_DECIMAL]  = TSEK_NUMPADDECIMAL;
  keycode_map[VK_RETURN]   = TSEK_NUMPADENTER;  // Distinguish via extended flag if needed
  keycode_map[VK_ADD]      = TSEK_NUMPADADD;
  keycode_map[VK_SUBTRACT] = TSEK_NUMPADSUBTRACT;
  keycode_map[VK_MULTIPLY] = TSEK_NUMPADMULTIPLY;
  keycode_map[VK_DIVIDE]   = TSEK_NUMPADDIVIDE;

  // Editing/navigation
  keycode_map[VK_INSERT]   = TSEK_INSERT;
  keycode_map[VK_DELETE]   = TSEK_DELETE;
  keycode_map[VK_HOME]     = TSEK_HOME;
  keycode_map[VK_END]      = TSEK_END;
  keycode_map[VK_PRIOR]    = TSEK_PAGEUP;
  keycode_map[VK_NEXT]     = TSEK_PAGEDOWN;

  // Other
  keycode_map[VK_SNAPSHOT] = TSEK_PRINTSCREEN;
  keycode_map[VK_SCROLL]   = TSEK_SCROLLLOCK;
  keycode_map[VK_PAUSE]    = TSEK_PAUSE;
}

tsekWWindow* Wget_window(tsekIWindow* window) {
  return (tsekWWindow*)(window->inner);
}

tsekWContext* Wget_context(tsekIContext* context) {
  return (tsekWContext*)(context->inner);
}

tsekIWindow* Wproc_get_window(HWND hwnd, UINT msg, WPARAM wp, LPARAM lP) {
  LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
  tsekIWindow* window = (tsekIWindow*)ptr;
  return window;
}

tsekIWindow* Wproc_create(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP) {
  CREATESTRUCT* pCreate = (CREATESTRUCT*)(lP);
  tsekIWindow* window = (tsekIWindow*)(pCreate->lpCreateParams);
  SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)window);
  return window;
}

tsekKeyCode Wget_keycode(WPARAM wP, LPARAM lP) {
  switch (wP)
  {
    case VK_SHIFT:
      {
        UINT scancode = (lP >> 16) & 0xFF;
        UINT vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);

        if (vk == VK_LSHIFT)
          return TSEK_LEFTSHIFT;
        else
          return TSEK_RIGHTSHIFT;
      }

    case VK_CONTROL:
      {
        bool extended = (lP & (1 << 24)) != 0;
        return extended ? TSEK_RIGHTCTRL : TSEK_LEFTCTRL;
      }

    case VK_MENU:
      {
        bool extended = (lP & (1 << 24)) != 0;
        return extended ? TSEK_RIGHTALT : TSEK_LEFTALT;
      }

    case VK_LWIN: return TSEK_LEFTMETA;
    case VK_RWIN: return TSEK_RIGHTMETA;
  }

  return keycode_map[wP];
}

void Wproc_keydown(tsekIWindow* window, WPARAM wP, LPARAM lP) {
  
  tsekWWindow* wwindow = Wget_window(window);

  tsekKeyCode key = Wget_keycode(wP, lP);

  if (wwindow->callbacks.keydown && wwindow->keymap[key] == false) {
    wwindow->callbacks.keydown(window, key);
  }

  if (wwindow->callbacks.keytype) {
    wwindow->callbacks.keytype(window, key);
  }

  wwindow->keymap[key] = true;
}

void Wproc_keyup(tsekIWindow* window, WPARAM wP, LPARAM lP) {

  tsekWWindow* wwindow = Wget_window(window);

  tsekKeyCode key = Wget_keycode(wP, lP);

  if (wwindow->callbacks.keyup) {
    wwindow->callbacks.keyup(window, key);
  }

  wwindow->keymap[key] = false;
}

void Wproc_mbdown(tsekIWindow* window, tsekKeyCode code) {

  tsekWWindow* wwindow = Wget_window(window);

  if (wwindow->callbacks.mbdown) {
    wwindow->callbacks.mbdown(window, code);
  }

  wwindow->keymap[code] = true;
}

void Wproc_mbup(tsekIWindow* window, tsekKeyCode code) {

  tsekWWindow* wwindow = Wget_window(window);

  if (wwindow->callbacks.mbup) {
    wwindow->callbacks.mbup(window, code);
  }

  wwindow->keymap[code] = false;
}

void Wproc_resize(tsekIWindow* window, WPARAM wP, LPARAM lP) {

  tsekWWindow* wwindow = Wget_window(window);

  if (wwindow->callbacks.size) {
    wwindow->callbacks.size(window, LOWORD(lP), HIWORD(lP));
  }

  if (wwindow->callbacks.tsegsize) {
    wwindow->callbacks.tsegsize(window, LOWORD(lP), HIWORD(lP));
  }
}

LRESULT CALLBACK Wproc_window(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP) {

  tsekIWindow* window;

  if (msg == WM_CREATE) { 
    window = Wproc_create(hwnd, msg, wP, lP);
  } else {
    window = Wproc_get_window(hwnd, msg, wP, lP);
  }

  switch (msg) {
    case (WM_SIZE): {
      Wproc_resize(window, wP, lP);
      break;
    } case (WM_MOVE): {
      break;
    } case (WM_KEYDOWN): {
      Wproc_keydown(window, wP, lP);
      break;
    } case (WM_KEYUP): {
      Wproc_keyup(window, wP, lP);
      break;
    } 

    case (WM_LBUTTONDOWN): {
      Wproc_mbdown(window, TSEK_MBL);
      break;
    } case (WM_LBUTTONUP): {
      Wproc_mbup(window, TSEK_MBL);
      break;
    }

    case (WM_RBUTTONDOWN): {
      Wproc_mbdown(window, TSEK_MBR);
      break;
    } case (WM_RBUTTONUP): {
      Wproc_mbup(window, TSEK_MBR);
      break;
    }

    case (WM_MBUTTONDOWN): {
      Wproc_mbdown(window, TSEK_MBM);
      break;
    } case (WM_MBUTTONUP): {
      Wproc_mbup(window, TSEK_MBM);
      break;
    }
  }

  return DefWindowProcW(hwnd, msg, wP, lP);
}



HINSTANCE Wget_hInstance() {
  return GetModuleHandle(NULL);
}

void Wregister_windowclass(tsekIWindowInfo* info) {

  WNDCLASSEXW windowClassInfo = {};

  printf("Preparing Windowclass with name: %s\n", info->wndClassName);

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

  printf("Registering Window Class");

  if (!RegisterClassExW(&windowClassInfo)) {
    fprintf(stderr, "Failed to register WNDCLASS\n");
  }

}

void Wload_gl() {
  printf("About to open window... \n");

  tsekIWindow* dummyWindow = malloc(sizeof(tsekIWindow));
  tsekW_create_dummy_window(dummyWindow);
  tsekWWindow* wwindow = Wget_window(dummyWindow);

  printf("Dummy window opened\n");

  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0,0,0,0,0,0,
    0,0,
    0,0,0,0,
    0,
    24, 8, 0,
    PFD_MAIN_PLANE, 0,
    0,0,0
  };

  int pf = ChoosePixelFormat(wwindow->deviceContext, &pfd);
  SetPixelFormat(wwindow->deviceContext, pf, &pfd);

  HGLRC dummyContext = wglCreateContext(wwindow->deviceContext);
  wglMakeCurrent(wwindow->deviceContext, dummyContext);

  Wcreate_gl_context = (wglCreateContextAttribsARB_t)wglGetProcAddress("wglCreateContextAttribsARB");
  Wchoose_pixel_format = (wglChoosePixelFormatARB_t)wglGetProcAddress("wglChoosePixelFormatARB");

  if (!Wchoose_pixel_format || !Wcreate_gl_context) {
    fprintf(stderr, "Failed to proc functions CreateContextAttribsARB_t or ChoosePixelFormatARB_T");
  }

  wglDeleteContext(dummyContext);
  tsekW_destroy_window(dummyWindow);
}

void Wbuild_wgl_attribs(const tsekPixelFormat* pf, int* outAttribs) {
    int i = 0;

    #define ADD(a, b) outAttribs[i++] = (a); outAttribs[i++] = (b)

    ADD(WGL_DRAW_TO_WINDOW_ARB, GL_TRUE);
    ADD(WGL_SUPPORT_OPENGL_ARB, GL_TRUE);
    ADD(WGL_DOUBLE_BUFFER_ARB,  GL_TRUE);
    ADD(WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB);

    ADD(WGL_RED_BITS_ARB,   pf->r_bits);
    ADD(WGL_GREEN_BITS_ARB, pf->g_bits);
    ADD(WGL_BLUE_BITS_ARB,  pf->b_bits);
    ADD(WGL_ALPHA_BITS_ARB, pf->a_bits);

    ADD(WGL_DEPTH_BITS_ARB,   pf->depth_bits);
    ADD(WGL_STENCIL_BITS_ARB, pf->stencil_bits);

    if (pf->samples > 0) {
        ADD(WGL_SAMPLE_BUFFERS_ARB, 1);
        ADD(WGL_SAMPLES_ARB,        pf->samples);
    }

    outAttribs[i++] = 0; // terminator

    #undef ADD
}

void Wcreate_tsekG_context(tsekPixelFormat* format, tsekIWindow* window) {
  tsekWWindow* wwindow = Wget_window(window);

  int pixelFormatAttribs[32];
  UINT numFormats = 0;
  int chosenFormat = 0;

  Wbuild_wgl_attribs(format, pixelFormatAttribs);

  BOOL result = Wchoose_pixel_format(wwindow->deviceContext, pixelFormatAttribs, NULL, 1, &chosenFormat, &numFormats);

  if (!result || numFormats == 0){
    format->samples = 0;
    BOOL result = Wchoose_pixel_format(wwindow->deviceContext, pixelFormatAttribs, NULL, 1, &chosenFormat, &numFormats);

    if (!result || numFormats == 0) {
      fprintf(stderr, "Failed to find pixel format\n");
    }
  }

  PIXELFORMATDESCRIPTOR pfd;
  DescribePixelFormat(wwindow->deviceContext, chosenFormat, sizeof(pfd), &pfd);
  if (!SetPixelFormat(wwindow->deviceContext, chosenFormat, &pfd)) {
    fprintf(stderr, "Failed to bind pixel format\n");
  }

  int attribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0,
  };

  wwindow->glContext = Wcreate_gl_context(wwindow->deviceContext, NULL, attribs);
  if (!wwindow->glContext) {
    fprintf(stderr, "Failed to create WGL context\n");
  }

  if (!wglMakeCurrent(wwindow->deviceContext, wwindow->glContext)) {
    fprintf(stderr, "Failed to bind WGL context\n");
  }

  if (!gladLoadGL()) {
    fprintf(stderr, "Failed to load GLAD\n");
  }
}


void tsekW_init(tsekIContext* context, tsekIWindow* window, tsekIWindowInfo* info, wchar_t* defaultTitle, bool createGlobalContext, bool console) {

  init_windows_keycode_map();

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
    .title = defaultTitle,
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

  printf("About to load opengl...\n");

  Wload_gl();

  printf("Loaded Opengl\n");

  Wregister_windowclass(info);

  printf("Window Class Registered\n");

  tsekW_create_window(window, info);
}

void tsekW_fill_context(tsekIContext* context, bool setGlobal) {
  context->inner = malloc(sizeof(tsekWContext));

  tsekWContext* wcontext = Wget_context(context);
  wcontext->hInstance = Wget_hInstance();

  LARGE_INTEGER start;
  QueryPerformanceCounter(&start);
  printf("%d\n", start.QuadPart);

  QueryPerformanceCounter(&wcontext->time);
  QueryPerformanceCounter(&wcontext->fixed_time);
  QueryPerformanceFrequency(&wcontext->freq);

  wcontext->isCursorVisible = true;

  if (setGlobal) {
    globalContext = wcontext;
  }
}

void tsekW_destroy_context(tsekIContext* context) {
  free(context->inner);
}

void tsekW_create_dummy_window(tsekIWindow* window) {
    HINSTANCE hInstance = globalContext->hInstance;
    window->inner = calloc(1, sizeof(tsekWWindow));

    Wregister_windowclass(&(tsekIWindowInfo){.wndClassName = L"DUMMY"});
    printf("WNDCLASS registered\n");
    tsekWWindow* wwindow = Wget_window(window);

    printf("Running CreateWindowExW... \n");
    wwindow->handle = CreateWindowExW(
        0,
        L"DUMMY",
        L"DUMMY",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        100,
        100,
        NULL,
        NULL,
        hInstance,
        window
    );
    wwindow->deviceContext = GetDC(wwindow->handle);

    printf("Window Created with error code: %d\n", GetLastError());

    if (wwindow->handle == NULL) {
      printf("Failed to create Dummy Window\n");
    }
}

void tsekW_create_window(tsekIWindow* window, tsekIWindowInfo* info) {
  window->inner = calloc(1, sizeof(tsekWWindow));
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

  Wcreate_tsekG_context(&info->pixelFormat, window);

  ShowWindow(wwindow->handle, SW_SHOW);
}

void tsekW_destroy_window(tsekIWindow* window) {
  DestroyWindow(Wget_window(window)->handle);
  free(window->inner);
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
  LARGE_INTEGER end;
  QueryPerformanceCounter(&end);

  return (double)(end.QuadPart - globalContext->time.QuadPart) / globalContext->freq.QuadPart;
}

double tsekW_get_fixed_time() {
  LARGE_INTEGER end;
  QueryPerformanceCounter(&end);

  return (double)(end.QuadPart - globalContext->fixed_time.QuadPart) / globalContext->freq.QuadPart;
}

void tsekW_set_time(double time) {
  LARGE_INTEGER curr;
  QueryPerformanceCounter(&curr);

  globalContext->time.QuadPart = (curr.QuadPart - time * globalContext->freq.QuadPart);
}

void tsekW_allocate_time(double framerate, double start, double end) {
  timeBeginPeriod(1);

  double frametime = 1000 / framerate;
  double elapsed_time = 1000 * (end - start);
  //double ease = frametime / 16;
  double ease = 0;
  double sleep_time = frametime - elapsed_time - ease;

  if (sleep_time > 0) {
    Sleep(sleep_time);
  }

  timeEndPeriod(1);
}


bool tsekW_get_cursor_visible(tsekIWindow* window) {
  return globalContext->isCursorVisible;
}

void tsekW_set_cursor_visible(tsekIWindow* window, bool visible) {
  ShowCursor(visible);
  globalContext->isCursorVisible = visible;
}


void tsekW_swap_buffers(tsekIWindow* window) {
  SwapBuffers(Wget_window(window)->deviceContext);
}


// messager

void Wget_window_rect(tsekIWindow* window, void* out, bool pos, bool dims) {
  RECT rect;
  GetWindowRect(Wget_window(window)->handle, &rect);

  POS result = {
    .x = rect.left,
    .y = rect.top,
    .width = rect.right - rect.left,
    .height = rect.bottom - rect.top
  };

  if (pos) {
    ((POS*)out)->x = result.x;
    ((POS*)out)->y = result.y;
  }
  if (dims) {
    ((POS*)out)->width = result.width;
    ((POS*)out)->height = result.height;
  }
}


void Wget_client_rect(tsekIWindow* window, void* out, bool pos, bool dims) {
  HWND windowHandle = Wget_window(window)->handle;
  RECT client_rect, window_rect;
  GetClientRect(windowHandle, &client_rect);
  GetWindowRect(windowHandle, &window_rect);

  int FrameExtentsH = (window_rect.bottom - window_rect.top) - client_rect.bottom;

  POS result = {
    .x = window_rect.left,
    .y = window_rect.top + FrameExtentsH,
    .width = window_rect.right - window_rect.left,
    .height = window_rect.bottom - (window_rect.top + FrameExtentsH)
  };

  if (pos) {
    ((POS*)out)->x = result.x;
    ((POS*)out)->y = result.y;
  }
  if (dims) {
    ((POS*)out)->width = result.width;
    ((POS*)out)->height = result.height;
  }
}

void Wget_mouse_pos(tsekIWindow* window, void* out, POS relativeTo) {
  POINT mousepos;
  GetCursorPos(&mousepos);

  ((int*)out)[0] = mousepos.x - relativeTo.x;
  ((int*)out)[1] = mousepos.y - relativeTo.y;
}


void tsekW_get_window_param(tsekIWindow* window, tsekIWindowParam param, void* out) {

  tsekWWindow* wwindow = Wget_window(window);

  switch (param) {
    case KEYMAP: {
      int** p = (int**)out;
      *p = wwindow->keymap;
      break;
    }
    case CALLBACKS: {
      tsekCallbacks** p = (tsekCallbacks**)out;
      *p = &wwindow->callbacks;
      break;
    }

    case WINDOW_RECT: {
      Wget_window_rect(window, out, true, true);
      break;
    }
    case WINDOW_POS: {
      Wget_window_rect(window, out, true, false);
      break;
    }
    case WINDOW_DIM: {
      Wget_window_rect(window, out, false, true);
      break;
    }

    case CLIENT_RECT: {
      Wget_client_rect(window, out, true, true);
      break;
    }
    case CLIENT_POS: {
      Wget_client_rect(window, out, true, false);
      break;
    }
    case CLIENT_DIM: {
      Wget_client_rect(window, out, false, true);
      break;
    }

    case CURSORPOS_DESKTOP: {
      Wget_mouse_pos(window, out, (POS){0, 0, 0, 0});
      break;
    }
    case CURSORPOS_WINDOW: {
      POS windowpos;
      Wget_window_rect(window, &windowpos, true, false);
      Wget_mouse_pos(window, out, windowpos);
      break;
    }
    case CURSORPOS_CLIENT: {
      POS clientpos;
      Wget_client_rect(window, &clientpos, true, false);
      Wget_mouse_pos(window, out, clientpos);
      break;
    }
  }
}

void Wset_window_rect(tsekIWindow* window, void* in, bool pos, bool dims) {
    POS* input = (POS*)in;
    if (pos) SetWindowPos(Wget_window(window)->handle, HWND_TOP, input->x, input->y, input->width, input->height, SWP_NOSIZE);
    if (dims) SetWindowPos(Wget_window(window)->handle, HWND_TOP, input->x, input->y, input->width, input->height, SWP_NOMOVE);
}

void Wset_client_rect(tsekIWindow* window, void* in, bool pos, bool dims) {
    POS* input = (POS*)in;
    RECT window_rect, client_rect;
    GetWindowRect(Wget_window(window)->handle, &window_rect); GetClientRect(Wget_window(window)->handle, &client_rect);
    int frameExtentsH = (window_rect.bottom - window_rect.top) - client_rect.bottom;

    int top = input->y - frameExtentsH;
    int height = input->height + frameExtentsH;

    if (pos) SetWindowPos(Wget_window(window)->handle, HWND_TOP, input->x, top, input->width, height, SWP_NOSIZE);
    if (dims) SetWindowPos(Wget_window(window)->handle, HWND_TOP, input->x, top, input->width, height, SWP_NOMOVE);
}

void tsekW_set_window_param(tsekIWindow* window, tsekIWindowParam param, void* in) {
  tsekWWindow* wwindow = Wget_window(window);

  switch (param) {
    case WINDOW_RECT: {
      Wset_window_rect(window, in, true, true);
      break;
    }
    case WINDOW_POS: {
      Wset_window_rect(window, in, true, false);
      break;
    }
    case WINDOW_DIM: {
      Wset_window_rect(window, in, false, true);
      break;
    }

    case CLIENT_RECT: {
      Wset_client_rect(window, in, true, true);
      break;
    }
    case CLIENT_POS: {
      Wset_client_rect(window, in, true, false);
      break;
    }
    case CLIENT_DIM: {
      Wset_client_rect(window, in, false, true);
      break;
    }

    case CALLBACKS: {
      tsekCallbacks* callbacks = (tsekCallbacks*)in;
      Wget_window(window)->callbacks = *callbacks;
      break;
    }
    case KEYMAP: {
      fprintf(stderr, "Keymap is read-only.");
      break;
    }

    case CURSORPOS_DESKTOP: {
      int* pos = (int*)in;
      SetCursorPos(pos[0], pos[1]);
      break;
    }
    case CURSORPOS_WINDOW: {
      int* pos = (int*) in;
      POS relativeTo;
      Wget_window_rect(window, &relativeTo, true, false);
      SetCursorPos(pos[0] + relativeTo.x, pos[1] + relativeTo.y);
      break;
    }
    case CURSORPOS_CLIENT: {
      int* pos = (int*) in;
      POS relativeTo;
      Wget_client_rect(window, &relativeTo, true, false);
      SetCursorPos(pos[0] + relativeTo.x, pos[1] + relativeTo.y);
      break;
    }
  }
}

#endif
