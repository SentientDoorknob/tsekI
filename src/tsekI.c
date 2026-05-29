#ifndef tsekI_c

#define tsekI_c

#include <stdio.h>
#include "tsekI.h"
#include "linux/tsekL.h"
#include "windows/tsekW.h"

void tsekI_init(tsekIContext* context, tsekIWindow *window, tsekIWindowInfo *info, wchar_t* defaultTitle, bool createGlobalContext, bool console) {
#ifdef PLATFORM_LINUX
  tsekL_init(context, window, info, defaultTitle, createGlobalContext, console);
#elif defined(PLATFORM_WINDOWS)
  tsekW_init(context, window, info, defaultTitle, createGlobalContext, console);
#endif
}

void tsekI_fill_context(tsekIContext* context, bool setGlobal) {
#ifdef PLATFORM_LINUX
  tsekL_fill_context(context, setGlobal);
#elif defined(PLATFORM_WINDOWS)
  tsekW_fill_context(context, setGlobal);
#endif
}

void tsekI_destroy_context(tsekIContext *context) {
#ifdef PLATFORM_LINUX
  tsekL_destroy_context(context);
#elif defined(PLATFORM_WINDOWS)
  tsekW_destroy_context(context);
#endif
}

void tsekI_create_dummy_window(tsekIWindow* window) {
#ifdef PLATFORM_LINUX
  tsekL_create_dummy_window(window);
#elif defined(PLATFORM_WINDOWS)
  tsekW_create_dummy_window(window);
#endif
}

void tsekI_create_window(tsekIWindow* window, tsekIWindowInfo *info) {
#ifdef PLATFORM_LINUX
  tsekL_create_window(window, info);
#elif defined(PLATFORM_WINDOWS)
  tsekW_create_window(window, info);
#endif
}

void tsekI_destroy_window(tsekIWindow *window) {
#ifdef PLATFORM_LINUX
  tsekL_destroy_window(window);
#elif defined(PLATFORM_WINDOWS)
  tsekW_destroy_window(window);
#endif
}

bool tsekI_get_closed_window(tsekIWindow *window) {
#ifdef PLATFORM_LINUX
  return tsekL_get_closed_window(window);
#elif defined(PLATFORM_WINDOWS)
  return tsekW_get_closed_window(window);
#endif
}

bool tsekI_update_window(tsekIWindow *window) {
#ifdef PLATFORM_LINUX
  return tsekL_update_window(window);
#elif defined(PLATFORM_WINDOWS)
  return tsekW_update_window(window);
#endif
}

double tsekI_get_time() {
#ifdef PLATFORM_LINUX
  return tsekL_get_time();
#elif defined(PLATFORM_WINDOWS)
  return tsekW_get_time();
#endif
}

double tsekI_get_fixed_time() {
#ifdef PLATFORM_LINUX
  return tsekL_get_fixed_time();
#elif defined(PLATFORM_WINDOWS)
  return tsekW_get_fixed_time();
#endif
}

void tsekI_set_time(double time) {
#ifdef PLATFORM_LINUX
  tsekL_set_time(time);
#elif defined(PLATFORM_WINDOWS)
  tsekW_set_time(time);
#endif
}

void tsekI_allocate_time(double framerate, double start, double end) {
#ifdef PLATFORM_LINUX
  tsekL_allocate_time(framerate, start, end);
#elif defined(PLATFORM_WINDOWS)
  tsekW_allocate_time(framerate, start, end);
#endif
}

bool tsekI_get_cursor_visible(tsekIWindow* window) {
#ifdef PLATFORM_LINUX
  return tsekL_get_cursor_visible(window);
#elif defined(PLATFORM_WINDOWS)
  return tsekW_get_cursor_visible(window);
#endif
}

void tsekI_set_cursor_visible(tsekIWindow* window, bool active) {
#ifdef PLATFORM_LINUX
  tsekL_set_cursor_visible(window, active);
#elif defined(PLATFORM_WINDOWS)
  tsekW_set_cursor_visible(window, active);
#endif
}

void tsekI_swap_buffers(tsekIWindow* window) {
#ifdef PLATFORM_LINUX
  tsekL_swap_buffers(window);
#elif defined(PLATFORM_WINDOWS)
  tsekW_swap_buffers(window);
#endif
}

