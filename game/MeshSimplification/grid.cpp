#include "Grid.h"
#ifdef WIN32
#include <windows.h>
#endif

int Grid::isContainedAt(const Vec3Df & pos){
    //returns index that contains the position
    int indice = 0;
    return indice;
}

int Grid::Vec3DiToInt(const Vec3Di & pos) {
	int index = pos[0] + pos[1]*r + pos[2]*r*r;
	return index;
}


void Grid::putVertices(const std::vector<Vertex1> & vertices){
	//iterate over all original vertices	
	for(auto vertiter: vertices) {

		int map_index = findGridCoord(vertiter);
		
		if(verticesInCell.find(map_index) == verticesInCell.end()) {// entry not found: lets create it	
	//		printf("first vertex in this cube. Creating entry...\n");
			std::vector<Vertex1> gridVector;
			gridVector.push_back(vertiter);
			verticesInCell.insert(std::pair< int, std::vector<Vertex1> >(map_index,gridVector)); 
		}
		else {
	//		printf("not the first one, adding it to the respective vector\n");
			verticesInCell.find(map_index)->second.push_back(vertiter);
			//(verticesInCell.second).push_back(vertiter);	// put vertex at the right place
		}
		boxIndices.push_back(map_index);		
	}
}

int Grid::findGridCoord(const Vertex1 vertiter) {
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
		representatives.insert(std::pair<int,Vertex1>(i->first,avg));
		avg = avg_ref; //reset the value of avg for the next iteration
	}

}

