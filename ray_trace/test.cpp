#include <fstream>
#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"


using namespace std;

vec3 color(const ray& r, hitable *world, int depth)
{
    /*if(abs((vec3(-7, 1, -25) - r.origin()).length())-1 < 0.00001)   //畫出光源
    {
        return vec3(1, 0, 0);
    }*/
    if(abs((vec3(0, 2, 0) - r.origin()).length())-1 < 0.00001)   //畫出光源
    {
        return vec3(1, 0, 0);
    }/*
    if(abs((vec3(7, 1, -25) - r.origin()).length())-0.1 < 0.00001)   //畫出光源
    {
        return vec3(0, 0, 1);
    }*/
    hit_record rec;
    if(world->hit(r, 0.001, INT_MAX, rec))
    {
        ray scattered;
        vec3 attenuation;   //衰減
        if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            /*
            vec3 light1(-7, 1, -25);
            vec3 light2(0, 1, -25);
            vec3 light3(7, 1, -25);

            vec3 light(0, 0, 0);
            
            if(abs((rec.p-light1).length()) <= abs((rec.p-light2).length()) && abs((rec.p-light1).length()) <= abs((rec.p-light3).length()))
                light = light1;
            else if(abs((rec.p-light2).length()) <= abs((rec.p-light1).length()) && abs((rec.p-light2).length()) <= abs((rec.p-light3).length()))
                light = light2;
            else
                light = light3;*/
            double weight = (cos(M_PI*dot(-1*r.direction(), rec.normal)/180.0))*(cos(M_PI*dot((vec3(0, 2, 0)-rec.p)/(vec3(0, 2, 0)-rec.p).length(), rec.normal)/180.0));

            return attenuation*weight*color(scattered, world, depth+1);
        }
        else
        {
            return vec3(0, 0, 0);
        }
    }
    else
    {
        /*
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5*(unit_direction.y() + 1.0);
        return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
        */

        //double distance = abs((vec3(0, 1, -1) - r.origin()).length()) - 0.5;
        /*double distance = abs((vec3(0, 1, -1) - r.origin()).length());
        if(distance > 0)
            return vec3(1, 1, 1)/pow(distance, 2);
        else
            return vec3(1, 1, 1);*/
        return vec3(0.5, 0.5, 0.5);
    }
}


int main()
{
    srand(time(NULL));

    int nx = 1200;
    int ny = 800;
    int ns = 30;
    hitable *list[2];
    //list[0] = new sphere(vec3(-7, 1, -25), 1, new lambertian(vec3(1, 1, 1)));   //表示光源的球體
    list[0] = new sphere(vec3(0, 2, 0), 1, new lambertian(vec3(1, 1, 1)));   //表示光源的球體
    //list[2] = new sphere(vec3(7, 1, -25), 0.1, new lambertian(vec3(1, 1, 1)));   //表示光源的球體

    list[1] = new sphere(vec3(0, -100.5, 0), 100, new lambertian(vec3(0.5, 0.5, 0.5)));
/*
    list[1] = new sphere(vec3(0, -100.5, 5), 100, new metal(vec3(1, 1, 1), 0.2));
    list[2] = new sphere(vec3(0, -100.5, 0), 100, new metal(vec3(0, 1, 0), 0.15));
    list[3] = new sphere(vec3(0, -100.5, -5), 100, new metal(vec3(0, 0, 1), 0.1));
*/

/*
    list[1] = new sphere(vec3(0, -100.5, 5), 100, new lambertian(vec3(1, 0, 0)));
    list[2] = new sphere(vec3(0, -100.5, 0), 100, new lambertian(vec3(0, 1, 0)));
    list[3] = new sphere(vec3(0, -100.5, -5), 100, new lambertian(vec3(0, 0, 1)));
*/

    list[2] = new sphere(vec3(-2, 0, 2), 0.5, new lambertian(vec3(0.5, 0.5, 0.5)));
    list[3] = new sphere(vec3(0, 0, 2), 0.5, new lambertian(vec3(0.5, 0.5, 0.5)));
    list[4] = new sphere(vec3(2, 0, 2), 0.5, new lambertian(vec3(0.5, 0.5, 0.5)));

    list[5] = new sphere(vec3(-2, 0, 0), 0.5, new lambertian(vec3(0.5, 0.5, 0.5)));
    list[6] = new sphere(vec3(0, 0, 0), 0.5, new lambertian(vec3(0.5, 0.5, 0.5)));
    list[7] = new sphere(vec3(2, 0, 0), 0.5, new lambertian(vec3(0.5, 0.5, 0.5)));

    list[8] = new sphere(vec3(-2, 0, -2), 0.5, new lambertian(vec3(0.5, 0.5, 0.5)));
    list[9] = new sphere(vec3(0, 0, -2), 0.5, new lambertian(vec3(0.5, 0.5, 0.5)));
    list[10] = new sphere(vec3(2, 0, -2), 0.5, new lambertian(vec3(0.5, 0.5, 0.5)));
    
    hitable *world = new hitable_list(list, 11);
    fstream file;
    file.open("Hello.ppm", ios::out);
    file << "P3\n" << nx << " " << ny << "\n255\n";

    vec3 lookfrom(0, 2, 10);
    vec3 lookat(0, 0, 0);
    double dist_to_focus = 10.0;
    double aperture = 0.0;  //光圈

    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, double(nx)/double(ny), aperture, dist_to_focus, 0.0, 1.0);
    
    for(int j = ny-1; j >= 0; j -= 1)
        for(int i = 0; i < nx; i += 1)
        {
            vec3 col(0, 0, 0);
            for(int k = 0; k < ns; k += 1)
            {
                double u = double(i + (double)rand()/(RAND_MAX + 1.0)) / double(nx);
                double v = double(j + (double)rand()/(RAND_MAX + 1.0)) / double(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }

            col /= double(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            file << ir << " " << ig << " " << ib <<"\n";
        }
}
