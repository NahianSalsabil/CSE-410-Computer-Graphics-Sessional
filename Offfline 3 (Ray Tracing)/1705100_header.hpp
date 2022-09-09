#include <vector>
#include <float.h>
#define pi (2.0*acos(0.0))
#define deg(a) ((a*180.0)/pi)
#define eps 0.000001
#define eps1 0.1
#define eps2 1

using namespace std;

class Object;
class PointLight;
class SpotLight;

vector <Object*> objects;
vector <PointLight> point_lights;
vector <SpotLight> spot_lights;

void test_func()
{
    cout << "hello from header file" << endl;
}

///------------------------------------------------------ VECTOR START --------------------------------------------------------
struct Vector
{
    double x, y, z;
    Vector(){ x = y = z = 0; }

    Vector(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    inline Vector operator+(Vector a)
    {
        return {x + a.x, y + a.y, z + a.z};
    }

    inline Vector operator-(Vector a)
    {
        return {x - a.x, y - a.y, z - a.z};
    }

    inline Vector operator*(double a)
    {
        return {a*x, a*y, a*z};
    }

    inline Vector operator/(double a)
    {
        return {x/a, y/a, z/a};
    }

    inline Vector operator-()
	{
	    return {-x, -y, -z};
	}

	inline double operator*(Vector v)
    {
        /// dot product
        return x*v.x + y*v.y + z*v.z;
    }

    inline Vector operator^(Vector v)
    {
        /// cross product
        return { (y*v.z - v.y*z), (z*v.x - v.z*x), (x*v.y - v.x*y) };
    }

    double magnitude()
    {
        return sqrt(x*x + y*y + z*z);
    }

    Vector normalize()
    {
        double mag = this->magnitude();
        if(!mag){
            cout << "Error: Can't normalize. Point magnitude is zero." << endl;
        }
        *this = *this/mag;
        return *this;
    }

    double calcDist(Vector v)
    {
        return ((*this) - v).magnitude();
    }

    friend ostream &operator<<(ostream &out, const Vector &p)
    {
        out << '(' << p.x << ", " << p.y << ", " << p.z << ')';
        return out;
    }

    friend ostream &operator<<=(ostream &out, const Vector &p)
    {
        out << p.x << ' ' << p.y << ' ' << p.z << endl;
        return out;
    }

    friend istream &operator>>(istream &in, Vector &p)
    {
        in >> p.x >> p.y >> p.z;
        return in;
    }
};

Vector camera_pos, up_vector, right_vector, look_vector;

///------------------------------------------------------ RAY START --------------------------------------------------------
class Ray
{
    Vector origin, dir;
public:
    Ray(){}

    Ray(Vector o, Vector d)
    {
        this->origin = o;
        this->dir = d.normalize();
    }

    // getters
    Vector getOrigin(){return origin;}
    Vector getDir(){return dir;}

    // setters
    void setOrigin(Vector origin){this->origin = origin;}
    void setDir(Vector dir){this->dir = dir;}

    Vector p(double t)
    {
        if(t == DBL_MAX) return {DBL_MAX, DBL_MAX, DBL_MAX};
        return origin + dir*t;
    }

    friend ostream &operator<<(ostream &out, const Ray &ray)
    {
        out << endl << "Ray:" << endl << endl;
        out << "Origin: " << ray.origin << endl;
        out << "Direction: " << ray.dir << endl;

        return out;
    }

};


///------------------------------------------------------ COLOR START --------------------------------------------------------
struct Color
{
    double r,g,b;

