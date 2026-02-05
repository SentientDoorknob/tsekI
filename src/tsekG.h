#include "tsekI.h"
#include <X11/X.h>

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
  int* vertices;
  int* indices;
  int vertex_count;
  int index_count;
} tsekGBufferData;

typedef struct {
  int VBO;
  int VAO;
  int EBO;
  tsekGBufferData data;
} tsekGBuffer;

typedef enum {
  I32 = 0,
  U32,
  I64,
  U64,
  FLOAT,
  BOOL,
} tsekGBufferDataType;

typedef struct {
  tsekGBufferDataType* format;
  int count;
} tsekGBufferFormat;

typedef enum {
  POINT = 0,
  LINE,
  TRIANGLE,
  FAN,
} tsekGPrimitive;

void tsekG_surface_init(tsekSurfaceContent*, tsekSurfaceType, tsekSurface*);
void tsekG_surface_destroy(tsekSurface* surface);

void tsekG_register_resize_callback(tsekSurface* surface);
void tsekG_bind_surface(tsekSurface* surface);

tsekSurface* tsekG_get_bound_surface();


void tsekG_clear();

void tsekG_describe_buffer(tsekGBuffer* buffer, tsekGBufferFormat format);
void tsekG_fill_buffer(tsekGBuffer* buffer, tsekGBufferData data);
void tsekG_render_buffer(tsekGBuffer* buffer, tsekGPrimitive primitive);



