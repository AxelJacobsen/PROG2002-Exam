/**
 *   Globally acessible functions,
 *	 CleanVAO, CompileShader,createObject and load_opengl_textures
 *   are mostly the teachers functions, though i have updated most of them.
 *
 *   @file     globfunc.h
 *   @author   Axel Jacobsen, Rafael P. Avalos
 */

#ifndef __GLOBFUNC_H
#define __GLOBFUNC_H

//Inclusions
#include "include.h"

// -----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
// -----------------------------------------------------------------------------

void    CleanVAO(GLuint& vao);
GLuint  CompileShader(const std::string& vertexShader, const std::string& fragmentShader);
GLuint  CreateObject(GLfloat *object, int size, const int stride, bool drawAsOutline);
GLuint  getIndices(      int out, int mid, int in);
GLuint  load_opengl_texture(const std::string& filepath, GLuint slot);

#endif