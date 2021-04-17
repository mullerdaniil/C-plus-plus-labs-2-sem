#pragma once

#include <vector>

// colors
extern float rCOLOR_BLACK[];
extern float rCOLOR_GRAY[];
extern float rCOLOR_WHITE[];
extern float rCOLOR_RED[];
extern float rCOLOR_GREEN[];
extern float rCOLOR_BLUE[];
extern float rCOLOR_YELLOW[];
extern float rCOLOR_ORANGE[];


// drawing
void rDrawVectorWithTriangles(std::vector<float>& coordinates, float color[]);
void rDrawVectorWithQuadrangles(std::vector<float>& coordinates, std::vector<float>& colors);
void rMoveVector(std::vector<float>& coordinates, float dx, float dy, float dz);
void r3DrawQuadrangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, float color[]);
void r3DrawParallelCube(float x, float y, float z, float length, float color[]);
void r3DrawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float color[]);
void r3DrawZeroYRectangle(float x1, float z1, float x2, float z2, float color[]);
