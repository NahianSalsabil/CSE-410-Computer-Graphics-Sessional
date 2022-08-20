#include "1705091_classes_v2.h"
#include "bitmap_image.hpp"

#include <GL/glut.h>



int pixel_number;
int no_of_objects;
int no_of_pointlight;
int no_of_spotlight;
string object_type;
double floor_width = 1000;
double tile_width = 20;


// for OpenGL
int drawaxes;
int clockwise = 1;
int counterclockwise = -1;
int rotate_angle = 2;
double rotation_angle = pi * rotate_angle / 180;
double sphere_stacks = 25;
double sphere_slices = 30;

Vector3D object_pos, l, u, r;
Vector3D  position;

// for bitmap image
int windowWidth = 500;
int windowHeight = 500;
double fovY = 80;
int bitmap_image_count = 1;

Vector3D Cross_Product(Vector3D u, Vector3D v){
	Vector3D cross;
	cross.x = u.y * v.z - u.z * v.y;
	cross.y = u.z * v.x - u.x * v.z;
	cross.z = u.x * v.y - u.y * v.x;

	return cross;
}

Vector3D Rotation(Vector3D v, Vector3D reference, int dir){
	Vector3D cross = Cross_Product(v, reference);
	Vector3D new_point;

	double actual_rotation_angle = dir * rotation_angle;
	
	new_point.x = v.x * cos(rotation_angle) + cross.x * sin(actual_rotation_angle);
	new_point.y = v.y * cos(rotation_angle) + cross.y * sin(actual_rotation_angle);
	new_point.z = v.z * cos(rotation_angle) + cross.z * sin(actual_rotation_angle); 

	return new_point;
}

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

void Capture()
{
	cout << "captured\n";

	int imageWidth = pixel_number;
	int imageHeight = pixel_number;

	bitmap_image image(imageWidth, imageHeight);
	image.clear();

	for(int i = 0; i < imageWidth; i++)
	{
        for(int j = 0; j < imageHeight; j++)
		{
            image.set_pixel(j, i, 0, 0, 0);
        }
    }	
	
	double planeDistance = ((double)windowHeight/2.0) / tan(((fovY*pi)/180)/2.0);
	// test << "planeDistance: " << planeDistance << endl;
	Vector3D topleft = position.Add(l.Multiply(planeDistance)).Subtract(r.Multiply((double)windowWidth/2.0)).Add(u.Multiply((double)windowHeight/2.0));; 
	

	double du = (double)windowWidth/(double)imageWidth;
	double dv = (double)windowHeight/(double)imageHeight;

	// Choose middle of the grid cell
	topleft = topleft.Add(r.Multiply(du/2.0)).Subtract(u.Multiply(dv/2.0));
	
	for(int i = 0; i < imageWidth; i++){
		for(int j = 0; j < imageHeight; j++){

			int nearest = -1;
			double t, tMin = 99999999;

			// test << "iteration: " << j << endl;
			Vector3D curPixel = topleft.Add(r.Multiply((double)i*du)).Subtract(u.Multiply((double)j*dv));
			// test << "curPixel: " << curPixel.x << " " << curPixel.y << " " << curPixel.z << endl;

			Vector3D direction = curPixel.Subtract(position);
			// test << "dir: " << direction.x << " " << direction.y << " " << direction.z << endl;
			// direction.normalize();
			Ray ray(position, direction);

			Color *color = new Color();
			color->red = 0.0; color->green = 0.0; color->blue = 0.0;
			
			for(int i = 0; i < objects.size(); i++){
				t = objects[i]->IntersectWithLighting(ray, color, 0);
				if(t < tMin && t > 0){
					tMin = t;
					nearest = i;
				}
			}
			
			if(nearest != -1){
				tMin = objects[nearest]->IntersectWithLighting(ray, color, 1);
				// test << color->red << " " << color->green << " " << color->blue << endl;
				color->clip();
				image.set_pixel(i, j, round(color->red*255), round(color->green*255), round(color->blue*255));
			}
		}
	}
	image.save_image("Output_1" + to_string(bitmap_image_count) + ".bmp");
	cout << "image captured\n";
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '0':      // capture
			Capture();
			bitmap_image_count++;
			break; 

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
			position.x -= l.x;
			position.y -= l.y;
			position.z -= l.z;
			break;

		case GLUT_KEY_UP:		// up arrow key
			position.x += l.x;
			position.y += l.y;
			position.z += l.z;
			break;

		case GLUT_KEY_RIGHT:   // right arrow
			position.x += r.x;
			position.y += r.y;
			position.z += r.z;
			break;

		case GLUT_KEY_LEFT:    // left arrow
			position.x -= r.x;
			position.y -= r.y;
			position.z -= r.z;
			break;

		case GLUT_KEY_PAGE_UP:  // page up
			position.x += u.x;
			position.y += u.y;
			position.z += u.z;
			break;

		case GLUT_KEY_PAGE_DOWN:   // page down
			position.x -= u.x;
			position.y -= u.y;
			position.z -= u.z;
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
	
    gluLookAt(position.x, position.y, position.z, position.x + l.x, position.y + l.y, position.z + l.z, u.x, u.y, u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	drawAxes();
	for(int i = 0; i < objects.size(); i++){
		objects[i]->draw();
	}

	for(int i = 0; i < no_of_pointlight; i++){
		pointLights[i]->draw();
	}

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

    position.x = 100;
	position.y = 100;
	position.z = 50;

	l.x = -1/sqrt(2.0);
	l.y = -1/sqrt(2.0);
	l.z = 0;

	r.x = -1/sqrt(2.0);
	r.y = 1/sqrt(2.0);
	r.z = 0;

	u.x = 0;
	u.y = 0;
	u.z = 1;

	//clear the screen
	glClearColor(0,0,0,0);

	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(fovY, 1,	1,	500.0);
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
        cout << "Position: " << pointLights[i]->getPosition().x << " " << pointLights[i]->getPosition().y << " " << pointLights[i]->getPosition().z << endl;
        cout << "Color: " << pointLights[i]->getColor().red << " " << pointLights[i]->getColor().green << " " << pointLights[i]->getColor().blue << endl; 
    }
}


