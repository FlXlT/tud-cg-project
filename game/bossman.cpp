
#include "bossman.h"

Mesh mesh; //Main mesh
Mesh simplified; //simplified mesh - to be built by you
Grid grid; //voxel grid to be filled by you

Mesh bossman::simplifyMesh(unsigned int r, Mesh mesh) {

	mesh.computeBoundingCube();
	//this will give the following variables:
	//bbOrigin: Vec3Df vertex point that represents the minimum corner of the bounding cube
	//bbEdgeSize: float that represents the length of the cube sides

   grid = Grid(mesh.bbOrigin, mesh.bbEdgeSize, r );
	
	//assign a grid box to all vertices
 	grid.putVertices(mesh.vertices);

	//compute representative vertex of vertices lying in the same grid box	
	grid.computeRepresentatives();	

	//work with a local reference	
    	const std::vector<Vertex1> & vertices = mesh.vertices;
    	const std::vector<Triangle> & triangles = mesh.triangles;

  std::map<unsigned int, unsigned int > newIndexRemapping;
  std::vector<Vertex1> simplifiedVertices;

    int count = 0;
    for(RepresentativeList::iterator it = grid.representatives.begin() ; it != grid.representatives.end (); it++, count++){
        newIndexRemapping[it->first] = count;
        simplifiedVertices.push_back(it->second);
	//newIndexRemapping now contains grid box IDs paired with a unique ID 'count', crucial for linking box indices to real indices
    }

  //triangles 'n' stuff
  std::vector<Triangle> simplifiedTriangles;

	for(int x=0;x<triangles.size();x++) {
		//retrieve vertices corresponding to the indices listed in the triangles entry
		
		std::vector<int> boxIndices_temp;
		for(int k=0;k<3;k++) { 
			boxIndices_temp.push_back(grid.boxIndices[triangles[x].v[k]]);
		}  

		//check if vertices of triangle fall in the same box
		//if this is the case, eliminate the triangle by not adding it to simplifiedTriangles

		if(boxIndices_temp[0] != boxIndices_temp[1]) {
			if(boxIndices_temp[0] != boxIndices_temp[2]) {
				if(boxIndices_temp[1] != boxIndices_temp[2]) {
					//all vertices are in different boxes, create new simplified triangle
					//the indices of this triangle have to point at vertices in simplifiedVertices
					Vec3Di tri;
					for(int space=0;space<3;space++) {
						tri[space] = newIndexRemapping.find(boxIndices_temp[space])->second;
					}
					simplifiedTriangles.push_back(Triangle(tri[0],tri[1],tri[2]));
				}	
			}
		}
	}
 //Build the simplified mesh from the CORRECT lists
  simplified = Mesh(simplifiedVertices , simplifiedTriangles);

  //recalculate the normals.
  simplified.computeVertexNormals();

	return simplified;
}


void bossman::buildGeometry()
{
	body.position = glm::vec3(0.0f,2.0f, 5.0f);
	mesh.loadMesh("assets/models/baws2.obj");
	simplified = simplifyMesh(128,mesh);
	convertedSimpleMesh128 = MeshToGeometricObject(simplified);
	simplified = simplifyMesh(64,mesh);
	convertedSimpleMesh64 = MeshToGeometricObject(simplified);
	simplified = simplifyMesh(32,mesh);
	convertedSimpleMesh32 = MeshToGeometricObject(simplified);
	simplified = simplifyMesh(16,mesh);
	convertedSimpleMesh16 = MeshToGeometricObject(simplified);
	updateGeometry();
}

void bossman::updateGeometry() {
	float angleX = 90 * atan(1) * 4 / 180;
	float angleY = 180 * atan(1) * 4 / 180;

	body.clearModelMatrix();
	body.position = position;
	body.applyPosition();
	body.translate(glm::vec3(0.0f, -1.0f, 0.0f));
}

GeometricObject bossman::MeshToGeometricObject(Mesh meshToGeo) {
	Vertex temp_vertex1 = {};
	Vertex temp_vertex2 = {};
	Vertex temp_vertex3 = {};
	GeometricObject convertedBoss = {};

	for(int i=0; i < meshToGeo.triangles.size(); i++) {
		temp_vertex1 = {};
		temp_vertex2 = {};
		temp_vertex3 = {};

		// POSTIONS
		temp_vertex1.pos = {
			meshToGeo.vertices[meshToGeo.triangles[i].v[0]].p[0],
			meshToGeo.vertices[meshToGeo.triangles[i].v[0]].p[1],
			meshToGeo.vertices[meshToGeo.triangles[i].v[0]].p[2]
		};

		temp_vertex2.pos = {
			meshToGeo.vertices[meshToGeo.triangles[i].v[1]].p[0],
			meshToGeo.vertices[meshToGeo.triangles[i].v[1]].p[1],
			meshToGeo.vertices[meshToGeo.triangles[i].v[1]].p[2]
		};

		temp_vertex3.pos = {
			meshToGeo.vertices[meshToGeo.triangles[i].v[2]].p[0],
			meshToGeo.vertices[meshToGeo.triangles[i].v[2]].p[1],
			meshToGeo.vertices[meshToGeo.triangles[i].v[2]].p[2]
		};


		// NORMALS
		temp_vertex1.normal = {
			meshToGeo.vertices[meshToGeo.triangles[i].v[0]].n[0],
			meshToGeo.vertices[meshToGeo.triangles[i].v[0]].n[1],
			meshToGeo.vertices[meshToGeo.triangles[i].v[0]].n[2]
		};

		temp_vertex2.normal = {
			meshToGeo.vertices[meshToGeo.triangles[i].v[1]].n[0],
			meshToGeo.vertices[meshToGeo.triangles[i].v[1]].n[1],
			meshToGeo.vertices[meshToGeo.triangles[i].v[1]].n[2]
		};

		temp_vertex3.normal = {
			meshToGeo.vertices[meshToGeo.triangles[i].v[2]].n[0],
			meshToGeo.vertices[meshToGeo.triangles[i].v[2]].n[1],
			meshToGeo.vertices[meshToGeo.triangles[i].v[2]].n[2]
		};

		glm::vec3 col = glm::vec3(52.0f / 255, 181.0f / 255, 23.0f / 255); 
		temp_vertex1.color = col;
		temp_vertex2.color = col;
		temp_vertex3.color = col;

		convertedBoss.vertices.push_back(temp_vertex1);
		convertedBoss.vertices.push_back(temp_vertex2);
		convertedBoss.vertices.push_back(temp_vertex3);
	}
	return convertedBoss;
}

std::vector<GeometricObject*> bossman::getGeometry() {
	std::vector<GeometricObject*> geometry;
	std::vector<GeometricObject*> g1;
	std::vector<GeometricObject*> g2;
	std::vector<GeometricObject*> g3;

	geometry.push_back(&body);
	geometry.push_back(&convertedSimpleMesh128);
	g1.push_back(&convertedSimpleMesh64);
	geometry.insert(geometry.end(), g1.begin(), g1.end());
	g2.push_back(&convertedSimpleMesh32);
	geometry.insert(geometry.end(), g2.begin(), g2.end());
	g3.push_back(&convertedSimpleMesh16);
	geometry.insert(geometry.end(), g3.begin(), g3.end());

	return geometry;
}

void bossman::update() {

}

