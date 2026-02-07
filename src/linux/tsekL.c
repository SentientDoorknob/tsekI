#include "../tsekI.h"
#include "../tsekG.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include "tsekL.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <linux/input-event-codes.h>
#include <X11/Xresource.h>
#include <X11/XKBlib.h>
#include <time.h>
#include <GL/glx.h>
#include <GL/gl.h>

tsekLContext* globalContext;

static tsekKeyCode keycode_map[MAX_LINUX_KEYCODE + 1];

tsekLContext* Lget_context(tsekIContext* context) {
  return (tsekLContext*)context->inner;
}

tsekLWindow* Lget_window(tsekIWindow* window) {
  return (tsekLWindow*)window->inner;
}

void Linit_keycode_map() {
  for (int i = 0; i <= MAX_LINUX_KEYCODE; i++)
    keycode_map[i] = TSEK_NONE;

  // Letters (X11 keycodes may vary per keyboard layout)
  keycode_map[38] = TSEK_A;
  keycode_map[56] = TSEK_B;
  keycode_map[54] = TSEK_C;
  keycode_map[40] = TSEK_D;
  keycode_map[26] = TSEK_E;
  keycode_map[41] = TSEK_F;
  keycode_map[42] = TSEK_G;
  keycode_map[43] = TSEK_H;
  keycode_map[31] = TSEK_I;
  keycode_map[44] = TSEK_J;
  keycode_map[45] = TSEK_K;
  keycode_map[46] = TSEK_L;
  keycode_map[58] = TSEK_M;
  keycode_map[57] = TSEK_N;
  keycode_map[32] = TSEK_O;
  keycode_map[33] = TSEK_P;
  keycode_map[24] = TSEK_Q;
  keycode_map[27] = TSEK_R;
  keycode_map[39] = TSEK_S;
  keycode_map[28] = TSEK_T;
  keycode_map[30] = TSEK_U;
  keycode_map[55] = TSEK_V;
  keycode_map[25] = TSEK_W;
  keycode_map[53] = TSEK_X;
  keycode_map[29] = TSEK_Y;
  keycode_map[52] = TSEK_Z;

  // Numbers
  keycode_map[10] = TSEK_1;
  keycode_map[11] = TSEK_2;
  keycode_map[12] = TSEK_3;
  keycode_map[13] = TSEK_4;
  keycode_map[14] = TSEK_5;
  keycode_map[15] = TSEK_6;
  keycode_map[16] = TSEK_7;
  keycode_map[17] = TSEK_8;
  keycode_map[18] = TSEK_9;
  keycode_map[19] = TSEK_0;

  // Symbols
  keycode_map[20] = TSEK_MINUS;        // -
  keycode_map[21] = TSEK_EQUAL;        // =
  keycode_map[34] = TSEK_LEFTBRACKET;  // [
  keycode_map[35] = TSEK_RIGHTBRACKET; // ]
  keycode_map[51] = TSEK_BACKSLASH;    // \
  keycode_map[47] = TSEK_SEMICOLON;    // ;
  keycode_map[48] = TSEK_APOSTROPHE;   // '
  keycode_map[49] = TSEK_GRAVE;        // `
  keycode_map[59] = TSEK_COMMA;        // ,
  keycode_map[60] = TSEK_PERIOD;       // .
  keycode_map[61] = TSEK_SLASH;        // /

  // Control keys
  keycode_map[36] = TSEK_ENTER;      // Return
  keycode_map[9]  = TSEK_ESCAPE;
  keycode_map[22] = TSEK_BACKSPACE;
  keycode_map[23] = TSEK_TAB;
  keycode_map[65] = TSEK_SPACE;
  keycode_map[66] = TSEK_CAPSLOCK;

  // Function keys
  keycode_map[67] = TSEK_F1;
  keycode_map[68] = TSEK_F2;
  keycode_map[69] = TSEK_F3;
  keycode_map[70] = TSEK_F4;
  keycode_map[71] = TSEK_F5;
  keycode_map[72] = TSEK_F6;
  keycode_map[73] = TSEK_F7;
  keycode_map[74] = TSEK_F8;
  keycode_map[75] = TSEK_F9;
  keycode_map[76] = TSEK_F10;
  keycode_map[95] = TSEK_F11;
  keycode_map[96] = TSEK_F12;

  // Arrow keys
  keycode_map[113] = TSEK_LEFT;
  keycode_map[111] = TSEK_UP;
  keycode_map[114] = TSEK_RIGHT;
  keycode_map[116] = TSEK_DOWN;

  // Modifier keys
  keycode_map[50]  = TSEK_LEFTSHIFT;
  keycode_map[62]  = TSEK_RIGHTSHIFT;
  keycode_map[37]  = TSEK_LEFTCTRL;
  keycode_map[105] = TSEK_RIGHTCTRL;
  keycode_map[64]  = TSEK_LEFTALT;
  keycode_map[108] = TSEK_RIGHTALT;
  keycode_map[133] = TSEK_LEFTMETA;
  keycode_map[134] = TSEK_RIGHTMETA;

  // Numpad keys
  keycode_map[90]  = TSEK_NUMPAD0;
  keycode_map[87]  = TSEK_NUMPAD1;
  keycode_map[88]  = TSEK_NUMPAD2;
  keycode_map[89]  = TSEK_NUMPAD3;
  keycode_map[83]  = TSEK_NUMPAD4;
  keycode_map[84]  = TSEK_NUMPAD5;
  keycode_map[85]  = TSEK_NUMPAD6;
  keycode_map[79]  = TSEK_NUMPAD7;
  keycode_map[80]  = TSEK_NUMPAD8;
  keycode_map[81]  = TSEK_NUMPAD9;
  keycode_map[91]  = TSEK_NUMPADDECIMAL;
  keycode_map[104] = TSEK_NUMPADENTER;
  keycode_map[86]  = TSEK_NUMPADADD;
  keycode_map[82]  = TSEK_NUMPADSUBTRACT;
  keycode_map[63]  = TSEK_NUMPADMULTIPLY;
  keycode_map[106] = TSEK_NUMPADDIVIDE;

  // Editing/navigation
  keycode_map[118] = TSEK_INSERT;
  keycode_map[119] = TSEK_DELETE;
  keycode_map[110] = TSEK_HOME;
  keycode_map[115] = TSEK_END;
  keycode_map[112] = TSEK_PAGEUP;
  keycode_map[117] = TSEK_PAGEDOWN;

  // Other keys
  keycode_map[107] = TSEK_PRINTSCREEN;
  keycode_map[78]  = TSEK_SCROLLLOCK;
  keycode_map[127] = TSEK_PAUSE;
}

