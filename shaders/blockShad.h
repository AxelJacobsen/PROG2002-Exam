#ifndef __BLOCKSHAD_H_
#define __BLOCKSHAD_H_

#include <string>

static const std::string blockVertexShaderSrc = R"(
#version 430 core

/** Inputs */
in vec3 bPosition;
in vec2 bTexcoord;

uniform mat4 view;
uniform mat4 projection;

/** Outputs */
out vec2 vsTexcoord;

void main()
{
vsTexcoord = bTexcoord;
gl_Position = projection * view * vec4(bPosition, 1.0f);
}
)";

static const std::string blockFragmentShaderSrc = R"(
#version 430 core

/** Inputs */
in vec2 vsTexcoord;

/** Outputs */
out vec4 color;

uniform sampler2D u_PlayerTexture;
uniform vec4 u_Color;

void main()
{
	vec4 textColorP = texture(u_PlayerTexture, vsTexcoord);
	if(textColorP.a < 0.1)
        discard;
	color = u_Color;
}
)";

#endif // __BLOCKSHAD_H_
