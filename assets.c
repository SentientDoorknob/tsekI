#include "src/tsekG.h"

const char point_vertex[] = {
#embed "assets/point.vert"
  , 0
};

const char point_frag[] = {
#embed "assets/point.frag"
  , 0
};

tsekShader pointShader = {
  .vertex_src = point_vertex,
  .fragment_src = point_frag
};

const char bitmap[] = {
#embed "assets/texture.bmp"
  , 0
};

const char wall[] = {
#embed "assets/wall.bmp"
  , 0
};

const char face[] = {
#embed "assets/awesomeface.bmp"
  , 0
};

const char container[] = {
#embed "assets/container.bmp"
  , 0
};
