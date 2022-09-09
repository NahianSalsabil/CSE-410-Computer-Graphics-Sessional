#include <iostream>
#include <bits/stdc++.h>
#include <GL/glut.h>
#include <math.h>

#include "1705100_header.hpp"
#include "bitmap_image.hpp"

#define CAMERA_MOVE_WEIGHT 5
#define CAMERA_ROTATE_WEIGHT 0.1
#define UP 1
#define RIGHT 2
#define LOOK 3
#define pi (2.0*acos(0.0))
#define rad(a) ((a*pi)/180.0)
#define deg(a) ((a*180.0)/pi)

using namespace std;

/// ------------------------------------------ GLOBALS ----------------------------------------------------->>>
string scene_abs_path = "test_scene.txt";
string image_abs_path = "output";
int image_size;
int image_count;

int windowHeight = 600;
int windowWidth = 600;
double viewAngle = 80.0;

extern Vector camera_pos, up_vector, right_vector, look_vector;
int drawgrid;
int drawaxes;

int level_of_recursion;
int num_of_obj;
int num_of_point_lights;
int num_of_spot_lights;

extern vector <Object*> objects;
extern vector <PointLight> point_lights;
extern vector <SpotLight> spot_lights;

///-------------------------------------------------- CAPTURE START -------------------------------------------------->>>

void capture()
{
    // initialize bitmap image and set background color
    cout << "capturing ... " << endl;
    bitmap_image image(image_size, image_size);

    for(int i = 0; i < image_size; i++){
        for(int j = 0; j < image_size; j++){
            // initialize background color
            image.set_pixel(i, j, 0, 0, 0);
        }
    }

    double planeDistance = (windowHeight/2.0)/tan(rad(viewAngle/2.0));
    Vector topleft = camera_pos + look_vector * planeDistance - right_vector * windowWidth/2 + up_vector * windowHeight/2;

    double du = (windowWidth*1.0) / image_size;
    double dv = (windowHeight*1.0) / image_size;

    // Choose middle of the grid cell
    topleft = topleft + right_vector * (0.5*du) - up_vector * (0.5*dv);

    int nearest = -1;  // nearest object
    double t, tmin = DBL_MAX;
    Vector curpix = topleft;

    for(int i = 1; i <= image_size; i++){
        for(int j = 1; j <= image_size; j++){
            // calculate curPixel using topleft,r,u,i,j,du,dv
            curpix = topleft + right_vector * (i * du) - up_vector * (j * dv);

            // cast ray from eye to (curPixel-eye) direction
            Ray ray(camera_pos, curpix - camera_pos);

            Color color;
            int k = 0;
            nearest = -1;
            tmin = DBL_MAX;
            for(Object* obj: objects){
                color = obj->getColor();
                t = obj->intersect(ray, color, 0);   // don't need to color

                // update tmin to nearest so far
                if(t > 0 and t < tmin) {
                    tmin = t;
                    nearest = k;
                }
                k++;
            }

            // coloring start
            if(nearest != -1) {
                color = objects[nearest]->getColor();
                tmin = objects[nearest]->intersect(ray, color, level_of_recursion);  // now color
                image.set_pixel(i, j, (int)round(color.r*255.0), (int)round(color.g*255.0), (int)round(color.b*255.0));
            }
        }
    }

    image_count++;
    image.save_image(image_abs_path + to_string(image_count) + ".bmp");
    cout << "image captured" << endl;
}


Vector rotate_vector(double angle, Vector v, Vector v_T)
{
    return v*cos(angle) + v_T*sin(angle);
}

// -------------------------------------------->>> Camera starts <<<--------------------------------------------------


void print_camera()
{
    cout << endl;
    cout << "camera pos: (" << camera_pos.x << ", " << camera_pos.y << ", " << camera_pos.z << ")" << endl;
    cout << "look vector: (" << look_vector.x << ", " << look_vector.y << ", " << look_vector.z << ")" << endl;
    cout << "right vector: (" << right_vector.x << ", " << right_vector.y << ", " << right_vector.z << ")" << endl;
    cout << "up vector: (" << up_vector.x << ", " << up_vector.y << ", " << up_vector.z << ")" << endl;
    cout << endl;
}

