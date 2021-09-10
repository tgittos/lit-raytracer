####################################################################
# Binaries to create
####################################################################

# The game binary
EXECUTABLES = raytracer

####################################################################
# Base variables
####################################################################

# OSTYPE is not set by default on OS X
ifndef OSTYPE
	OSTYPE = $(shell uname -s)
endif

# Select compiler
CXX = g++

# Flags passed to the C++ compiler.
CXXFLAGS = -g -Wall -Wextra -fopenmp

# Source code directory
SRC_DIR = src

####################################################################
# Game related variables
####################################################################

# Include directory
IFLAGS = -Isrc -Iinclude -Iglm

# Link flags for Linux
LDFLAGS =

# Link flags for OS X, overrides Linux
ifeq "$(OSTYPE)" "Darwin"
	LDFLAGS = -lfreeimage
endif

####################################################################
# Project build targets
####################################################################

.PHONY: all clean includes
.DEFAULT: all

all: $(EXECUTABLES)

clean :
	rm -Rf bin
	rm *.o *.png

includes:
	literati tangle -o src/. lit/include

raytracer: Main.o raytracer.o
	mkdir -p bin
	$(CXX) Main.o raytracer.o $(IFLAGS) $(LDFLAGS) $(CXXFLAGS) -o bin/$@

####################################################################
# Library build targets
####################################################################

raytracer.o: includes fileformats.o shapes.o renderobjects.o Octree.o Intersection.o Light.o Material.o Ray.o Image.o Scene.o Transform.o Camera.o
	ld -r fileformats.o shapes.o renderobjects.o Octree.o Intersection.o Light.o Material.o Ray.o Image.o Scene.o Transform.o Camera.o -o raytracer.o

shapes.o: includes BoundingBox.o Sphere.o TriMesh.o
	ld -r BoundingBox.o Sphere.o TriMesh.o -o shapes.o

renderobjects.o: includes Primitive.o
	ld -r Primitive.o -o renderobjects.o

fileformats.o: includes FileFormat.o CS1841X.o
	ld -r FileFormat.o CS1841X.o -o fileformats.o

####################################################################
# Auto generated build targets
####################################################################
BoundingBox.hpp:
	literati tangle -o src/. lit/include/BoundingBox.hpp.lit

Camera.hpp:
	literati tangle -o src/. lit/include/Camera.hpp.lit

FileFormat.hpp:
	literati tangle -o src/. lit/include/FileFormat.hpp.lit

Image.hpp:
	literati tangle -o src/. lit/include/Image.hpp.lit

Intersectable.hpp:
	literati tangle -o src/. lit/include/Intersectable.hpp.lit

Intersection.hpp:
	literati tangle -o src/. lit/include/Intersection.hpp.lit

Light.hpp:
	literati tangle -o src/. lit/include/Light.hpp.lit

Material.hpp:
	literati tangle -o src/. lit/include/Material.hpp.lit

Octree.hpp:
	literati tangle -o src/. lit/include/Octree.hpp.lit

Ray.hpp:
	literati tangle -o src/. lit/include/Ray.hpp.lit

Renderer.hpp:
	literati tangle -o src/. lit/include/Renderer.hpp.lit

RenderObject.hpp:
	literati tangle -o src/. lit/include/RenderObject.hpp.lit

Scene.hpp:
	literati tangle -o src/. lit/include/Scene.hpp.lit

Shape.hpp:
	literati tangle -o src/. lit/include/Shape.hpp.lit

Transform.hpp:
	literati tangle -o src/. lit/include/Transform.hpp.lit

CS1841X.hpp:
	literati tangle -o src/. lit/include/fileformats/CS1841X.hpp.lit

Composite.hpp:
	literati tangle -o src/. lit/include/renderobjects/Composite.hpp.lit

Primitive.hpp:
	literati tangle -o src/. lit/include/renderobjects/Primitive.hpp.lit

Sphere.hpp:
	literati tangle -o src/. lit/include/shapes/Sphere.hpp.lit

TriMesh.hpp:
	literati tangle -o src/. lit/include/shapes/TriMesh.hpp.lit


BoundingBox.cpp:
	literati tangle -o src/. lit/src/BoundingBox.cpp.lit

Camera.cpp:
	literati tangle -o src/. lit/src/Camera.cpp.lit

FileFormat.cpp:
	literati tangle -o src/. lit/src/FileFormat.cpp.lit

Image.cpp:
	literati tangle -o src/. lit/src/Image.cpp.lit

Intersection.cpp:
	literati tangle -o src/. lit/src/Intersection.cpp.lit

Light.cpp:
	literati tangle -o src/. lit/src/Light.cpp.lit

Main.cpp:
	literati tangle -o src/. lit/src/Main.cpp.lit

Material.cpp:
	literati tangle -o src/. lit/src/Material.cpp.lit

Octree.cpp:
	literati tangle -o src/. lit/src/Octree.cpp.lit

Ray.cpp:
	literati tangle -o src/. lit/src/Ray.cpp.lit

Renderer.cpp:
	literati tangle -o src/. lit/src/Renderer.cpp.lit

Scene.cpp:
	literati tangle -o src/. lit/src/Scene.cpp.lit

Transform.cpp:
	literati tangle -o src/. lit/src/Transform.cpp.lit

CS1841X.cpp:
	literati tangle -o src/. lit/src/fileformats/CS1841X.cpp.lit

Primitive.cpp:
	literati tangle -o src/. lit/src/renderobjects/Primitive.cpp.lit

Sphere.cpp:
	literati tangle -o src/. lit/src/shapes/Sphere.cpp.lit

TriMesh.cpp:
	literati tangle -o src/. lit/src/shapes/TriMesh.cpp.lit

BoundingBox.o: BoundingBox.hpp BoundingBox.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/BoundingBox.cpp

Camera.o: Camera.hpp Camera.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Camera.cpp

FileFormat.o: FileFormat.hpp FileFormat.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/FileFormat.cpp

Image.o: Image.hpp Image.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Image.cpp

Intersection.o: Intersection.hpp Intersection.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Intersection.cpp

Light.o: Light.hpp Light.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Light.cpp

Main.o:  Main.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Main.cpp

Material.o: Material.hpp Material.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Material.cpp

Octree.o: Octree.hpp Octree.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Octree.cpp

Ray.o: Ray.hpp Ray.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Ray.cpp

Renderer.o: Renderer.hpp Renderer.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Renderer.cpp

Scene.o: Scene.hpp Scene.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Scene.cpp

Transform.o: Transform.hpp Transform.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Transform.cpp

CS1841X.o: CS1841X.hpp CS1841X.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/CS1841X.cpp

Primitive.o: Primitive.hpp Primitive.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Primitive.cpp

Sphere.o: Sphere.hpp Sphere.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/Sphere.cpp

TriMesh.o: TriMesh.hpp TriMesh.cpp
	$(CXX) $(IFLAGS) $(CXXFLAGS) -c -o $@ src/TriMesh.cpp



