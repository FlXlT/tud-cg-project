#include "grid.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>

int Grid::isContainedAt(const Vec3Df & pos){
    //returns index that contains the position
    int indice = 0;
    return indice;
}

int Grid::Vec3DiToInt(const Vec3Di & pos) {
	int index = pos[0] + pos[1]*r + pos[2]*r*r;
	return index;
}


void Grid::putVertices(const std::vector<Vertex> & vertices){
	//iterate over all original vertices	
	for(auto vertiter: vertices) {

		int map_index = findGridCoord(vertiter);
		
		if(verticesInCell.find(map_index) == verticesInCell.end()) {// entry not found: lets create it	
	//		printf("first vertex in this cube. Creating entry...\n");
			std::vector<Vertex> gridVector;
			gridVector.push_back(vertiter);
			verticesInCell.insert(std::pair< int, std::vector<Vertex> >(map_index,gridVector)); 
		}
		else {
	//		printf("not the first one, adding it to the respective vector\n");
			verticesInCell.find(map_index)->second.push_back(vertiter);
			//(verticesInCell.second).push_back(vertiter);	// put vertex at the right place
		}
		boxIndices.push_back(map_index);		
	}
}

int Grid::findGridCoord(const Vertex vertiter) {
		//find out which grid block the current vertex belongs to
		Vec3Di gridCoord;

		for(int t=0;t<3;t++) { //iterate three times: over x,y and z coordinates
			for(int i=0;i<r;i++) {
				if((vertiter.p[t]-origin[t]) <= (i+1)*size/r) { 	//find position of grid block
					gridCoord[t] = i;
					break;
				}
			}
		}

		//because std::map doesn't like vec3Di, map the vector to an int as key
		int map_index = Vec3DiToInt(gridCoord);
return map_index;	
}

void Grid::computeRepresentatives() {
	Vec3Df avg,avg_ref;
	for(int i=0;i<3;i++){avg[i]=0;}
	avg_ref = avg;

	for(auto i =  verticesInCell.cbegin(); i != verticesInCell.cend(); ++i) { //iterate over all grid cells
		for(int j=0;j<i->second.size();j++) { //iterate over all vertices in one grid cell
      			avg += i->second[j].p;
		}
		avg /= i->second.size();  
		representatives.insert(std::pair<int,Vertex>(i->first,avg));
		avg = avg_ref; //reset the value of avg for the next iteration
	}

}

void Grid::drawRepresentatives() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glDisable(GL_LIGHTING);
	glPointSize(0.1f);
	glBegin(GL_POINTS);
	glColor3f(1,0,1);

	for(auto iter = representatives.cbegin(); iter != representatives.cend(); ++iter) {
		glVertex3f(iter->second.p[0],iter->second.p[1],iter->second.p[2]);
		printf("draw vertex %f %f %f\n",iter->second.p[0],iter->second.p[1],iter->second.p[2]);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glPopAttrib();
}

void Grid::drawCell(const Vec3Df & Min,const Vec3Df& Max) {

    const Vec3Df corners[8] =
    {
        Vec3Df(Min[0],Min[1],Min[2]),
        Vec3Df(Max[0],Min[1],Min[2]),
        Vec3Df(Min[0],Max[1],Min[2]),
        Vec3Df(Max[0],Max[1],Min[2]),
        Vec3Df(Min[0],Min[1],Max[2]),
        Vec3Df(Max[0],Min[1],Max[2]),
        Vec3Df(Min[0],Max[1],Max[2]),
        Vec3Df(Max[0],Max[1],Max[2])
    };


    static const unsigned short faceCorners[6][4] =
    {
        { 0,4,6,2 },
        { 5,1,3,7 },
        { 0,1,5,4 },
        { 3,2,6,7 },
        { 0,2,3,1 },
        { 6,4,5,7 }
    };

    glBegin(GL_QUADS);
    for (unsigned short f=0;f<6;++f)
    {
        const unsigned short* face = faceCorners[f];
        for(unsigned int v = 0; v<4; v++)
            glVertex3f(corners[face[v]][0], corners[face[v]][1], corners[face[v]][2]);

    }
    glEnd();

}

