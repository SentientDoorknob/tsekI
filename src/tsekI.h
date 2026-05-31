#ifndef TSEKI_H

#define TSEKI_H 

#include "../libs/glad.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_KEYMAP_SIZE 256
#define TARGET_FRAMERATE 60

typedef enum {
    // Letters
    TSEK_NONE = 0,
    TSEK_A = 1,
    TSEK_B,
    TSEK_C,
    TSEK_D,
    TSEK_E,
    TSEK_F,
    TSEK_G,
    TSEK_H,
    TSEK_I,
    TSEK_J,
    TSEK_K,
    TSEK_L,
    TSEK_M,
    TSEK_N,
    TSEK_O,
    TSEK_P,
    TSEK_Q,
    TSEK_R,
    TSEK_S,
    TSEK_T,
    TSEK_U,
    TSEK_V,
    TSEK_W,
    TSEK_X,
    TSEK_Y,
    TSEK_Z,

    // Numbers
    TSEK_0,
    TSEK_1,
    TSEK_2,
    TSEK_3,
    TSEK_4,
    TSEK_5,
    TSEK_6,
    TSEK_7,
    TSEK_8,
    TSEK_9,

    // Symbols
    TSEK_MINUS,
    TSEK_EQUAL,
    TSEK_LEFTBRACKET,
    TSEK_RIGHTBRACKET,
    TSEK_BACKSLASH,
    TSEK_SEMICOLON,
    TSEK_APOSTROPHE,
    TSEK_GRAVE,
    TSEK_COMMA,
    TSEK_PERIOD,
    TSEK_SLASH,

    // Control keys
    TSEK_ENTER,
    TSEK_ESCAPE,
    TSEK_BACKSPACE,
    TSEK_TAB,
    TSEK_SPACE,
    TSEK_CAPSLOCK,

    // Function keys
    TSEK_F1,
    TSEK_F2,
    TSEK_F3,
    TSEK_F4,
    TSEK_F5,
    TSEK_F6,
    TSEK_F7,
    TSEK_F8,
    TSEK_F9,
    TSEK_F10,
    TSEK_F11,
    TSEK_F12,

    // Arrow keys
    TSEK_LEFT,
    TSEK_UP,
    TSEK_RIGHT,
    TSEK_DOWN,

    // Modifier keys
    TSEK_LEFTSHIFT,
    TSEK_RIGHTSHIFT,
    TSEK_LEFTCTRL,
    TSEK_RIGHTCTRL,
    TSEK_LEFTALT,
    TSEK_RIGHTALT,
    TSEK_LEFTMETA,
    TSEK_RIGHTMETA,

    // Numpad keys
    TSEK_NUMPAD0,
    TSEK_NUMPAD1,
    TSEK_NUMPAD2,
    TSEK_NUMPAD3,
    TSEK_NUMPAD4,
    TSEK_NUMPAD5,
    TSEK_NUMPAD6,
    TSEK_NUMPAD7,
    TSEK_NUMPAD8,
    TSEK_NUMPAD9,
    TSEK_NUMPADDECIMAL,
    TSEK_NUMPADENTER,
    TSEK_NUMPADADD,
    TSEK_NUMPADSUBTRACT,
    TSEK_NUMPADMULTIPLY,
    TSEK_NUMPADDIVIDE,

    // Editing/navigation keys
    TSEK_INSERT,
    TSEK_DELETE,
    TSEK_HOME,
    TSEK_END,
    TSEK_PAGEUP,
    TSEK_PAGEDOWN,

    // Other keys
    TSEK_PRINTSCREEN,
    TSEK_SCROLLLOCK,
    TSEK_PAUSE,

    // Buttons
    TSEK_MBL,
    TSEK_MBR,
    TSEK_MBM,
    TSEK_MB4,
    TSEK_MB5,
} tsekKeyCode;

typedef enum {
  WINDOW_RECT,
  WINDOW_POS,
  WINDOW_DIM,

  CLIENT_RECT,
  CLIENT_POS,
  CLIENT_DIM,

  CURSORPOS_DESKTOP,
  CURSORPOS_WINDOW,
  CURSORPOS_CLIENT,

  KEYMAP,
  CALLBACKS,
} tsekIWindowParam;

typedef struct {
  void* inner;
} tsekIWindow;

typedef struct {
  void* inner;
} tsekIContext;

