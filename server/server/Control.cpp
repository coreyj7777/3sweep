#include "Control.h"

EdgeSample* Control::getNewEdge(Vector2D &mousePosition){
	EdgeSample edgeSample = topEdge->clone();
	if (status == straight){
		Vector2D direct = edgeSample->getNormal()*
						 (edgeSample->getNormal()*(mousePosition - oldMousePosition));
	
		edgeSample.endpoint[0] += direct;
		edgeSample.endpoint[1] += direct;

		edgeSample.samples.clear();
		edgeSample.sample();

	}else if(status == bend){
		// the way we get normal should be discussed again
		Vector2D n = mousePosition - oldMousePosition;
		n.normalize();
		edgeSample.setNormal(n);

		double angle = getRotateAngle(n.x, n.y, 
					   topEdge->getNormal().x, topEdge->getNormal().y);
		// direct means the direct from center to endpoint
		Vector2D oldDirect = topEdge->getEndpoint() - topEdge->getCenter();
		double x = oldDirect.x* cos(angle) - oldDirect.y* sin(angle);
		double y = oldDirect.x* sin(angle) + oldDirect.y* cos(angle);
		Vector2D newDirect(x, y);

		// set new center and endpoint 
		edgeSample.setCenter(edgeSample.getCenter() + n);
		std::vector<Vector2D> endpoint;
		endpoint.push_back(edgeSample.getCenter() - newDirect);
		endpoint.push_back(edgeSample.getCenter() + newDirect);
		edgeSample.setEndpoint(edgeSample.boundary->intersection(endpoint));

		// set sample points 
		edgeSample.sample();
	}else{
		std::cout << "status is wrong" << endl;
		return NULL;
	}

	delete topEdge;
	topEdge = &edgeSample;
	oldMousePosition = mousePosition;
	return topEdge;
}

void Control::setFirstEdge(std::vector<Vector2D> &v){
	switch(bottomShape){
		case circle:
			topEdge = new CyliEdgeSample;
			break;
		case square:
		default:
			break;
	}
	topEdge->init(v);
	topEdge->setBoundary(boundary);
}

void Control::buildObj(Vector2D &mousePosition){
	getNewEdge(mousePosition);	
	obj->insertSample(compute3D->compute3D(topEdge->getSample()));
}