void LoadData(){
    freopen("scene.txt", "r", stdin);
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
            cin >> object_pos.x >> object_pos.y >> object_pos.z >> radius;
            object = new Sphere(object_pos, radius);
        }
        // read triangle
        else if(object_type == "triangle"){
            Vector3D position2, position3;
            cin >> object_pos.x >> object_pos.y >> object_pos.z >> position2.x >> position2.y >> position2.z >> position3.x >> position3.y >> position3.z;
            object = new Triangle(object_pos, position2, position3); 
        }
        // read general quad shape
        else if(object_type == "general"){
            QuadraticCoefficients quadcoeff; double height, width, length;
            cin >> quadcoeff.a >> quadcoeff.b >> quadcoeff.c >> quadcoeff.d >> quadcoeff.e >> quadcoeff.f >> quadcoeff.g >> quadcoeff.h >> quadcoeff.i >> quadcoeff.j;
            cin >> object_pos.x >> object_pos.y >> object_pos.z >> length >> width >> height;

            object = new GeneralQuadraticShape(quadcoeff, object_pos, height, width, length);
        }

        cin >> color.red >> color.green >> color.blue;
        object->setColor(color);
        cin >> coefficient.ambient >> coefficient.diffuse >> coefficient.specular >> coefficient.reflection;
        object->setCoEfficients(coefficient);
        cin >> shininess;
        object->setShine(shininess);
        objects.push_back(object);

        // printObjects();
    }
    // Read PointLight
    cin >> no_of_pointlight;
    PointLight *pointLight;
    for(int i = 0; i < no_of_pointlight; i++){
        cin >> object_pos.x >> object_pos.y >> object_pos.z;
        cin >> color.red >> color.green >> color.blue;
        pointLight = new PointLight(object_pos, color);
        pointLights.push_back(pointLight);
    }
    printPointLight();

    // Read SpotLight
    cin >> no_of_spotlight;
    SpotLight *spotLight; double cutoff_angle; Vector3D direction;
    for(int i = 0; i < no_of_spotlight; i++){
        cin >> object_pos.x >> object_pos.y >> object_pos.z;
        cin >> color.red >> color.green >> color.blue;   // color
        cin >> direction.x >> direction.y >> direction.z;  // direction
        cin >> cutoff_angle;    // cutoff angle
        spotLight = new SpotLight(object_pos, color, direction, cutoff_angle);
        spotLights.push_back(spotLight);
    }
    // Read Floor
    Object *object;
    object = new Floor(floor_width, tile_width);
	Color floor_color = {1.0, 1.0, 1.0};
    object->setColor(floor_color);
    object->setShine(10);
	Coefficients floor_coeff = {0.5, 0.3, 0.4, 0.2};
    object->setCoEfficients(floor_coeff);
    objects.push_back(object);

	cin.clear();
}


int main(int argc, char** argv){
	glutInit(&argc,argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Task 1 and 2: Cube and Sphere");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	LoadData();

	glutMainLoop();		//The main loop of OpenGL
    
}