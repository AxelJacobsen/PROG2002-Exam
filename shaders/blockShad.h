#ifndef __BLOCKSHAD_H_
#define __BLOCKSHAD_H_

#include <string>

static const std::string blockVertexShaderSrc = R"(
#version 430 core

/** Inputs */
in vec3 bPosition;
in vec3 bColor;

uniform mat4 view;
uniform mat4 projection;

/** Uniform */
layout(location=0) uniform mat4 u_TransformationMat = mat4(1);

void main()
{
gl_Position =  projection * view  * u_TransformationMat * vec4(bPosition, 1.0f);
}
)";

static const std::string blockFragmentShaderSrc = R"(
#version 430 core

/** Outputs */
out vec4 color;

uniform vec4 u_Color;

void main()
{
color = u_Color;
}
)";

#endif // __BLOCKSHAD_H_