    Color(){r=g=b=1;}
    Color(double r, double g, double b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    inline Color operator*(double a)
    {
        return {a*r, a*g, a*b};
    }

    inline Color operator*(Color c)
    {
        return {this->r * c.r, this->g * c.g, this->b * c.b};
    }

    inline Color operator+(Color c)
    {
        return {this->r + c.r, this->g + c.g, this->b + c.b};
    }

    void clip()
    {
        /// clips out of range [0, 1]
        r = min(max(r, 0.0), 1.0);
        g = min(max(g, 0.0), 1.0);
        b = min(max(b, 0.0), 1.0);
    }

    friend ostream &operator<<(ostream &out, const Color &p)
    {
        /**
            (red, green, blue)
        */
        out << '(' << p.r << ", " << p.g << ", " << p.b << ')';
        return out;
    }

    friend istream &operator>>(istream &in, Color &p)
    {
        /**
            (red, green, blue)
        */

        in >> p.r >> p.g >> p.b;
        return in;
    }
};


/// ------------------------------------------------- POINTLIGHT START ---------------------------------------------------------

class PointLight{
    Vector src_pos;
    Color color;

public:
    PointLight(){}
    PointLight(Vector src_pos, Color color)
    {
        this->src_pos = src_pos;
        this->color = color;
    }

    // getters
    Vector getSrcPosition() {return src_pos;}
    Color getColor() {return color;}
};

/// ------------------------------------------------- SPOTLIGHT START ---------------------------------------------------------

class SpotLight{
    Vector src_pos;
    Color color;
    Vector dir;
    double cut_off_angle; // deg

public:
    SpotLight(){}
    SpotLight(Vector src_pos, Color color, Vector dir, double cut_off_angle)
    {
        this->src_pos = src_pos;
        this->color = color;
        this->dir = dir.normalize();
        this->cut_off_angle = cut_off_angle;
    }

    // getters
    Vector getSrcPosition() {return src_pos;}
    Vector getDir() {return dir;}
    Color getColor() {return color;}
    double getCutOffAngle() {return cut_off_angle;}
};


///-------------------------------------------------- COEFFICIENT START ---------------------------------------------------------
struct Coefficient
{
    //  ambient, diffuse, specular, recursive reflection coefficient
    double amb, diff, spec, recur;

    Coefficient(){ amb = diff = spec = recur = 0; }

    Coefficient(double amb, double diff, double spec, double recur)
    {
        this->amb = amb;
        this->diff = diff;
        this->spec = spec;
        this->recur = recur;
    }


    friend ostream &operator<<(ostream &out, const Coefficient &p)
    {
        /**
            (ambient, diffuse, specular, recursive)
        */
        out << '(' << p.amb << ", " << p.diff << ", " << p.spec << ", " << p.recur << ')';
        return out;
    }

    friend istream &operator>>(istream &in, Coefficient &p)
    {
        /**
            (ambient, diffuse, specular, recursive)
        */

        in >> p.amb >> p.diff >> p.spec >> p.recur;
        return in;
    }
} ;


///-------------------------------------------------- OBJECT START ---------------------------------------------------------

class Object
{
protected:
    Color color;
    Coefficient coeff;
    int shine;

public:

    Object(){ shine = 0; }

    // getters
    Color getColor() {return this->color;}
    Coefficient getCoefficient() {return this->coeff;}
    int getShine() {return this->shine;}

    // setters
    void setColor(Color c){ this->color = c; }
    void setShine(int shine){ this->shine = shine; }
    void setCoefficients(Coefficient c){ this->coeff = c;}

