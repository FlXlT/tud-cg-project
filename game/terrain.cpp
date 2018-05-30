#include "terrain.h"

void Terrain::buildGeometry() {
	body.diffuseColor = { 133.0f / 255.0f, 104.0f / 255.0f, 238.0f / 255.0f };
	body.generateSurface(surface_modes::sinusoidal, 10, 20, 1.5f, 20, 40, 10, 10, 2, 4);
	body.specularColor = { 0, 0, 0 };
	body.specularIntensity = 0;
	body.useTex = true;
	updateGeometry();
}

void Terrain::updateGeometry() {
	body.clearModelMatrix();
	body.position = position;
	body.applyPosition();
}

std::vector<GeometricObject*> Terrain::getGeometry() {
	std::vector<GeometricObject*> geometry;
	geometry.push_back(&body);
	return geometry;
}