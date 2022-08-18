#include<bits/stdc++.h>
using namespace std;

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
    double x, y, z;
public:
    Vector3D(){
        x = 0; y = 0; z = 0;
    }
    Vector3D(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    double getX(){
        return x;
    }
    double getY(){
        return y;
    }
    double getZ(){
        return z;
    }
    void setX(double x){
        this->x = x;
    }
    void setY(double y){
        this->y = y;
    }
    void setZ(double z){
        this->z = z;
    }
    void set(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    void set(Vector3D v){
        this->x = v.getX();
        this->y = v.getY();
        this->z = v.getZ();
    }
    void add(Vector3D v){
        this->x += v.getX();
        this->y += v.getY();
        this->z += v.getZ();
    }
    void add(double x, double y, double z){
        this->x += x;
        this->y += y;
        this->z += z;
    }
    void subtract(Vector3D v){
        this->x -= v.getX();
        this->y -= v.getY();
        this->z -= v.getZ();
    }
    void subtract(double x, double y, double z){
        this->x -= x;
        this->y -= y;
        this->z -= z;
    }
    void multiply(double x){
        this->x *= x;
        this->y *= x;
        this->z *= x;
    }

    ~Vector3D(){
        x = 0;
        y = 0;
        z = 0;
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

    virtual void draw(){

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
public:
    Sphere(){
        radius = 0;
    }
    Sphere(Vector3D center, double radius){
        this->center = center;
        this->radius = radius;
    }
    void setCenter(Vector3D center){
        this->center = center;
    }
    void setRadius(double radius){
        this->radius = radius;
    }
    void draw(){
        cout << "Sphere" << endl;
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
        cout << "Triangle" << endl;
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
    void draw(){
        cout << "QuadraticShape" << endl;
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

    void Draw(){
        cout << "floor: " << endl;
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
