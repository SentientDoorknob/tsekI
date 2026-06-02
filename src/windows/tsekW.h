#ifdef PLATFORM_WINDOWS

#include "../tsekI.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define WINDOWS_MAX_KEYMAP_SIZE 255

typedef struct {
  HWND handle;
  HGLRC glContext;
  HDC deviceContext;
  int minMaxDims[4];

  int keymap[255];
  tsekCallbacks callbacks;
} tsekWWindow;

typedef struct {
  HINSTANCE hInstance;

  LARGE_INTEGER time;
  LARGE_INTEGER fixed_time;
  LARGE_INTEGER freq;

  bool isCursorVisible;
} tsekWContext;

void tsekW_init(tsekIContext*, tsekIWindow*, tsekIWindowInfo*, wchar_t* defaultTitle, bool createGlobalContext, bool console);

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


// networking

typedef struct {
  void* inner;
} tsekWConnection;

typedef struct {
  struct addrinfo* info;
} tsekWAddressInfo;

void tsekW_network_init();
void tsekW_network_cleanup();

void tsekW_get_address_info(char* url, int port, tsekIAddressInfo* info);
void tsekW_display_addrinfo(tsekIAddressInfo* info);
void tsekW_destroy_address_info(tsekIAddressInfo* info);
void tsekW_socket_create(tsekISocket* socket);
void tsekW_socket_close(tsekISocket* socket);

// server

void tsekW_socket_bind(tsekISocket* socket, tsekIAddressInfo* address);
void tsekW_socket_listen(tsekISocket* socket, int backlog);
void tsekW_socket_accept(tsekISocket* server, tsekISocket* client, tsekIAddressInfo* address);

// client 

void tsekW_socket_connect(tsekISocket* socket, tsekIAddressInfo* address);

// messaging

int tsekW_socket_send(tsekISocket* socket, char* message, int length, bool OOB, bool dontroute);
int tsekW_socket_recv(tsekISocket* socket, char* message, int length, bool OOB, bool peek, bool waitall);

int tsekW_socket_geterror(tsekISocket* socket);
void tsekW_socket_set_nonblocking(tsekISocket* socket, int mode);

// TLS 

void tsekW_TLS_init(tsekITLSContext* context);
void tsekW_TLS_bind(tsekITLSSocket* tls_socket, char* host, tsekISocket* socket, tsekITLSContext* context);
int tsekW_TLS_send(tsekITLSSocket* socket, char* message, int length);
int tsekW_TLS_recv(tsekITLSSocket* socket, char* buffer, int length);
void tsekW_TLS_destroy_socket(tsekITLSSocket* tls_socket, tsekISocket* socket);
void tsekW_TLS_destroy_context(tsekITLSContext* context);

#endif
