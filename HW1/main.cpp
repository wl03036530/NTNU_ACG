#include <fstream>
#include <iostream>
#include "algebra3.h"

using namespace std;

struct Triangle
{
    vec3 p1;
    vec3 p2;
    vec3 p3;
};

struct Sphere
{
    vec3 center;
    double r = 0.0;
};


int main()
{
    vec3 eye(0, 0, 0);
    vec3 output_UL(0, 0, 0);
    vec3 output_UR(0, 0, 0);
    vec3 output_LL(0, 0, 0);
    vec3 output_LR(0, 0, 0);
    double resolution_w = 0.0, resolution_h = 0.0;
    Sphere sphere;
    Triangle triangle1, triangle2;

    FILE *fp;
    fp = fopen("hw1_input.txt", "r");
    if(!fp)
        cout << "read file failed." << endl;
    char c;
    bool triangle1_fin = false;
    bool triangle2_fin = false;
    fscanf(fp, "%c", &c);
    while (!triangle2_fin)
    {
        if(c == 'E')
        {
            double x, y, z;
            fscanf(fp, "%lf %lf %lf", &x, &y, &z);
            eye = vec3(x, y, z);
        }
        else if(c == 'O')
        {
            double x, y, z;
            fscanf(fp, "%lf %lf %lf", &x, &y, &z);
            output_UL = vec3(x, y, z);

            fscanf(fp, "%lf %lf %lf", &x, &y, &z);
            output_UR = vec3(x, y, z);

            fscanf(fp, "%lf %lf %lf", &x, &y, &z);
            output_LL = vec3(x, y, z);

            fscanf(fp, "%lf %lf %lf", &x, &y, &z);
            output_LR = vec3(x, y, z);
        }
        else if(c == 'R')
        {
            fscanf(fp, "%lf %lf", &resolution_w, &resolution_h);
        }
        else if(c == 'S')
        {
            double x, y, z;
            fscanf(fp, "%lf %lf %lf %lf", &x, &y, &z, &sphere.r);
            sphere.center = vec3(x, y, z);
        }
        else if(c == 'T' && !triangle1_fin)
        {
            double x, y, z;
            fscanf(fp, "%lf %lf %lf", &x, &y, &z);
            triangle1.p1 = vec3(x, y, z);

            fscanf(fp, "%lf %lf %lf", &x, &y, &z);
            triangle1.p2 = vec3(x, y, z);

            fscanf(fp, "%lf %lf %lf", &x, &y, &z);
            triangle1.p3 = vec3(x, y, z);

            triangle1_fin = true;
        }
        else if(triangle1_fin && !triangle2_fin)
        {
            double x, y, z;
            fscanf(fp, "%lf %lf %lf", &x, &y, &z);
            triangle2.p1 = vec3(x, y, z);

            fscanf(fp, "%lf %lf %lf", &x, &y, &z);
            triangle2.p2 = vec3(x, y, z);

            fscanf(fp, "%lf %lf %lf", &x, &y, &z);
            triangle2.p3 = vec3(x, y, z);

            triangle2_fin = true;
        }
        fscanf(fp, " %c", &c);
    }
    fclose(fp);

    fstream file;
    file.open("Hw1.ppm", ios::out);
    file << "P3\n" << resolution_w << " " << resolution_h << "\n255\n";
    double step_w = (output_UR.x()-output_UL.x())/resolution_w;
    double step_h = (output_LL.y()-output_UL.y())/resolution_h;
    for(int j=0; j<resolution_h; j+=1)
        for(int i=0; i<resolution_w; i+=1)
        {
            vec3 ray = (output_UL + vec3(step_w*i, 0, 0) + vec3(0, step_h*j, 0)) - eye;
            //--sphere
            vec3 oc = eye - sphere.center;
            double a = ray*ray;
            double b = oc*ray;
            double c = oc*oc - sphere.r*sphere.r;
            if((b*b-a*c) >= 0)
            {
                file << "255" << " " << "255" << " " << "255" <<"\n";
                continue;
            }
            //--triangle1
            vec3 v1 = triangle1.p2-triangle1.p1;
            vec3 v2 = triangle1.p3-triangle1.p1;
            mat3 temp(v1, v2, ray*-1);
            mat3 matrix = temp.transpose().inverse();
            vec3 discriminant = matrix*(eye-triangle1.p1);

            if(discriminant.x()<=1 && discriminant.x()>=0 && discriminant.y()<=1 && discriminant.y()>=0 && discriminant.x()+discriminant.y()<=1 && discriminant.z()>0)
            {
                file << "255" << " " << "255" << " " << "255" <<"\n";
                continue;
            }
            
            //--triangle2
            v1 = triangle2.p2-triangle2.p1;
            v2 = triangle2.p3-triangle2.p1;
            temp = mat3(v1, v2, ray*-1);
            matrix = temp.transpose().inverse();
            discriminant = matrix*(eye-triangle2.p1);
            if(discriminant.x()<=1 && discriminant.x()>=0 && discriminant.y()<=1 && discriminant.y()>=0 && discriminant.x()+discriminant.y()<=1 && discriminant.z()>0)
            {
                file << "255" << " " << "255" << " " << "255" <<"\n";
                continue;
            }

            file << "0" << " " << "0" << " " << "0" <<"\n";
        }
        file.close();
}