void tsekI_get_window_param(tsekIWindow *window, tsekIWindowParam param, void *out) {
#ifdef PLATFORM_LINUX
  tsekL_get_window_param(window, param, out);
#elif defined(PLATFORM_WINDOWS)
  tsekW_get_window_param(window, param, out);
#endif
}

void tsekI_set_window_param(tsekIWindow *window, tsekIWindowParam param, void *in) {
#ifdef PLATFORM_LINUX
  tsekL_set_window_param(window, param, in);
#elif defined(PLATFORM_WINDOWS)
  tsekW_set_window_param(window, param, in);
#endif
}

void tsekI_network_init() {
#ifdef PLATFORM_LINUX
  tsekL_network_init();
#elif defined(PlATFORM_WINDOWS)
  tsekW_network_init();
#endif
}

void tsekI_network_cleanup() {
#ifdef PLATFORM_LINUX
  tsekL_network_cleanup();
#elif defined(PlATFORM_WINDOWS)
  tsekW_network_cleanup();
#endif
}

void tsekI_get_address_info(char* url, int port, tsekIAddressInfo* info) {
#ifdef PLATFORM_LINUX
  tsekL_get_address_info(url, port, info);
#elif defined(PlATFORM_WINDOWS)
  tsekW_get_address_info(url, port, info);
#endif
}

void tsekI_socket_create(tsekISocket* socket, int domain, int type, int protocol) {
#ifdef PLATFORM_LINUX
  tsekL_socket_create(socket, domain, type, protocol);
#elif defined(PlATFORM_WINDOWS)
  tsekW_socket_create(socket, domain, type, protocol);
#endif
}

void tsekI_socket_close(tsekISocket* socket) {
#ifdef PLATFORM_LINUX
  tsekL_socket_close(socket);
#elif defined(PlATFORM_WINDOWS)
  tsekW_socket_close(socket);
#endif
}


// server

void tsekI_socket_bind(tsekISocket* socket, tsekIAddressInfo* address) {
#ifdef PLATFORM_LINUX
  tsekL_socket_bind(socket, address);
#elif defined(PlATFORM_WINDOWS)
  tsekW_socket_bind(socket, address);
#endif
}

void tsekI_socket_listen(tsekISocket* socket, int backlog) {
#ifdef PLATFORM_LINUX
  tsekL_socket_listen(socket, backlog);
#elif defined(PlATFORM_WINDOWS)
  tsekW_socket_listen(socket, backlog);
#endif
}

void tsekI_socket_accept(tsekISocket* server, tsekISocket* client, tsekIAddressInfo* address) {
#ifdef PLATFORM_LINUX
  tsekL_socket_accept(server, client, address);
#elif defined(PlATFORM_WINDOWS)
  tsekW_socket_accept(server, client, address);
#endif
}


// client 

void tsekI_socket_connect(tsekISocket* socket, tsekIAddressInfo* address) {
#ifdef PLATFORM_LINUX
  tsekL_socket_connect(socket, address);
#elif defined(PlATFORM_WINDOWS)
  tsekW_socket_connect(socket, address);
#endif
}


// messaging

int tsekI_socket_send(tsekISocket* socket, char* message, int length, bool OOB, bool dontroute) {
#ifdef PLATFORM_LINUX
  return tsekL_socket_connect(socket, message, length, OOB, dontroute);
#elif defined(PlATFORM_WINDOWS)
  return tsekW_socket_connect(socket, message, length, OOB, dontroute);
#endif
  return 0;
}

int tsekI_socket_recv(tsekISocket* socket, char* message, int length, bool OOB, bool peek, bool waitall) {
#ifdef PLATFORM_LINUX
  return tsekL_socket_recv(socket, message, length, OOB, peek, waitall);
#elif defined(PlATFORM_WINDOWS)
  return tsekW_socket_recv(socket, message, length, OOB, peek, waitall);
#endif
  return 0;
}


int tsekI_socket_geterror(tsekISocket* socket) {
#ifdef PLATFORM_LINUX
  return tsekL_socket_geterror(socket);
#elif defined(PlATFORM_WINDOWS)
  return tsekW_socket_geterror(socket);
#endif
  return 0;
}


#endif
