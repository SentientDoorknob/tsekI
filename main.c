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


#include "src/tsekI.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int client_http() {
  char* host = "pubchem.ncbi.nlm.nih.gov";
  int port = 443;

  tsekIAddressInfo addrinfo;
  tsekISocket socket;
  tsekITLSSocket tls_socket;

  tsekITLSContext context;
  tsekI_TLS_init(&context);

  tsekI_get_address_info(host, port, &addrinfo);
  tsekI_socket_create(&socket);
  tsekI_socket_connect(&socket, &addrinfo);

  tsekI_TLS_bind(&tls_socket, host, &socket, &context);

  char* req = "GET /rest/pug/compound/name/1-butene/property/MolecularFormula/TXT HTTP/1.1\r\n"
    "Host: pubchem.ncbi.nlm.nih.gov\r\n"
    "Connection: close\r\n"
    "\r\n";

  int w = tsekI_TLS_send(&tls_socket, req, strlen(req));

  printf("Sent!\n");

  while (true) {
    char buffer[4096];

    int bytes = tsekI_TLS_recv(&tls_socket, buffer, 4096);

    if (bytes <= 0) {
      break;
    }

    printf("%s\n", buffer);
  }

  tsekI_socket_close(&socket);
  tsekI_destroy_address_info(&addrinfo);

  tsekI_TLS_destroy_socket(&tls_socket, &socket);
  tsekI_TLS_destroy_context(&context);

  return 0;
}


void server() {
  tsekIAddressInfo server_address;
  tsekISocket server_socket;

  tsekI_get_address_info(NULL, 25000, &server_address);
  tsekI_socket_create(&server_socket);
  
  tsekI_socket_bind(&server_socket, &server_address);
  tsekI_socket_listen(&server_socket, 1);

  tsekIAddressInfo client_address;
  tsekISocket client_socket;

  tsekI_socket_accept(&server_socket, &client_socket, &client_address);
  tsekI_socket_set_nonblocking(&client_socket, 1);

  while (true) {
    char buffer[4096];
    int bytes = tsekI_socket_recv(&client_socket, buffer, 4096, 0, 0, 0);

    if (bytes == 0) {
      tsekI_socket_close(&client_socket);
      break;
    }

    if (bytes == -1) {
      continue;
    }

    printf("%s", buffer);

    tsekI_socket_send(&client_socket, buffer, 4096, 0, 0);
  }

  tsekI_destroy_address_info(&server_address);
  tsekI_destroy_address_info(&client_address);
}


void client(int argc, char** argv) {
  tsekIAddressInfo server_address;
  tsekISocket socket;

  char* server = "127.0.0.1";

  if (argc > 1) {
    server = argv[1];
  }

  tsekI_get_address_info(server, 25000, &server_address);
  tsekI_socket_create(&socket);
  tsekI_socket_connect(&socket, &server_address);

  char* message = "What a mess! (age)";
  tsekI_socket_send(&socket, message, strlen(message), 0, 0);

  while (true) {
    char buffer[4096];
    int bytes = tsekI_socket_recv(&socket, buffer, 4096, 0, 0, 0);

    if (bytes == 0) {
      break;
    }

    printf("%s", buffer);

    char message[4096];
    fgets(message, sizeof(message), stdin);
    tsekI_socket_send(&socket, message, 4096, 0, 0);
  }

  tsekI_socket_close(&socket);
  tsekI_destroy_address_info(&server_address);
}



int main(int argc, char** argv) {
  client(argc, argv);
}
