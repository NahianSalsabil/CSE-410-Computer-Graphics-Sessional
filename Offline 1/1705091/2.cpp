#include <bits/stdc++.h>

#include <GL/glut.h>

using namespace std;

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
double camera_pos_value;
int drawgrid;
int drawaxes;


double wheel_radius;
double wheel_slices;
double depth;


double rec_width;
double rec_length;

int z_angle;
int x_angle;
double x_dist;
double y_dist;
double angle_dist;
double dist;


struct point
{
	double x = 0, y = 0, z = 0;
};


void drawAxes(){
	if(drawaxes==1){
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 150,0,0);
			glVertex3f(-150,0,0);

			glVertex3f(0,-150,0);
			glVertex3f(0, 150,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid(){
	if(drawgrid==1){
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(int i = -15; i <= 15; i++){

				//lines parallel to Y-axis
				glVertex3f(i*10, -150, 0);
				glVertex3f(i*10,  150, 0);

				//lines parallel to X-axis
				glVertex3f(-150, i*10, 0);
				glVertex3f( 150, i*10, 0);
			}
		}glEnd();
	}
}


void drawrectangle(double a, double b){
    glBegin(GL_QUADS);{
		glVertex3f(0, a, b);
		glVertex3f(0, a, -b);
		glVertex3f(0, -a, -b);
		glVertex3f(0, -a, b);
	}glEnd();
}


void drawcylinder(int radius, int slices, int stacks){
	struct point pos[100][100];
	double h,r;
	int height = 0;
	for(int i = 0; i < stacks; i++){
		h = height;
		r = radius;
		for(int j = 0; j <= slices; j++){
			pos[i][j].x=h;
			pos[i][j].y=r*cos(((double)j/(double)slices)*2*pi);
			pos[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
		}
		height = depth/2;
	}

	//draw quads using generated pos
	for(int i = 0; i < stacks-1; i++){
		for(int j = 0; j < slices; j++){

            if(j < slices/2)
                glColor3f(0.05*j, 0.05*j, 0.05*j);
            else if(j >= slices/2)
                glColor3f(0.05*(slices-j), 0.05*(slices-j), 0.05*(slices-j));
                
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(pos[i][j].x,pos[i][j].y,pos[i][j].z);
				glVertex3f(pos[i][j+1].x,pos[i][j+1].y,pos[i][j+1].z);
				glVertex3f(pos[i+1][j+1].x,pos[i+1][j+1].y,pos[i+1][j+1].z);
				glVertex3f(pos[i+1][j].x,pos[i+1][j].y,pos[i+1][j].z);

				//lower hemisphere
                glVertex3f(-pos[i][j].x,pos[i][j].y,pos[i][j].z);
				glVertex3f(-pos[i][j+1].x,pos[i][j+1].y,pos[i][j+1].z);
				glVertex3f(-pos[i+1][j+1].x,pos[i+1][j+1].y,pos[i+1][j+1].z);
				glVertex3f(-pos[i+1][j].x,pos[i+1][j].y,pos[i+1][j].z);
			}glEnd();
		}
	}
}


void DrawWheel(){
    // Cylinder
    glPushMatrix();{
    glTranslatef(0, 0, wheel_radius);
    glTranslatef(x_dist, y_dist, 0);
    glRotatef(z_angle, 0, 0, 1);
    glRotatef(x_angle, 1, 0, 0);
    drawcylinder(wheel_radius, wheel_slices, 2);
    }
    glPopMatrix();

    glColor3f(0.6, 0.6, 0.6);
    
    // vertical Bar
    glPushMatrix();{
    glTranslatef(0, 0, wheel_radius);
    glTranslatef(x_dist, y_dist, 0);
    glRotatef(z_angle, 0, 0, 1);
    glRotatef(x_angle, 1, 0, 0);
    glRotatef(90, 0, 0, 1);
    drawrectangle(rec_width/2, rec_length/2);
    }
    glPopMatrix();

    // Horizontal bar
    glPushMatrix();{
    glTranslatef(0, 0, wheel_radius);
    glTranslatef(x_dist, y_dist, 0);
    glRotatef(z_angle, 0, 0, 1);
    glRotatef(x_angle, 1, 0, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(90, 0, 0, 1);
    drawrectangle(rec_width/2, rec_length/2);
    }
    glPopMatrix();
}

    
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case 'w':    // move forward
			x_angle += angle_dist;
            x_dist += dist * sin((z_angle * pi) / 180);
            y_dist -= dist * cos((z_angle * pi) / 180);
			break;

        case 's':    // move backward
			x_angle -= angle_dist;
            x_dist -= dist * sin((z_angle * pi) / 180);
            y_dist += dist * cos((z_angle * pi) / 180);
            break;

        case 'a':    // rotate left
			z_angle -= 5;
            break;

        case 'd':    // rotate right
			z_angle += 5;
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3;
			break;

		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3;
			break;

		case GLUT_KEY_RIGHT:   // right arrow
			cameraAngle += 0.05;
			break;

		case GLUT_KEY_LEFT:    // left arrow
			cameraAngle -= 0.05;
			break;

		case GLUT_KEY_PAGE_UP:  // page up
            camera_pos_value -= 5;
			break;

		case GLUT_KEY_PAGE_DOWN:   // page down
            camera_pos_value += 5;
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
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawgrid=1-drawgrid;
			}
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

	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//Camera Position, position of where camera is looking, aand the up direction of camera
    gluLookAt(camera_pos_value*cos(cameraAngle), camera_pos_value*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	//add objects
	drawAxes();
	drawGrid();
	DrawWheel();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=0;

    cameraAngle = 1;
    cameraHeight = 150;
	camera_pos_value = 200;

    wheel_radius = 40;
    wheel_slices = 30;
    depth = 16;
    

    rec_width = depth/2;
    rec_length = 2*wheel_radius;

    z_angle = 0;
    x_angle = 0;
    x_dist = 0;
    y_dist = 0;
    angle_dist = 10;
    dist = 2 * wheel_radius * pi * angle_dist / 360;
    

	//clear the screen
	glClearColor(0,0,0,0);

	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Task 3: Wheel");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
