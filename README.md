# 2D Drawing Program

## Overview

This repository showcases a 2D drawing program that allows users to create and manipulate pictures composed of lines, rectangles, circles, and polygons using a scene graph data structure. Key features of the program include:

* Drawing and deleting shapes using mouse input
* Shape type and color customization
* Shape node selection and transformation (translation, rotation, shearing, scaling)
* Grouping and parenting nodes within the scene graph

## Class Guide

* TransformNode: Represents nodes in the scene graph.
* ShapeNode: Abstract class for shapes attached to transform nodes.
* ShapeNode Subclasses: Line, Rectangle, Circle, Polygon.
* Vector: Represents 2D points in homogeneous coordinates.
* Matrix: Implements 2D transformations in homogeneous coordinates.
* TransformStack: Maintains a stack of transformation matrices.
* Graphics Package: Provides routines for drawing shapes in world coordinates.

## Demo
<img width="400" alt="Screenshot 2024-08-25 at 9 23 10 PM" src="https://github.com/user-attachments/assets/5945f2b3-683b-4b9d-a1d7-6c18161284fa">

[Demo Video](https://youtu.be/Nooe58z5YPg)

## Setup

Open the provided MS Visual Studio project to access the files and run the solution. It can also be run from Visual Studio code by running `g++ -framework GLUT -framework OpenGL ././drawing.cpp ././graphics.cpp ././matrix.cpp ././scene.cpp && ././a.out` in the Drawing terminal. A mouse is needed for the user interface.