typedef struct {
  uint16_t r_bits;
  uint16_t g_bits;
  uint16_t b_bits;
  uint16_t a_bits;
  uint16_t depth_bits;
  uint16_t stencil_bits;
  uint8_t samples;
} tsekPixelFormat;

typedef struct {
  const wchar_t* title;
  uint32_t width, height;
  int32_t x, y;

  uint32_t borderWidth;

  uint32_t classId;
  const wchar_t* wndClassName;

  uint32_t style;
  uint32_t extendedStyle; 

  tsekPixelFormat pixelFormat;

  uint32_t minMaxDims[4];
} tsekIWindowInfo;

typedef struct {
  void (*keydown)(tsekIWindow*, tsekKeyCode);
  void (*keytype)(tsekIWindow*, tsekKeyCode);
  void (*keyup)(tsekIWindow*, tsekKeyCode);
  void (*mbdown)(tsekIWindow*, tsekKeyCode);
  void (*mbup)(tsekIWindow*, tsekKeyCode);
  void (*kstate)(tsekIWindow*, tsekKeyCode, bool);

  void (*tsegsize)(tsekIWindow*, uint32_t width, uint32_t height);
  void (*size)(tsekIWindow*, uint32_t width, uint32_t height);
} tsekCallbacks;

typedef struct {
  uint32_t x, y, width, height;
} POS;

void tsekI_init(tsekIContext*, tsekIWindow*, tsekIWindowInfo*, wchar_t* defualtTitle, bool createGlobalContext, bool console);

void tsekI_fill_context(tsekIContext* context, bool setGlobal);
void tsekI_destroy_context(tsekIContext* context);

void tsekI_create_dummy_window(tsekIWindow* window);
void tsekI_create_window(tsekIWindow* window, tsekIWindowInfo* info);
void tsekI_destroy_window(tsekIWindow* window);

bool tsekI_get_closed_window(tsekIWindow*);
bool tsekI_update_window(tsekIWindow* window);

double tsekI_get_time();
double tsekI_get_fixed_time();

void tsekI_set_time(double time);
void tsekI_allocate_time(double framerate, double start, double end);

bool tsekI_get_cursor_visible(tsekIWindow*);
void tsekI_set_cursor_visible(tsekIWindow*, bool);

void tsekI_swap_buffers(tsekIWindow*);

// messager

void tsekI_get_window_param(tsekIWindow* window, tsekIWindowParam param, void* out);
void tsekI_set_window_param(tsekIWindow* window, tsekIWindowParam param, void* in);


// networking

typedef struct {
  int handle;
} tsekISocket;

typedef struct {
  void* inner;
} tsekIConnection;

typedef struct {
  void* inner;
} tsekIAddressInfo;

void tsekI_network_init();
void tsekI_network_cleanup();

void tsekI_get_address_info(char* url, int port, tsekIAddressInfo* info);
void tsekI_display_addrinfo(tsekIAddressInfo* info);
void tsekI_destroy_address_info(tsekIAddressInfo* info);
void tsekI_socket_create(tsekISocket* socket);
void tsekI_socket_close(tsekISocket* socket);

// server

void tsekI_socket_bind(tsekISocket* socket, tsekIAddressInfo* address);
void tsekI_socket_listen(tsekISocket* socket, int backlog);
void tsekI_socket_accept(tsekISocket* server, tsekISocket* client, tsekIAddressInfo* address);

// client 

void tsekI_socket_connect(tsekISocket* socket, tsekIAddressInfo* address);

// messaging

int tsekI_socket_send(tsekISocket* socket, char* message, int length, bool OOB, bool dontroute);
int tsekI_socket_recv(tsekISocket* socket, char* message, int length, bool OOB, bool peek, bool waitall);

int tsekI_socket_geterror(tsekISocket* socket);
void tsekI_socket_set_nonblocking(tsekISocket* socket, int mode);

// TLS 

typedef struct {
  void* context;
} tsekITLSContext;

typedef struct {
  void* socket;
} tsekITLSSocket;

void tsekI_TLS_init(tsekITLSContext* context);
void tsekI_TLS_bind(tsekITLSSocket* tls_socket, char* host, tsekISocket* socket, tsekITLSContext* context);
int tsekI_TLS_send(tsekITLSSocket* socket, char* message, int length);
int tsekI_TLS_recv(tsekITLSSocket* socket, char* buffer, int length);
void tsekI_TLS_destroy_socket(tsekITLSSocket* tls_socket, tsekISocket* socket);
void tsekI_TLS_destroy_context(tsekITLSContext* context);

#endif
