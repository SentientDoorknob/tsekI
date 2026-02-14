#include "tsekI.h"

#define COL(r, g, b, a) ((Color){r, g, b, a})
#define POS2(x, y) ((Pos2){x, y})
#define POS3(x, y, z) ((Pos3){x, y, z})

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
  COL,
  POS3,
  POS2
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

typedef struct {
  const char* vertex;
  const char* fragment;
  uint32_t shader;
} tsekGShader;

typedef struct {
  float r;
  float g;
  float b;
  float a;
} Color;

typedef struct {
  float x;
  float y;
  float z;
} Pos3;

typedef struct {
  float x;
  float y;
} Pos2;

void tsekG_surface_init(tsekSurfaceContent*, tsekSurfaceType, tsekSurface*, bool);
void tsekG_surface_destroy(tsekSurface* surface);

void tsekG_surface_register_resize(tsekSurface* surface);
void tsekG_surface_bind(tsekSurface* surface);

tsekSurface* tsekG_get_bound_surface();


void tsekG_clear(Color c);

void tsekG_describe_buffer(tsekGBuffer* buffer, tsekGBufferFormat format);
void tsekG_fill_buffer(tsekGBuffer* buffer, tsekGBufferData data);
void tsekG_render_buffer(tsekGBuffer* buffer, tsekGShader* shader, tsekGPrimitive primitive);

void tsekG_compile_shader(tsekGShader* shader);