    void colorObjectAt(Color &color, Color color_of_intersection, Vector point_of_intersection, Vector normal, Ray ray)
    {
        // compute ambient light component of reflected ray
        color = color_of_intersection * this->coeff.amb;

        /// --------------------------------------------------------------------------- POINT LIGHTS START --------------------------
        // compute diffuse and specular reflection components of reflected ray
        for(PointLight light : point_lights){
            // cast ray from pl.light_pos to intersectionPoint
            Ray i_ray(light.getSrcPosition(), point_of_intersection - light.getSrcPosition());

            // if intersectionPoint is in shadow, the diffuse and specular components need not be calculated
            double temp_t, temp_tmin = DBL_MAX;

            for(Object* obj : objects) {
                Color temp_c(0, 0, 0);
                temp_t = obj->intersect(i_ray, temp_c, 0);    // coloring is not required
                if(temp_t > 0 and temp_t < temp_tmin) {
                    temp_tmin = temp_t;
                }
            }

            if(point_of_intersection.calcDist(light.getSrcPosition()) - eps <= i_ray.p(temp_tmin).calcDist(light.getSrcPosition())) {


                /// PHONG MODEL START

                // point of intersection is closest to light source. therefore no shadow. calculate diffuse and specular component

                // diffuse
                double lambertValue = (-(i_ray.getDir()))*normal;
                color = color + light.getColor() * this->coeff.diff * max(lambertValue, 0.0) * color_of_intersection;

                // specular
                Ray r_ray(point_of_intersection, i_ray.getDir() - normal*((i_ray.getDir()*normal)*2));
                double phongValue = (-ray.getDir())*r_ray.getDir();
                color = color + light.getColor() * this->coeff.spec * max( pow(phongValue, this->shine) , 0.0) * color_of_intersection;

            }

        }

        /// --------------------------------------------------------------------------- SPOT LIGHTS START --------------------------

        // compute diffuse and specular reflection components of reflected ray
        for(SpotLight light : spot_lights){
            double cos_B = light.getDir()*((point_of_intersection - light.getSrcPosition()).normalize());

            if(light.getCutOffAngle() <= deg(acos(cos_B))){
                // outside cutoff
                continue;
            }

            double intensity_weight = pow(cos_B, eps1);


            // cast ray from pl.light_pos to intersectionPoint
            Ray i_ray(light.getSrcPosition(), point_of_intersection - light.getSrcPosition());

            // if intersectionPoint is in shadow, the diffuse and specular components need not be calculated
            double temp_t, temp_tmin = DBL_MAX;

            for(Object* obj : objects) {
                Color temp_c(0, 0, 0);
                temp_t = obj->intersect(i_ray, temp_c, 0);    // coloring is not required
                if(temp_t > 0 and temp_t < temp_tmin) {
                    temp_tmin = temp_t;
                }
            }

            if(point_of_intersection.calcDist(light.getSrcPosition()) - eps <= i_ray.p(temp_tmin).calcDist(light.getSrcPosition())) {


                /// PHONG MODEL START

                // point of intersection is closest to light source. therefore no shadow. calculate diffuse and specular component

                // diffuse
                double lambertValue = (-(i_ray.getDir()))*normal;
                color = color + light.getColor() * intensity_weight * this->coeff.diff * max(lambertValue, 0.0) * color_of_intersection;

                // specular
                Ray r_ray(point_of_intersection, i_ray.getDir() - normal*((i_ray.getDir()*normal)*2));
                double phongValue = (-ray.getDir())*r_ray.getDir();
                color = color + light.getColor() * intensity_weight * this->coeff.spec * max( pow(phongValue, this->shine) , 0.0) * color_of_intersection;

            }

        }

    }

    void recursiveReflection(Ray ray, Vector point_of_intersection, Color &color, Vector normal, int level)
    {

        /*
            construct reflected ray slightly forward from the intersection point,
            by moving the start a little bit towards the reflection direction,
            to avoid self intersection
        */
        Vector r_dir = ray.getDir() - normal*((ray.getDir()*normal)*2);
        Ray reflected_ray(point_of_intersection + r_dir, r_dir);

        // find tmin from the nearest intersecting object
        int nearest = -1, i = 0;
        double temp_t, temp_tmin = DBL_MAX;


        for(Object* obj : objects) {
            Color temp_c(0, 0, 0);
            temp_t = obj->intersect(reflected_ray, temp_c, 0);    // coloring is not required

            if(temp_t > 0.0 and temp_t < temp_tmin) {
                temp_tmin = temp_t;
                nearest = i;
            }
            i++;
        }

        if(nearest == -1) {
            // no reflection
            return;
        }

        objects[nearest]->intersect(reflected_ray , color, level-1);

    }


    virtual void draw(){}
    virtual double intersect(Ray r, Color& color, int level)
    {
        cout << "why am i here?" << endl;
        return -1.0;
    }
    virtual void print(){
        cout << "why am i here?" << endl;
    }

    ~Object(){}
};


/// -------------------------------------------------- FLOOR START ---------------------------------------------------------
class Floor : public Object
{
    /**
        * parallel to the xy plane
        */
    double floorWidth, tileWidth;
    Vector center;
    Vector n;

