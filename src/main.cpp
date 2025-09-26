#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include "../include/Vector3.hpp"
#include <string>

struct Ponto3d {
    float x, y, z;
};

struct FontInt {
    float r, g, b;
};

struct FontPoint {
    float x, y, z;
};

struct Font {
    FontInt I;
    FontPoint Fp;
};

float dist = 10;
float width = 1280, height = 720;
const float wj = 1920, hj = 1080;
const float d_x = wj/width, d_y = hj/height;
std::string backgroundColor = "100 100 100 ";
FontPoint fp = {0, 1100, 1100};
// Intensidade ambiente
Vector3 cor_esf(1, 0, 0);
Vector3 luz_amb(.1, .1, .1);
Vector3 cor_amb(cor_esf.x*luz_amb.x, cor_esf.y*luz_amb.y, cor_esf.z*luz_amb.z);

float SphereIntersect(Ponto3d origin, Ponto3d centro, float raio, Vector3 dir) {
    Vector3 w(origin.x-centro.x, origin.y-centro.y, origin.z-centro.z);
    float b = 2.0f*dot(dir, w);
    float c = dot(w, w) - raio*raio;

    float delta = b*b - 4*c;
    if(delta < 0) return -1;

    float t1 = (-b + sqrt(delta))/2;
    float t2 = (-b - sqrt(delta))/2;

    return (t1 > t2) ? t2 : t1;
}

int main(){
    std::ofstream image("image.ppm");
    const Ponto3d pos_obs = {0, 0, 0};
    const float r_esf = 1000;
    Ponto3d c_esf = {0, 0, -dist-r_esf};

    if (image.is_open()){
        image << "P3" << "\n";
        image << width << " " << height << "\n";
        image << 255 << "\n";

        for(int y = 0; y < height; y++){
            Ponto3d ponto;
            ponto.z = -dist;
            ponto.y = hj/2 - d_y/2 - y*d_y;
            for(int x = 0; x < width; x++){
                ponto.x = -wj/2 + d_x/2 + x*d_x;
                Vector3 dr(ponto.x - pos_obs.x, ponto.y - pos_obs.y, ponto.z - pos_obs.z);
                dr = normalize(dr);
                float disc = SphereIntersect(pos_obs, c_esf, r_esf, dr);
                if(disc != -1) {
                    Vector3 origin(pos_obs.x, pos_obs.y, pos_obs.z);
                    Vector3 p_int(origin + dr*disc); // ponto interseção

                    Vector3 normal(p_int.x-c_esf.x, p_int.y-c_esf.y,p_int.z-c_esf.z);
                    normal = normalize(normal);
                    Vector3 dir_luz(fp.x-p_int.x,fp.y-p_int.y,fp.z-p_int.z);
                    dir_luz = normalize(dir_luz);
                    Vector3 dr_inv(-dr.x,-dr.y,-dr.z);

                    // Intensidade difusa
                    Vector3 luz_dif(1, 1, 1);
                    Vector3 mat_dif(.3, 0, 0);
                    float i_dif = std::max(0.0f, dot(normal, dir_luz));
                    Vector3 cor_dif(
                        luz_dif.x*mat_dif.x*i_dif,
                        luz_dif.y*mat_dif.y*i_dif,
                        luz_dif.z*mat_dif.z*i_dif
                    );

                    // Intensidade especular
                    Vector3 luz_esp(1, 1, 1);
                    Vector3 mat_esp(.7, 0, 0);
                    int brightness = 1; // nível de foco da luz
                    Vector3 reflection(reflect(normal, dir_luz));
                    float i_esp = pow(std::max(0.0f, dot(reflection, dr_inv)), brightness);
                    Vector3 cor_esp(
                        luz_esp.x*mat_esp.x*i_esp,
                        luz_esp.y*mat_esp.y*i_esp,
                        luz_esp.z*mat_esp.z*i_esp
                    );
                    Vector3 cor_final(cor_amb+cor_dif+cor_esp);
                    cor_final.clamp(); // limitando os valores das componentes entre 0 e 1

                    image << (int)(cor_final.x * 255.999) << " "
                          << (int)(cor_final.y * 255.999) << " "
                          << (int)(cor_final.z * 255.999) << " ";
                }else{
                    image << backgroundColor;
                }
            }
            image << "\n";
        }

        image.close();
    }

    return 0;
}
