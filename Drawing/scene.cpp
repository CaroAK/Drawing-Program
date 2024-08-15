#include <iostream>
#include <algorithm>
//#include <GL/glut.h> UNCOMMENT
#include "scene.h"
#include "drawing.h"
#include "graphics.h"

#define GL_SILENCE_DEPRECATION //DELETE
#include <OpenGL/gl.h> //DELETE
#include <GLUT/glut.h> //DELETE


TransformNode::TransformNode(TransformNode* p)
: parent(p), shapeNode(NULL), transform(NULL), selected(false), identifier(count)
{

}

int TransformNode::count = 0;
map<int, TransformNode*> TransformNode::idNodeTable;

TransformNode::TransformNode(TransformNode* p, ShapeNode* s, Matrix* t)
  : parent(p), shapeNode(s), transform(t), selected(false), identifier(count)
{
   count++;
   idNodeTable[identifier] = this;
}

TransformNode::~TransformNode()
{
    if (transform != nullptr) {
        delete transform;
        transform = nullptr;
    }

   for (list<TransformNode*>::iterator it = children.begin(); it != children.end(); ++it) {
      delete *it;
   }
    children.clear();

    idNodeTable.erase(identifier);
}


void TransformNode::translate(double deltaX, double deltaY)
{
    Matrix* translationMatrix = Matrix::translation(deltaX, deltaY);
    
    // update transformation matrix of the parent node
    if (transform != nullptr) {
        Matrix* newTransform = translationMatrix->multiply(transform);
        transform = newTransform;
    } else {
        transform = translationMatrix;
    }
}


void TransformNode::rotate(double theta)
{
   Matrix* rotMatrix = Matrix::rotation(theta);
   
   if (transform != nullptr) {
      Matrix* newTransform = rotMatrix->multiply(transform);
      transform = newTransform;
   }
   else {
      transform = rotMatrix;
   }

   // Prints for testing
   // for (int i = 0; i< 3; i++){
   //    for (int j = 0; j < 3; j++){
   //       cout << (*transform)[i][j] << " ";
      
   // }cout << endl;
   // }

}

void TransformNode::shear(double shearXY, double shearYX)
{
   Matrix* shearMatrix = Matrix::shearing(shearXY, shearYX);
   
   if (transform != nullptr) {
      Matrix* newTransform = shearMatrix->multiply(transform);
      transform = newTransform;
   }
   else {
      transform = shearMatrix;
   }
}

void TransformNode::scale(double scaleX, double scaleY)
{
   Matrix* scaleMatrix = Matrix::scaling(scaleX, scaleY);
   
   if (transform != nullptr) {
      Matrix* newTransform = scaleMatrix->multiply(transform);
      transform = newTransform;
   }
   else {
      transform = scaleMatrix;
   }

   // Prints for testing
   // for (int i = 0; i< 3; i++){
   //    for (int j = 0; j < 3; j++){
   //       cout << (*transform)[i][j] << " ";
      
   // }cout << endl;
   // }
}

// colorType HELPER_COLOR = YELLOW;
// colorType HIGHLIGHT_COLOR = WHITE;

void TransformNode::draw(bool displayHelpers) const
{
    bool highlight = getHighlight();
    setHighlight(selected);

    glPushName(identifier);
    gPush(transform);
    if (shapeNode)
        shapeNode->draw();
    else if (displayHelpers)
        drawTransformGismo();
    for (list<TransformNode*>::const_iterator iter = children.begin();
        iter != children.end();
        ++iter)
        (*iter)->draw(displayHelpers);
    gPop();
    setHighlight(highlight);
    glPopName();
}

TransformNode* TransformNode::getParent() const 
{ 
   return parent;
}

void TransformNode::setParent(TransformNode* p) 
{ 
   parent = p;
}

void TransformNode::changeParent(TransformNode* newParent)
{
   if (newParent == nullptr || newParent == parent) return;

   parent->removeChild(this);
   newParent->addChild(this);
   parent = newParent;
}

