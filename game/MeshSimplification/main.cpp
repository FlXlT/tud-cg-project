#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "traqueboule.h"
#include "mesh.h"
#include "grid.h"

typedef enum {Wireframe, Flat, Gouraud} PolygonMode;
static PolygonMode polygonMode = Gouraud;

using namespace std;

Mesh mesh; //Main mesh
Mesh simplified; //simplified mesh - to be built by you
Grid grid; //voxel grid to be filled by you
//Number of cells along an axis in the grid 
unsigned int r = 10;


bool displayGrid = false;
bool simplifiedDisplay = false;

// Different rendering modes, you can add the ones you need
enum { TRIANGLE=0, SIMPLE=1};
unsigned int type = TRIANGLE;

unsigned int W_fen = 800;  // window width
unsigned int H_fen = 800;  // window height


/************************************************************
 * Initialization function; here we load the mesh and you can add additional precomputations
 ************************************************************/

Mesh simplifyMesh(unsigned int r, Mesh mesh) {

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
    	const vector<Vertex1> & vertices = mesh.vertices;
    	const vector<Triangle> & triangles = mesh.triangles;

  std::map<unsigned int, unsigned int > newIndexRemapping;
  vector<Vertex1> simplifiedVertices;

    int count = 0;
    for(RepresentativeList::iterator it = grid.representatives.begin() ; it != grid.representatives.end (); it++, count++){
        newIndexRemapping[it->first] = count;
        simplifiedVertices.push_back(it->second);
	//newIndexRemapping now contains grid box IDs paired with a unique ID 'count', crucial for linking box indices to real indices
    }

  //triangles 'n' stuff
  vector<Triangle> simplifiedTriangles;

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
void init(const char * fileName){
    mesh.loadMesh(fileName);
	simplified = simplifyMesh(32,mesh);
}




void drawBoundingCube() {

	//remember all states of the GPU
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	glColor3f(1,1,0);
    	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1],mesh.bbOrigin[2]);
	glVertex3f(mesh.bbOrigin[0] + mesh.bbEdgeSize,mesh.bbOrigin[1],mesh.bbOrigin[2]);

	glVertex3f(mesh.bbOrigin[0] + mesh.bbEdgeSize,mesh.bbOrigin[1],mesh.bbOrigin[2]);
   	glVertex3f(mesh.bbOrigin[0] + mesh.bbEdgeSize,mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2]);

	glVertex3f(mesh.bbOrigin[0] + mesh.bbEdgeSize,mesh.bbOrigin[1],mesh.bbOrigin[2]);
   	glVertex3f(mesh.bbOrigin[0] + mesh.bbEdgeSize,mesh.bbOrigin[1],mesh.bbOrigin[2] + mesh.bbEdgeSize);


	//glColor3f(1,1,0);
	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1],mesh.bbOrigin[2]);
   	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2]);

   	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2]);
   	glVertex3f(mesh.bbOrigin[0] +mesh.bbEdgeSize,mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2]);

   	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2]);
   	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2] + mesh.bbEdgeSize);


	//glColor3f(0,0,1);
	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1],mesh.bbOrigin[2]);
	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1],mesh.bbOrigin[2] + mesh.bbEdgeSize);

	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1],mesh.bbOrigin[2] + mesh.bbEdgeSize);
	glVertex3f(mesh.bbOrigin[0] + mesh.bbEdgeSize,mesh.bbOrigin[1],mesh.bbOrigin[2] + mesh.bbEdgeSize);

	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1],mesh.bbOrigin[2] + mesh.bbEdgeSize);
	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2] + mesh.bbEdgeSize);


	//lines from 'far vertex'
	glVertex3f(mesh.bbOrigin[0] + mesh.bbEdgeSize,mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2] + mesh.bbEdgeSize);
	glVertex3f(mesh.bbOrigin[0] + mesh.bbEdgeSize,mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2]);

	glVertex3f(mesh.bbOrigin[0] + mesh.bbEdgeSize,mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2] + mesh.bbEdgeSize);
	glVertex3f(mesh.bbOrigin[0] + mesh.bbEdgeSize,mesh.bbOrigin[1],mesh.bbOrigin[2] + mesh.bbEdgeSize);

	glVertex3f(mesh.bbOrigin[0] + mesh.bbEdgeSize,mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2] + mesh.bbEdgeSize);
	glVertex3f(mesh.bbOrigin[0],mesh.bbOrigin[1] + mesh.bbEdgeSize,mesh.bbOrigin[2] + mesh.bbEdgeSize);


	glEnd();


	glEnable(GL_LIGHTING);
	//restore to previous state
	glPopAttrib();



}


/************************************************************
 * Function calls for drawing operations
 ************************************************************/
