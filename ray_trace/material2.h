#ifndef MATERIALH
#define MATERIALH

#include "hitable.h"
#include "ray.h"

struct hit_record;

vec3 random_in_unit_sphere()
{
    vec3 p;
    do{
        p = 2.0*vec3((double)rand()/(RAND_MAX + 1.0), (double)rand()/(RAND_MAX + 1.0), (double)rand()/(RAND_MAX + 1.0)) - vec3(1, 1, 1);
    }while(p.squared_length() >= 1.0);
    return p;
}

double schlick(double cosine, double ref_idx)
{
    double r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2*dot(v, n)*n;
}

bool refract(const vec3& v, const vec3& n, double ni_over_nt, vec3& refracted)
{
    vec3 uv = unit_vector(v);
    double dt = dot(uv, n);
    double discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if(discriminant > 0)
    {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}

class material
{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
    virtual bool scatter2(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target-rec.p, r_in.time());
        attenuation = albedo;
        return true;
    }
    virtual bool scatter2(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target-rec.p, r_in.time());
        attenuation = albedo;
        return true;
    }

    vec3 albedo;
};

class metal : public material
{
public:
    metal(const vec3& a, double f) : albedo(a) {if(f < 1) fuzz = f; else fuzz = 1;}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 light = vec3(0, 2, 0);

        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    virtual bool scatter2(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 light = vec3(0, 2, 0);

        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        vec3 target = rec.p + rec.normal + reflected + (light-rec.p) + fuzz*random_in_unit_sphere();
        scattered = ray(rec.p, reflected + (light-rec.p)/(light-rec.p).squared_length() + fuzz*random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    vec3 albedo;
    double fuzz;
};

#endif