    // has 2 different colors
    Color floorColor[2];
    vector < vector < Vector > > points;

public:
    Floor()
    {
        floorWidth = tileWidth = 0;
        n = {0, 0, 1};
    }

    Floor(double floorWidth, double tileWidth){
        this->floorWidth = floorWidth;
        this->tileWidth = tileWidth;
        n = {0, 0, 1};
    }

    // setters
    void setCenter(Vector center) { this->center = center; }
    void setColor1(Color c) { this->floorColor[0] = c; }
    void setColor2(Color c) { this->floorColor[1] = c; }
    void setColors(Color c1, Color c2) { this->floorColor[0] = c1; this->floorColor[1] = c2; }

    // getters
    double getFloorWidth() {return floorWidth;}
    double getTileWidth() {return tileWidth;}
    Color getColor1() {return floorColor[0];}
    Color getColor2() {return floorColor[1];}
    Color getFloorColorAt(Vector p)
    {
        /// assumption p is on the floor
        int i = abs( p.x + floorWidth/2.0 )/tileWidth;
        int j = abs( p.y - floorWidth/2.0 )/tileWidth;

        return ((i+j)&1 ? floorColor[0]: floorColor[1]);
    }

    void draw(){

        int i, j;
        int tiles_num = ceil(floorWidth/tileWidth) + 1;

        // calc points
        if(points.empty())
        {
            double x, y;

            y = floorWidth/2;
            for(i = 0; i < tiles_num; i++){
                vector <Vector> temp;
                for(j = 0, x = -floorWidth/2; j < tiles_num; j++){
                    temp.push_back({x, y, 0});
                    x += tileWidth;
                }
                points.push_back(temp);
                y -= tileWidth;
            }

        }


        // start drawing
        glTranslated(center.x, center.y, center.z);

        for(i = 0; i < tiles_num; i++){
            for(j = 0; j < tiles_num; j++){
                Color c = ((i+j)&1 ? floorColor[0]: floorColor[1]);
                glColor3f(c.r, c.g, c.b);

                glBegin(GL_QUADS);{
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i][(j+1)%tiles_num].x, points[i][(j+1)%tiles_num].y, points[i][(j+1)%tiles_num].z);
                    glVertex3f(points[(i+1)%tiles_num][(j+1)%tiles_num].x, points[(i+1)%tiles_num][(j+1)%tiles_num].y, points[(i+1)%tiles_num][(j+1)%tiles_num].z);
                    glVertex3f(points[(i+1)%tiles_num][j].x, points[(i+1)%tiles_num][j].y, points[(i+1)%tiles_num][j].z);
                }glEnd();
            }
        }
        glTranslated(-center.x, -center.y, -center.z);
    }

    double intersect(Ray ray, Color& color, int level)
    {

        //---------------------------------------------------------------------------------->>> floor-ray intersect start ->>>

        double tmin = DBL_MAX;    // t for closest intersect

        // verify normal vector direction with respect to camera
        if((this->n)*camera_pos < 0.0) this->n = -(this->n);

        if((this->n)*ray.getDir() != 0.0){
            tmin = - ( (this->n*ray.getOrigin()) / (this->n*ray.getDir()) );
        }

        if(0.0 < tmin && tmin < DBL_MAX) {
            // verify point is on plane
            Vector p = ray.p(tmin);

            if(p.x < -this->floorWidth/2 || p.x > this->floorWidth/2 || p.y < -this->floorWidth/2 || p.y > this->floorWidth/2 ) {
                // point not on the floor
                tmin = DBL_MAX;
            }
        }

        if(tmin != DBL_MAX){
            color = getFloorColorAt(ray.p(tmin));
        }
        else {
            color = {0, 0, 0};
        }
        if(level == 0) {
            // the purpose of the intersect() method is to determine the nearest object only. No color computation is required
            return tmin;
        }


        ///--------------------------------------- FLOOR LIGHTING START -------------------------------------------------

        // get intersection point
        Vector point_of_intersection = ray.p(tmin);

        // get normal to surface for the current pixel
        Vector normal = this->n;
        normal.normalize();

        // get color at intersection point
        Color color_of_intersection = getFloorColorAt(point_of_intersection);

        // phong model
        colorObjectAt(color, color_of_intersection, point_of_intersection, normal, ray);

        ///----------------------------------------- Floor recursive reflection ----------------------------------------------------

        Color reflected_color(0, 0, 0);
        recursiveReflection(ray, point_of_intersection, reflected_color, normal, level);

        // update color using the impact of reflection
        color = color + reflected_color * this->coeff.recur;
        color.clip();

        return tmin;
    }

