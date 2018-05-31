#ifndef GRID_H
#define GRID_H

#include "Vec3D.h"
#include <vector>
#include "Vertex1.h"
#include <map>

typedef std::map<unsigned int, std::vector<Vertex1>> CellContent;
typedef std::map<unsigned int, Vertex1> RepresentativeList;


class Grid
{
public:
    Grid(){}
    inline Grid (const Vec3Df & origin, float size, unsigned int r) : origin (origin), size(size) , r(r) {}
	//The corners with the smallest and largest coordinates.
    void drawCell(const Vec3Df & min,const Vec3Df& Max);
    void drawCellWire(const Vec3Df & min,const Vec3Df& Max);
	//draw all the cells
    void drawGrid();
	int Vec3DiToInt(const Vec3Di & pos);
	//number of grid cells
    unsigned int r;
	
	int findGridCoord(const Vertex1 vert);
	//position of the grid (min corner is at origin and its extent is defined by size).
    Vec3Df origin;
    float size;
	//remark: for simplicity, we work with a cube and the size/extent values are the same for all axes.

    //add a point to a cell
	void addToCell(const Vec3Df & vertexPos);
    //add all vertices of the model to the cells
	void putVertices(const std::vector<Vertex1> & vertices);
    //find the index containing the given point, return -1 if it cannot be found
	int isContainedAt(const Vec3Df & pos);


	//for each cell, compute a representative point of all contained points
	void computeRepresentatives();
	void drawRepresentatives();
    //retrieve the representative from representatives by first finding the cell containing pos 
	//then looking up the representative
	Vec3Df getRepresentative(const Vec3Df & pos);

	std::vector<int> boxIndices;	
    CellContent verticesInCell;
	RepresentativeList representatives;
};

#endif // GRID_H