void drawAxis(float length)
{
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(length,0,0);

    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,length,0);

    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,length);
    glEnd();
    glEnable(GL_LIGHTING);

}


void mainDraw( )
{
	drawBoundingCube();

    /*
    RepresentativeList & representatives = grid.representants;
    glDisable(GL_LIGHTING);
    glPointSize(5.);
    glBegin(GL_POINTS);


    for(RepresentantList::iterator it = representatives.begin() ; it != representatives.end (); it++){
        Vec3Df v = (*it).second.p;
        glVertex3f(v[0], v[1], v[2]);
    }
    glEnd();
    glEnable(GL_LIGHTING);
*/

    if (polygonMode != Gouraud)
        simplifiedDisplay? simplified.draw() : mesh.draw();
    else
        simplifiedDisplay? simplified.drawSmooth() : mesh.drawSmooth();

    if (displayGrid){
        grid.drawGrid();
	grid.drawRepresentatives();
	}
    return;

    switch( type )
    {
    case TRIANGLE:
        drawAxis(1);
        break;
    default:
        drawAxis(1);
        break;
    }
}

void animate()
{

}

void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

void printUsage () {
    cerr << endl
         << "Practical" << endl
         << "for 3D graphics." << endl
         << "Usage : ./test [<file.obj>]" << endl
         << "Keyboard commands" << endl
         << "------------------" << endl
         << " ?: Print help" << endl
         << " w: Toggle Wireframe/Flat/Gouraud Rendering Mode" << endl
         << " <drag>+<left button>: rotate model" << endl
         << " <drag>+<right button>: move model" << endl
         << " <drag>+<middle button>: zoom" << endl
         << " q, <esc>: Quit" << endl << endl;
}

/************************************************************
 * Main Program
 ************************************************************/
int main(int argc, char** argv)
{

    if (argc > 2) {
        printUsage ();
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);

    init(argc == 2 ? argv[1] : "dodge.obj");


    // setup framebuffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

    // define window position and size for the application
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(W_fen,H_fen);
    glutCreateWindow(argv[0]);	

    //init trackball for mouse mouvement
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-4);
    tbInitTransform();     // Trackball
    tbHelp();              // show help message

    //
    // Let there be LIGHT :) 

    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable(GL_COLOR_MATERIAL);
    int LightPos[4] = {0,0,3,1};
    int MatSpec [4] = {1,1,1,1};
    glLightiv(GL_LIGHT0,GL_POSITION,LightPos);
    glMaterialiv(GL_FRONT_AND_BACK,GL_SPECULAR,MatSpec);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,10);
    glEnable(GL_NORMALIZE);
    

    // callbacks
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMouseFunc(tbMouseFunc);    // trackball uses mouse
    glutMotionFunc(tbMotionFunc);  // trackball uses mouse
    glutIdleFunc( animate);

    // main loop
    glutMainLoop();

    return 0;  // never reached
}


/************************************************************
 * Functions for OpenGL no need to touch the code below!
 ************************************************************/
// draw 
void display(void)
{
    // Setup OpengGL for each frame
    glEnable( GL_DEPTH_TEST );            // depth test

    if(polygonMode == Wireframe){
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    }

    glShadeModel(GL_SMOOTH);

    // Effacer tout
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); //clear color/value
    

    glLoadIdentity();  // init camera

    tbVisuTransform(); // trackball

    mainDraw( );    

    glutSwapBuffers();
}

// Window size changed
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50, (float)w/h, 1, 10);
    glMatrixMode(GL_MODELVIEW);
}

// keyboard input
void keyboard(unsigned char key, int x, int y)
{
    printf("key %d pressed at %d,%d\n",key,x,y);
    fflush(stdout);
    switch (key)
    {
    case 'a':     
        ++type;
        if (type==5)
            type=0;
        break;
    case 'w':
        if (polygonMode == Flat) {
            polygonMode = Wireframe;
        } else if (polygonMode == Wireframe) {
            polygonMode = Gouraud;
        } else {
            polygonMode = Flat;
        }
        break;
    case 's':
	//simplifyMesh(32);
	break;
    case '1':
	simplified = simplifyMesh(8,mesh);
	break;
    case '2':
	simplified = simplifyMesh(16,mesh);
	break;
    case '3':
	simplified = simplifyMesh(32,mesh);
	break;
    case '4':
	simplified = simplifyMesh(64,mesh);
	break;
    case '5':
	simplified = simplifyMesh(128,mesh);
	break;
    case 'g':
	if(displayGrid){displayGrid=0;}
	else{displayGrid=1;}
	break;
    case 'p':
	if(simplifiedDisplay){simplifiedDisplay=0;}
	else{simplifiedDisplay=1;}
	break;
    case 27:     // ESC
        exit(0);
    }
    display();
}

