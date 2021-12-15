#ifndef __DEADBLOCKSHAD_H_
#define __DEADBLOCKSHAD_H_

#include <string>

static const std::string deadBlockVertexShaderSrc = R"(
#version 430 core

/** Inputs */
in vec3 dbPosition;
in vec3 dbColor;

uniform mat4 view;
uniform mat4 projection;

/** Outputs */
out vec3 vsColors;

void main()
{
vsColors    =  dbColor;
gl_Position =  projection * view * vec4(dbPosition, 1.0f);
}
)";

static const std::string deadBlockFragmentShaderSrc = R"(
#version 430 core

/** Inputs */
in vec3 vsColors;

/** Outputs */
out vec4 color;

void main()
{
	color = vec4(vsColors, 1.0f);
}
)";

#endif // __DEADBLOCKSHAD_H_