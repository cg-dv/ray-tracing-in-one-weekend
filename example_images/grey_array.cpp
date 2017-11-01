#include <iostream>
#include "sphere.h" 
#include "hitable_list.h" 
#include "float.h" 
#include "camera.h"
#include "material.h"

vec3 color(const ray& r, hitable *world, int depth)  {
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec))  {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        }
        else {
            return vec3(0,0,0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }    
}


hitable *colorful_spheres() {
    int n = 16;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    int inc = 0;
    for (int a = 0; a < 4; a++) {
        for (int b = 0; b < 4; b++) {
            vec3 center(4,(a * 0.4) + 0.2,(b * 0.4) + 0.5);
            list[i++] = new sphere(center, 0.2, new lambertian(vec3((inc/16.0),(inc/16.0),(inc/16.0))));
            inc += 1;
        }
    }
    return new hitable_list(list,i);
}


int main() {
    int nx = 400;
    int ny = 300;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    hitable *list[0];
    float R = cos(M_PI/4);
    hitable *world = new hitable_list(list,0);
    world = colorful_spheres();
    
    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    float dist_to_focus = 10.0; 
    float aperture = 0.1;
    camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny), aperture, dist_to_focus);
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s=0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0); 
                col += color(r, world,0);
            }
            col /= float(ns);
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            int ir = int(255.99*col[0]); 
            int ig = int(255.99*col[1]); 
            int ib = int(255.99*col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n"; 
        }
    }
}