void camera_setup()
{
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
           camera_pos.x,
           camera_pos.y,
           camera_pos.z,

           camera_pos.x + look_vector.x,
           camera_pos.y + look_vector.y,
           camera_pos.z + look_vector.z,

           up_vector.x,
           up_vector.y,
           up_vector.z

           );
	glMatrixMode(GL_MODELVIEW);
}

void camera_move_along_v(double t, Vector v)
{
    camera_pos = camera_pos + v*t;
}

void camera_rotate(double angle, int axis)
{
    /**
        axis = 1,   right_vector    x axis
        axis = 2,   up_vector       y axis
        axis = 3,   look_vector    -z axis
    */

    switch(axis)
    {
    case UP:
    {
        // up axis
        Vector new_right = rotate_vector(angle, right_vector, look_vector);
        Vector new_look = rotate_vector(angle, look_vector, -right_vector);

        right_vector = new_right;
        look_vector = new_look;
        break;
    }
    case RIGHT:
    {
        // right axis
        Vector new_look = rotate_vector(angle, look_vector, up_vector);
        Vector new_up = rotate_vector(angle, up_vector, -look_vector);

        look_vector = new_look;
        up_vector = new_up;
        break;
    }
    case LOOK:
    {
        // look axis
        Vector new_up = rotate_vector(angle, up_vector, right_vector);
        Vector new_right = rotate_vector(angle, right_vector, -up_vector);

        right_vector = new_right;
        up_vector = new_up;
        break;
    }
    default:
        cout << "Error: invalid axis" << endl;
        break;
    }
}



// ------------------------------------------------>>> Camera ends <<<--------------------------------------------------

// -------------------------------------------->>> All drawing starts <<<--------------------------------------------------

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


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}


// -------------------------------------------->>> All drawing ends <<<--------------------------------------------------


// -------------------------------------------->>> All callbacks starts <<<--------------------------------------------------

void animate() {
	/* codes for any changes in Models, Camera */
	glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // camera setup
    camera_setup();

    // --------------------->>> Drawing Begins <<<-----------------------------
    glMatrixMode(GL_MODELVIEW);

    drawAxes();
    drawGrid();


    for(Object* obj: objects){
        obj->draw();
    }

    // ----------------------->>> Drawing Ends <<<-----------------------------

    glutSwapBuffers();
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
        case '0':
            capture();
            break;
		case '1':
		    camera_rotate(CAMERA_ROTATE_WEIGHT, UP);
			break;
		case '2':
		    camera_rotate(-CAMERA_ROTATE_WEIGHT, UP);
			break;
		case '3':
		    camera_rotate(CAMERA_ROTATE_WEIGHT, RIGHT);
			break;
		case '4':
		    camera_rotate(-CAMERA_ROTATE_WEIGHT, RIGHT);
			break;
		case '5':
		    camera_rotate(CAMERA_ROTATE_WEIGHT, LOOK);
			break;
		case '6':
		    camera_rotate(-CAMERA_ROTATE_WEIGHT, LOOK);
			break;
        case 'g':
            drawgrid = 1 - drawgrid;
            break;
        case 'x':
            drawaxes = 1 - drawaxes;
            break;
        case 'r':
            // reset_cam
            camera_pos = {0, 0, 10};
            look_vector = {0, 0, -1};
            up_vector = {0, 1, 0};
            right_vector = {1, 0, 0};
            break;
		default:
			break;
	}

	glutPostRedisplay();
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_UP:		// up arrow key
		    camera_move_along_v(CAMERA_MOVE_WEIGHT, look_vector);
			break;
		case GLUT_KEY_DOWN:		//down arrow key
			camera_move_along_v(-CAMERA_MOVE_WEIGHT, look_vector);
			break;
		case GLUT_KEY_RIGHT:
			camera_move_along_v(CAMERA_MOVE_WEIGHT, right_vector);
			break;
		case GLUT_KEY_LEFT:
			camera_move_along_v(-CAMERA_MOVE_WEIGHT, right_vector);
			break;

		case GLUT_KEY_PAGE_UP:
		    camera_move_along_v(CAMERA_MOVE_WEIGHT, up_vector);
			break;
		case GLUT_KEY_PAGE_DOWN:
		    camera_move_along_v(-CAMERA_MOVE_WEIGHT, up_vector);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:

			break;
		case GLUT_KEY_END:

			break;

		default:
			break;
	}

	glutPostRedisplay();
}

