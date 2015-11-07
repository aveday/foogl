#pragma once

#include <cstdlib>
#include <glm/glm.hpp>

float random_f();
float random_f(float max);
float random_f(float min, float max);
int random_i(int min, int max);

glm::vec3 getNormal(glm::vec3 v[3]);
