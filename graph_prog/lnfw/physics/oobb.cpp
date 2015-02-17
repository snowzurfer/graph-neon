// Includes
#include "oobb.h"


namespace lnfw {

void OOBB::addVertex(const abfw::Vector2& v) {
	vertices_.push_back(v);
}


void OOBB::setPosition(const float x, const float y) {
	// Calculate distance between centre and the new position
	abfw::Vector2 translation = abfw::Vector2(x, y) - centre_;

	// Set the new centre
	centre_ += translation;

	// Move all the vertices
	for(verticesItor_ vertex = vertices_.begin(); vertex != vertices_.end(); ++vertex) {
		(*vertex) += translation;
	}
}

std::vector<abfw::Vector2> createNorms(const OOBB& oobb) {

	// Declare a vector to hold the normals which will then be returned
	std::vector<abfw::Vector2> normals;

	// For each vertex
	// Note: not using an iterator because there is the need to use an integer counter.
	// Generally, iterators should be preferred when working with containers.
	for (int i = 0; i < oobb.getVertices().size(); ++i) {
		// Get P1
		abfw::Vector2 p1 = oobb.getVertices()[i];
		// Get P2. The statement in the accessor allows the latest edge of the shape
		// to be correctly calculated.
		// The corner is the one defined by vertex[totNumVertex] and vertex[0] 
		abfw::Vector2 p2 = oobb.getVertices()[(i + 1) == oobb.getVertices().size() ? 0 : (i + 1)];

		// Calculate edge
		abfw::Vector2 edge = p2 - p1;

		// Calculate the right normal and push it into the container
		normals.push_back(normalise(createRNormal(edge)));
	}

	return normals;
}

const Projection computeMinMax(const OOBB& oobb, const abfw::Vector2& axis) {
	// Calculate initial max and minimum projections on the axis
	float min = dotProd(oobb.getVertices()[0], axis); 
	float max = min;
	
	// For each remaining normal of the polygon
	for (int i = 1; i < oobb.getVertices().size(); ++i) { 
		// Calculate the projection for the current normal
		float currProj = dotProd(oobb.getVertices()[i], axis);

		// If the current projection is larger than the previous max one
		if (currProj > max) {
			// Save it as the maximum one
			max = currProj;
		}
		// If the current projection is smaller than the previous min one
		else if (currProj < min) {
			// Save it as the minimum one
			min = currProj;
		}
	}

	return Projection(min, max);
}

const bool SAT(const IShape &a, const IShape &b, Manifold& mfold) {
	/*int aType = mfold.a_->type_;
	int bType = mfold.a_->type_;

	if (aType == lnfw::bird)
	{
		if(bType == lnfw::bird)
		{
			return SAT((CircleShape&)a, (CircleShape&)b, mfold);
		}
		else
		{
			return SAT((CircleShape&)a, (OOBB&)b, mfold);
		}
	}
	else if (aType == lnfw::block) {
		if(bType == lnfw::bird)
		{
			return SAT((OOBB&)a, (CircleShape&)b, mfold);
		}
		else
		{
			return SAT((OOBB&)a, (OOBB&)b, mfold);
		}
	}*/

	return true;
}

const bool SAT(const OOBB& a, const OOBB& b, Manifold& mfold) {
	float smallestOverlap = 100000.f;
	abfw::Vector2 smallestAxis;

	// Calculate the normals of the two OOBBs
	std::vector<abfw::Vector2> aNormals = createNorms(a);
	std::vector<abfw::Vector2> bNormals = createNorms(b);

	// Loop over normals of first object
	for(int i = 0; i < aNormals.size(); ++i) {
		// Project both OOBBs on the current axis
		Projection aProjection = computeMinMax(a, aNormals[i]);
		Projection bProjection = computeMinMax(b, aNormals[i]);

		// If the objects do not overlap on this axis
		if(aProjection.getMax() < bProjection.getMin() || 
			bProjection.getMax() < aProjection.getMin()) {
			// Exit. SAT gurantees OOBBs do not overlap
			return false;
		}
		else {
			// Create a variable to hold the calculated overlap value
			float overlap = 0;

			// Calculate the overlap value
			if((aProjection.getMax() > bProjection.getMin()) && 
				aProjection.getMax() < bProjection.getMax()) {
				overlap = aProjection.getMax() - bProjection.getMin(); 
			}
			else {
				overlap = bProjection.getMax() - aProjection.getMin(); 
			}
			
			// If it is the smallest projection so far
			if(overlap < smallestOverlap) {
				// Set this one as the smallest
				smallestOverlap = overlap;
				// Also save the index of the axis onto which the projection has been found
				smallestAxis = aNormals[i];
			}
		}
	}
	// EO First loop

	// Loop over normals of first object
	for(int i = 0; i < bNormals.size(); ++i) {
		// Project both OOBBs on the current axis
		Projection aProjection = computeMinMax(a, bNormals[i]);
		Projection bProjection = computeMinMax(b, bNormals[i]);

		// If the objects do not overlap on this axis
		if(aProjection.getMax() < bProjection.getMin() || 
			bProjection.getMax() < aProjection.getMin()) {
			// Exit. SAT gurantees OOBBs do not overlap
			return false;
		}
		else {
			// Create a variable to hold the calculated overlap value
			float overlap = 0;

			// Calculate the overlap value
			if((aProjection.getMax() > bProjection.getMin()) && 
				aProjection.getMax() < bProjection.getMax()) {
				overlap = aProjection.getMax() - bProjection.getMin(); 
			}
			else {
				overlap = bProjection.getMax() - aProjection.getMin(); 
			}
			
			// If it is the smallest projection so far
			if(overlap < smallestOverlap) {
				// Set this one as the smallest
				smallestOverlap = overlap;
				// Also save the index of the axis onto which the projection has been found
				smallestAxis = bNormals[i];
			}
		}
	}
	// EO Second loop

	
	// If here, the objects overlap on every axis and therefore intersection is guaranteed
	// to have happened by SAT

	// Populate the manifold which holds the collision data
	mfold.setMinDirection(smallestAxis);
	mfold.setMinMagnitude(smallestOverlap);

	return true;
}

const bool SAT(const OOBB& oobb, const CircleShape& circle, Manifold& mfold) {
	// Calculate distance vector between centre of OOBB and centre of circle.
	abfw::Vector2 distCentre2Centre = oobb.getCentre() - circle.getCentre(); 

	// Also calculate the normalised axis vector corresponding to the distance vector
	// (Used to project the corners of the box onto).
	abfw::Vector2 projAxis = (normalise(distCentre2Centre));

	
	// Calculate the maxium projection from the vertices of the OOBB
	// by checking every vertex.
		// Save the longest (max) projection on the normalised axis so that 
		// it will be used for collision detection.
		float maxProj = -9999.f;
		// Also save the vertex wich resulted in the closes longest projection
		int maxProjVertex = -1;
	for(int i = 0; i < oobb.getVertices().size(); ++i) {
		// Translate the current vertex back to the origin.
		abfw::Vector2 currentVertex = oobb.getVertices()[i] - oobb.getCentre();

		// Compute projection.
		float proj = dotProd(currentVertex, projAxis);

		// If this projection is the longest so far
		if(maxProj < proj) {
			// Set it to be so
			maxProj = proj;
			maxProjVertex = i;
		}
	}

	// If there is a collision (calculated by using squared values, so that square roots are not required
	if(magnitudeSqrd(distCentre2Centre) - (circle.getRadius() * circle.getRadius()) - (maxProj * maxProj) > 0) {
		// Calculate the vector between the closest vertex and the centre of the circle
		abfw::Vector2 closestVx2CircleCentre = circle.getCentre() - oobb.getVertices()[maxProjVertex];

		// Calculate penetration depth
		float penetrationDepth = circle.getRadius() - magnitude(closestVx2CircleCentre); 

		// Populate the manifold which holds the collision data
		mfold.setMinDirection(normalise(closestVx2CircleCentre));
		mfold.setMinMagnitude(penetrationDepth);


		return true;
	}

	return false;
}

const bool SAT(const CircleShape& a, const OOBB& b, Manifold& mfold) {
	return SAT(b, a, mfold);
}

const bool SAT(const CircleShape& a, const CircleShape& b, Manifold& mfold) {
	// Calculate distance vector between centre of the two circles.
	abfw::Vector2 distCentre2Centre = a.getCentre() - b.getCentre();

	// If the distance squared is longer than the sum of the square of the radius
	if(magnitudeSqrd(distCentre2Centre) < ((a.getRadius() * a.getRadius()) + (b.getRadius() * b.getRadius()))) {
		// Calculate the penetration depth
		float penetrationDepth = a.getRadius() + b.getRadius() - magnitude(distCentre2Centre);

		// Populate the manifold which holds the collision data
		mfold.setMinDirection(normalise(distCentre2Centre));
		mfold.setMinMagnitude(penetrationDepth);

		return true;
	}	

	return false;
}

void OOBB::setAngleAC(const float angle) {
	// Calculate the difference between the curretn angle and the one passed
	float angleDiff = angle - angle_;

	// If the object has to actually be rotated
	if(angleDiff != 0.f) {
		// Compute sin and cos
		// NOTE: VERY DAMN EXPENSIVE. Should be subsituted by a LUT
		float sin = std::sinf(angleDiff);
		float cos = std::cosf(angleDiff);

		//std::cout << "AngleDiff:" << angleDiff << std::endl;

		// For each vertex
		for(std::vector<abfw::Vector2>::iterator itor = vertices_.begin(); itor != vertices_.end(); ++itor) {
			//std::cout << (*itor).x << "," << (*itor).y << "  -  ";

			// Traslate to origin
			(*itor) -= centre_;

			// Rotate
			float rotatedX = ((*itor).x * cos) - ((*itor).y * sin);
			float rotatedY = ((*itor).x * sin) + ((*itor).y * cos);

			// Assign the new, rotated values to the vector
			itor->x = rotatedX;
			itor->y = rotatedY;

			// Traslate back
			(*itor) += centre_;

			//std::cout << (*itor).x << "," << (*itor).y << std::endl;
		}

		// Set the new angle
		angle_ = angle;
	}
}


}
// EO Namespace