    friend ostream &operator<<(ostream &out, const Floor &floor)
    {
        out << endl << "Floor:" << endl << endl;
        out << "Color: " << floor.floorColor[0] << ' ' << floor.floorColor[1] << endl;
        out << "Floor width: " << floor.floorWidth << endl;
        out << "Tile width: " << floor.tileWidth << endl;
        out << "Center: " << floor.center << endl;
        out << "Normal: " << floor.n << endl;

        return out;
    }

    void print()
    {
        cout << (*this) << endl;
    }

    ~Floor()
    {
        points.clear();
    }
};



///-------------------------------------------------- SPHERE START ---------------------------------------------------------

class Sphere : public Object
{
    Vector center;
    double radius;


    double angle1;      /*   angle at the center
                                * in radian
                                * [0, 2pi)
                                * on the xy plane
                                * with -ve y axis
                                */
    double angle2;      /*  angle at the center
                                * in radian
                                * [0, pi)
                                * on the xz plane
                                * with +ve x axis
                                */
    int accuracy;       //  smoothness of curve ; value >= 1
    vector < vector < Vector > > points;

public:
    Sphere(){
        radius = 0;
        angle1 = 2*pi;
        angle2 = pi;
        accuracy = 100;
    }

    Sphere( Vector c, double r)
    {
        center = c;
        radius = r;
        angle1 = 2*pi;
        angle2 = pi;
        accuracy = 100;
    }

    void setCenter(Vector center) { this->center = center; }
    void setRadius(double radius) { this->radius = radius; points.clear(); }
    void setAngle1(double angle1) { this->angle1 = angle1; points.clear(); }
    void setAngle2(double angle2) { this->angle2 = angle2; points.clear(); }
    void setAccuracy(int accuracy) { this->accuracy = accuracy; points.clear(); }

    void draw()
    {
        /**
            draws a sphere or spherical segment with center at origin
            parameters:
                double radius:  radius of the sphere
                double angle1:      angle at the center
                                            - in radian
                                            - [0, 2pi)
                                            - on the xy plane
                                            - with -ve y axis
                double angle2:      angle at the center
                                            - in radian
                                            - [0, pi)
                                            - on the xz plane
                                            - with +ve x axis
                int accuracy:   smoothness of curve ; value >= 1
        */

        int i,j;

        // calc points
        if(points.empty()){
            double longitude, dangle1 = angle1/accuracy;
            double latitude, dangle2 = (angle2*2)/accuracy;

            for(i = 0, latitude = -pi ; i < accuracy; i++) {
                vector <Vector> temp;
                for(j = 0, longitude = 0 ; j < accuracy; j++) {
                    temp.push_back({ radius*cos(longitude)*sin(latitude), radius*sin(longitude)*sin(latitude), radius*cos(latitude)});
                    longitude += dangle1;
                }
                points.push_back(temp);
                latitude += dangle2;
            }
        }

        // start drawing
        glColor3f(color.r, color.g, color.b);
        glTranslated(center.x, center.y, center.z);

        for(i = 0; i < accuracy; i++){
            for(j = 0; j < accuracy; j++){
                glBegin(GL_QUADS);{
                    glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                    glVertex3f(points[i][(j+1)%accuracy].x, points[i][(j+1)%accuracy].y, points[i][(j+1)%accuracy].z);
                    glVertex3f(points[(i+1)%accuracy][(j+1)%accuracy].x, points[(i+1)%accuracy][(j+1)%accuracy].y, points[(i+1)%accuracy][(j+1)%accuracy].z);
                    glVertex3f(points[(i+1)%accuracy][j].x, points[(i+1)%accuracy][j].y, points[(i+1)%accuracy][j].z);
                }glEnd();
            }
        }
        glTranslated(-center.x, -center.y, -center.z);
    }

