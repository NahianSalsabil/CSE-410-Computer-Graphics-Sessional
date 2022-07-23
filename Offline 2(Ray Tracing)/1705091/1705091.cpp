#include<bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;

#define INF numeric_limits<double>::infinity()

double pi = 2*acos(0.0);

struct point{
    double x, y, z;
};

//=== for stage 4 (Clipping) ====//
struct color{
    double R, G, B;
};

struct Triangle{
    point Points[3];
    color Color;
};
//==================//

//=========== For stage 1 ============//
vector<vector<double>> Matrix_multiply(vector<vector<double>> top_matrix, vector<vector<double>> input_matrix){
    vector<vector<double>> mult_matrix(4);
    for(int i = 0; i < 4; i++)
        mult_matrix[i].resize(4);

    for(int i = 0; i < 4; i++){    
        for(int j = 0; j < 4; j++){    
            mult_matrix[i][j] = 0;    
            for(int k = 0; k < 4; k++){    
                mult_matrix[i][j] += top_matrix[i][k] * input_matrix[k][j];    
            }    
        }    
    } 
    return mult_matrix;
}

point Rodrigues(point x, point a, double rotation_angle){
    double cosa = cos((rotation_angle*pi) / 180);
    double sina = sin((rotation_angle*pi) / 180);

    point part1 = {cosa * x.x, cosa * x.y, cosa * x.z};

    double first_part2 = (1 - cosa) * (x.x * a.x + x.y * a.y + x.z * a.z);
    point part2 = {first_part2 * a.x, first_part2 * a.y, first_part2 * a.z};

    point part3_cross_product = {a.y * x.z - a.z * x.y, a.z * x.x - a.x * x.z, a.x * x.y - a.y * x.x};
    point part3 = {part3_cross_product.x * sina, part3_cross_product.y * sina, part3_cross_product.z * sina};

    return {part1.x + part2.x + part3.x, part1.y + part2.y + part3.y, part1.z + part2.z + part3.z};
}

void print_matrix(vector<vector<double>> points){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cout << setprecision(7) << points[i][j] << " ";
        }
        cout << endl;
    }
}

//=================//


//========== For stage 4 =================//
int findMaxY(Triangle triangle){
    if(triangle.Points[0].y > triangle.Points[1].y){
        if(triangle.Points[0].y > triangle.Points[2].y) return 0;
        else    return 2;
    }
    else{
        if(triangle.Points[1].y > triangle.Points[2].y) return 1;
        else    return 2;
    }
}

int findMinY(Triangle triangle){
    if(triangle.Points[0].y < triangle.Points[1].y){
        if(triangle.Points[0].y < triangle.Points[2].y) return 0;
        else    return 2;
    }
    else{
        if(triangle.Points[1].y < triangle.Points[2].y) return 1;
        else    return 2;
    }
}

int findMaxX(Triangle triangle){
    if(triangle.Points[0].x > triangle.Points[1].x){
        if(triangle.Points[0].x > triangle.Points[2].x) return 0;
        else    return 2;
    }
    else{
        if(triangle.Points[1].x > triangle.Points[2].x) return 1;
        else    return 2;
    }
}

int findMinX(Triangle triangle){
    if(triangle.Points[0].x < triangle.Points[1].x){
        if(triangle.Points[0].x < triangle.Points[2].x) return 0;
        else    return 2;
    }
    else{
        if(triangle.Points[1].x < triangle.Points[2].x) return 1;
        else    return 2;
    }
}

void print_triangle(vector<vector<double>> output_Point, int dimension){
    // cout << "triangle new point: \n"; 
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            if(i != 3 && j != 3)
                cout << fixed << setprecision(7) << output_Point[j][i]/output_Point[dimension-1][i] << " ";
        }   
        cout << endl;
    }
}



