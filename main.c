/* #include "src/tsekI.h"
#include "src/tsekM.h"
#include "src/tsekG.h"
#include <stdio.h>
#include <time.h>

void onKeyDown(tsekIWindow* window, tsekKeyCode key) {
}

void onKeyUp(tsekIWindow* window, tsekKeyCode key) {
}

void onKeyType(tsekIWindow* window, tsekKeyCode key) {
}

void onMouseDown(tsekIWindow* window, tsekKeyCode key) {
}

void onMouseUp(tsekIWindow* window, tsekKeyCode key) {
}

void onResize(tsekIWindow* window, uint32_t width, uint32_t height) {
}

void tsekOnResize(tsekIWindow* window, uint32_t width, uint32_t height) {
}

#include "assets.c"

void Setup(tsekIWindow* window, tsekIContext* context, tsekSurface* surface) {

  tsekI_init(context, window, NULL, L"TSEKI", true, true);

  int *keymap;
  //tsekI_get_window_param(window, KEYMAP, keymap);

  tsekCallbacks callbacks = {.keydown = onKeyDown, .keyup = onKeyUp, .keytype = onKeyType, .mbdown = onMouseDown, .mbup = onMouseUp,
  .tsegsize = tsekOnResize, .size = onResize};
  tsekI_set_window_param(window, CALLBACKS, &callbacks);

  tsekSurfaceContent surfaceContant = {.tsekIWindow = window};
  tsekG_surface_init(&surfaceContant, TSEKI_WINDOW, surface, true);
}

tsekBuffer vertexBuffer;
tsekUniform* pointTimeHandle;
tsekTexture containerTexture;
tsekTexture faceTexture;

void FillBuffers() {

  printf("%s\n", point_vertex);
  printf("%s\n", point_frag);

  tsekG_compile_shader(&pointShader);

  struct Vertex {
    float pos[2];
    float col[4];
    float tex[2];
  };

  struct Vertex vertices[] = {
    {{-0.5f, -0.5f}, {1, 0, 1, 1}, {0, 0}},
    {{-0.5f, 0.5f}, {1, 0, 1, 1}, {0, 1}},
    {{0.5f, -0.5f}, {1, 0, 1, 1}, {1, 0}},
    {{0.5f, 0.5f}, {1, 0, 1, 1}, {1, 1}}
  };

  uint32_t indices[] = {
    0, 1, 2,
    1, 2, 3
  };

  tsekFormat format = {
    .attributes = {
      {GL_FLOAT, 2, false, 0},
      {GL_FLOAT, 4, false, 1},
      {GL_FLOAT, 2, false, 2}
    },
    .count = 3
  };

  tsekG_describe_buffer(&vertexBuffer, format);
  tsekG_fill_buffer(&vertexBuffer,
      vertices, sizeof(vertices),
      indices, sizeof(indices));

  float time = 0.1;
  tsekG_set_uniform(&pointShader, "time", GL_FLOAT, 1, 0, &time);

  tsekG_create_texture(&containerTexture, container, 0, GL_CLAMP, GL_CLAMP, GL_LINEAR, GL_NEAREST);

  tsekG_create_texture(&faceTexture, face, 1, GL_CLAMP, GL_CLAMP, GL_LINEAR, GL_NEAREST);
}

void Mainloop() {
  tsekG_clear(0, 0, 0, 1);
  tsekG_render_buffer(&vertexBuffer, &pointShader, GL_TRIANGLES);

  float time = tsekI_get_time();
  tsekG_set_uniform_name(&pointShader, "time", &time);

  tsekG_bind_texture(&containerTexture, &pointShader, "container");
  tsekG_bind_texture(&faceTexture, &pointShader, "face");
}

int main() {
  tsekIContext context;
  tsekIWindow window;
  tsekSurface surface;

  Setup(&window, &context, &surface);
  FillBuffers();

  float mat1[] = {2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  float mat2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  float out[16] = {};

  tsekM_mul(out, mat1, mat2, 4);
  tsekM_display_matrix(out, 4);

  while (tsekI_get_closed_window(&window)) {
    tsekI_update_window(&window);

    Mainloop();

    tsekI_swap_buffers(&window);
  }

  tsekG_surface_destroy(&surface);
  tsekI_destroy_context(&context);
}

/
TODO: Textures
TODO: UBOs
TODO: SSBOs
 /

*/
