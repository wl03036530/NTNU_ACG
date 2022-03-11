#pragma warning(disable:4996)

#include <fstream>
#include <iostream>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include "algebra3.h"

using namespace std;

struct Triangle
{
	vec3 p1;
	vec3 p2;
	vec3 p3;
	vec3 color;
	double ambient, diffuse, specular, exp, reflect;
};

struct Sphere
{
	vec3 center;
	double r = 0.0;
	vec3 color;
	double ambient, diffuse, specular, exp, reflect;
};

vec3 color(vec3 eye, vec3 ray, int depth, Sphere sphere1, Sphere sphere2, Sphere sphere3, Triangle triangle1, Triangle triangle2, vec3 light)
{
	//--sphere1
	vec3 oc = eye - sphere1.center;
	double a = ray.normalize() * ray.normalize();
	double b = oc * ray.normalize();
	double c = oc * oc - sphere1.r * sphere1.r;
	if ((b * b - a * c) >= 0)
	{
		double t = (-b - sqrt(b * b - a * c)) / a;
		vec3 hit_point = eye + t * ray.normalize();
		vec3 N = (hit_point - sphere1.center).normalize();
		vec3 L = (light - hit_point).normalize();

		vec3 V = (eye - hit_point).normalize();
		vec3 H = ((L + V) / 2).normalize();
		vec3 temp = N * H;
		for (int i = 0; i < sphere1.exp; i++)
			temp = temp * (N * H);
		return (1 - sphere1.reflect) * (sphere1.ambient * vec3(1, 1, 1) + sphere1.diffuse * sphere1.color * (N * L) + sphere1.specular * sphere1.color * temp);
	}
	//--sphere2
	oc = eye - sphere2.center;
	b = oc * ray.normalize();
	c = oc * oc - sphere2.r * sphere2.r;
	if ((b * b - a * c) >= 0)
	{
		double t = (-b - sqrt(b * b - a * c)) / a;
		vec3 hit_point = eye + t * ray.normalize();
		vec3 N = (hit_point - sphere2.center).normalize();
		vec3 L = (light - hit_point).normalize();

		vec3 V = (eye - hit_point).normalize();
		vec3 H = ((L + V) / 2).normalize();
		vec3 temp = N * H;
		for (int i = 0; i < sphere2.exp; i++)
			temp = temp * (N * H);
		return (1 - sphere2.reflect) * (sphere2.ambient * sphere2.color + sphere2.diffuse * sphere2.color * (N * L) + sphere2.specular * sphere2.color * temp);
	}
	//--sphere3
	oc = eye - sphere3.center;
	b = oc * ray.normalize();
	c = oc * oc - sphere3.r * sphere3.r;
	if ((b * b - a * c) >= 0)
	{
		double t = (-b - sqrt(b * b - a * c)) / a;
		vec3 hit_point = eye + t * ray.normalize();
		vec3 N = (hit_point - sphere3.center).normalize();
		vec3 L = (light - hit_point).normalize();

		vec3 V = (eye - hit_point).normalize();
		vec3 H = ((L + V) / 2).normalize();
		vec3 temp = N * H;
		for (int i = 0; i < sphere3.exp; i++)
			temp = temp * (N * H);
		return (1 - sphere3.reflect) * (sphere3.ambient * sphere3.color + sphere3.diffuse * sphere3.color * (N * L) + sphere3.specular * sphere3.color * temp);
	}
	//--triangle1
	vec3 v1 = triangle1.p2 - triangle1.p1;
	vec3 v2 = triangle1.p3 - triangle1.p1;
	mat3 temp(v1, v2, ray .normalize()* -1);
	mat3 matrix = temp.transpose().inverse();
	vec3 discriminant = matrix * (eye - triangle1.p1);

	if (depth < 500 && discriminant.x() <= 1 && discriminant.x() >= 0 && discriminant.y() <= 1 && discriminant.y() >= 0 && discriminant.x() + discriminant.y() <= 1 && discriminant.z()>0)
	{
		vec3 hit_point = eye + discriminant.z() * ray.normalize();
		vec3 N = (v1 ^ v2).normalize();
		vec3 L = (light - hit_point).normalize();

		vec3 V = (eye - hit_point).normalize();
		vec3 H = ((L + V) / 2).normalize();
		vec3 temp = N * H;
		for (int i = 0; i < triangle1.exp; i++)
			temp = temp * (N * H);
		return (1 - triangle1.reflect) * (triangle1.ambient * triangle1.color + triangle1.diffuse * triangle1.color * (N * L) + triangle1.specular * triangle1.color * temp) + triangle1.reflect * color(hit_point, (ray + 2 * N), depth + 1, sphere1, sphere2, sphere3,  triangle1, triangle2, light);
	}

	//--triangle2
	v1 = triangle2.p2 - triangle2.p1;
	v2 = triangle2.p3 - triangle2.p1;
	temp = mat3(v1, v2, ray.normalize() * -1);
	matrix = temp.transpose().inverse();
	discriminant = matrix * (eye - triangle2.p1);
	if (depth < 500 && discriminant.x() <= 1 && discriminant.x() >= 0 && discriminant.y() <= 1 && discriminant.y() >= 0 && discriminant.x() + discriminant.y() <= 1 && discriminant.z()>0)
	{
		vec3 hit_point = eye + discriminant.z() * ray.normalize();
		vec3 N = -(v1 ^ v2).normalize();
		vec3 L = (light - hit_point).normalize();

		vec3 V = (eye - hit_point).normalize();
		vec3 H = ((L + V) / 2).normalize();
		vec3 temp = N * H;
		for (int i = 0; i < triangle2.exp; i++)
			temp = temp * (N * H);
		return (1 - triangle2.reflect) * (triangle2.ambient * triangle2.color + triangle2.diffuse * triangle2.color * (N * L) + triangle2.specular * triangle2.color * temp) + triangle2.reflect * color(hit_point, (ray + 2 * N), depth + 1, sphere1, sphere2, sphere3, triangle1, triangle2, light);
	}

	return vec3(0.1, 0.1, 0.1);
}

