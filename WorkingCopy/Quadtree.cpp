#include "Quadtree.h"

Quadtree::~Quadtree()
{
    if (rootNode != NULL) {
        delete rootNode;
    }
}

Quadtree::Quadtree()
{
    rootNode = NULL;
}

void Quadtree::CreateQuadtree(vector<Collider*> newObjects)
{
    if (rootNode != NULL) {
        delete rootNode;
    }
    rootNode =  new QuadNode(newObjects);
}

void Quadtree::addToTree(Collider* newVal)
{
   
        rootNode->AddToTree(newVal);
    
    
}

std::vector<Collider*> Quadtree::GetCollidableObjects(Collider* playerCol)
{
    return rootNode->getInteractions(playerCol);
}