    double intersect(Ray ray, Color& color, int level)
    {
        //---------------------------------------------------------------------------------->>> sphere-ray intersect start ->>>

        double tmin = DBL_MAX;    // t for closest intersect
        double rad_sq = this->radius*this->radius;

        // translate ray so that sphere center is the origin
        ray.setOrigin(ray.getOrigin() - this->center);

        // is ray origin inside/outside/on the sphere?
        int ray_origin_pos;
        if(ray.getOrigin()*ray.getOrigin() < rad_sq) {
            // inside
            ray_origin_pos = -1;
        }
        else if(ray.getOrigin()*ray.getOrigin() > rad_sq) {
            // outside
            ray_origin_pos = 1;
        }
        else {
            // on -> be careful of degeneracies
            ray_origin_pos = 0;
            ///------***
        }

        // find closest point to sphere center
        double tp = - ray.getOrigin()*ray.getDir();

        if(ray_origin_pos == 1 and tp < 0){
            // origin outside and tp < 0 -> no hit
            tmin = DBL_MAX;
        }
        else {
            // find squared distance
            double d_sq = ray.getOrigin()*ray.getOrigin() - tp*tp;

            if(d_sq > rad_sq) {
                // no hit
                tmin = DBL_MAX;
            }
            else {
                // find distance from tp to correct intersection
                double t_p  = sqrt(rad_sq - d_sq);

                if(ray_origin_pos == 1){
                    // origin outside
                    tmin = tp - t_p;
                }
                else if(ray_origin_pos == -1){
                    // origin inside
                    tmin = tp + t_p;
                }
            }
        }

        // undo translation
        ray.setOrigin(ray.getOrigin() + this->center);

        if(level == 0) {
            // the purpose of the intersect() method is to determine the nearest object only. No color computation is required
            return tmin;
        }

        ///--------------------------------------- SPHERE LIGHTING START -------------------------------------------------

        // get intersection point
        Vector point_of_intersection = ray.p(tmin);

        // get normal to surface for the current pixel
        Vector normal = point_of_intersection - this->center;
        normal.normalize();

        // get color at intersection point
        Color color_of_intersection = this->color;

        colorObjectAt(color, color_of_intersection, point_of_intersection, normal, ray);

        ///----------------------------------------- Sphere recursive reflection ----------------------------------------------------


        Color reflected_color(0, 0, 0);
        recursiveReflection(ray, point_of_intersection, reflected_color, normal, level);

        // update color using the impact of reflection
        color = color + reflected_color * this->coeff.recur;
        color.clip();

        return tmin;
    }


    friend ostream &operator<<(ostream &out, const Sphere &sphere)
    {
        out << endl << "Sphere:" << endl << endl;
        out << "Color: " << sphere.color << endl;
        out << "Center: " << sphere.center << endl;
        out << "Radius: " << sphere.radius << endl;

        return out;
    }

    virtual void print()
    {
        cout << (*this) << endl;
    }

    ~Sphere()
    {
        points.clear();
    }
};

///-------------------------------------------------- TRIANGLE START ---------------------------------------------------------

class Triangle : public Object
{
    Vector one, two, three;

    double calcArea(Vector a, Vector b, Vector c)
    {
        return a.x*(b.y*c.z - b.z*c.y) + a.y*(b.z*c.x - b.x*c.z) + a.z*(b.x*c.y - b.y*c.x);
    }

public:
    Triangle(){}

    Triangle( Vector one, Vector two, Vector three)
    {
        this->one = one;
        this->two = two;
        this->three = three;
    }

    void setOne(Vector one) { this->one = one; }
    void setTwo(Vector two) { this->two = two; }
    void setThree(Vector three) { this->three = three; }

    void draw()
    {
        // start drawing
        glColor3f(color.r, color.g, color.b);

        glBegin(GL_TRIANGLES);{
            glVertex3f(one.x, one.y, one.z);
            glVertex3f(two.x, two.y, two.z);
            glVertex3f(three.x, three.y, three.z);
        }glEnd();

    }