void tsekL_fill_context(tsekIContext *context, bool setGlobal) {

  context->inner = malloc(sizeof(tsekLContext));

  tsekLContext* LContext = Lget_context(context);

  LContext->displayName = NULL;
  LContext->display = XOpenDisplay(LContext->displayName);

  LContext->WM_DELETE = XInternAtom(LContext->display, "WM_DELETE_WINDOW", false);

  LContext->context = XUniqueContext();

  printf("Display Name NULL\n");

  if (setGlobal) {
    globalContext = Lget_context(context);
  }

  XSynchronize(LContext->display, true);
}

void tsekL_destroy_context(tsekIContext* context) {
  tsekLContext* c = Lget_context(context);
  XCloseDisplay(c->display);

  free(context->inner);
}

GLXFBConfig Lget_FBConfig(tsekIWindowInfo* info) {
    int visual_attribs[] = {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,

    GLX_RED_SIZE, info->pixelFormat.r_bits,
    GLX_GREEN_SIZE, info->pixelFormat.g_bits,
    GLX_BLUE_SIZE, info->pixelFormat.b_bits,
    GLX_ALPHA_SIZE, info->pixelFormat.a_bits,

    GLX_DEPTH_SIZE, info->pixelFormat.depth_bits,
    GLX_STENCIL_SIZE, info->pixelFormat.stencil_bits,

    GLX_DOUBLEBUFFER, True,

    GLX_SAMPLE_BUFFERS, 1,
    GLX_SAMPLES, info->pixelFormat.samples,          // MSAA 4x

    None
  };

  int fbcount;
  GLXFBConfig* fbConfigList = glXChooseFBConfig(globalContext->display, DefaultScreen(globalContext->display), visual_attribs, &fbcount);
  if (fbcount == 0) {
      printf("Couldn't find any valid FBConfigs\n");
  }
  GLXFBConfig fbConfig = fbConfigList[0];

  XFree(fbConfigList);

  return fbConfig;
}

