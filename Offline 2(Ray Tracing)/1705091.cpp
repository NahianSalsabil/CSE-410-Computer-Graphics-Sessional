#include<bits/stdc++.h>
using namespace std;

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

//=======//

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

vector<double> R(vector<double> i, vector<double> a, vector<double> sina, double rotation_angle){
    vector<double> part1(3);
    for(int m = 0; m < 3; m++){
        part1[m] = cos(rotation_angle*pi/180) * i[m];
    }

    vector<double> part2(3);
    for(int m = 0; m < 3; m++){
        part2[m] = (1 - cos(rotation_angle*pi/180)) * a[m];
    }

    vector<double> part3(3);
    for(int m = 0; m < 3; m++){
        part3[m] = sin(rotation_angle*pi/180)*sina[m];
    }

    vector<double> column(3);
    for(int m = 0; m < 3; m++)
        column[m] = part1[m] + part2[m] + part3[m];

    return column;
}

vector<vector<double>> make_vector(vector<double> a, double rotation_angle){
    vector<double> i, j, k, ai, aj, ak, sinai, sinaj, sinak;;
    i.push_back(1);
    i.push_back(0);
    i.push_back(0);

    j.push_back(0);
    j.push_back(1);
    j.push_back(0);

    k.push_back(0);
    k.push_back(0);
    k.push_back(1);

    ai.push_back(a.at(0) * a.at(1));
    ai.push_back(a.at(0) * a.at(1));
    ai.push_back(a.at(0) * a.at(2));

    aj.push_back(a.at(0) * a.at(1));
    aj.push_back(a.at(1) * a.at(1));
    aj.push_back(a.at(1) * a.at(2));

    ak.push_back(a.at(0) * a.at(2));
    ak.push_back(a.at(1) * a.at(2));
    ak.push_back(a.at(2) * a.at(2));

    sinai.push_back(0);
    sinai.push_back(a.at(2));
    sinai.push_back(-a.at(1));

    sinaj.push_back(-a.at(2));
    sinaj.push_back(0);
    sinaj.push_back(a.at(0));

    sinak.push_back(a.at(1));
    sinak.push_back(-a.at(0));
    sinak.push_back(0);

    vector<double> c1 = R(i, ai, sinai, rotation_angle);
    vector<double> c2 = R(j, aj, sinaj, rotation_angle);
    vector<double> c3 = R(k, ak, sinak, rotation_angle);

    vector<vector<double>> rotation_matrix(4);
    for(int m = 0; m < 4; m++){
        rotation_matrix[m].resize(4);
    }

    for(int n = 0; n < 4; n++){
        for(int m = 0; m < 4; m++){
            if(m == 3 && n == 3) rotation_matrix[m][n] = 1;
            else if(m == 3 || n == 3) rotation_matrix[m][n] = 0;
            else if(n == 0) rotation_matrix[m][n] = c1.at(m);
            else if(n == 1) rotation_matrix[m][n] = c2.at(m);
            else if(n == 2) rotation_matrix[m][n] = c3.at(m);
        }
    }
    return rotation_matrix;
}

void print_triangle(vector<vector<double>> output_Point, int dimension){
    // cout << "triangle new point: \n"; 
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            if(i != 3 && j != 3)
                cout << fixed << setprecision(7) << output_Point[j][i]/output_Point[dimension-1][0] << " ";
        }   
        cout << endl;
    }
}

void print_matrix(vector<vector<double>> points){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cout << setprecision(7) << points[i][j] << " ";
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

    // cout << eye.x << " " << eye.y << " " << eye.z << endl;
    // cout << look.x << " " << look.y << " " << look.z << endl;
    // cout << up.x << " " << up.y << " " << up.z << endl;
    // cout << fovY << " " << aspectratio << " " << near << " " << far << endl;

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

            // print_matrix(translation_matrix);

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
            vector<double> a(3);
            cin >> rotation_angle >> a[0] >> a[1] >> a[2];
            double norm = sqrt(pow(a[0],2) + pow(a[1],2) + pow(a[2],2));
            a[0] /= norm;
            a[1] /= norm;
            a[2] /= norm;
            
            vector<vector<double>> rotation_matrix = make_vector(a, rotation_angle);

            // cout << "rotation matrix: \n";
            // print_matrix(rotation_matrix);
            
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
    // print_matrix(view_transformation_matrix);

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
    double t = near * tan((fovY/2) * pi / 180);
    double projection_r = near * tan((fovX/2) * pi / 180);

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
    freopen("output.txt", "w", stdout);

    double screen_width, screen_height, left_lim_x, right_lim_x, bottom_lim_y, top_lim_y, front_limit, rear_limit;

    cin >> screen_width >> screen_height >> left_lim_x >> bottom_lim_y >> front_limit >> rear_limit;
    right_lim_x = -left_lim_x;
    bottom_lim_y = -top_lim_y;

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
    // input_triangle = 0;
    // while(input_triangle < triangle_count){
    //     cout << triangles[input_triangle].Points[0].x << " " << triangles[input_triangle].Points[0].y << " " << triangles[input_triangle].Points[0].z << endl;
    //     cout << triangles[input_triangle].Points[1].x << " " << triangles[input_triangle].Points[1].y << " " << triangles[input_triangle].Points[1].z << endl;
    //     cout << triangles[input_triangle].Points[2].x << " " << triangles[input_triangle].Points[2].y << " " << triangles[input_triangle].Points[2].z << endl;

    //     cout << triangles[input_triangle].Color.R << endl;
    //     cout << triangles[input_triangle].Color.G << endl;
    //     cout << triangles[input_triangle].Color.B << endl;

    //     input_triangle++;
    // }

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



    return 0;
}