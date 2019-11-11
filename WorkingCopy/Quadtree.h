

#pragma once
#include "Node.h"
#include <vector>
#include <DirectXMath.h>




using namespace std;
class Quadtree
{

public:

    ~Quadtree();

    Quadtree();
    void CreateQuadtree(std::vector<Collider*> newObjects);

    void addToTree(Collider* newVal);
    std::vector<Collider*> GetCollidableObjects(Collider* playerCol);

private:
    QuadNode* rootNode;

    

};



