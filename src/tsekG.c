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
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
}

uint32_t Gget_size_glenum(GLenum type) {
  switch (type) {
    case GL_FLOAT: return sizeof(float);
    case GL_INT: return sizeof(int);
    case GL_BOOL: return sizeof(GLboolean);
    default: return sizeof(float);
  }
}

void tsekG_describe_buffer(tsekBuffer* buffer, tsekFormat format) {
  glGenVertexArrays(1, &buffer->VAO);
  glGenBuffers(1, &buffer->VBO);
  glGenBuffers(1, &buffer->EBO);

  glBindVertexArray(buffer->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO);

  // calculating stride
  size_t stride = 0;
  for (int i = 0; i < format.count; i++) {
    stride += format.attributes[i].count * Gget_size_glenum(format.attributes[i].type);
  }

  // registering attributes
  size_t offset = 0;
  for (int i = 0; i < format.count; i++) {
    tsekAttribute attribute = format.attributes[i];

    if (attribute.type == GL_INT || attribute.type == GL_UNSIGNED_INT) {
      glVertexAttribIPointer(
          attribute.location,
          attribute.count,
          attribute.type,
          stride,
          (void*)offset
          );
    } else {
      glVertexAttribPointer(
          attribute.location,
          attribute.count,
          attribute.type,
          attribute.normalised,
          stride,
          (void*)offset
          );
    }
    glEnableVertexAttribArray(i);

    offset += attribute.count * Gget_size_glenum(attribute.type);
  }

  glBindVertexArray(0);
  buffer->format = format;
}

void tsekG_fill_buffer(tsekBuffer* buffer, void* vertices, uint32_t vertices_size, uint32_t* indices, uint32_t indices_size) {

  glBindVertexArray(buffer->VAO);

  glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  buffer->index_count = indices_size / sizeof(uint32_t);
}

void tsekG_render_buffer(tsekBuffer* buffer, tsekShader* shader, GLenum primitive) {
  glBindVertexArray(buffer->VAO);

  glUseProgram(shader->program);

  GLenum err = glGetError();

  glDrawElements(primitive, buffer->index_count, GL_UNSIGNED_INT, 0);
}

uint32_t Gcompile_shader(GLenum type, const char* src) {
  uint32_t shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char log[1024];
    glGetShaderInfoLog(shader, sizeof(log), NULL, log);
    printf("Shader Compile Error: \n %s \n", log);
  }

  return shader;
}

void tsekG_compile_shader(tsekShader* shader) {
  uint32_t vertex = Gcompile_shader(GL_VERTEX_SHADER, shader->vertex_src);
  uint32_t fragment = Gcompile_shader(GL_FRAGMENT_SHADER, shader->fragment_src);

  shader->program = glCreateProgram();

  glAttachShader(shader->program, vertex);
  glAttachShader(shader->program, fragment);

  glLinkProgram(shader->program);

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