void tsekL_create_window(tsekIWindow* window, tsekIWindowInfo* info) {

  XVisualInfo *visual = glXGetVisualFromFBConfig(globalContext->display, Lget_FBConfig(info));

  printf("Creating Window\n");

  Colormap map = XCreateColormap(globalContext->display, XDefaultRootWindow(globalContext->display), visual->visual, AllocNone);

  XSetWindowAttributes attribs = {
    .background_pixel = WhitePixel(globalContext->display, 0),
    .colormap = map};

  Window windowHandle = XCreateWindow(
      globalContext->display,
      XDefaultRootWindow(globalContext->display),
      info->x, info->y,
      info->width, info->height,
      info->borderWidth,
      visual->depth,
      info->classId,
      visual->visual, 
      CWBackPixel | CWColormap,
      &attribs);

  printf("Window Opened\n");

  window->inner = calloc(1, sizeof(tsekLWindow));

  tsekLWindow* LWindow = Lget_window(window);
  LWindow->window = windowHandle;
  LWindow->isOpen = true;
  LWindow->isCursorVisible = true;

  printf("Window Created\n");

  XMapRaised(globalContext->display, windowHandle);
  XFlush(globalContext->display);

  XSaveContext(globalContext->display, LWindow->window, globalContext->context, (XPointer)window);

  printf("Window Mapped. Window ID: %lu\n", LWindow->window);

  XSetWMProtocols(globalContext->display, Lget_window(window)->window, &globalContext->WM_DELETE, 1);
  XSelectInput(globalContext->display, Lget_window(window)->window, 
      StructureNotifyMask |
      KeymapStateMask |
      KeyPressMask |
      KeyReleaseMask |
      ButtonPressMask |
      ButtonReleaseMask |
      ResizeRequest);

  glXMakeCurrent(globalContext->display, LWindow->window, globalContext->glContext);
  
  XFree(visual);
}

void tsekL_destroy_window(tsekIWindow* window) {
  glXMakeCurrent(globalContext->display, None, NULL);
  tsekL_set_cursor_visible(window, true);
  free(window->inner);
  free(window);
}

void tsekL_create_dummy_window(tsekIWindow* window) {
}

Cursor Lget_invisible_cursor(tsekIWindow* window) {
  Pixmap bm = XCreatePixmap(globalContext->display, Lget_window(window)->window, 1, 1, 1);

    XColor black;
    memset(&black, 0, sizeof(black));

    Cursor cursor = XCreatePixmapCursor(
        globalContext->display, bm, bm,
        &black, &black,
        0, 0
    );

    XFreePixmap(globalContext->display, bm);
    return cursor;
}

double Lget_time() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}


void tsekL_init(tsekIContext *context, tsekIWindow *window, tsekIWindowInfo *info, bool createGlobalContext, bool console) {

  Linit_keycode_map();

  tsekPixelFormat defaultFormat = {
    .r_bits = 8,
    .g_bits = 8,
    .b_bits = 8,
    .a_bits = 8,
    .depth_bits = 24,
    .stencil_bits = 8,
    .samples = 4,
  };

  tsekIWindowInfo defaultInfo = {
    L"Default Title",
    500, 500,
    50, 50,
    0,
    0, L"DEFAULTWNDCLASS",
    0, 0,
    defaultFormat,
    {0, 0, 0, 0}
  };

  if (!info) {
    info = &defaultInfo;
  }

  int context_attribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
    GLX_CONTEXT_MINOR_VERSION_ARB, 5,
    GLX_CONTEXT_PROFILE_MASK_ARB,
    GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
    None
  };

  typedef GLXContext (*glXCreateContextAttribsARBProc)(
    Display*, GLXFBConfig, GLXContext, Bool, const int*);

  glXCreateContextAttribsARBProc glXCreateContextAttribsARB =
    (void*)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

  printf("X: %d Y: %d Width: %d Height: %d\n", info->x, info->y, info->width, info->height);

  tsekL_fill_context(context, createGlobalContext);

  printf("Context Filled!\n");

  tsekLContext* LContext = Lget_context(context);

  LContext->glContext = glXCreateContextAttribsARB(LContext->display, Lget_FBConfig(info), 0, True, context_attribs);

  printf("Display Opened\n");

  tsekL_create_window(window, info);

  printf("Window Created\n");

  Bool supported;
  XkbSetDetectableAutoRepeat(globalContext->display, True, &supported);

  LContext->invisibleCursor = Lget_invisible_cursor(window);

  LContext->fixedTimeOffset = Lget_time();
  LContext->timeOffset = Lget_time();

  if (!gladLoadGL()) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return;
  }
}

