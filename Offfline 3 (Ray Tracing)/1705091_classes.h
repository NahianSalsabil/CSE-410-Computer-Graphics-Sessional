#include<bits/stdc++.h>
#include <GL/glut.h>

using namespace std;

#define pi (2*acos(0.0))

int recursion_level;

ofstream test("test.txt");

struct Color{
    double red, green, blue;
    Color(){
        red = 0;
        green = 0;
        blue = 0;
    }

    Color(double r, double g, double b) {
        red = r;
        green = g;
        blue = b;
    }

    void clip(){
        if(this->red > 1) this->red = 1;
        if(this->red < 0) this->red = 0;
        if(this->green > 1) this->green = 1;
        if(this->green < 0) this->green = 0;
        if(this->blue > 1) this->blue = 1;
        if(this->blue < 0) this->blue = 0;
    }
};

struct Coefficients{
    double ambient, diffuse, specular, reflection;
};

struct QuadraticCoefficients{
    double A, B, C, D, E, F, G, H, I, J;
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

    Vector3D Add(Vector3D vector){
        Vector3D newPoint;
        newPoint.x = this->x + vector.x;
        newPoint.y = this->y + vector.y;
        newPoint.z = this->z + vector.z;
        return newPoint;
    }

    Vector3D Add(double value){
        Vector3D newPoint;
        newPoint.x = this->x + value;
        newPoint.y = this->y + value;
        newPoint.z = this->z + value;
        return newPoint;
    }

    Vector3D Subtract(Vector3D vector){
        Vector3D newPoint;
        newPoint.x = this->x - vector.x;
        newPoint.y = this->y - vector.y;
        newPoint.z = this->z - vector.z;
        return newPoint;
    }

    Vector3D Multiply(Vector3D vector){
        Vector3D newPoint;
        newPoint.x = this->x * vector.x;
        newPoint.y = this->y * vector.y;
        newPoint.z = this->z * vector.z;
        return newPoint;
    }

    Vector3D Multiply(double value){
        Vector3D newPoint;
        newPoint.x = this->x * value;
        newPoint.y = this->y * value;
        newPoint.z = this->z * value;
        return newPoint;
    }

    Vector3D Cross_Product(Vector3D v){
        Vector3D cross;
        cross.x = this->y * v.z - this->z * v.y;
        cross.y = this->z * v.x - this->x * v.z;
        cross.z = this->x * v.y - this->y * v.x;

        return cross;
    }

    double dot(Vector3D v){
        return x*v.x + y*v.y + z*v.z;
    }

    double computeDistance(Vector3D targetvector){
        return sqrt(pow(this->x - targetvector.x, 2) + pow(this->y - targetvector.y, 2) + pow(this->z - targetvector.z, 2));
    }
};

