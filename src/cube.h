#pragma once
#include "assets.h"

vec3 cube_positions[]{
    {-0.5, -0.5, -0.5}, {-0.5, -0.5,  0.5},
    {-0.5,  0.5, -0.5}, {-0.5,  0.5,  0.5},
    { 0.5, -0.5, -0.5}, { 0.5, -0.5,  0.5},
    { 0.5,  0.5, -0.5}, { 0.5,  0.5,  0.5}};
int cube_indices[]{
    0,1,2, 1,3,2,  0,2,6, 0,6,4,  2,3,7, 2,7,6,
    4,6,7, 4,7,5,  1,7,3, 1,5,7,  0,4,1, 1,4,5};
vec3 cube_normals[]{
    {-1, 0, 0},    { 0, 0,-1},    { 0, 1, 0},
    { 1, 0, 0},    { 0, 0, 1},    { 0,-1, 0}};

MeshDef cube_def = { 36, cube_positions, cube_indices, cube_normals };
MeshDef *CUBE = &cube_def;

