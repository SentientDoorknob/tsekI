#ifdef PLATFORM_LINUX

#warning "Including Linux-Specific Headers"

#include <X11/Xutil.h>
#include "../tsekI.h"
#include <GL/glx.h>
#include <sys/socket.h>
#include <netdb.h>
#include "../../libs/glad.h"

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

void tsekL_init(tsekIContext*, tsekIWindow*, tsekIWindowInfo*, wchar_t* defaultTitle, bool createGlobalContext, bool console);

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

// networking

typedef struct {
  void* inner;
} tsekLConnection;

typedef struct {
  struct addrinfo* info;
} tsekLAddressInfo;

void tsekL_network_init();
void tsekL_network_cleanup();

void tsekL_get_address_info(char* url, int port, tsekIAddressInfo* info);
void tsekL_display_addrinfo(tsekIAddressInfo* info);
void tsekL_destroy_address_info(tsekIAddressInfo* info);
void tsekL_socket_create(tsekISocket* socket);
void tsekL_socket_close(tsekISocket* socket);

// server

void tsekL_socket_bind(tsekISocket* socket, tsekIAddressInfo* address);
void tsekL_socket_listen(tsekISocket* socket, int backlog);
void tsekL_socket_accept(tsekISocket* server, tsekISocket* client, tsekIAddressInfo* address);

// client 

void tsekL_socket_connect(tsekISocket* socket, tsekIAddressInfo* address);

// messaging

int tsekL_socket_send(tsekISocket* socket, char* message, int length, bool OOB, bool dontroute);
int tsekL_socket_recv(tsekISocket* socket, char* message, int length, bool OOB, bool peek, bool waitall);

int tsekL_socket_geterror(tsekISocket* socket);
void tsekL_socket_set_nonblocking(tsekISocket* socket, int mode);

// TLS 

void tsekL_TLS_init(tsekITLSContext* context);
int tsekL_TLS_connect(tsekITLSSocket* tls_socket, char* host, tsekISocket* socket, tsekITLSContext* context);
int tsekL_TLS_send(tsekITLSSocket* socket, char* message, int length);
int tsekL_TLS_recv(tsekITLSSocket* socket, char* buffer, int length);
void tsekL_TLS_destroy_socket(tsekITLSSocket* tls_socket, tsekISocket* socket);
void tsekL_TLS_destroy_context(tsekITLSContext* context);

#endif