class Ray{
public:
    Vector3D origin, direction;
    Ray(){
        origin = Vector3D();
        direction = Vector3D();  // Rd
    }
    Ray(Vector3D origin, Vector3D direction){
        this->origin = origin;
        direction.normalize();
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
        glPointSize(5);
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

class Object{

public:
    Vector3D reference_point;
    double height, width, length;
    Color color;
    Coefficients coefficient;
    int shininess;


    Object(){
        shininess = 0;
    }

    virtual void draw(){}

    virtual double intersect(Ray ray, Color *color, int level){
        return -1.0;
    }

    void setReferencePoint(double x, double y, double z){
        reference_point = Vector3D(x, y, z);
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

    void ComputeCoefficients(Vector3D intersecting_point, Ray lightray, Ray ray_normal, Ray ray, PointLight* pointlight, Color intersect_point_color, Color *color){
        // diffuse
        double lambert_value = max(0.0,ray_normal.direction.dot(lightray.direction));
        color->red += pointlight->getColor().red * intersect_point_color.red * coefficient.diffuse * lambert_value;
        color->green += pointlight->getColor().green * intersect_point_color.green * coefficient.diffuse * lambert_value;
        color->blue += pointlight->getColor().blue * intersect_point_color.blue * coefficient.diffuse * lambert_value;
        
        // specular
        Vector3D dir =  (ray_normal.direction.Multiply(lightray.direction.dot(ray_normal.direction) * 2)).Subtract(lightray.direction);
        Ray reflected_ray = Ray(intersecting_point, dir);
        Vector3D V = ray.direction.Multiply(-1);
        double phong_value = max(0.0,reflected_ray.direction.dot(V));
        color->red += pointlight->getColor().red * intersect_point_color.red * coefficient.specular * pow(phong_value, shininess);
        color->green += pointlight->getColor().green * intersect_point_color.green * coefficient.specular * pow(phong_value, shininess);
        color->blue += pointlight->getColor().blue * intersect_point_color.blue * coefficient.specular * pow(phong_value, shininess);
    }

    ~Object(){

    }
};

vector<Object*> objects;
vector<PointLight*> pointLights;
vector<SpotLight*> spotLights;

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

    Vector3D getNormal(Vector3D point){
        Vector3D normal = Vector3D(point.x - center.x, point.y - center.y, point.z - center.z);
        normal.normalize();
        return normal;
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

    double intersect(Ray ray, Color *color, int level){
        // Intersection tMin //
        ray.origin = ray.origin.Subtract(center); // translate the ray origin to the center of the sphere
        
        double ld, d, l;
        ld = ray.direction.dot(ray.origin.Multiply(-1));
        if(ld < 0){
         return -1.0;
        }
        
        d = sqrt(ray.origin.dot(ray.origin) - ld * ld);

        if((d*d) > (radius*radius)) {
            return -1.0;
        }
        
        l = sqrt(pow(radius,2) - pow(d,2));
        
        double t1 = ld - l;
        double t2 = ld + l;
        double tmin;
        if(level == 0){
            if(t1 < t2){
                if(t1 < 0)
                    return -1.0;
                tmin = t1;
                return tmin;
            }
            else{
                if(t2 < 0)
                    return -1.0;
                tmin = t2;
                return tmin;
            }
        }

        // Lighting Code according to the phong model //
        ray.origin = ray.origin.Add(center); 
        Vector3D intersecting_point = ray.origin.Add(ray.direction.Multiply(tmin));

        Color intersectpoint_color = getColor();
        color->red = intersectpoint_color.red * getCoEfficients().ambient;
        color->green = intersectpoint_color.green * getCoEfficients().ambient;
        color->blue = intersectpoint_color.blue * getCoEfficients().ambient;

        Vector3D intersecting_point_normal = intersecting_point.Subtract(center);
        Ray ray_normal = Ray(intersecting_point, intersecting_point_normal);
        

        for(int i = 0; i < pointLights.size(); i++){
            // Intersecting point of light ray and object //
            Vector3D dir = pointLights[i]->getPosition().Subtract(intersecting_point);

            Ray lightray = Ray(pointLights[i]->getPosition(),dir);
            

            // check if any other object is in the way //
            double tMinimmum = 99999999, t;
            for(int j = 0; j < objects.size(); j++){
                Color *tempcolor = new Color;
                double t = objects[j]->intersect(lightray, tempcolor, 0);

                if(t > 0 && t < tMinimmum){
                    tMinimmum = t;
                }
            }
            // get the intersecting point of the light ray and the object //
            Vector3D min_obj_intersecting_point = lightray.origin.Add(lightray.direction.Multiply(tMinimmum));

            // if intersecting point is in shadow
            if(min_obj_intersecting_point.computeDistance(lightray.origin) < intersecting_point.computeDistance(lightray.origin)){
                continue;
            }

            // Calculate Diffuse and Spcecular lighting //
            ComputeCoefficients(intersecting_point, lightray, ray_normal, ray, pointLights[i], intersectpoint_color, color);
        }

        // Reflection //
        if(level >= recursion_level) return tmin;

        double epsilon = 0.0000001;
        Vector3D eye_dir =  ray.direction.Subtract(ray_normal.direction.Multiply(ray.direction.dot(ray_normal.direction) * 2));
        intersecting_point.Add(eye_dir.Multiply(epsilon));
        Ray reflected_ray = Ray(intersecting_point, eye_dir);

        double t, tMinimum = 99999999;
        int nearest = 9999999;

        for(int i = 0; i < objects.size(); i++){
            Color *tempcolor = new Color;
            t = objects[i]->intersect(reflected_ray, tempcolor, 0);
            // cout << "t: " << t << endl;
            if(t > 0 && t < tMinimum){
                tMinimum = t;
                nearest = i;
            }
        }
        
        if(nearest != 9999999){
            Color *reflected_color = new Color;
            objects[nearest]->intersect(reflected_ray, reflected_color, level + 1);
            test << "ref: " << getCoEfficients().reflection << endl;
            color->red += reflected_color->red * getCoEfficients().reflection;
            color->green += reflected_color->green * getCoEfficients().reflection;
            color->blue += reflected_color->blue * getCoEfficients().reflection;
        }

        return -1.0;
    }

    ~Sphere(){

    }
};

class Triangle : public Object{
    
public:
    Vector3D a, b, c;

    Triangle(){
        a = Vector3D();
        b = Vector3D();
        c = Vector3D();
    }
    Triangle(Vector3D a, Vector3D b, Vector3D c){
        this->a = a;
        this->b = b;
        this->c = c;
    }
    void seta(Vector3D a){
        this->a = a;
    }
    void setb(Vector3D b){
        this->b = b;
    }
    void setc(Vector3D c){
        this->c = c;
    }

    void draw(){
        glColor3f(getColor().red, getColor().green, getColor().blue);
        glBegin(GL_TRIANGLES);{
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
        }glEnd();
    }

    double intersect(Ray ray, Color *color, int level){
        double beta_matrix[3][3], gamma_matrix[3][3], T_matrix[3][1], A_matrix[3][3];
        double determinantBeta, determinantGamma, determinantA, determinantT;
        double beta, gamma, T, A;
         // beta matrix
        beta_matrix[0][0] = a.x - ray.origin.x;
        beta_matrix[0][1] = a.y - c.x;
        beta_matrix[0][2] = ray.direction.x;
        beta_matrix[1][0] = a.y - ray.origin.y;
        beta_matrix[1][1] = a.y - c.y;
        beta_matrix[1][2] = ray.direction.y;
        beta_matrix[2][0] = a.z - ray.origin.z;
        beta_matrix[2][1] = a.z - c.z;
        beta_matrix[2][2] = ray.direction.z;

        // gamma matrix
        gamma_matrix[0][0] = a.x - b.x;
        gamma_matrix[0][1] = a.x - ray.origin.x;
        gamma_matrix[0][2] = ray.direction.x;
        gamma_matrix[1][0] = a.y - b.y;
        gamma_matrix[1][1] = a.y - ray.origin.y;
        gamma_matrix[1][2] = ray.direction.y;
        gamma_matrix[2][0] = a.z - b.z;
        gamma_matrix[2][1] = a.z - ray.origin.z;
        gamma_matrix[2][2] = ray.direction.z;

        // t matrix
        T_matrix[0][0] = a.x - b.x;
        T_matrix[0][1] = a.x - c.x;
        T_matrix[0][2] = a.x - ray.origin.x;
        T_matrix[1][0] = a.y - b.y;
        T_matrix[1][1] = a.y - c.y;
        T_matrix[1][2] = a.y - ray.origin.y;
        T_matrix[2][0] = a.z - b.z;
        T_matrix[2][1] = a.z - c.z;
        T_matrix[2][2] = a.z - ray.origin.z;

        // A matrix
        A_matrix[0][0] = a.x - b.x;
        A_matrix[0][1] = a.x - c.x;
        A_matrix[0][2] = ray.direction.x;
        A_matrix[1][0] = a.y - b.y;
        A_matrix[1][1] = a.y - c.y;
        A_matrix[1][2] = ray.direction.y;
        A_matrix[2][0] = a.z - b.z;
        A_matrix[2][1] = a.z - c.z;
        A_matrix[2][2] = ray.direction.z;

        // Calculating determinant
        determinantBeta = beta_matrix[0][0] * (beta_matrix[1][1] * beta_matrix[2][2] - beta_matrix[1][2] * beta_matrix[2][1]) - beta_matrix[0][1] * (beta_matrix[1][0] * beta_matrix[2][2] - beta_matrix[1][2] * beta_matrix[2][0]) + beta_matrix[0][2] * (beta_matrix[1][0] * beta_matrix[2][1] - beta_matrix[1][1] * beta_matrix[2][0]);
        determinantGamma = gamma_matrix[0][0] * (gamma_matrix[1][1] * gamma_matrix[2][2] - gamma_matrix[1][2] * gamma_matrix[2][1]) - gamma_matrix[0][1] * (gamma_matrix[1][0] * gamma_matrix[2][2] - gamma_matrix[1][2] * gamma_matrix[2][0]) + gamma_matrix[0][2] * (gamma_matrix[1][0] * gamma_matrix[2][1] - gamma_matrix[1][1] * gamma_matrix[2][0]);
        determinantA = A_matrix[0][0] * (A_matrix[1][1] * A_matrix[2][2] - A_matrix[1][2] * A_matrix[2][1]) - A_matrix[0][1] * (A_matrix[1][0] * A_matrix[2][2] - A_matrix[1][2] * A_matrix[2][0]) + A_matrix[0][2] * (A_matrix[1][0] * A_matrix[2][1] - A_matrix[1][1] * A_matrix[2][0]);
        determinantT = T_matrix[0][0] * (T_matrix[1][1] * T_matrix[2][2] - T_matrix[1][2] * T_matrix[2][1]) - T_matrix[0][1] * (T_matrix[1][0] * T_matrix[2][2] - T_matrix[1][2] * T_matrix[2][0]) + T_matrix[0][2] * (T_matrix[1][0] * T_matrix[2][1] - T_matrix[1][1] * T_matrix[2][0]);

        // Calculating beta, gamma, t, A
        if(determinantA == 0) return -1;

        beta = determinantBeta / determinantA;
        gamma = determinantGamma / determinantA;
        T = determinantT / determinantA;
        A = determinantA / determinantA;

        double tmin;
        if(beta + gamma < 1 && beta > 0 && gamma > 0 && T > 0){
            tmin = T;
        }
        else tmin = -1;
        if(level == 0) {
            return tmin;
        }


        // Lighting Code according to the phong model //
        Vector3D intersecting_point = ray.origin.Add(ray.direction.Multiply(tmin));
        Color intersectpoint_color = getColor();

        color->red = intersectpoint_color.red * getCoEfficients().ambient;
        color->green = intersectpoint_color.green * getCoEfficients().ambient;
        color->blue = intersectpoint_color.blue * getCoEfficients().ambient;

        // normal
        Ray ray_normal;
        Vector3D intersecting_point_normal = b.Subtract(a).Cross_Product(c.Subtract(a));
        intersecting_point_normal.normalize();
        if(((ray.direction.Multiply(-1)).dot(intersecting_point_normal)) > 0){
            ray_normal.origin = intersecting_point;
            ray_normal.direction = intersecting_point_normal;
        }
        else{
            ray_normal.origin = intersecting_point;
            ray_normal.direction = intersecting_point_normal.Multiply(-1);
        }    
        
        for(int i = 0; i < pointLights.size(); i++){
            // Intersecting point of light ray and object //
            Vector3D dir = pointLights[i]->getPosition().Subtract(intersecting_point);

            Ray lightray = Ray(pointLights[i]->getPosition(),dir);
            

            // check if any other object is in the way //
            double tMinimmum = 99999999, t;
            for(int j = 0; j < objects.size(); j++){
                Color *tempcolor = new Color;
                double t = objects[j]->intersect(lightray, tempcolor, 0);

                if(t > 0 && t < tMinimmum){
                    tMinimmum = t;
                }
            }
            // get the intersecting point of the light ray and the object //
            Vector3D min_obj_intersecting_point = lightray.origin.Add(lightray.direction.Multiply(tMinimmum));

            // if intersecting point is in shadow
            if(min_obj_intersecting_point.computeDistance(lightray.origin) < intersecting_point.computeDistance(lightray.origin)){
                continue;
            }

            // Calculate Diffuse and Spcecular lighting //
            ComputeCoefficients(intersecting_point, lightray, ray_normal, ray, pointLights[i], intersectpoint_color, color);
        }

        // Reflection //
        if(level >= recursion_level) return tmin;

        double epsilon = 0.0000001;
        Vector3D eye_dir =  ray.direction.Subtract(ray_normal.direction.Multiply(ray.direction.dot(ray_normal.direction) * 2));
        intersecting_point.Add(eye_dir.Multiply(epsilon));
        Ray reflected_ray = Ray(intersecting_point, eye_dir);

        double t, tMinimum = 99999999;
        int nearest = 9999999;

        for(int i = 0; i < objects.size(); i++){
            Color *tempcolor = new Color;
            t = objects[i]->intersect(reflected_ray, tempcolor, 0);
            // cout << "t: " << t << endl;
            if(t > 0 && t < tMinimum){
                tMinimum = t;
                nearest = i;
            }
        }
        
        // cout << "near: " << nearest << endl;
        if(nearest != 9999999){
            Color *reflected_color = new Color;
            objects[nearest]->intersect(reflected_ray, reflected_color, level + 1);
            color->red += reflected_color->red * getCoEfficients().reflection;
            color->green += reflected_color->green * getCoEfficients().reflection;
            color->blue += reflected_color->blue * getCoEfficients().reflection;
        }


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

    double intersect(Ray ray, Color *color, int level){
        double Aq, Bq, Cq;
        Aq = quadcoefficients.A * ray.direction.x * ray.direction.x + quadcoefficients.B * ray.direction.y * ray.direction.y + quadcoefficients.C * ray.direction.z * ray.direction.z;
        Aq += quadcoefficients.D * ray.direction.x * ray.direction.y + quadcoefficients.E * ray.direction.x * ray.direction.z + quadcoefficients.F * ray.direction.y * ray.direction.z;

        Bq = 2 * quadcoefficients.A * ray.direction.x * ray.origin.x + 2 * quadcoefficients.B * ray.direction.y * ray.origin.y + 2 * quadcoefficients.C * ray.direction.z * ray.origin.z;
        Bq += quadcoefficients.D * (ray.direction.x * ray.origin.y + ray.direction.y * ray.origin.x) + quadcoefficients.E * (ray.direction.x * ray.origin.z + ray.direction.z * ray.origin.x) + quadcoefficients.F * (ray.direction.y * ray.origin.z + ray.direction.z * ray.origin.y);
        Bq += quadcoefficients.G * ray.direction.x + quadcoefficients.H * ray.direction.y + quadcoefficients.I * ray.direction.z;

        Cq = quadcoefficients.A * ray.origin.x * ray.origin.x + quadcoefficients.B * ray.origin.y * ray.origin.y + quadcoefficients.C * ray.origin.z * ray.origin.z;
        Cq += quadcoefficients.D * ray.origin.x * ray.origin.y + quadcoefficients.E * ray.origin.x * ray.origin.z + quadcoefficients.F * ray.origin.y * ray.origin.z;
        Cq += quadcoefficients.G * ray.origin.x + quadcoefficients.H * ray.origin.y + quadcoefficients.I * ray.origin.z;
        Cq += quadcoefficients.J;
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
        setReferencePoint(-floor_width / 2, -floor_width/2, 0);
    }

    void draw(){
        for( int i = -floor_width/(2*tile_width); i < floor_width/(2*tile_width); i++){
            for( int j = -floor_width/(2*tile_width); j < floor_width/(2*tile_width); j++){
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

    Color getColorAt(Vector3D intPoint)
    {
        // if (intPoint.x < reference_point.x || intPoint.x > -reference_point.x) {
        //     return Color();
        // } 
        
        // if (intPoint.y < reference_point.y || intPoint.y > -reference_point.y) {
        //     return Color();
        // } 

        int row = (intPoint.x)/tile_width;
        int col = (intPoint.y)/tile_width;

        if ((row + col) % 2 == 0) {
            return Color(0, 0, 0);
        }
        else {
            return Color(1, 1, 1);
        }
    }

    double intersect(Ray ray, Color *color, int level) {
        Vector3D n(0, 0, 1);
        if(n.dot(ray.direction) == 0) return -1.0;
        double tmin = (-1) * (n.dot(ray.origin) / n.dot(ray.direction));

        // check if intersection point is on the floor
        Vector3D intersectionPoint = ray.origin.Add(ray.direction.Multiply(tmin));
        if (intersectionPoint.x < reference_point.x || intersectionPoint.x > -reference_point.x) return -1;
        if (intersectionPoint.y < reference_point.y || intersectionPoint.y > -reference_point.y) return -1;
        if(level == 0)
            return tmin;

        // Lighting Code according to the phong model //

        Color intersectpoint_color = getColorAt(intersectionPoint);
        color->red = intersectpoint_color.red * getCoEfficients().ambient;
        color->green = intersectpoint_color.green * getCoEfficients().ambient;
        color->blue = intersectpoint_color.blue * getCoEfficients().ambient;


        Ray ray_normal = Ray(intersectionPoint, n);
        

        for(int i = 0; i < pointLights.size(); i++){
            // Intersecting point of light ray and object //
            Vector3D dir = pointLights[i]->getPosition().Subtract(intersectionPoint);

            Ray lightray = Ray(pointLights[i]->getPosition(),dir);
            

            // check if any other object is in the way //
            double tMinimmum = 99999999, t;
            for(int j = 0; j < objects.size(); j++){
                Color *tempcolor = new Color;
                double t = objects[j]->intersect(lightray, tempcolor, 0);

                if(t > 0 && t < tMinimmum){
                    tMinimmum = t;
                }
            }
            // get the intersecting point of the light ray and the object //
            Vector3D min_obj_intersecting_point = lightray.origin.Add(lightray.direction.Multiply(tMinimmum));

            // if intersecting point is in shadow
            if(min_obj_intersecting_point.computeDistance(lightray.origin) < intersectionPoint.computeDistance(lightray.origin)){
                continue;
            }

            // Calculate Diffuse and Spcecular lighting //
            ComputeCoefficients(intersectionPoint, lightray, ray_normal, ray, pointLights[i], intersectpoint_color, color);
        }

        // Reflection //
        if(level >= recursion_level) return tmin;

        double epsilon = 0.0000001;
        Vector3D eye_dir =  ray.direction.Subtract(ray_normal.direction.Multiply(ray.direction.dot(ray_normal.direction) * 2));
        intersectionPoint.Add(eye_dir.Multiply(epsilon));
        Ray reflected_ray = Ray(intersectionPoint, eye_dir);

        double t, tMinimum = 99999999;
        int nearest = 9999999;

        for(int i = 0; i < objects.size(); i++){
            Color *tempcolor = new Color;
            t = objects[i]->intersect(reflected_ray, tempcolor, 0);
            if(t > 0 && t < tMinimum){
                tMinimum = t;
                nearest = i;
            }
        }

        if(nearest != 9999999){
            Color *reflected_color = new Color;
            objects[nearest]->intersect(reflected_ray, reflected_color, level + 1);
            color->red += reflected_color->red * getCoEfficients().reflection;
            color->green += reflected_color->green * getCoEfficients().reflection;
            color->blue += reflected_color->blue * getCoEfficients().reflection;
        }

        return tmin;
    }
};