int main(){
    //================================ Modeling Transformation ================================//

    freopen("scene.txt", "r", stdin);
    freopen("stage1.txt", "w", stdout);

    int dimension = 4;
    int triangle_count = 0;

    struct point eye, look, up;
    double fovY, aspectratio, near, far;
    cin >> eye.x >> eye.y >> eye.z;
    cin >> look.x >> look.y >> look.z;
    cin >> up.x >> up.y >> up.z;
    cin >> fovY >> aspectratio >> near >> far;

    stack<vector<vector<double>>> S;
    stack<int> pushed_count;
    int count = 0;
    int ispush = 0;

    vector<vector<double>> Identity_matrix(dimension);

    for(int i = 0; i < dimension; i++)
        Identity_matrix[i].resize(dimension);

    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            if(i == j)
                Identity_matrix[i][j] = 1;
            else
                Identity_matrix[i][j] = 0;
        }
    }

    S.push(Identity_matrix);
    while(true){
        string command;
        cin >> command;
        
        if(command == "triangle"){
            triangle_count++;
            vector<vector<double>> triangle_matrix(dimension);
            for(int i = 0; i < dimension; i++)
                triangle_matrix[i].resize(dimension);

            cin >> triangle_matrix[0][0] >> triangle_matrix[1][0] >> triangle_matrix[2][0];
            cin >> triangle_matrix[0][1] >> triangle_matrix[1][1] >> triangle_matrix[2][1];
            cin >> triangle_matrix[0][2] >> triangle_matrix[1][2] >> triangle_matrix[2][2];

            for(int i = 0; i < dimension; i++){
                for(int j = 0; j < dimension; j++){
                    if(i == 3) triangle_matrix[i][j] = 1;
                    else if(j == 3) triangle_matrix[i][j] = 0;
                }
            }

            vector<vector<double>> output_Point = Matrix_multiply(S.top(), triangle_matrix);

            print_triangle(output_Point, dimension);
        }

        else if(command == "translate"){
            vector<vector<double>> translation_matrix(dimension);
            for(int i = 0; i < dimension; i++)
                translation_matrix[i].resize(4);

            for(int i = 0; i < dimension; i++){
                for(int j = 0; j < dimension; j++){
                    if(i == j) translation_matrix[i][j] = 1;
                    else if(i < 3 && j == 3) cin >> translation_matrix[i][j];
                    else translation_matrix[i][j] = 0;
                }
            }
            S.push(Matrix_multiply(S.top(), translation_matrix));
            if(ispush == 1) count++;
        }

        else if(command == "scale"){
            vector<vector<double>> scaling_matrix(dimension);
            for(int i = 0; i < dimension; i++)
                scaling_matrix[i].resize(4);

            for(int i = 0; i < dimension; i++){
                for(int j = 0; j < dimension; j++){
                    if(i == j && i != dimension-1) cin >> scaling_matrix[i][j];
                    else if(i == dimension-1 && j == dimension-1) scaling_matrix[i][j] = 1;
                    else scaling_matrix[i][j] = 0;
                }
            }
            S.push(Matrix_multiply(S.top(), scaling_matrix));
            if(ispush == 1) count++;
        }

        else if(command == "rotate"){
            double rotation_angle;
            struct point a;
            cin >> rotation_angle >> a.x >> a.y >> a.z;
            double norm = sqrt(pow(a.x,2) + pow(a.y,2) + pow(a.z,2));
            a.x /= norm;
            a.y /= norm;
            a.z /= norm;

            struct point x;
            x.x = 1; x.y = 0; x.z = 0;
            struct point c1 = Rodrigues(x, a, rotation_angle);

            x.x = 0; x.y = 1; x.z = 0;
            struct point c2 = Rodrigues(x, a, rotation_angle);

            x.x = 0; x.y = 0; x.z = 1;
            struct point c3 = Rodrigues(x, a, rotation_angle);

            vector<vector<double>> rotation_matrix(dimension);
            for(int i = 0; i < dimension; i++) 
                rotation_matrix[i].resize(dimension);
            
            rotation_matrix[0][0] = c1.x; rotation_matrix[1][0] = c1.y; rotation_matrix[2][0] = c1.z;
            rotation_matrix[0][1] = c2.x; rotation_matrix[1][1] = c2.y; rotation_matrix[2][1] = c2.z;
            rotation_matrix[0][2] = c3.x; rotation_matrix[1][2] = c3.y; rotation_matrix[2][2] = c3.z;

            for(int i = 0; i < dimension; i++){
                for(int j = 0; j < dimension; j++){
                    if(i == 3 && j == 3) rotation_matrix[i][j] = 1;
                    else if(i == 3) rotation_matrix[i][j] = 0;
                    else if(j == 3) rotation_matrix[i][j] = 0;
                    
                }
            }
            
            S.push(Matrix_multiply(S.top(), rotation_matrix));
            if(ispush == 1) count++;
        }

        else if(command == "push"){
            if(ispush == 0)
                ispush = 1;
            else if(ispush == 1){
                pushed_count.push(count);
                count = 0;
            }
        }

        else if(command == "pop"){
            for(int i = 0; i < count; i++)
                S.pop();
            if(!pushed_count.empty()){
                count = pushed_count.top();
                pushed_count.pop();
            }
            else count = 0;
        }

        else if(command == "end"){
            break;
        }
    }
    cin.clear(); 
    
    fclose(stdin);
    fclose(stdout);


    //========================== View Transformation ==============================//

    freopen("stage1.txt", "r", stdin);
    freopen("stage2.txt", "w", stdout);

    vector<double> l(3), r(3), u(3);
    l[0] = look.x - eye.x;
    l[1] = look.y - eye.y;
    l[2] = look.z - eye.z;

    double norm = sqrt(pow(l[0], 2) + pow(l[1], 2) + pow(l[2], 2));
    l[0] /= norm;
    l[1] /= norm;
    l[2] /= norm;

    r[0] = (l[1] * up.z) - (l[2] * up.y);
    r[1] = (l[2] * up.x) - (l[0] * up.z);
    r[2] = (l[0] * up.y) - (l[1] * up.x);

    norm = sqrt(pow(r[0], 2) + pow(r[1], 2) + pow(r[2], 2));
    r[0] /= norm;
    r[1] /= norm;
    r[2] /= norm;

    u[0] = (r[1] * l[2]) - (r[2] * l[1]);
    u[1] = (r[2] * l[0]) - (r[0] * l[2]);
    u[2] = (r[0] * l[1]) - (r[1] * l[0]); 

    norm = sqrt(pow(u[0], 2) + pow(u[1], 2) + pow(u[2], 2));
    u[0] /= norm;
    u[1] /= norm;
    u[2] /= norm;

    vector<double> eye_vec(dimension-1);
    eye_vec[0] = eye.x;
    eye_vec[1] = eye.y;
    eye_vec[2] = eye.z;

    // Translation matrix
    vector<vector<double>> translation_matrix(dimension);
    for(int i = 0; i < dimension; i++)
        translation_matrix[i].resize(dimension);

    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            if(i == j) translation_matrix[i][j] = 1;
            else if(j == 3 && i < 3) translation_matrix[i][j] = -eye_vec[i];
            else translation_matrix[i][j] = 0;
        }
    }

    //Rotation matrix
    vector<vector<double>> rotation_matrix(dimension);
    for(int i = 0; i < dimension; i++)
        rotation_matrix[i].resize(dimension);
    
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            if(i == 0 && j < 3) rotation_matrix[i][j] = r[j];
            else if(i == 1 && j < 3) rotation_matrix[i][j] = u[j];
            else if(i == 2 && j < 3) rotation_matrix[i][j] = -l[j];
            else if(i == 3 && j == 3) rotation_matrix[i][j] = 1;
            else rotation_matrix[i][j] = 0;
        }
    }

    // View Transformation
    vector<vector<double>> view_transformation_matrix = Matrix_multiply(rotation_matrix, translation_matrix);

    int input_triangle = 0;
    while(input_triangle < triangle_count){

        vector<vector<double>> triangle_matrix(dimension);
        for(int i = 0; i < dimension; i++)
            triangle_matrix[i].resize(dimension);
        
        cin >> triangle_matrix[0][0] >> triangle_matrix[1][0] >> triangle_matrix[2][0];
        cin >> triangle_matrix[0][1] >> triangle_matrix[1][1] >> triangle_matrix[2][1];
        cin >> triangle_matrix[0][2] >> triangle_matrix[1][2] >> triangle_matrix[2][2];

        for(int i = 0; i < dimension; i++){
            for(int j = 0; j < dimension; j++){
                if(i == 3) triangle_matrix[i][j] = 1;
                else if(j == 3) triangle_matrix[i][j] = 0;
            }
        }

        vector<vector<double>> output_Point = Matrix_multiply(view_transformation_matrix, triangle_matrix);

        print_triangle(output_Point, dimension);
        input_triangle++;
    }

    fclose(stdin);
    fclose(stdout);

    //================================= Projection Transformation ==================================//
    freopen("stage2.txt", "r", stdin);
    freopen("stage3.txt", "w", stdout);

    // Create Projection Matrix
    double fovX = fovY * aspectratio;
    double t = near * tan(((fovY/2) * pi) / 180);
    double projection_r = near * tan(((fovX/2) * pi) / 180);

    vector<vector<double>> projection_matrix(dimension);
    for(int i = 0; i < dimension; i++)
        projection_matrix[i].resize(dimension);

    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            if(i == 0 && j == 0) projection_matrix[i][j] = near/projection_r;
            else if(i == 1 && j == 1) projection_matrix[i][j] = near / t;
            else if(i == 2 && j == 2) projection_matrix[i][j] = -(far+near)/(far-near);
            else if(i == 2 && j == 3) projection_matrix[i][j] = -(2*far*near)/(far-near);
            else if(i == 3 && j == 2) projection_matrix[i][j] = -1;
            else projection_matrix[i][j] = 0;
        }
    }

    input_triangle = 0;
    while(input_triangle < triangle_count){
        
        //Read triangle data
        vector<vector<double>> triangle_matrix(dimension);
        for(int i = 0; i < dimension; i++)
            triangle_matrix[i].resize(dimension);
        
        cin >> triangle_matrix[0][0] >> triangle_matrix[1][0] >> triangle_matrix[2][0];
        cin >> triangle_matrix[0][1] >> triangle_matrix[1][1] >> triangle_matrix[2][1];
        cin >> triangle_matrix[0][2] >> triangle_matrix[1][2] >> triangle_matrix[2][2];

        for(int i = 0; i < dimension; i++){
            for(int j = 0; j < dimension; j++){
                if(i == 3) triangle_matrix[i][j] = 1;
                else if(j == 3) triangle_matrix[i][j] = 0;
            }
        }
        // Apply Projection Transformation
        vector<vector<double>> output_Point = Matrix_multiply(projection_matrix, triangle_matrix);

        print_triangle(output_Point, dimension);
        input_triangle++;
    }

    fclose(stdin);
    fclose(stdout);



    //===========================Clipping and Scan Conversion Using z buffer=============================//
    // Read File
    freopen("config.txt", "r", stdin);
    freopen("z_buffer.txt", "w", stdout);

    double screen_width, screen_height, left_lim_x, right_lim_x, bottom_lim_y, top_lim_y, front_limit, rear_limit;

    cin >> screen_width >> screen_height >> left_lim_x >> bottom_lim_y >> front_limit >> rear_limit;
    cin.clear();
    
    right_lim_x = -left_lim_x;
    top_lim_y = -bottom_lim_y;

    fclose(stdin);
    freopen("stage3.txt", "r", stdin);

    struct Triangle triangles[triangle_count];
    input_triangle = 0;

    
    while(input_triangle < triangle_count){
        cin >> triangles[input_triangle].Points[0].x >> triangles[input_triangle].Points[0].y >> triangles[input_triangle].Points[0].z;
        cin >> triangles[input_triangle].Points[1].x >> triangles[input_triangle].Points[1].y >> triangles[input_triangle].Points[1].z;
        cin >> triangles[input_triangle].Points[2].x >> triangles[input_triangle].Points[2].y >> triangles[input_triangle].Points[2].z;

        triangles[input_triangle].Color.R = rand() % 256;
        triangles[input_triangle].Color.G = rand() % 256;
        triangles[input_triangle].Color.B = rand() % 256;

        input_triangle++;
    }
    input_triangle = 0;
    
    // Initialize frame buffer with background color and z buffer with max z;
    vector<vector<double>> z_buffer(screen_width);
    vector<vector<color>> frame_buffer(screen_width);
    for(int i = 0; i < screen_width; i++){
        z_buffer[i].resize(screen_height);
        frame_buffer[i].resize(screen_height);  
    }  
    
    for(int i = 0; i < screen_width; i++){
        for(int j  = 0; j < screen_height; j++){
            z_buffer[i][j] = rear_limit;
            
            frame_buffer[i][j].R = 0;
            frame_buffer[i][j].G = 0;
            frame_buffer[i][j].B = 0;
        }
    }

    //Apply Procedure
    double dx = (right_lim_x - left_lim_x) / screen_width;
    double dy = (top_lim_y - bottom_lim_y) / screen_height;
    double Top_y = top_lim_y - (dy/2);
    double Left_x = left_lim_x + (dx/2);

    for(int i = 0; i < triangle_count; i++){
        // top scanline
        int ret_topy_index = findMaxY(triangles[i]);
            // clipping along y axis
        double top_scan_y = triangles[i].Points[ret_topy_index].y > top_lim_y ? top_lim_y : (triangles[i].Points[ret_topy_index].y < bottom_lim_y ? 0 : triangles[i].Points[ret_topy_index].y);
        int top_scanline = round((Top_y - top_scan_y) / dy);

        // bottom scanline
        int ret_bottomy_index = findMinY(triangles[i]);
            // clipping along y axis
        double bottom_scan_y = triangles[i].Points[ret_bottomy_index].y < bottom_lim_y ? bottom_lim_y : (triangles[i].Points[ret_bottomy_index].y > top_lim_y ? 0: triangles[i].Points[ret_bottomy_index].y);
        int bottom_scanline = round((Top_y - bottom_scan_y) / dy);

        // leftmost x
        int ret_leftx_index = findMinX(triangles[i]);
        double leftmost_x = triangles[i].Points[ret_leftx_index].x;

        //rightmost x
        int ret_rightx_index = findMaxX(triangles[i]);
        double rightmost_x = triangles[i].Points[ret_rightx_index].x;

        //middle x
        double middle_x = triangles[i].Points[3-ret_leftx_index-ret_rightx_index].x; 

        //===//
        double x1 = leftmost_x;
        double x2 = rightmost_x;
        double x3 = middle_x;
        double y1 = triangles[i].Points[ret_leftx_index].y;
        double y2 = triangles[i].Points[ret_rightx_index].y;
        double y3 = triangles[i].Points[3-ret_leftx_index-ret_rightx_index].y;
        double z1 = triangles[i].Points[ret_leftx_index].z;
        double z2 = triangles[i].Points[ret_rightx_index].z;
        double z3 = triangles[i].Points[3-ret_leftx_index-ret_rightx_index].z;
        
        // loop over scanline from top to bottom
        for(int j = top_scanline; j <= bottom_scanline; j++){
            double cur_scanline_y = Top_y - (j * dy);; 
            // intersecting x value
            double xa = x1 - ((x1 - x2) * ((y1 - cur_scanline_y) / (y1 - y2)));
            double za = z1 - ((z1 - z2) * ((y1 - cur_scanline_y) / (y1 - y2)));
            double xb = x1 - ((x1 - x3) * ((y1 - cur_scanline_y) / (y1 - y3)));
            double zb = z1 - ((z1 - z3) * ((y1 - cur_scanline_y) / (y1 - y3)));
            double xc = x2 - ((x2 - x3) * ((y2 - cur_scanline_y) / (y2 - y3)));
            double zc = z2 - ((z2 - z3) * ((y2 - cur_scanline_y) / (y2 - y3)));

            if(xa != INF && !isnan(xa))
                if(xa < leftmost_x || xa > rightmost_x) xa = INF;
            if(xb != INF && !isnan(xb))
                if(xb < leftmost_x || xb > middle_x) xb = INF;
            if(xc != INF && !isnan(xc))
                if(xc < middle_x || xc > rightmost_x) xc = INF;

            if(xa == INF && xb != INF && xc != INF){
                xa = xc;
                za = zc;
            }
            else if(xb ==  INF && xa != INF && xc != INF){
                xb = xc;
                zb = zc;
            }
            else if((xa == INF && xb == INF) || (xa == INF && xc == INF) || (xb == INF && xc == INF))
                continue;

            // find the leftmost x and rightmost x;
            double tempx = xa, tempz = za;
            if(xa > xb){
                xa = xb;
                za = zb;
                xb = tempx;
                zb = tempz;
            }

            // Clipping along x axis
            xa = xa < left_lim_x ? left_lim_x : (xa > right_lim_x ? right_lim_x : xa);
            xb = xb > right_lim_x ? right_lim_x : (xb < left_lim_x ? left_lim_x : xb); 

            // intersecting columns
            int left_intersecting_col = round(abs(xa - Left_x) / dx);
            int right_intersecting_col = round(abs(xb - Left_x) / dx);
          
            // loop from leftmost x to rightmost x
            // calculating z values
            double zp;
            double constant = dx * (zb - za) / (xb - xa);
            for(int k = left_intersecting_col; k <= right_intersecting_col; k++){
                if(k == left_intersecting_col)
                    zp = za + (((Left_x + left_intersecting_col * dx) - xa) * (zb - za) / (xb - xa));
                else
                    zp = zp + constant;
                if(zp > front_limit && zp < z_buffer[j][k]){
                    z_buffer[j][k] = zp;
                    frame_buffer[j][k] = triangles[i].Color;
                }
            }
        }
    }

    // Save Z buffer into file
    for(int i = 0; i < screen_width; i++){
        for(int j = 0; j < screen_height; j++){
            if(z_buffer[i][j] < rear_limit)
                cout << fixed << setprecision(6) << z_buffer[i][j] << "\t";
        }
        cout << endl;
    }

    fclose(stdout);

    // Draw Image from frame buffer
    bitmap_image image(screen_height, screen_width);
    for(int i = 0; i < screen_width; i++){
        for(int j = 0; j < screen_height; j++){
            image.set_pixel(j,i,frame_buffer[i][j].R, frame_buffer[i][j].G,frame_buffer[i][j].B);
        }
    }
    image.save_image("out.bmp");

    for(int i = 0; i < screen_width; i++){
        z_buffer[i].clear();
        frame_buffer[i].clear();
        z_buffer[i].shrink_to_fit();
        frame_buffer[i].shrink_to_fit();
    }
    z_buffer.clear();
    frame_buffer.clear();
    z_buffer.shrink_to_fit();
    frame_buffer.shrink_to_fit();


    return 0;
}