// include/navi/gfx/GLTypes.h
#pragma once

#if defined(NAVI_USE_GLAD) || !defined(NAVI_GL_HEADER_CUSTOM)
  #include <glad/glad.h>
#else
  #if defined(__APPLE__)
    #include <OpenGL/gl3.h>
  #else
    #include <GL/gl.h>
  #endif
#endif
