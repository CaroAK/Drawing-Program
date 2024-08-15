#include "graphics.h"
#include "drawing.h"
#include <iostream>

//#include <GL/glut.h> - UNCOMMENT

#define GL_SILENCE_DEPRECATION //DELETE
#include <OpenGL/gl.h> //DELETE
#include <GLUT/glut.h> //DELETE

#include <cfloat>
#include <cmath>

using namespace std;

const colorType HIGHLIGHT_COLOR = WHITE;

const double TRANSFORM_GISMO_SCALE = 25.0;


bool highlight = false;

bool getHighlight()
{
  return highlight;
}

void setHighlight(bool h)
{
  if (h) setColor(HIGHLIGHT_COLOR);
  highlight = h;
}

colorType getHighlightColor() {
    return HIGHLIGHT_COLOR; 
}


colorType intToColor(int item)
{
  switch ( item ) {
    case 0 : return BLACK   ;
    case 1 : return BLUE    ;
    case 2 : return GREEN   ;
    case 3 : return CYAN    ;
    case 4 : return RED     ;
    case 5 : return MAGENTA ;
    case 6 : return YELLOW  ;
    case 7 : return WHITE   ;
    case 8 : return GRAY    ;
   default : return BLACK   ;
  }
}


void setColor( colorType color )
{
  if (!highlight)
    switch ( color )
    {
      case BLACK   : glColor3f( 0.0, 0.0, 0.0 ); break;
      case BLUE    : glColor3f( 0.0, 0.0, 1.0 ); break;
      case GREEN   : glColor3f( 0.0, 1.0, 0.0 ); break;
      case CYAN    : glColor3f( 0.0, 1.0, 1.0 ); break;
      case RED     : glColor3f( 1.0, 0.0, 0.0 ); break;
      case MAGENTA : glColor3f( 1.0, 0.0, 1.0 ); break;
      case YELLOW  : glColor3f( 1.0, 1.0, 0.0 ); break;
      case WHITE   : glColor3f( 1.0, 1.0, 1.0 ); break;
      case GRAY    : glColor3f( 0.5, 0.5, 0.5 ); break;
    }
}


// Stack of matrices that represent transformations
TransformStack transformStack;

TransformStack::TransformStack()
{

}

void TransformStack::push(Matrix* transform)
{
  matrixStack.push(transform);
}

void TransformStack::pop()
{
  if (!matrixStack.empty()) {
    Matrix* topMatrix = matrixStack.top();
    // delete topMatrix;
    matrixStack.pop();
  }
}

Matrix* TransformStack::top()
{
  if (matrixStack.empty()) {
    return nullptr;
  }
  else {
    return matrixStack.top();
  }
}

// void gPush(Matrix* transform)
// {
//    Matrix* top = transformStack.top();
//    if (top == nullptr) {
//        top = new Matrix();
//    }
//    Matrix* mult = top->multiply(transform);
//    transformStack.push(mult);
// }

void gPush(Matrix* transform)
{
  Matrix* top = transformStack.top();
  if (top != nullptr) {
    Matrix* mult = top->multiply(transform);
    transformStack.push(mult);
  }
  else {
    transformStack.push(transform);
  }
}

void gPop()
{
  transformStack.pop();
}

void drawLine(double x0, double y0, double x1, double y1)
{
  // cout << "drawing line" << endl;
  Matrix* topMatrix = transformStack.top();

  if (topMatrix != nullptr) {
    Vector* p0 = new Vector(x0, y0);
    Vector* p1 = new Vector(x1, y1);

    Vector* v0 = topMatrix->multiply(p0);
    Vector* v1 = topMatrix->multiply(p1);
    drawLine(v0, v1);
  }
  else {
    Vector* p0 = new Vector(x0, y0);
    Vector* p1 = new Vector(x1, y1);
    drawLine(p0, p1);
  }
  
}

void drawLine(Vector* p0, Vector* p1)
{
  // Multiply by the top of transformStack to get World Coordinates
  // Vector* v1 = transformStack.top();
  double x0 = (*p0)[0];
  double y0 = (*p0)[1];
  double x1 = (*p1)[0];
  double y1 = (*p1)[1];

  glBegin(GL_LINES);
  glVertex2d(x0, y0);
  glVertex2d(x1, y1);
  glEnd();
  glFlush();
}

