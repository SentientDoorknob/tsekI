#include "src/tsekI.h"
#include <stdio.h>
#include <time.h>

void onKeyDown(tsekIWindow* window, tsekKeyCode key) {
}

void onKeyUp(tsekIWindow* window, tsekKeyCode key) {
}

void onKeyType(tsekIWindow* window, tsekKeyCode key) {
}

void onMouseDown(tsekIWindow* window, tsekKeyCode key) {
}

void onMouseUp(tsekIWindow* window, tsekKeyCode key) {
}

void onResize(tsekIWindow* window, uint32_t width, uint32_t height) {
  printf("Width: %d Height: %d\n", width, height);
}

void tsekOnResize(tsekIWindow* window, uint32_t width, uint32_t height) {
  printf("Width: %d Height: %d\n", width, height);
}

int main() {
  tsekIContext context;
  tsekIWindow *window = (tsekIWindow *)malloc(sizeof(tsekIWindow));

  tsekI_init(&context, window, NULL, True, True);

  int *keymap;
  tsekI_get_window_param(window, KEYMAP, &keymap);

  tsekCallbacks callbacks = {.keydown = onKeyDown, .keyup = onKeyUp, .keytype = onKeyType, .mbdown = onMouseDown, .mbup = onMouseUp,
  .tsegsize = tsekOnResize, .size = onResize};
  tsekI_set_window_param(window, CALLBACKS, &callbacks);

  while (tsekI_get_closed_window(window)) {
    tsekI_update_window(window);

    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(0.0f, 1.0f, 0.25f, 1.0f);

    tsekI_swap_buffers(window);
  }

  tsekI_destroy_window(window);
  tsekI_destroy_context(&context);
}
