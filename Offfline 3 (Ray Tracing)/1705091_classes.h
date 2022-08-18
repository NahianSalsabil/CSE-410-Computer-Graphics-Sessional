#include<bits/stdc++.h>
#include <GL/glut.h>

using namespace std;

#define pi (2*acos(0.0))


struct Color{
    double red, green, blue;
};

struct Coefficients{
    double ambient, diffuse, specular, reflection;
};

struct QuadraticCoefficients{
    double a, b, c, d, e, f, g, h, i, j;
};

class Vector3D{
public:
    double x, y, z;
    Vector3D(){
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    Vector3D(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // normalize
    void normalize(){
        double length = sqrt(x*x + y*y + z*z);
        x /= length;
        y /= length;
        z /= length;
    }
};

class Ray{
    Vector3D origin, direction;
public:
    Ray(){
        origin = Vector3D();
        direction = Vector3D();  // Rd
    }
    Ray(Vector3D origin, Vector3D direction){
        this->origin = origin;
        this->direction = direction;
    }
    void setOrigin(Vector3D origin){
        this->origin = origin;
    }
    void setDirection(Vector3D direction){
        this->direction = direction;
    }
    Vector3D getOrigin(){
        return origin;
    }
    Vector3D getDirection(){
        return direction;
    }
};

class Object{
    Vector3D reference_point;
    double height, width, length;
    Color color;
    Coefficients coefficient;
    int shininess;

public:
    Object(){
        shininess = 0;
    }

    virtual void draw(){}

    virtual double intersect(Ray ray, Color color, int level){
        return -1.0;
    }

    void setColor(Color color){
        this->color = color;
    }

    Color getColor(){
        return this->color;
    }

    void setShine(int shininess){
        this->shininess = shininess;
    }

    int getShininess(){
        return this->shininess;
    }

    void setCoEfficients(Coefficients coefficient){
        this->coefficient = coefficient;
    }

    Coefficients getCoEfficients(){
        return this->coefficient;
    }

    ~Object(){

    }
};

class Sphere : public Object{
    Vector3D center;
    double radius;
    double sphere_stacks;
    double sphere_slices;
public:
    Sphere(){
        radius = 0;
    }
    Sphere(Vector3D center, double radius){
        this->center = center;
        this->radius = radius;
        this->sphere_stacks = 25;
        this->sphere_slices = 30;
    }
    void setCenter(Vector3D center){
        this->center = center;
    }
    void setRadius(double radius){
        this->radius = radius;
    }

    void drawSphere(){
        Vector3D points[100][100];
        int i,j;
        double h,r;

        //generate points
        for(i=0;i<=sphere_stacks;i++){
            h=radius*sin(((double)i/(double)sphere_stacks)*(pi/2));
            r=radius*cos(((double)i/(double)sphere_stacks)*(pi/2));

            for(j = 0;j <= sphere_slices;j++){
                points[i][j].x=r*cos(((double)j/(sphere_slices)*2*pi));
                points[i][j].y=r*sin(((double)j/(sphere_slices)*2*pi));
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        for(i=0;i<sphere_stacks;i++){
            for(j = 0; j < sphere_slices;j++){
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

    void draw(){
        glColor3f(getColor().red, getColor().green, getColor().blue);
        glPushMatrix();{
            glTranslatef(center.x, center.y, center.z);
            drawSphere();
        }
        glPopMatrix();
    }

    double intersect(Ray ray, Color color, int level){
        Vector3D R0; R0.x = ray.getOrigin().x - center.x; R0.y = ray.getOrigin().y - center.y; R0.z = ray.getOrigin().z - center.z;
        Vector3D Rd; Rd.x = ray.getDirection().x; Rd.y = ray.getDirection().y; Rd.z = ray.getDirection().z;
        
        double ld, d, l;
        ld = Rd.x * (-R0.x) + Rd.y * (-R0.y) + Rd.z * (-R0.z);
        if(ld < 0) return -1.0;
        d = sqrt(R0.x * R0.x + R0.y * R0.y + R0.z * R0.z - ld * ld);
        if(pow(d,2) > pow(radius,2)) return -1.0;
        l = sqrt(pow(radius,2) - pow(d,2));
        
        double t1 = ld - l;
        double t2 = ld + l;
        if(level == 0){
            if(t1 < t2){
                if(t1 < 0)
                    return -1.0;
                return t1;
            }
            else{
                if(t2 < 0)
                    return -1.0;
                return t2;
            }
        }
        return -1.0;

    }

    ~Sphere(){

    }
};

class Triangle : public Object{
    Vector3D v1, v2, v3;
public:
    Triangle(){
        v1 = Vector3D();
        v2 = Vector3D();
        v3 = Vector3D();
    }
    Triangle(Vector3D v1, Vector3D v2, Vector3D v3){
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
    }
    void setV1(Vector3D v1){
        this->v1 = v1;
    }
    void setV2(Vector3D v2){
        this->v2 = v2;
    }
    void setV3(Vector3D v3){
        this->v3 = v3;
    }

    void draw(){
        glColor3f(getColor().red, getColor().green, getColor().blue);
        glBegin(GL_TRIANGLES);{
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(v3.x, v3.y, v3.z);
        }glEnd();
    }

    double intersect(Ray ray, Color color, int level){
        return -1.0;
    }

    ~Triangle(){

    }
};

class GeneralQuadraticShape : public Object{
    QuadraticCoefficients quadcoefficients;
    Vector3D cube_reference_point;
    double cube_height, cube_width, cube_length;

public:
    GeneralQuadraticShape(){
        quadcoefficients = QuadraticCoefficients();
        cube_reference_point = Vector3D();
        cube_height = 0; cube_width = 0; cube_length = 0;
    }
    
    GeneralQuadraticShape(QuadraticCoefficients quadcoefficients, Vector3D cube_reference_point, double cube_height, double cube_width, double cube_length){
        this->quadcoefficients = quadcoefficients;
        this->cube_reference_point = cube_reference_point;
        this->cube_height = cube_height;
        this->cube_width = cube_width;
        this->cube_length = cube_length;
    }
    void setQuadCoefficients(QuadraticCoefficients quadcoefficients){
        this->quadcoefficients = quadcoefficients;
    }
    void draw(){}

    double intersect(Ray ray, Color color, int level){
        return -1.0;
    }

    ~GeneralQuadraticShape(){

    }
};

class Floor: public Object{
    double floor_width, tile_width;

public:
    Floor(double floor_width, double tile_width){
        this->floor_width = floor_width;
        this->tile_width = tile_width;
    }

    void draw(){
        for( int i = -floor_width/tile_width; i < floor_width/(2*tile_width); i++){
            for( int j = -floor_width/tile_width; j < floor_width/(2*tile_width); j++){
                if((i+j)%2 == 0){
                    glColor3f(0.0, 0.0, 0.0);
                }
                else{
                    glColor3f(1.0, 1.0, 1.0);
                }
                glBegin(GL_QUADS);{
                    glVertex3f(i*tile_width, j*tile_width, 0);
                    glVertex3f(i*tile_width, (j+1)*tile_width, 0);
                    glVertex3f((i+1)*tile_width, (j+1)*tile_width, 0);
                    glVertex3f((i+1)*tile_width, j*tile_width, 0);
                }glEnd();
            }
        }
    }
};

class PointLight{
    Vector3D position;
    Color color;
public:
    PointLight(){
        position = Vector3D();
        color = Color();
    }
    PointLight(Vector3D position, Color color){
        this->position = position;
        this->color = color;
    }
    void setPosition(Vector3D position){
        this->position = position;
    }
    void setColor(Color color){
        this->color = color;
    }
    Vector3D getPosition(){
        return this->position;
    }
    Color getColor(){
        return this->color;
    }
    void draw(){
        glColor3f(color.red, color.green, color.blue);
        glBegin(GL_POINTS);
        glVertex3f(position.x, position.y, position.z);
        glEnd();
    }
    ~PointLight(){

    }
};

class SpotLight{
    Vector3D position; Color color;
    Vector3D direction;
    double cutoff_angle;
public:
    SpotLight(){
        position = Vector3D();
        direction = Vector3D();
        cutoff_angle = 0;
    }
    SpotLight(Vector3D position, Color color, Vector3D direction, double cutoff_angle){
        this->position = position;
        this->color = color;
        this->direction = direction;
        this->cutoff_angle = cutoff_angle;
    }
    void setPosition(Vector3D position){
        this->position = position;
    }
    void setColor(Color color){
        this->color = color;
    }

    void setDirection(Vector3D direction){
        this->direction = direction;
    }
    void setCutoffAngle(double cutoff_angle){
        this->cutoff_angle = cutoff_angle;
    }
    Vector3D getPosition(){
        return this->position;
    }
    Color getColor(){
        return this->color;
    }
    Vector3D getDirection(){
        return this->direction;
    }
    double getCutoffAngle(){
        return this->cutoff_angle;
    }
    ~SpotLight(){

    }
};



vector<Object*> objects;
vector<PointLight*> pointLights;
vector<SpotLight*> spotLights;
