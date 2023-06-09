/**
 *   @file     include.h
 * 
 *	Includes all libraries for other files, as well as shaderss
 * 
 *   @author   Axel Jacobsen
 */
#ifndef __INCLUDE_H
#define __INDLUDE_H

 //Shader Inclusion
#include "shaders/gridShad.h"
#include "shaders/blockShad.h"
#include "shaders/deadBlockShad.h"

//Library Inclusion
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

//STD Inclusions
#include <iostream>
#include <fstream>
#include <set>
#include <cmath>
#include <vector>

#endif