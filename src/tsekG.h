#include "tsekI.h"

typedef enum {
  TSEKI_WINDOW
} tsekSurfaceType;

typedef union {
  tsekIWindow* tsekIWindow;
} tsekSurfaceContent;

typedef struct {
  tsekSurfaceType type;
  tsekSurfaceContent* content;
} tsekSurface;

typedef struct {
} tsekGLContext;

void tsekG_surface_init(tsekSurfaceContent*, tsekSurfaceType, tsekSurface*);
void tsekG_surface_destroy(tsekSurface* surface);

void tsekG_bind_context(tsekSurface*, tsekGLContext*);
void tsekG_register_resize_callback(tsekSurface* surface);

