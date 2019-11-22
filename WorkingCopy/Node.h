#pragma once
#include <DirectXMath.h>
#include "Collider.h"
#include <vector>
using namespace std;

struct QuadNode {


    std::vector<Collider*> objects;
    QuadNode* child1;
    QuadNode* child2;
    QuadNode* child3;
    QuadNode* child4;
    int depth;
    bool isLeaf = true;
    float xPos;
    float yPos;
    float length;

    QuadNode(std::vector<Collider*> newObjects) {
        xPos = 0;
        yPos = 0;
        length = 200;
        objects = newObjects;
        depth = 0;
        if (objects.size() >= 3) {
            CreateChildNodes();
            isLeaf = false;

        }
        isLeaf = true;
    }

    ~QuadNode() {
        if (!isLeaf) {
            delete child1;
            delete child2;
            delete child3;
            delete child4;
        }
    }

    QuadNode(std::vector<Collider*> newObjects, float n_Xpos, float n_Ypos, float n_length, int n_depth) {
        xPos = n_Xpos;
        yPos = n_Ypos;
        length = n_length;
        depth = n_depth;

        GetObjectsInBounds(newObjects);

        if (objects.size() >= 3 && depth <= 5) {
            CreateChildNodes();
            isLeaf = false;
        }
        else {
            isLeaf = true;
        }

    }

    void GetObjectsInBounds(std::vector<Collider*> newObjects) {
        for (int i = 0; i < newObjects.size(); i++) {

            if (isInBounds(newObjects[i])) {
                objects.push_back(newObjects[i]);
            }
        }

    }

    bool isInBounds(Collider* obj) {
        float eXpos = obj->GetCenter().x;
        float eYpos = obj->GetCenter().y;
        if (obj->GetType() == 0) {

            float radius = obj->GetRadius();

            if (eXpos <= xPos + length + radius && eXpos >= xPos - length - radius) {
                if (eYpos <= yPos + length + radius && eYpos >= yPos - length - radius) {
                    return true;
                }

            }
        }
        else {
            float width = obj->GetSize().x;
            float heigh = obj->GetSize().y;
            if (abs(eXpos - xPos) <= width + length && abs(eYpos - yPos) <= heigh + length) {
                return true;
            }
        }
        return false;

    }


    void CreateChildNodes() {
        child1 = new QuadNode(objects, xPos + (length / 2), yPos + (length / 2), length / 2, depth + 1);
        child2 = new QuadNode(objects, xPos + (length / 2), yPos - (length / 2), length / 2, depth + 1);
        child3 = new QuadNode(objects, xPos - (length / 2), yPos + (length / 2), length / 2, depth + 1);
        child4 = new QuadNode(objects, xPos - (length / 2), yPos - (length / 2), length / 2, depth + 1);


    }


    std::vector<Collider*> getInteractions(Collider* playerCol) {
        if (isLeaf) {
            return objects;
        }

        std::vector<Collider*> interactibles;
        if (child1->isInBounds(playerCol)) {
            std::vector<Collider*> c1Obj = child1->getInteractions(playerCol);
            interactibles.insert(interactibles.end(), c1Obj.begin(), c1Obj.end());
        }
        if (child2->isInBounds(playerCol)) {
            std::vector<Collider*> c2Obj = child2->getInteractions(playerCol);
            interactibles.insert(interactibles.end(), c2Obj.begin(), c2Obj.end());
        }
        if (child3->isInBounds(playerCol)) {
            std::vector<Collider*> c3Obj = child3->getInteractions(playerCol);
            interactibles.insert(interactibles.end(), c3Obj.begin(), c3Obj.end());
        }
        if (child4->isInBounds(playerCol)) {
            std::vector<Collider*> c4Obj = child4->getInteractions(playerCol);
            interactibles.insert(interactibles.end(), c4Obj.begin(), c4Obj.end());
        }
        return interactibles;
    }


    void AddToTree(Collider* newObj) {
        if (isInBounds(newObj)) {
            if (isLeaf) {

                objects.push_back(newObj);
                if (objects.size() >= 3 && depth <= 5) {
                    CreateChildNodes();
                    isLeaf = false;
                }
            }
            else {

                child1->AddToTree(newObj);
                child2->AddToTree(newObj);
                child3->AddToTree(newObj);
                child4->AddToTree(newObj);

            }
        }

    }

};

