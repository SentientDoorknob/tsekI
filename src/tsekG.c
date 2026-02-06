#include "tsekI.h"
#include "tsekG.h"


void tsekG_surface_init(tsekSurfaceContent* surfaceContent , tsekSurfaceType type, tsekSurface* surface) {
  surface->content = surfaceContent;
  surface->type = type;
}

void tsekG_surface_destroy(tsekSurface* surface) {
  switch (surface->type) {
    case TSEKI_WINDOW: {
      tsekI_destroy_window(surface->content->tsekIWindow);
      break; 
    }
  }
}

void tsekG_regi

