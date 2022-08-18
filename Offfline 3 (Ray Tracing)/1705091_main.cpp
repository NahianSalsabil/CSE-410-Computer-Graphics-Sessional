#include "1705091_classes.h"

#include<GL/glut.h>

extern vector <Object*> objects;
extern vector <PointLight*> pointLights;
extern vector <SpotLight*> spotLights;


int recursion_level;
int pixel_number;
int no_of_objects;
int no_of_pointlight;
int no_of_spotlight;
string object_type;
double floor_width = 1000;
double tile_width = 20;

Vector3D position, l, u, r;

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '1':    // Look left
			l = Rotation(l, u, counterclockwise);
			r = Rotation(r, u, counterclockwise);
			break;

        case '2':    // Look right
			l = Rotation(l, u, clockwise);
			r = Rotation(r, u, clockwise);
            break;

        case '3':    // Look up
			l = Rotation(l, r, counterclockwise);
			u = Rotation(u, r, counterclockwise);
            break;

        case '4':    // Look down
			l = Rotation(l, r, clockwise);
			u = Rotation(u, r, clockwise);
            break;

        case '5':    // tilt clockwise
			r = Rotation(r, l, counterclockwise);
			u = Rotation(u, l, counterclockwise);
            break;

        case '6':    // tilt counter clockwise
			r = Rotation(r, l, clockwise);
			u = Rotation(u, l, clockwise);
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){

	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			position.getX() -= l.getX;
			position.getY() -= l.getY();
			position.getZ() -= l.getZ();
			break;

		case GLUT_KEY_UP:		// up arrow key
			position.getX() += l.getX;
			position.getY() += l.getY();
			position.getZ() += l.getZ();
			break;

		case GLUT_KEY_RIGHT:   // right arrow
			position.getX() += r.getX();
			position.getY() += r.getY();
			position.getZ() += r.getZ();
			break;

		case GLUT_KEY_LEFT:    // left arrow
			position.getX() -= r.getX();
			position.getY() -= r.getY();
			position.getZ() -= r.getZ();
			break;

		case GLUT_KEY_PAGE_UP:  // page up
			position.getX() += u.getX();
			position.getY() += u.getY();
			position.getZ() += u.getZ();
			break;

		case GLUT_KEY_PAGE_DOWN:   // page down
			position.getX() -= u.getX();
			position.getY() -= u.getY();
			position.getZ() -= u.getZ();
			break;

		case GLUT_KEY_INSERT:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();
	
    gluLookAt(position.getX(), position.getY(), position.getZ(), position.getX() + l.getX(), position.getY() + l.getY(), position.getZ() + l.getZ(), u.getX(), u.getY(), u.getZ());

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	drawAxes();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawaxes=1;

    position.set(100.0, 100.0, 0.0);
    l.set(-1/sqrt(2.0), -1/sqrt(2.0), 0);
    r.set(-1/sqrt(2.0), 1/sqrt(2.0), 0);
    u.set(0,0,1);

	//clear the screen
	glClearColor(0,0,0,0);

	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
}

// ############################################################################################ //


void printObjects(){
    for(int i = 0; i < no_of_objects; i++){
        cout << "Object " << i << ": " << endl;
        cout << "Color: " << objects[i]->getColor().red << " " << objects[i]->getColor().green << " " << objects[i]->getColor().blue << endl;
        cout << "coefficients: " << objects[i]->getCoEfficients().ambient << " " << objects[i]->getCoEfficients().diffuse << " " << objects[i]->getCoEfficients().specular << " " << objects[i]->getCoEfficients().reflection << endl;
        cout << "shininess: " << objects[i]->getShininess() << endl;
    }
}

void printPointLight(){
    for(int i = 0; i < no_of_pointlight; i++){
        cout << "PointLight " << i << ": " << endl;
        cout << "Position: " << pointLights[i]->getPosition().getX() << " " << pointLights[i]->getPosition().getY() << " " << pointLights[i]->getPosition().getZ() << endl;
        cout << "Color: " << pointLights[i]->getColor().red << " " << pointLights[i]->getColor().green << " " << pointLights[i]->getColor().blue << endl; 
    }
}


void LoadData(){
    freopen("test_scene.txt", "r", stdin);
    cin >> recursion_level >> pixel_number;
    cin >> no_of_objects;
    double x, y, z;
    Color color;
    Coefficients coefficient; int shininess;

    // Read Objects
    for( int i = 0; i < no_of_objects; i++){
        cin >> object_type;
        Object *object;
    
        // read sphere 
        if(object_type == "sphere"){
            double radius; 
            cin >> x >> y >> z >> radius;
            position.set(x, y, z);
            object = new Sphere(position, radius);
        }
        // read triangle
        else if(object_type == "triangle"){
            Vector3D position2, position3;
            double x1, y1, z1, x2, y2, z2, x3, y3, z3;
            cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
            position.set(x1, y1, z1);
            position2.set(x2, y2, z2);
            position3.set(x3, y3, z3);
            object = new Triangle(position, position2, position3); 
        }
        // read general quad shape
        else if(object_type == "general"){
            QuadraticCoefficients quadcoeff; double height, width, length;
            cin >> quadcoeff.a >> quadcoeff.b >> quadcoeff.c >> quadcoeff.d >> quadcoeff.e >> quadcoeff.f >> quadcoeff.g >> quadcoeff.h >> quadcoeff.i >> quadcoeff.j;
            cin >> x >> y >> z >> length >> width >> height;
            position.set(x, y, z);

            object = new GeneralQuadraticShape(quadcoeff, position, height, width, length);
        }

        cin >> color.red >> color.green >> color.blue;
        object->setColor(color);
        cin >> coefficient.ambient >> coefficient.diffuse >> coefficient.specular >> coefficient.reflection;
        object->setCoEfficients(coefficient);
        cin >> shininess;
        object->setShine(shininess);
        objects.push_back(object);

        printObjects();
    }
    // Read PointLight
    cin >> no_of_pointlight;
    PointLight *pointLight;
    for(int i = 0; i < no_of_pointlight; i++){
        cin >> x >> y >> z;
        position.set(x, y, z);
        cin >> color.red >> color.green >> color.blue;
        pointLight = new PointLight(position, color);
        pointLights.push_back(pointLight);
    }
    printPointLight();

    // Read SpotLight
    cin >> no_of_spotlight;
    SpotLight *spotLight; double cutoff_angle; Vector3D direction;
    for(int i = 0; i < no_of_spotlight; i++){
        cin >> x >> y >> z;
        position.set(x, y, z);  // position
        cin >> color.red >> color.green >> color.blue;   // color
        cin >> x >> y >> z;
        direction.set(x, y, z);  // direction
        cin >> cutoff_angle;    // cutoff angle
        spotLight = new SpotLight(position, color, direction, cutoff_angle);
        spotLights.push_back(spotLight);
    }
    // Read Floor
    Object *object;
    object = new Floor(floor_width, tile_width);
    object->setColor(color);
    object->setShine(10);
    object->setCoEfficients(coefficient);
    objects.push_back(object);
}


int main(){

    LoadData();
}