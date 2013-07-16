/********************************************************************************
* ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/      *
* Copyright (c) 2010-2013 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

// Libraries
#include "SphereVsSphereAlgorithm.h"
#include "../../collision/shapes/SphereShape.h"

// We want to use the ReactPhysics3D namespace
using namespace reactphysics3d;

// Constructor
SphereVsSphereAlgorithm::SphereVsSphereAlgorithm(MemoryAllocator& memoryAllocator)
                        :NarrowPhaseAlgorithm(memoryAllocator) {
    
}

// Destructor
SphereVsSphereAlgorithm::~SphereVsSphereAlgorithm() {
    
}   

bool SphereVsSphereAlgorithm::testCollision(CollisionShape* collisionShape1,
                                            const Transform& transform1,
                                            CollisionShape* collisionShape2,
                                            const Transform& transform2,
                                            ContactPointInfo*& contactInfo) {
    
    // Get the sphere collision shapes
    const SphereShape* sphereShape1 = dynamic_cast<const SphereShape*>(collisionShape1);
    const SphereShape* sphereShape2 = dynamic_cast<const SphereShape*>(collisionShape2);
    
    // Compute the distance between the centers
    Vector3 vectorBetweenCenters = transform2.getPosition() - transform1.getPosition();
    decimal squaredDistanceBetweenCenters = vectorBetweenCenters.lengthSquare();

    // Compute the sum of the radius
    decimal sumRadius = sphereShape1->getRadius() + sphereShape2->getRadius();
    
    // If the sphere collision shapes intersect
    if (squaredDistanceBetweenCenters <= sumRadius * sumRadius) {
        Vector3 centerSphere2InBody1LocalSpace = transform1.getInverse() * transform2.getPosition();
        Vector3 centerSphere1InBody2LocalSpace = transform2.getInverse() * transform1.getPosition();
        Vector3 intersectionOnBody1 = sphereShape1->getRadius() *
                                      centerSphere2InBody1LocalSpace.getUnit();
        Vector3 intersectionOnBody2 = sphereShape2->getRadius() *
                                      centerSphere1InBody2LocalSpace.getUnit();
        decimal penetrationDepth = sumRadius - std::sqrt(squaredDistanceBetweenCenters);
        
        // Create the contact info object
        contactInfo = new (mMemoryAllocator.allocate(sizeof(ContactPointInfo))) ContactPointInfo(
                           vectorBetweenCenters.getUnit(), penetrationDepth,
                           intersectionOnBody1, intersectionOnBody2);
    
        return true;
    }
    
    return false;
}