    double intersect(Ray ray, Color& color, int level)
    {
        //---------------------------------------------------------------------------------->>> triangle-ray intersect start ->>>

        double tmin = DBL_MAX;    // t for closest intersect

        double area = calcArea(one - two, one - three, ray.getDir());
        double beta = 0;
        double gamma = 0;
//        double alpha = 0;

        if(area == 0.0){
            // no intersection
            return DBL_MAX;
        }
        else{
            beta = calcArea(one - ray.getOrigin(), one - three, ray.getDir())/area;
            gamma = calcArea(one - two, one - ray.getOrigin(), ray.getDir())/area;
//            alpha = 1 - beta - gamma;

            if(beta > 0.0 and gamma > 0.0 and beta+gamma < 1.0){
                // point is on the triangle
                tmin = calcArea(one - two, one - three, one - ray.getOrigin())/area;
            }
            else {
                // no intersection
                tmin = DBL_MAX;
                return tmin;
            }
        }

        if(level == 0) {
            // the purpose of the intersect() method is to determine the nearest object only. No color computation is required
            return tmin;
        }
        ///--------------------------------------- TRIANGLE LIGHTING START -------------------------------------------------

        // get intersection point
        Vector point_of_intersection = ray.p(tmin);

        // get normal to surface for the current pixel
        Vector normal = ((two - one)^(three - one)).normalize();
        if(normal*(ray.getDir()*(-1.0)) < 0.0) normal = -normal;


        // get color at intersection point
        Color color_of_intersection = this->color;

        // phong model
        colorObjectAt(color, color_of_intersection, point_of_intersection, normal, ray);

        ///----------------------------------------- TRIANGLE recursive reflection ----------------------------------------------------

        Color reflected_color(0, 0, 0);
        recursiveReflection(ray, point_of_intersection, reflected_color, normal, level);

        // update color using the impact of reflection
        color = color + reflected_color * this->coeff.recur;
        color.clip();

        return tmin;
    }


    friend ostream &operator<<(ostream &out, const Triangle &triangle)
    {
        out << endl << "Triangle:" << endl << endl;
        out << "Color: " << triangle.color << endl;
        out << "Vertex a: " << triangle.one << endl;
        out << "Vertex b: " << triangle.two << endl;
        out << "Vertex c: " << triangle.three << endl;

        return out;
    }

    virtual void print()
    {
        cout << (*this) << endl;
    }

    ~Triangle(){}
};

///-------------------------------------------------- GENERAL START ---------------------------------------------------------

class General : public Object
{
    double a, b, c, d, e, f, g, h, i, j;
    Vector cube_reference_point;
    double length, width, height;

    bool clip(Vector point)
    {
        if( length != 0.0 && (point.x < cube_reference_point.x || point.x > cube_reference_point.x + length)) return false;
        if( width  != 0.0 && (point.y < cube_reference_point.y || point.y > cube_reference_point.y + width )) return false;
        if( height != 0.0 && (point.z < cube_reference_point.z || point.z > cube_reference_point.z + height)) return false;

        //cout << "here "<< point << endl;
        return true;
    }

public:
    General()
    {
        a = b = c = d = e = f = g = h = i = j = length = width = height = 0.0;
    }

