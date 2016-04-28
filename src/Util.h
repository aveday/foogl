#pragma once

#include <cstdlib>
#include "glm.h"

float random_f();
float random_f(float max);
float random_f(float min, float max);
int random_i(int min, int max);

vec3 getNormal(vec3 v[3]);