double tsekL_get_time() {
  return Lget_time() - globalContext->timeOffset;
}

double tsekL_get_fixed_time() {
  return Lget_time() - globalContext->fixedTimeOffset;
}

void tsekL_set_time(double time) {
  globalContext->timeOffset = Lget_time() - time;
}

void tsekL_allocate_time(double frametime, double start, double end) {
  double sleepTime = fabs(frametime - (end - start));
  usleep(sleepTime * 1000000);
}

bool tsekL_get_cursor_visible(tsekIWindow* window) {
  return Lget_window(window)->isCursorVisible;
}

void tsekL_set_cursor_visible(tsekIWindow* window, bool active) {
  XUndefineCursor(globalContext->display, Lget_window(window)->window);

  if (!active) {
    XDefineCursor(globalContext->display, Lget_window(window)->window, globalContext->invisibleCursor);
  }

  Lget_window(window)->isCursorVisible = active;
}

void tsekL_swap_buffers(tsekIWindow* window) {
  glXSwapBuffers(globalContext->display, Lget_window(window)->window);
}

bool tsekL_get_closed_window(tsekIWindow* window) {
  return Lget_window(window)->isOpen;
}

tsekKeyCode Lget_keycode(int linuxcode) {
  if (linuxcode < 0 || linuxcode > MAX_LINUX_KEYCODE) return TSEK_NONE;
  return keycode_map[linuxcode];
}

void Lproc_keydown(XKeyEvent event, tsekIWindow* window) {
  tsekKeyCode code = Lget_keycode(event.keycode);
  tsekLWindow* LWindow = Lget_window(window);

  if (LWindow->callbacks.keydown && (LWindow->keymap[code] == 0)) {
    LWindow->callbacks.keydown(window, code);
  }
  LWindow->callbacks.keytype(window, code);

  LWindow->keymap[code] = true;
}

void Lproc_keyup(XKeyEvent event, tsekIWindow* window) {
  tsekKeyCode code = Lget_keycode(event.keycode);

  if (Lget_window(window)->callbacks.keyup) {
    Lget_window(window)->callbacks.keyup(window, code);
  }
  Lget_window(window)->keymap[code] = false;
}

tsekKeyCode Lget_buttoncode(uint32_t code) {
  switch (code) {
    case Button1: return TSEK_MBL;
    case Button2: return TSEK_MBM;
    case Button3: return TSEK_MBR;
    case Button4: return TSEK_MB4;
    case Button5: return TSEK_MB5;
    default: return TSEK_NONE;
  }
}

void Lproc_mousedown(XButtonEvent event, tsekIWindow* window) {
  tsekKeyCode code = Lget_buttoncode(event.button);
  tsekLWindow* LWindow = Lget_window(window);

  if (LWindow->callbacks.mbdown) {
    LWindow->callbacks.mbdown(window, code);
  }

  LWindow->keymap[code] = true;
}

void Lproc_mouseup(XButtonEvent event, tsekIWindow* window) {
  tsekKeyCode code = Lget_buttoncode(event.button);
  tsekLWindow* LWindow = Lget_window(window);

  if (LWindow->callbacks.mbup) {
    LWindow->callbacks.mbup(window, code);
  }

  LWindow->keymap[code] = false;
}

void Lproc_resize(XConfigureEvent event, tsekIWindow* window) {
  tsekLWindow* w = Lget_window(window);

  if (w->callbacks.size) {
    w->callbacks.size(window, event.width, event.height);
  }

  if (w->callbacks.tsegsize) {
    w->callbacks.tsegsize(window, event.width, event.height);
  }
}

