#include "tsekI.h"
#include "tsekG.h"
#include <stdio.h>
#include <string.h>

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

    printf("%d %d %d %d\n", attribute.type, attribute.count, attribute.normalised, attribute.location);

    if (attribute.type == GL_INT || attribute.type == GL_UNSIGNED_INT) {
      printf("%d\n", GL_INT);
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
    glEnableVertexAttribArray(attribute.location);

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

void tsekG_set_uniform_handle(tsekShader* shader, tsekUniform* handle, void* data) {

  //printf("Name %s | Location %d | Int: %d | Type: %d | Count: %d | Matrix?: %d\n", handle->name, handle->location, *(int*)data, handle->type, handle->count, handle->is_matrix);

  glUseProgram(shader->program);

  GLint loc = handle->location;
  if (loc == -1) return;

  if (handle->is_matrix) {
    switch (handle->count) {
      case 4: glUniformMatrix2fv(loc, 1, GL_FALSE, (float*)data); break;
      case 9: glUniformMatrix3fv(loc, 1, GL_FALSE, (float*)data); break;
      case 16: glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)data); break;
      default: fprintf(stderr, "Invalid Matrix Count: %d\n", handle->count);
    }
    return;
  }

  if (handle->type == GL_FLOAT) {
    switch (handle->count) {
      case 1: glUniform1fv(loc, 1, (float*)data); break;
      case 2: glUniform2fv(loc, 1, (float*)data); break;
      case 3: glUniform3fv(loc, 1, (float*)data); break;
      case 4: glUniform4fv(loc, 1, (float*)data); break;
      default: fprintf(stderr, "Invalid Float Vector Count: %d\n", handle->count);
    }
    return;
  }

  if (handle->type == GL_INT || handle->type == GL_UNSIGNED_INT) {
    switch (handle->count) {
      case 1: glUniform1iv(loc, 1, (int*)data); printf("Here!\n"); break;
      case 2: glUniform2iv(loc, 1, (int*)data); break;
      case 3: glUniform3iv(loc, 1, (int*)data); break;
      case 4: glUniform4iv(loc, 1, (int*)data); break;
      default: fprintf(stderr, "Invalid Integer Vector Count: %d\n", handle->count);
    }
    return;
  }

  glUseProgram(0);
}

tsekUniform* tsekG_set_uniform_name(tsekShader* shader, const char* name, void* data) {
  for (int i = 0; i < shader->uniform_cache.count; i++) {
    if (strcmp(name, shader->uniform_cache.uniforms[i].name) == 0) {

      tsekUniform* handle = &shader->uniform_cache.uniforms[i];
      tsekG_set_uniform_handle(shader, handle, data);
      return handle;
    }
  }

  fprintf(stderr, "No cached uniform with name '%s'\n", name);
  return NULL;
}


tsekUniform* tsekG_set_uniform(tsekShader* shader, const char* name, GLenum type, GLint count, int is_matrix, void* data) {

  // check cache for existing uniform 

  for (int i = 0; i < shader->uniform_cache.count; i++) {
    if (strcmp(name, shader->uniform_cache.uniforms[i].name) == 0) {

      tsekUniform* handle = &shader->uniform_cache.uniforms[i];
      handle->type = type;
      handle->count = count;
      handle->is_matrix = is_matrix;

      tsekG_set_uniform_handle(shader, handle, data);
      return handle;
    }
  }

  // name is unrecognised; get location

  GLint loc = glGetUniformLocation(shader->program, name);
  if (loc == -1) {
    fprintf(stderr, "Warning: Uniform '%s' not found\n", name);
  }

  // cache it 
  if (shader->uniform_cache.count >= TSEKG_MAX_ATTRIBUTE_SIZE) {
    fprintf(stderr, "Warning: Uniform cache capacity of %d filled. Not caching uniform '%s'", TSEKG_MAX_ATTRIBUTE_SIZE, name);
  } else {
    shader->uniform_cache.uniforms[shader->uniform_cache.count] = (tsekUniform){
      .name = name, .location = loc, .type = type, .count = count, .is_matrix = is_matrix};
    shader->uniform_cache.count++;
  }

  tsekUniform* handle = &shader->uniform_cache.uniforms[shader->uniform_cache.count -1];
  tsekG_set_uniform_handle(shader, handle, data);
  return handle;
}

u_char* Gparse_bitmap(const char* bitmap, uint32_t* out_size, uint32_t* width, uint32_t* height, uint32_t* nr_channels) {
  uint32_t pixel_data_offset;
  memcpy(&pixel_data_offset, bitmap + 0x0A, 4);

  memcpy(width, bitmap + 0x12, 4);
  memcpy(height, bitmap + 0x16, 4);

  u_short pixel_size_bits;
  memcpy(&pixel_size_bits, bitmap + 0x1C, 2);
  uint32_t byte_count = pixel_size_bits / 8;
  *nr_channels = byte_count;

  uint32_t row_size = ((*width * pixel_size_bits + 31) / 32) * 4;
  uint32_t buffer_size = row_size * *height;

  *out_size = (uint32_t)(*width * *height * byte_count);
  u_char* out = (u_char*)malloc(*out_size);

  for (int r = 0; r < *height; r++) {
    int src_row = (*height - 1 - r);
    for (int c = 0; c < *width; c++) {
      u_char* pixel = out + (r * *width + c) * byte_count;
      memcpy(pixel,
          bitmap + pixel_data_offset + src_row * row_size + c * byte_count,
          byte_count);

      if (byte_count >= 3) {
        u_char temp = pixel[0];
        pixel[0] = pixel[2];
        pixel[2] = temp;
      }
    }
  }

  return out;
}

void tsekG_create_texture(tsekTexture *texture, const char *bitmap, uint32_t unit, int wrapS, int wrapT, int filterMin, int filterMax) {
  texture->unit = unit;
  texture->wrapS = wrapS;
  texture->wrapT = wrapT;
  texture->filterMax = filterMax;
  texture->filterMin = filterMin;

  uint32_t width, height, out_size, nr_channels;
  u_char* raw_texture = Gparse_bitmap(bitmap, &out_size, &width, &height, &nr_channels);

  texture->width = width;
  texture->height = height;
  texture->channels = nr_channels;

  glGenTextures(1, &texture->texture);

  uint32_t format = nr_channels == 3 ? GL_RGB : GL_RGBA;
  glBindTexture(GL_TEXTURE_2D, texture->texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrapT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->filterMin);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->filterMax);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, raw_texture);
  glGenerateMipmap(GL_TEXTURE_2D);

  free(raw_texture);
}

void tsekG_set_texture_unit(tsekTexture* texture, uint32_t unit) {
  texture->unit = unit;
}

void tsekG_bind_texture(tsekTexture* texture) {
  glActiveTexture(GL_TEXTURE0 + texture->unit);
  glBindTexture(GL_TEXTURE_2D, texture->texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrapT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->filterMin);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->filterMax);
}

void tsekG_set_border_color(tsekTexture* texture, float* color) {
  glBindTexture(GL_TEXTURE_2D, texture->texture);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
}
