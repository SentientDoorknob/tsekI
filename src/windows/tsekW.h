#ifdef PLATFORM_WINDOWS

#include "../tsekI.h"
#include <windows.h>

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
} tsekMSocket;

typedef struct {
  void* inner;
} tsekMConnection;

typedef struct {
  void* inner;
} tsekMAddressInfo;

void tsekM_network_init();
void tsekM_network_cleanup();

void tsekM_get_address_info(char* url, int port, tsekIAddressInfo* info);
void tsekM_socket_create(tsekISocket* socket, int domain, int type, int protocol);
void tsekM_socket_close(tsekISocket* socket);

// server

void tsekM_socket_bind(tsekISocket* socket, tsekIAddressInfo* address);
void tsekM_socket_listen(tsekISocket* socket, int backlog);
void tsekM_socket_accept(tsekISocket* server, tsekISocket* client, tsekIAddressInfo* address);

// client 

void tsekM_socket_connect(tsekISocket* socket, tsekIAddressInfo* address);

// messaging

int tsekM_socket_send(tsekISocket* socket, char* message, int length, bool OOB, bool dontroute);
int tsekM_socket_recv(tsekISocket* socket, char* message, int length, bool OOB, bool peek, bool waitall);

int tsekM_socket_geterror(tsekISocket* socket);
#endif
