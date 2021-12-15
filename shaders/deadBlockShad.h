#ifndef __DEADBLOCKSHAD_H_
#define __DEADBLOCKSHAD_H_

#include <string>

static const std::string deadBlockVertexShaderSrc = R"(
#version 430 core

/** Inputs */
in vec3 dbPosition;
in vec2 dbTexture;
in vec3 dbColor;

uniform mat4 view;
uniform mat4 projection;

/** Outputs */
out vec3 vsColors;
out vec2 vsTextures;

void main()
{
gl_Position =  projection * view * vec4(dbPosition, 1.0f);
vsColors    =  dbColor;
vsTextures  =  dbTexture;
}
)";

static const std::string deadBlockFragmentShaderSrc = R"(
#version 430 core

/** Inputs */
in vec2 vsTextures;
in vec3 vsColors;

/** Outputs */
out vec4 color;

uniform sampler2D u_deadBlockTexture;

void main()
{
	vec4 textColorDB = texture(u_deadBlockTexture, vsTextures);
	if (textColorDB.a < 0.1f){
		 discard;}
	color = (textColorDB + vec4(vsColors, 1.0f))/2.0f;
}
)";

#endif // __DEADBLOCKSHAD_H_