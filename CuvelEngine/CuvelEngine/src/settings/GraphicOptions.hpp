#pragma once

// --------------------------------------------------------------------------
//#define USE_OPENGL
#define USE_OPENGL
// --------------------------------------------------------------------------

// General options
#define GRAPHICSDEBUG true
#define RESIZABLE true
#define CLEARCOLOR 0.1f, 0.1f, 0.3f

// Projection
#define INITIAL_FOV 90.0f
#define NEAR_PLANE 0.001f
#define FAR_PLANE 1000.0f

// Shaders
#define VERTEX_LOCATION "src/graphics/shaders/vertex.glsl"
#define FRAGMENT_LOCATION "src/graphics/shaders/fragment.glsl"
#define GEOMETRY_LOCATION ""

// OPENGL
#define GLSTOPERROR true

#define GLMAYOR 4
#define GLMINOR 5
#define GLSAMPLES 16