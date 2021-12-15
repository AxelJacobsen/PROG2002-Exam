#ifndef __BLOCKSHAD_H_
#define __BLOCKSHAD_H_

#include <string>

static const std::string blockVertexShaderSrc = R"(
#version 430 core

/** Inputs */
in vec3 bPosition;
in vec2 btext;

uniform mat4 view;
uniform mat4 projection;

/** Out */
out vec2 gridTexcoord;

/** Uniform */
layout(location=0) uniform mat4 u_TransformationMat = mat4(1);

void main()
{
gridTexcoord = btext;
gl_Position =  projection * view  * u_TransformationMat * vec4(bPosition, 1.0f);
}
)";

static const std::string blockFragmentShaderSrc = R"(
#version 430 core

/** Inputs */
in vec2 gridTexcoord;

/** Outputs */
out vec4 color;

uniform sampler2D u_BlockTexture;
uniform vec4 u_Color;

void main()
{
vec4 textColorG = texture(u_BlockTexture, gridTexcoord);

color = textColorG;
}
)";

#endif // __BLOCKSHAD_H_