void Grid::drawCellWire(const Vec3Df & Min,const Vec3Df& Max) {

    const Vec3Df corners[8] =
    {
        Vec3Df(Min[0],Min[1],Min[2]),
        Vec3Df(Max[0],Min[1],Min[2]),
        Vec3Df(Min[0],Max[1],Min[2]),
        Vec3Df(Max[0],Max[1],Min[2]),
        Vec3Df(Min[0],Min[1],Max[2]),
        Vec3Df(Max[0],Min[1],Max[2]),
        Vec3Df(Min[0],Max[1],Max[2]),
        Vec3Df(Max[0],Max[1],Max[2])
    };


    static const unsigned short faceCorners[2][4] =
    {
        { 0,4,6,2 },
        { 5,1,3,7 },
    };
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	glColor3f(0,0,1);
	for(unsigned short f=0; f<2; f++)
	{
		const unsigned short* face = faceCorners[f];
		for(unsigned int v=0;v<3;v++) {
			glVertex3f(corners[face[v]][0], corners[face[v]][1], corners[face[v]][2]);	
			glVertex3f(corners[face[v+1]][0], corners[face[v+1]][1], corners[face[v+1]][2]);	
			if(v==2) {
			glVertex3f(corners[face[0]][0], corners[face[0]][1], corners[face[0]][2]);	
			glVertex3f(corners[face[v+1]][0], corners[face[v+1]][1], corners[face[v+1]][2]);	
			}
		}
	}

	//draw the remaining four lines
	glColor3f(1,0,0);
	for (unsigned short f=0; f<8; f+=2) {
		glVertex3f(corners[f][0],corners[f][1],corners[f][2]);
		glVertex3f(corners[f+1][0],corners[f+1][1],corners[f+1][2]);
	}

	glEnd();
	
	glEnable(GL_LIGHTING);

}

void Grid::drawGrid(){
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glLineWidth(1.0f);
    glColor3f(1.0f,1.0f,0.0f);
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	Vec3Df minPosi, maxPosi, maxPosi_ref, minPosi_ref;
	float gridCubeSize = size / r;

	//create the right offset before entering the for loop
	//maxPosi will be translated to its final position during the for loop
	//minPosi also, will become the origin before first call of drawCellWire()
	maxPosi = origin;
	//for(int i=0;i<3;i++){ maxPosi[i] = origin[i] + gridCubeSize; }
	for(int i=0;i<3;i++){ minPosi[i] = origin[i] - gridCubeSize; }

	int construct_map_index;
	//create reference point
	maxPosi_ref = maxPosi;
	minPosi_ref = minPosi;

	//three for loops: one for each dimension
	for(unsigned short i=0; i<r; i++) {
		maxPosi[2] += gridCubeSize;
		minPosi[2] += gridCubeSize;
		for(unsigned short j=0; j<r; j++) {
			maxPosi[1] += gridCubeSize;
			minPosi[1] += gridCubeSize;
			for(unsigned short k=0; k<r; k++) {
				//translate every coordinate accordingly
				maxPosi[0] += gridCubeSize;
				minPosi[0] += gridCubeSize;
			
				//reconstruct box position from index 	
				construct_map_index = k + j*r + i*r*r;
					
				//check if current grid box has vertices in it
				if(verticesInCell.find(construct_map_index) == verticesInCell.end()) { //grid box is empty 
				}
				else {
				drawCell(minPosi,maxPosi);//this is a grid cube 
				}
			}
			maxPosi[0] = maxPosi_ref[0];
			minPosi[0] = minPosi_ref[0];
		}
		maxPosi[1] = maxPosi_ref[1];
		minPosi[1] = minPosi_ref[1];

	}	
	maxPosi[2] = maxPosi_ref[2];
	minPosi[2] = minPosi_ref[2];
    //Complete this function by calling draw cell several times.

    glPopAttrib();
}