void mouseListener(int button, int state, int x, int y) {
	switch(button) {
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN) {

			}
			break;
        case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN) {

			}
			break;
		default:
			break;
	}
}


// -------------------------------------------->>> All drawing ends <<<--------------------------------------------------


/// ---------------------------------------------------- Start ---------------------------------------------------------------

void loadData()
{
    ifstream fin;

    fin.open(scene_abs_path);


    // insert floor
    Floor* floor;
    floor = new Floor(1000 , 20);
    floor->setColor1({1,1,1});
    floor->setColor2({0,0,0});
    floor->setCoefficients(Coefficient(0.4, 0.3, 0.1, 0.2));
    floor->setShine(5);
    objects.push_back(floor);


    fin >> level_of_recursion >> image_size >> num_of_obj;

    /// ---------------------------------------------------- LOAD OBJECTS START -------------------------------------------
    string shape;
    for(int i  = 0 ; i < num_of_obj; i++){
        fin >> shape;

        if(shape == "triangle"){
            Vector a, b, c;
            Color color;
            Coefficient coeffs;
            int shine;

            fin >> a >> b >> c >> color >> coeffs >> shine;

            Triangle* triangle;
            triangle = new Triangle(a, b, c);

            triangle->setColor(color);
            triangle->setCoefficients(coeffs);
            triangle->setShine(shine);

            objects.push_back(triangle);
        }
        else if(shape == "sphere"){
            Vector center;
            Color color;
            double radius;
            Coefficient coeffs;
            int shine;

            fin >> center >> radius >> color >> coeffs >> shine;

            Sphere* sphere;
            sphere = new Sphere(center, radius);

            sphere->setColor(color);
            sphere->setCoefficients(coeffs);
            sphere->setShine(shine);

            objects.push_back(sphere);
        }
        else if(shape == "general") {
            double a, b, c, d, e, f, g, h, i, j;
            Vector cube_reference_point;
            double length, width, height;

            Color color;
            Coefficient coeffs;
            int shine;

            fin >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j >> cube_reference_point >> length >> width >> height >> color >> coeffs >> shine;

            General* general;
            general = new General(a, b, c, d, e, f, g, h, i, j, length, width, height, cube_reference_point);

            general->setColor(color);
            general->setCoefficients(coeffs);
            general->setShine(shine);

            objects.push_back(general);
        }
    }


    /// ---------------------------------------------------- LOAD POINT LIGHTS START -------------------------------------------
    fin >> num_of_point_lights;
    for(int i = 0 ; i < num_of_point_lights; i++){
        Vector src_pos;
        Color color;

        fin >> src_pos >> color;

        point_lights.push_back(PointLight(src_pos, color));
    }


    /// ---------------------------------------------------- LOAD SPOT LIGHTS START -------------------------------------------
    fin >> num_of_spot_lights;
    for(int i = 0 ; i < num_of_spot_lights; i++){
        Vector src_pos;
        Color color;
        Vector dir;
        double cut_off;

        fin >> src_pos >> color >> dir >> cut_off;

        spot_lights.push_back(SpotLight(src_pos, color, dir, cut_off));
    }

    fin.close();
}

/// -------------------------------------------------- end ----------------------------------------------------------------

void init()
{
    loadData();

    for(Object* obj: objects){
        obj->print();
    }

    // background color
    glClearColor(0, 0, 0, 0);

    // camera
    camera_pos = {0, -200, 10};
    look_vector = {0, 1, 0};
    up_vector = {0, 0, 1};
    right_vector = {1, 0, 0};
    camera_setup();


    // perspective
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewAngle,	1,	1,	1000.0);

	drawaxes = 1;
	drawgrid = 0;
}

void clean()
{
    for(Object* obj: objects){
        delete obj;
    }
    objects.clear();
    point_lights.clear();
    spot_lights.clear();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowPosition(200, 100);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Ray Tracing");
    init();

    glEnable(GL_DEPTH_TEST);

    // Callbacks
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

    glutMainLoop();

    clean();

    return 0;
}
