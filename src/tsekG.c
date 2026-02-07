#include "tsekI.h"
#include "tsekG.h"
#include <GL/gl.h>
#include <stdio.h>

void tsekG_surface_init(tsekSurfaceContent* surfaceContent , tsekSurfaceType type, tsekSurface* surface) {
  surface->content = surfaceContent;
  surface->type = type;
  tsekG_surface_register_resize(surface);
}

void tsekG_surface_destroy(tsekSurface* surface) {
  switch (surface->type) {
    case TSEKI_WINDOW: {
      tsekI_destroy_window(surface->content->tsekIWindow);
      break; 
    }
  }
}

void GtsekI_callback(tsekIWindow* window, uint32_t width, uint32_t height) {
  printf("TSEKG CALLBACK\n");
  glViewport(0, 0, width, height);
}

void tsekG_surface_register_resize(tsekSurface* surface) {
  switch (surface->type) {
    case TSEKI_WINDOW: {
      tsekCallbacks* callbacks;
      tsekI_get_window_param(surface->content->tsekIWindow, CALLBACKS, &callbacks);
      callbacks->tsegsize = GtsekI_callback;
      printf("Set Resize Callback\n");
      callbacks->tsegsize(NULL, 0, 0);
    }
  }
}
