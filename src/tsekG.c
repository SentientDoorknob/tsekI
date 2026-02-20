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

tsekSurface* tsekG_get_bound_surface() {
  return activeSurface;
}

void tsekG_clear(float r, float g, float b, float a) {
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(r, g, b, a);
}


void tsekG_describe_buffer(tsekGBuffer* buffer, tsekGBufferFormat format) {

  glGenVertexArrays(1, &buffer->VAO);
  glGenBuffers(1, &buffer->VBO);
  glGenBuffers(1, &buffer->EBO);

  glBindVertexArray(buffer->VAO);
  glBindBuffer(GL_VERTEX_ARRAY, buffer->VBO);

  // 1. parse format

  int stride = 0;
  int gl_buffer_format[format.count * 3];

  for (int i = 0; i < format.count; i++) {
    int type;
    int count;
    int size;

    switch (format.format[i]) {

      case I32:
        type = GL_INT;
        count = 1; 
        size = sizeof(uint32_t);
        break;
      case U32:
        type = GL_UNSIGNED_INT;
        count = 1;
        size = sizeof(uint32_t);
        break;
      case FLOAT:
        type = GL_FLOAT;
        count = 1;
        size = sizeof(float);
        break;
      case FLAG:
        type = GL_UNSIGNED_BYTE;
        count = 1;
        size = sizeof(u_char);
        break;
      case COL:
        type = GL_FLOAT;
        count = 4;
        size = sizeof(float);
        break;
      case POS3:
        type = GL_FLOAT;
        count = 3;
        size = sizeof(float);
        break;
      case POS2:
        type = GL_FLOAT;
        count = 2;
        size = sizeof(float);
        break;
    }

    stride += count;
    gl_buffer_format[3 * i] = type;
    gl_buffer_format[3 * i + 1] = count;
    gl_buffer_format[3 * i + 2] = size;
  }

  // 2. register format
  
  size_t offset;
  for (int i = 0; i < format.count; i++) {
    glVertexAttribPointer(i, gl_buffer_format[3 * i + 1], gl_buffer_format[3 * i], GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(i);
    offset += gl_buffer_format[3 * i + 1] * gl_buffer_format[3 * i + 2];
  }

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void tsekG_fill_buffer(tsekGBuffer* buffer, tsekGBufferData data) {

  glBindVertexArray(buffer->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO);
  glBufferData(GL_ARRAY_BUFFER, data.vertex_count, data.vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.index_count, data.indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void tsekG_render_buffer(tsekGBuffer* buffer, tsekGShader* shader, tsekGPrimitive primitive) {
  glBindVertexArray(buffer->VAO);

  uint32_t glPrimitive;
  switch (primitive) {
    case POINT:
      glPrimitive = GL_POINTS; break;
    case LINE:
      glPrimitive = GL_LINE; break;
    case TRIANGLE:
      glPrimitive = GL_TRIANGLES; break;
    case FAN:
      glPrimitive = GL_TRIANGLE_FAN; break;
  }

  glDrawElements(glPrimitive, buffer->data.index_count, GL_UNSIGNED_INT, buffer->data.indices);
}


void tsekG_compile_shader(tsekGShader* shader) {
}
