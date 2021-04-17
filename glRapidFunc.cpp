#include "glRapidFunc.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <vector>



float rCOLOR_BLACK[]    = { 0.0f, 0.0f, 0.0f };
float rCOLOR_GRAY[]     = { 0.7f, 0.7f, 0.7f };
float rCOLOR_WHITE[]    = { 1.0f, 1.0f, 1.0f };
float rCOLOR_RED[]      = { 1.0f, 0.0f, 0.0f };
float rCOLOR_GREEN[]    = { 0.0f, 1.0f, 0.0f };
float rCOLOR_BLUE[]     = { 0.0f, 0.0f, 1.0f };
float rCOLOR_YELLOW[]   = { 1.0f, 1.0f, 0.0f };
float rCOLOR_ORANGE[]   = { 1.0f, 0.5f, 0.0f };



void rDrawVectorWithTriangles(std::vector<float>& coordinates, float color[])
{
    glColor3f(color[0], color[1], color[2]);
    for (int i = 0; i < coordinates.size(); i += 9)
    {
        glBegin(GL_TRIANGLES);
        glVertex3f(coordinates[i], coordinates[i + 1], coordinates[i + 2]);
        glVertex3f(coordinates[i + 3], coordinates[i + 4], coordinates[i + 5]);
        glVertex3f(coordinates[i + 6], coordinates[i + 7], coordinates[i + 8]);
        glEnd();
    }
}

void rDrawVectorWithQuadrangles(std::vector<float>& coordinates, std::vector<float>& colors)
{
    int currentColorIndex = 0;
    for (int i = 0; i < coordinates.size(); i += 12)
    {
        float currentColor[3] = { colors[currentColorIndex], colors[currentColorIndex + 1], colors[currentColorIndex + 2] };
        r3DrawQuadrangle(coordinates[i], coordinates[i + 1], coordinates[i + 2], coordinates[i + 3], coordinates[i + 4], coordinates[i + 5],
            coordinates[i + 6], coordinates[i + 7], coordinates[i + 8], coordinates[i + 9], coordinates[i + 10], coordinates[i + 11], currentColor);

        currentColorIndex += 3;
    }


}

void rMoveVector(std::vector<float>& coordinates, float dx, float dy, float dz)
{
    for (int i = 0; i < coordinates.size(); i += 3)
    {
        coordinates[i] += dx;
        coordinates[i + 1] += dy;
        coordinates[i + 2] += dz;
    }
}






void r3DrawQuadrangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, float color[])
{
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glVertex3f(x4, y4, z4);
    glEnd();
}


void r3DrawParallelCube(float x, float y, float z, float length, float color[])
{
    r3DrawQuadrangle(x, y, z, x + length, y, z, x, y, z + length, x + length, y, z + length, color);
    r3DrawQuadrangle(x, y + length, z, x + length, y + length, z, x, y + length, z + length, x + length, y + length, z + length, color);
    r3DrawQuadrangle(x, y, z, x + length, y, z, x, y + length, z, x + length, y + length, z, color);
    r3DrawQuadrangle(x, y, z + length, x + length, y, z + length, x, y + length, z + length, x + length, y + length, z + length, color);
    r3DrawQuadrangle(x, y, z, x, y + length, z, x, y, z + length, x, y + length, z + length, rCOLOR_BLACK);
    r3DrawQuadrangle(x + length, y, z, x + length, y + length, z, x + length, y, z + length, x + length, y + length, z + length, color);
}

void r3DrawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float color[])
{
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_TRIANGLES);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glEnd();
}



void r3DrawZeroYRectangle(float x1, float z1, float x2, float z2, float color[])
{
    r3DrawTriangle(x1, 0, z1, x1, 0, z2, x2, 0, z1, color);
    r3DrawTriangle(x2, 0, z2, x1, 0, z2, x2, 0, z1, color);
}