// Not finished implementing! Doesn't work
void TransformNode::groupObjects(set<TransformNode*>& groupMembers)
{
    TransformNode* newGroup = new TransformNode(nullptr);

    for (set<TransformNode*>::iterator it = groupMembers.begin(); it != groupMembers.end(); ++it)
    {
        TransformNode* member = *it;
        newGroup->addChild(member);
        member->setParent(newGroup);
    }

    groupMembers.clear();
}

Matrix* TransformNode::getTransform() const
{
   return transform;
}

TransformNode* TransformNode::clone() const
{
    TransformNode* newNode = new TransformNode(nullptr, shapeNode, new Matrix(*transform));

    for (list<TransformNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        TransformNode* childClone = (*it)->clone();
        newNode->addChild(childClone);
        childClone->setParent(newNode);
    }

    return newNode;
}

void TransformNode::addChild(TransformNode* child)
{
   children.push_front(child);
   child->setParent(this);
}

void TransformNode::removeChild(TransformNode* child)
{
   children.remove(child);
   child->setParent(nullptr);
}

TransformNode* TransformNode::firstChild() const
{
   if (children.empty()) {
      return nullptr;
   }
   return children.front();
}

TransformNode* TransformNode::lastChild() const
{
	if (children.empty()) {
      return nullptr;
   }
   return children.back();
}

// Not tested
TransformNode* TransformNode::nextChild(TransformNode* child) const
{
    if (children.empty()) return nullptr;

    for (list<TransformNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        if (*it == child)
        {
            if (++it != children.end())
            {
                return *it;
            }
            else
            {
                return nullptr;
            }
        }
    }
    return nullptr;
}

// Not tested
TransformNode* TransformNode::previousChild(TransformNode* child) const
{
    if (children.empty()) return nullptr;

    for (list<TransformNode*>::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        if (*it == child)
        {
            if (it != children.begin())
            {
                --it;
                return *it;
            }
            else
            {
                return nullptr;
            }
        }
    }
    return nullptr;
}


void TransformNode::select() 
{
   selected = true;
}

void TransformNode::deSelect() 
{
   selected = false;
}

TransformNode* TransformNode::nodeLookup(int identifier)
{
    return idNodeTable[identifier];
}


ShapeNode::ShapeNode(colorType c) 
{
   color = c;
}

void ShapeNode::setTransformNode(TransformNode* tn)
{
   transformNode = tn;
}

TransformNode* ShapeNode::getTransformNode()
{
	return transformNode;
}

Line::Line(double xx0, double yy0, double xx1, double yy1, colorType c)
	:ShapeNode(c), x0(xx0), y0(yy0), x1(xx1), y1(yy1)
{
}


ShapeNode* Line::clone() const
{
   return new Line(x0, y0, x1, y1, color);
}

void Line::draw() const
{
   setColor(color);
   drawLine(x0, y0, x1, y1);
}



Rectangle::Rectangle(double xx0, double yy0, double xx1, double yy1, colorType c)
	:ShapeNode(c), x0(xx0), y0(yy0), x1(xx1), y1(yy1)
{
}


ShapeNode* Rectangle::clone()  const
{
   return new Rectangle(x0, y0, x1, y1, color);
}

void Rectangle::draw() const
{
   setColor(color);
   drawRectangle(x0, y0, x1, y1);
}

Circle::Circle(double ccX, double ccY, double r, colorType c) 
	: ShapeNode(c), cX(ccX), cY(ccY), radius(r)

{}

ShapeNode* Circle::clone() const
{
   return new Circle(cX, cY, radius, color);
}

void Circle::draw() const
{
   setColor(color);
   drawCircle(cX, cY, radius);
}


Polygon::Polygon(const list<Vector*>& vs, colorType c) 
	: ShapeNode(c), vertices(vs)
{

}

Polygon::~Polygon()
{
    for (list<Vector*>::iterator iter = vertices.begin(); iter != vertices.end(); ++iter) {
        delete *iter;
    }
}

ShapeNode* Polygon::clone() const
{
   return new Polygon(vertices, color);
}

void Polygon::draw() const
{
   setColor(color);
   drawPolygon(vertices, true);
}

