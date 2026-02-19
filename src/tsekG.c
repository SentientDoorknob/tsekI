#include "tsekI.h"
#include "tsekG.h"
#include <stdio.h>

tsekSurface* activeSurface;

void tsekG_surface_bind(tsekSurface* surface) {
  activeSurface = surface;
}

void tsekG_surface_init(tsekSurfaceContent* surfaceContent , tsekSurfaceType type, tsekSurface* surface, bool bind) {
  surface->content = surfaceContent;
  surface->type = type;
  tsekG_surface_register_resize(surface);

  if (bind) {
    tsekG_surface_bind(surface);
  }
}

void tsekG_surface_destroy(tsekSurface* surface) {
  switch (surface->type) {
    case TSEKI_WINDOW: {
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
    }
  }
}

void tsekG_clear(float r, float g, float b, float a) {
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(r, g, b, a);
}

