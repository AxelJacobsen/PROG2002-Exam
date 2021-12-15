#ifndef __GRIDSHAD_H_
#define __GRIDSHAD_H_

#include <string>

static const std::string gridVertexShaderSrc = R"(
#version 430 core

/** Inputs */
in vec3 gPosition;
in vec2 gTexcoord;

/** Uniform */
uniform mat4 view;
uniform mat4 projection;

/** Outputs */
out vec2 gridTexcoord;

void main()
{
//We multiply our matrices with our position to change the positions of vertices to their final destinations.
gridTexcoord = gTexcoord;
gl_Position =  projection * view * vec4(gPosition, 1.0f); //
}
)";

static const std::string gridFragmentShaderSrc = R"(
#version 430 core

/** Inputs */
in vec2 gridTexcoord;

/** Outputs */
out vec4 color;

uniform sampler2D u_gridTexture;
uniform vec4 u_Color;
uniform float boolText;

void main()
{
vec4 textColorG = texture(u_gridTexture, gridTexcoord);
if (0.5f < boolText){
	color = textColorG;
} else if (boolText < 1.0f){color = u_Color;}
else {color = textColorG;}
}
)";


#endif // __GRIDSHAD_H_