void drawRectangle(double x0, double y0, double x1, double y1)
{
    Matrix* topMatrix = transformStack.top();

    Vector* p0 = new Vector(x0, y0);
    Vector* p1 = new Vector(x0, y1);
    Vector* p2 = new Vector(x1, y1);
    Vector* p3 = new Vector(x1, y0);

    if (topMatrix != nullptr) {
        p0 = topMatrix->multiply(p0);
        p1 = topMatrix->multiply(p1);
        p2 = topMatrix->multiply(p2);
        p3 = topMatrix->multiply(p3);
    }

    drawLine(p0, p1);
    drawLine(p1, p2);
    drawLine(p2, p3);
    drawLine(p3, p0);

    delete p0;
    delete p1;
    delete p2;
    delete p3;
}

void drawCircle(double x0, double y0, double x1, double y1)
{
    double radius = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    drawCircle(x0, y0, radius);
}

// Rotation of circle wasn't working here:

// void drawCircle(double cX, double cY, double radius)
// {
//     const double pi = 3.14159265358979323846;
//     const int segments = 100;
//     double angleIncrement = 2.0 * pi / segments;

//     // Get the top transformation matrix
//     Matrix* topMatrix = transformStack.top();

//     // Begin drawing the circle
//     glBegin(GL_LINE_LOOP);
//     for (int i = 0; i < segments; ++i)
//     {
//         double angle = i * angleIncrement;
//         double x = radius * cos(angle);
//         double y = radius * sin(angle);

//         // Apply transformation to the point
//         if (topMatrix != nullptr) {
//             Vector point(x, y);
//             Vector transformedPoint = *topMatrix->multiply(&point);
//             glVertex2d(transformedPoint[0] + cX, transformedPoint[1] + cY);
//         } else {
//             glVertex2d(x + cX, y + cY);
//         }
//     }
//     glEnd();
// }

void drawCircle(double cX, double cY, double radius) {
    Matrix* currentTransform = transformStack.top();
    const double pi = 3.14159265358979323846;

    if (currentTransform == nullptr) {
        const int segments = 100;
        double angleIncrement = 2.0 * pi / segments;

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; ++i) {
            double angle = i * angleIncrement;
            double x = cX + radius * cos(angle);
            double y = cY + radius * sin(angle);
            glVertex2d(x, y);
        }
        glEnd();
        return;
    }

    Vector center(cX, cY);
    Vector* transformedCenter = currentTransform->multiply(&center);

    double scaleX = sqrt((*currentTransform)[0][0] * (*currentTransform)[0][0] + (*currentTransform)[0][1] * (*currentTransform)[0][1]);
    double scaleY = sqrt((*currentTransform)[1][0] * (*currentTransform)[1][0] + (*currentTransform)[1][1] * (*currentTransform)[1][1]);
    radius *= sqrt(scaleX * scaleY);

    int segments = 100; 
    double angleIncrement = 2.0 * pi / segments;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        double angle = angleIncrement * i;
        double x = (*transformedCenter)[0] + radius * cos(angle);
        double y = (*transformedCenter)[1] + radius * sin(angle);
        glVertex2d(x, y);
    }
    glEnd();
}

void drawPolygon(const list<Vector*>& vertices, bool close)
{
    if (vertices.empty()) return;

    Matrix* topMatrix = transformStack.top();

    glBegin(GL_LINE_STRIP);
    for (list<Vector*>::const_iterator iter = vertices.begin(); iter != vertices.end(); ++iter)
    {
        Vector* vertex = *iter;
        if (topMatrix != nullptr) {
            vertex = topMatrix->multiply(vertex);
        }
        double* vertexData = vertex->getData();
        glVertex2d(vertexData[0], vertexData[1]);
    }
    glEnd();

    if (close && !vertices.empty())
    {
        glBegin(GL_LINE_LOOP);
        for (list<Vector*>::const_iterator iter = vertices.begin(); iter != vertices.end(); ++iter)
        {
            Vector* vertex = *iter;
            if (topMatrix != nullptr) {
                vertex = topMatrix->multiply(vertex);
            }
            double* vertexData = vertex->getData();
            glVertex2d(vertexData[0], vertexData[1]);
        }
        glEnd();
    }

    glFlush();
}

void drawTransformGismo()
{
  double halfSize = TRANSFORM_GISMO_SCALE/2.0;
  drawRectangle(-halfSize,-halfSize,halfSize,halfSize);
}