int main()
{
	vec3 eye(0, 0, 0);
	vec3 view_direction(0, 0, 0);
	vec3 vup(0, 0, 0);
	double field_of_view = 0.0;
	double resolution_w = 0.0, resolution_h = 0.0;
	Sphere sphere1,  sphere2, sphere3;
	Triangle triangle1, triangle2;
	vec3 light(0, 0, 0);

	FILE* fp;
	fp = fopen("hw3_input.txt", "r");
	if (!fp)
		cout << "read file failed." << endl;
	char c;
	bool triangle1_fin = false;
	bool read_file_end = false;
	fscanf(fp, "%c", &c);
	while (!read_file_end)
	{
		if (c == 'E')
		{
			double x, y, z;
			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			eye = vec3(x, y, z);
		}
		else if (c == 'V')
		{
			double x, y, z;
			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			view_direction = vec3(x, y, z);
			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			vup = vec3(x, y, z);
		}
		else if (c == 'F')
		{
			fscanf(fp, "%lf", &field_of_view);
		}
		else if (c == 'R')
		{
			fscanf(fp, "%lf %lf", &resolution_w, &resolution_h);
		}
		else if (c == 'M')
		{
			double r, g, b;
			fscanf(fp, "%lf %lf %lf", &r, &g, &b);
			if (sphere1.r == 0)
			{
				sphere1.color = vec3(r, g, b);
				fscanf(fp, "%lf %lf %lf %lf %lf", &sphere1.ambient, &sphere1.diffuse, &sphere1.specular, &sphere1.exp, &sphere1.reflect);
			
				sphere2.color = vec3(r, g, b);
				sphere2.ambient = sphere1.ambient;
				sphere2.diffuse = sphere1.diffuse;
				sphere2.specular = sphere1.specular;
				sphere2.exp = sphere1.exp;
				sphere2.reflect = sphere1.reflect;
			
				sphere3.color = vec3(r, g, b);
				sphere3.ambient = sphere1.ambient;
				sphere3.diffuse = sphere1.diffuse;
				sphere3.specular = sphere1.specular;
				sphere3.exp = sphere1.exp;
				sphere3.reflect = sphere1.reflect;
			}
			else
			{
				triangle1.color = triangle2.color = vec3(r, g, b);
				fscanf(fp, "%lf %lf %lf %lf %lf", &triangle1.ambient, &triangle1.diffuse, &triangle1.specular, &triangle1.exp, &triangle1.reflect);
				triangle2.ambient = triangle1.ambient;
				triangle2.diffuse = triangle1.diffuse;
				triangle2.specular = triangle1.specular;
				triangle2.exp = triangle1.exp;
				triangle2.reflect = triangle1.reflect;
			}

		}
		else if (c == 'S')
		{
			double x, y, z;
			if (sphere1.r == 0)
			{
				fscanf(fp, "%lf %lf %lf %lf", &x, &y, &z, &sphere1.r);
				sphere1.center = vec3(x, y, z);
			}
			else if (sphere2.r == 0)
			{
				fscanf(fp, "%lf %lf %lf %lf", &x, &y, &z, &sphere2.r);
				sphere2.center = vec3(x, y, z);
			}
			else
			{
				fscanf(fp, "%lf %lf %lf %lf", &x, &y, &z, &sphere3.r);
				sphere3.center = vec3(x, y, z);
			}
		}
		else if (c == 'T' && !triangle1_fin)
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
		else if (c == 'T' && triangle1_fin)
		{
			double x, y, z;
			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			triangle2.p1 = vec3(x, y, z);

			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			triangle2.p2 = vec3(x, y, z);

			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			triangle2.p3 = vec3(x, y, z);
		}
		else if (c == 'L')
		{
			double x, y, z;
			fscanf(fp, "%lf %lf %lf", &x, &y, &z);
			light = vec3(x, y, z);
			read_file_end = true;
		}
		fscanf(fp, " %c", &c);
	}
	fclose(fp);

	fstream file;
	file.open("Hw3.ppm", ios::out);
	file << "P3\n" << resolution_w << " " << resolution_h << "\n255\n";

	double theta = field_of_view * M_PI / 180;
	double half_height = tan(theta / 2);
	double half_width = (resolution_w / resolution_h) * half_height;
	
	vec3 w = -1 * (view_direction.normalize());
	vec3 u = w ^ vup;
	vec3 v = u ^ w;

	vec3 lookat(0, 0, 30);
	double dist_to_focus = (lookat - eye).length();	//焦距
	double aperture = 15 / 2.2;	//光圈

	vec3 output_UL = eye - half_width * dist_to_focus * u + half_height * dist_to_focus * v - dist_to_focus * w;
	for (int j = 0; j < resolution_h; j += 1)
		for (int i = 0; i < resolution_w; i += 1)
		{
			vec3 col(0, 0, 0);
			int times = 30;
			for (int k = 0; k < times; k++)
			{
				int u = (((double)rand() / (RAND_MAX + 1.0))*2-1) * (aperture/2);
				int v = (((double)rand() / (RAND_MAX + 1.0))*2-1) * (aperture/2);
				vec3 new_eye = eye + vec3(u, 0, 0) + vec3(0, v, 0);
				vec3 ray = (output_UL + vec3((half_width * 2 * dist_to_focus / resolution_w) * i, 0, 0) - vec3(0, (half_height * 2 * dist_to_focus / resolution_h) * j, 0)) - new_eye;
				col += color(new_eye, ray, 0, sphere1, sphere2, sphere3, triangle1, triangle2, light);
			}
			col /= times;
			col *= 255.99;
			if (col.x() > 255) col = vec3(255, col.y(), col.z());
			if (col.y() > 255) col = vec3(col.x(), 255, col.z());
			if (col.z() > 255) col = vec3(col.x(), col.y(), 255);

			if (col.x() < 0) col = vec3(0, col.y(), col.z());
			if (col.y() < 0) col = vec3(col.x(), 0, col.z());
			if (col.z() < 0) col = vec3(col.x(), col.y(), 0);

			file << col.x() << " " << col.y() << " " << col.z() << "\n";
		}
	file.close();
}