    General(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double length, double width, double height, Vector ref_point)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
        this->e = e;
        this->f = f;
        this->g = g;
        this->h = h;
        this->i = i;
        this->j = j;
        this->cube_reference_point = ref_point;
        this->length = length;
        this->width = width;
        this->height = height;
    }

    void draw()
    {
        // cant draw ;-;
    }

    double intersect(Ray ray, Color& color, int level)
    {
        //---------------------------------------------------------------------------------->>> general surface-ray intersect start ->>>

        double tmin = DBL_MAX;    // t for closest intersect
        double xd = ray.getDir().x;
        double yd = ray.getDir().y;
        double zd = ray.getDir().z;

        double xo = ray.getOrigin().x;
        double yo = ray.getOrigin().y;
        double zo = ray.getOrigin().z;

        // Aq = Axd2 + Byd2 + Czd2 + Dxdyd + Exdzd + Fydzd
        double Aq = a*xd*xd + b*yd*yd + c*zd*zd + d*xd*yd + e*xd*zd + f*yd*zd;

        // Bq = 2*Axoxd + 2*Byoyd + 2*Czozd + D(xoyd + yoxd) + E(xozd + zoxd) + F(yozd + ydzo) + Gxd + Hyd + Izd
        double Bq = 2.0*a*xo*xd + 2.0*b*yo*yd + 2.0*c*zo*zd + d*(xo*yd + yo*xd) + e*(xo*zd + zo*xd) + f*(yo*zd + yd*zo) + g*xd + h*yd + i*zd;

        // Cq = Axo2 + Byo2 + Czo2 + Dxoyo + Exozo + Fyozo + Gxo + Hyo + Izo + J
        double Cq = a*xo*xo + b*yo*yo + c*zo*zo + d*xo*yo + e*xo*zo + f*yo*zo + g*xo + h*yo + i*zo + j;

        double disc = Bq*Bq - 4.0*Aq*Cq;

        if(disc < 0.0){
            // no intersection
            tmin = DBL_MAX;
            return tmin;
        }

        //t0 =( - Bq - ((Bq2 - 4AqCq))^0.5)/ 2Aq
        //t1 =( - Bq + ((Bq2 - 4AqCq))^0.5)/ 2Aq
        double t0 = ( - Bq - sqrt(disc))/ (2.0*Aq);
        double t1 = ( - Bq + sqrt(disc))/ (2.0*Aq);

        t0 = (t0 <= 0.0)? DBL_MAX:t0;
        t1 = (t1 <= 0.0)? DBL_MAX:t1;

        // clipping
        t0 = (clip(ray.p(t0)))? t0:DBL_MAX;
        t1 = (clip(ray.p(t1)))? t1:DBL_MAX;

        tmin = min(t0, t1);


        if(level == 0) {
            // the purpose of the intersect() method is to determine the nearest object only. No color computation is required
            return tmin;
        }
        ///--------------------------------------- GENERAL SURFACE SURFACEGLE LIGHTING START -------------------------------------------------

        // get intersection point
        Vector point_of_intersection = ray.p(tmin);

        // get normal to surface for the current pixel, use partial derivative
        double xn = a * point_of_intersection.x + Vector(a, d, e)*point_of_intersection + g;
        double yn = b * point_of_intersection.y + Vector(d, b, f)*point_of_intersection + h;
        double zn = c * point_of_intersection.z + Vector(e, f, c)*point_of_intersection + i;

        // Rn = [xn yn zn] = [dF/dx dF/dy dF/dz]
        Vector normal = Vector(xn, yn, zn).normalize();
        if(normal*(ray.getDir()*(-1.0)) < 0.0) normal = -normal;


        // get color at intersection point
        Color color_of_intersection = this->color;

        // phong model
        colorObjectAt(color, color_of_intersection, point_of_intersection, normal, ray);

        ///----------------------------------------- GENERAL SURFACE recursive reflection ----------------------------------------------------

        Color reflected_color(0, 0, 0);
        recursiveReflection(ray, point_of_intersection, reflected_color, normal, level);

        // update color using the impact of reflection
        color = color + reflected_color * this->coeff.recur;
        color.clip();

        return tmin;
    }


    friend ostream &operator<<(ostream &out, const General &general)
    {
        out << endl << "General:" << endl << endl;
        out << "Color: " << general.color << endl;
        out << "Eqn: f(x, y, z) = ";
        out << general.a << "x2 + ";
        out << general.b << "y2 + ";
        out << general.c << "z2 + ";
        out << general.d << "xy + ";
        out << general.e << "xz + ";
        out << general.f << "yz + ";
        out << general.g << "x + ";
        out << general.h << "y + ";
        out << general.i << "z + ";
        out << general.j << endl;

        return out;
    }

    virtual void print()
    {
        cout << (*this) << endl;
    }

    ~General(){}
};

