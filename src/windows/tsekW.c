
#include "tsekW.h"
#include <stdio.h>

void tsekW_init(tsekIContext* context, tsekIWindow* window, tsekIWindowInfo* info, bool createGlobalContext, bool console) {
  printf("Hello TsekW");
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

