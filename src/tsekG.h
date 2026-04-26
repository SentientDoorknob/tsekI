#ifndef TSEKG 
#define TSEKG 

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
  GLenum type;
  GLint count;
  bool normalised;
  GLint location;
} tsekAttribute;

typedef struct {
  char* name;
  GLint location;
  GLenum type;
  GLint count;
  int is_matrix;
} tsekUniform;

#define TSEKG_MAX_ATTRIBUTE_SIZE 32

typedef struct {
  tsekAttribute attributes[TSEKG_MAX_ATTRIBUTE_SIZE];
  uint32_t count;
} tsekFormat;

typedef struct {
  tsekUniform uniforms[TSEKG_MAX_ATTRIBUTE_SIZE];
  uint32_t count;
} tsekUniformCache;

typedef struct {
  uint32_t VAO;
  uint32_t VBO;
  uint32_t EBO;
  uint32_t index_count;
  tsekFormat format;
} tsekBuffer;

typedef struct {
  const char* vertex_src;
  const char* fragment_src;
  uint32_t program;

  tsekUniformCache uniform_cache;

} tsekShader;

typedef struct {
  uint32_t texture;
  uint32_t unit;
  uint32_t width, height, channels;
  int wrapS, wrapT;
  int filterMin, filterMax;
  int mipmaps;
} tsekTexture;

void tsekG_surface_init(tsekSurfaceContent*, tsekSurfaceType, tsekSurface*, bool);
void tsekG_surface_destroy(tsekSurface* surface);

void tsekG_surface_register_resize(tsekSurface* surface);
void tsekG_surface_bind(tsekSurface* surface);

tsekSurface* tsekG_get_bound_surface();


void tsekG_clear(float r, float g, float b, float a);

void tsekG_describe_buffer(tsekBuffer* buffer, tsekFormat format);
void tsekG_fill_buffer(tsekBuffer* buffer, void* vertices, uint32_t vertices_size, uint32_t* indices, uint32_t index_count);
void tsekG_render_buffer(tsekBuffer* buffer, tsekShader* shader, GLenum primitive);

void tsekG_compile_shader(tsekShader* shader);


tsekUniform* tsekG_set_uniform(tsekShader* shader, const char* name, GLenum type, GLint count, int is_matrix, void* data);
tsekUniform* tsekG_set_uniform_name(tsekShader* shader, const char* name, void* data);
void tsekG_set_uniform_handle(tsekShader* shader, tsekUniform* uniform, void* data);


void tsekG_create_texture(tsekTexture* texture, const char* bitmap, uint32_t unit, int wrapS, int wrapT, int filterMin, int filterMax);
void tsekG_bind_texture(tsekTexture* texture, tsekShader* shader, char* name);
void tsekG_set_texture_unit(tsekTexture* texture, uint32_t unit);
void tsekG_set_border_color(tsekTexture* texture, float* color);

#endif
