#include <bits/stdc++.h>

#include <GL/glut.h>

using namespace std;

#define pi (2*acos(0.0))

int drawgrid;
int drawaxes;

double sphere_radius = 25;
double radius2 = 15;
double radius3 = 10;
double radius4 = 5;
double radius5 = 3;
double sphere_stacks = 25;
double sphere_slices = 30;
double translate_value = 10;


int clockwise = 1;
int counterclockwise = -1;
double angle = 0;
double rotate_angle = 2;
double rot_angle = 0;
double rotation_angle = pi * rotate_angle / 180;


struct point
{
	double x = 0, y = 0, z = 0;
};

struct point pos, u, r, l;

point Cross_Product(point u, point v){
	point cross;
	cross.x = u.y * v.z - u.z * v.y;
	cross.y = u.z * v.x - u.x * v.z;
	cross.z = u.x * v.y - u.y * v.x;

	return cross;
}

point Rotation(point v, point reference, int dir){
	point cross = Cross_Product(v, reference);
	point new_point;

	double actual_rotation_angle = dir * rotation_angle;
	
	new_point.x = v.x * cos(rotation_angle) + cross.x * sin(actual_rotation_angle);
	new_point.y = v.y * cos(rotation_angle) + cross.y * sin(actual_rotation_angle);
	new_point.z = v.z * cos(rotation_angle) + cross.z * sin(actual_rotation_angle); 

	return new_point;
}



void drawAxes(){
	if(drawaxes==1){
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


void drawGrid(){
	if(drawgrid==1){
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(int i = -8; i <= 8; i++){
				if(i == 0)
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

void drawSquare(double a){
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
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
        // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
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


void DrawSolarSystem(){
    glColor3f(1, 1, 1);
    drawSphere(sphere_radius, sphere_slices, sphere_stacks);

    glPushMatrix();{
        // mercury
        glColor3f(1, 0, 0);
        // glTranslatef(-5*cos(rotate_angle*pi/180), -5*sin(rotate_angle*pi/180), 0);
        glRotatef(angle, 0, 1, 0);
        glTranslatef(sphere_radius+radius2 + translate_value, 0, 0);
        drawSphere(radius2, sphere_slices, sphere_stacks);
    }
    glPopMatrix();

    glPushMatrix();{
        // venus
        glColor3f(0, 1, 0);
        // glTranslatef(-5*cos(rotate_angle*pi/180), -5*sin(rotate_angle*pi/180), 0);
        glRotatef(rotate_angle, 0, 1, 0);
        glTranslatef(sphere_radius+2*radius2 + radius3+ 2*translate_value, 0, 0);
        drawSphere(radius3, sphere_slices, sphere_stacks);
    }
    glPopMatrix();

    glPushMatrix();{
        // earth
        glColor3f(0, 0, 1);
        
        glRotatef(rotate_angle, 0, 0, 1);
        glTranslatef(sphere_radius+2*radius2 + 2*radius3+ radius4 + 3*translate_value, 0, 0);
        // glTranslatef((sphere_radius+2*radius2 + 2*radius3+ radius4 + 3*translate_value)*cos(rotate_angle*pi/180), (sphere_radius+2*radius2 + 2*radius3+ radius4 + 3*translate_value)*sin(rotate_angle*pi/180), 0);
        drawSphere(radius4, sphere_slices, sphere_stacks);
        

        // moon
        glColor3f(0.6, 0.6, 0.6);
        glRotatef(rot_angle, 1, 0, 0);
        glTranslatef(0, radius4+ radius5+translate_value, 0);
        drawSphere(radius5, sphere_slices, sphere_stacks);
    }
    glPopMatrix();
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
			pos.x -= l.x;
			pos.y -= l.y;
			pos.z -= l.z;
			break;

		case GLUT_KEY_UP:		// up arrow key
			pos.x += l.x;
			pos.y += l.y;
			pos.z += l.z;
			break;

		case GLUT_KEY_RIGHT:   // right arrow
			pos.x += r.x;
			pos.y += r.y;
			pos.z += r.z;
			break;

		case GLUT_KEY_LEFT:    // left arrow
			pos.x -= r.x;
			pos.y -= r.y;
			pos.z -= r.z;
			break;

		case GLUT_KEY_PAGE_UP:  // page up
			pos.x += u.x;
			pos.y += u.y;
			pos.z += u.z;
			break;

		case GLUT_KEY_PAGE_DOWN:   // page down
			pos.x -= u.x;
			pos.y -= u.y;
			pos.z -= u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			// if(translate_value > 0){
			// 	sphere_radius += 5;
			// 	translate_value -= 5;
			// }
			break;

		case GLUT_KEY_END:
			// if(sphere_radius > 0){
			// 	sphere_radius -= 5;
			// 	translate_value += 5;
			// }
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
    gluLookAt(pos.x, pos.y, pos.z,	pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);

	//add objects
	drawAxes();
	drawGrid();

	DrawSolarSystem();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
    rot_angle += 0.5;
    rotate_angle += 0.5;
    angle += 10;
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;

	pos.x = 100;
	pos.y = 100;
	pos.z = 0;

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

	glutCreateWindow("Task 1 and 2: Cube and Sphere");

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