bool tsekL_update_window(tsekIWindow* window) {

  while (XPending(globalContext->display) > 0) {

    XEvent event = {0};
    XNextEvent(globalContext->display, &event);

    tsekIWindow* eWindow = NULL;
    XFindContext(globalContext->display, event.xany.window, globalContext->context, (XPointer*)&eWindow);
    switch (event.type) {
      case ClientMessage: {
        if ((Atom)event.xclient.data.l[0] == globalContext->WM_DELETE) {
          printf("Window Delete requested by WM\n");
          Lget_window(eWindow)->isOpen = false;
        }
        break;
      }
      case KeyPress: {
        Lproc_keydown(event.xkey, eWindow);
        break;
      }
      case KeyRelease: {
        Lproc_keyup(event.xkey, eWindow);
        break;
      }
      case ButtonPress: {
        Lproc_mousedown(event.xbutton, eWindow);
        break;
      }
      case ButtonRelease: {
        Lproc_mouseup(event.xbutton, eWindow);
        break;
      }
      case ConfigureNotify: {
        printf("Resize\n");
        Lproc_resize(event.xconfigure, eWindow);
        break;
      }
    }
  }
  return true;
}

void tsekL_get_window_param(tsekIWindow* window, tsekIWindowParam param, void* out) {
    XWindowAttributes attribs;
    Status s = XGetWindowAttributes(globalContext->display, Lget_window(window)->window, &attribs);

    if (!s) {
      printf("Failed XGet allocation");
    }

    switch (param) {
      case TITLE: {
        fprintf(stderr, "Linux Window has no Title");
        break;
      }
      case CLIENT_RECT:
      case CLIENT_DIM:
      case CLIENT_POS:
      case WINDOW_RECT:
      case WINDOW_POS:
      case WINDOW_DIM: {
        int screen_x, screen_y;
        Window temp;
        XTranslateCoordinates(globalContext->display, Lget_window(window)->window,
            DefaultRootWindow(globalContext->display), 
            0, 0,
            &screen_x, &screen_y,
            &temp);

        POS* pout = (POS*)out;
        pout->x = screen_x;
        pout->y = screen_y;
        pout->width = attribs.width;
        pout->height = attribs.height;
        break;
      }
      case CURSORPOS_CLIENT:
      case CURSORPOS_WINDOW:
      case CURSORPOS_DESKTOP: {
        Window temp;
        int x, y;
        int rx, ry;
        uint32_t mask;

        Window w = Lget_window(window)->window;

        bool success = XQueryPointer(globalContext->display, w, &temp, &temp, 
            &rx, &ry, &x, &y, &mask);

        int* pos = (int*)out;
        pos[0] = param == CURSORPOS_DESKTOP ? rx : x;
        pos[1] = param == CURSORPOS_DESKTOP ? ry : y;
        break;
      }
      case CALLBACKS: {
        tsekLWindow* w = Lget_window(window);
        tsekCallbacks** callbacks = (tsekCallbacks**)out;
        *callbacks = &w->callbacks;
        break;
      }
      case KEYMAP: {
        tsekLWindow* w = Lget_window(window);
        int** p = (int**)out;
        *p = w->keymap;
        break;
      }
      default: {
        fprintf(stderr, "Invalid PARAMETER");
        break;
      }
    }
  }

  void tsekL_set_window_param(tsekIWindow* window, tsekIWindowParam param, void* in) {
    switch (param) {
      case TITLE: {
        fprintf(stderr, "Linux Window has no Title.");
        break;
      }
      case WINDOW_RECT:
      case WINDOW_DIM:
      case WINDOW_POS:
      case CLIENT_RECT:
      case CLIENT_DIM:
      case CLIENT_POS: {
        fprintf(stderr, "Window Resizing / Moving is delegated to Window Manager. To quote make, Stop.");
        break;
      }
      case CURSORPOS_DESKTOP: {
        int* pos = (int*)in;
        XWarpPointer(globalContext->display, 0,
            XDefaultRootWindow(globalContext->display),
            0, 0, 0, 0,
            pos[0], pos[1]);
        break;
      }
      case CURSORPOS_CLIENT:
      case CURSORPOS_WINDOW: {
        int* pos = (int*)in;
        XWarpPointer(globalContext->display, 0,
            Lget_window(window)->window,
            0, 0, 0, 0,
            pos[0], pos[1]);
        break;
      }
      case KEYMAP: {
        fprintf(stderr, "Linux Keymap is Read-Only.");
        break;
      }
      case CALLBACKS: {
        tsekCallbacks* callbacks = (tsekCallbacks*)in;
        Lget_window(window)->callbacks = *callbacks;
      }
    }
  }

