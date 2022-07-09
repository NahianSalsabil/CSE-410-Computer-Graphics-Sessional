#include<bits/stdc++.h>
using namespace std;

#define pi 3.1416

int dimension = 4;

struct point{
    double x, y, z;
};

void print_triangle(vector<vector<double>> output_Point, int dimension){
    // cout << "triangle new point: \n"; 
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            if(i != 3 && j != 3)
                cout << setprecision(7) << output_Point[j][i] << " ";
        }   
        cout << endl;
    }
}

int main(){
    freopen("scene.txt", "r", stdin);
    freopen("stage1.txt", "w", stdout);

    struct point eye, look, up;
    double fovY, aspectratio, near, far;
    cin >> eye.x >> eye.y >> eye.z;
    cin >> look.x >> look.y >> look.z;
    cin >> up.x >> up.y >> up.z;
    cin >> fovY >> aspectratio >> near >> far;

    string command;
    cin >> command;

    vector<vector<double>> triangle_matrix(dimension);
    for(int i = 0; i < dimension; i++)
        triangle_matrix[i].resize(dimension);
    
    cin >> triangle_matrix[0][0] >> triangle_matrix[1][0] >> triangle_matrix[2][0];
    cin >> triangle_matrix[0][1] >> triangle_matrix[1][1] >> triangle_matrix[2][1];
    cin >> triangle_matrix[0][2] >> triangle_matrix[1][2] >> triangle_matrix[2][2];

    print_triangle(triangle_matrix, dimension);

    fclose(stdin);
    fclose(stdout);


    freopen("stage1.txt", "r", stdin);
    freopen("stage2.txt", "w", stdout);
    int triangle = 1;
    int count = 0;
    while(count < triangle){
        vector<vector<double>> triangle_matrix1(dimension);
        for(int i = 0; i < dimension; i++)
            triangle_matrix1[i].resize(dimension);
        
        cin >> triangle_matrix1[0][0] >> triangle_matrix1[1][0] >> triangle_matrix1[2][0];
        cin >> triangle_matrix1[0][1] >> triangle_matrix1[1][1] >> triangle_matrix1[2][1];
        cin >> triangle_matrix1[0][2] >> triangle_matrix1[1][2] >> triangle_matrix1[2][2];

        for(int i = 0; i < dimension; i++){
            for(int j = 0; j < dimension; j++){
                if(i == 3) triangle_matrix1[i][j] = 1;
                else if(j == 3) triangle_matrix1[i][j] = 0;
            }
        }
        cout << "input :\n";
        print_triangle(triangle_matrix1, dimension);
        count++;
    }
}