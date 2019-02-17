#pragma once

#include <include/glm.h>
#include <Projects/ProceduralCity/Entities/Entity.h>
#include <vector>
#include <Core/GPU/Mesh.h>

using namespace glm;

template <class T>
class QuadTree {
public:
    // Children
    QuadTree *northWest;
    QuadTree *northEast;
    QuadTree *southWest;
    QuadTree *southEast;

    // Axis-aligned bounding box stored to represent the boundaries of this
    // quad tree
    AABB boundary;

    // Buildings in this quad tree node
    std::vector<T> objects;

    // Arbitrary constant to indicate how many elements can be stored in this
    // quad tree node
    static constexpr int capacity = 4;

public:
    QuadTree<T>();
    QuadTree<T>(AABB boundary, std::vector<T> objects);
    QuadTree<T>(AABB boundary, std::vector<T> objects, QuadTree *northWest, 
        QuadTree *northEast, QuadTree *southWest, QuadTree *southEast);
    ~QuadTree<T>();
};

template<class T>
QuadTree<T>::QuadTree() {
    northWest = nullptr;
    northEast = nullptr;
    southWest = nullptr;
    southEast = nullptr;
    boundary = AABB();
    objects = std::vector<T>();
}

template<class T>
QuadTree<T>::QuadTree(AABB boundary, std::vector<T> objects) {
    northWest = nullptr;
    northEast = nullptr;
    southWest = nullptr;
    southEast = nullptr;
    this->boundary = boundary;
    this->objects  = objects;
}

template<class T>
QuadTree<T>::QuadTree(AABB boundary, std::vector<T> objects, QuadTree *northWest,
    QuadTree *northEast, QuadTree *southWest, QuadTree *southEast) {

    this->northWest = northWest;
    this->northEast = northEast;
    this->southWest = southWest;
    this->southEast = southEast;
    this->boundary = boundary;
    this->objects  = objects;
}

template <class T>
QuadTree<T>::~QuadTree() {
    delete northWest;
    delete northEast;
    delete southWest;
    delete southEast;
    objects.clear();
}