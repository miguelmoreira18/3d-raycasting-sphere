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

float dist = 10.01;
float width = 640, height = 360;
const float wj = 1920, hj = 1080;
const float d_x = wj/width, d_y = hj/height;
std::string backgroundColor = "24 13 53 ";
FontPoint fp = {-100, 100, 50};
// Intensidade ambiente
Vector3 cor_esf(.3, .1, .7);
Vector3 luz_amb(.2, .2, .2);
Vector3 cor_amb(cor_esf.x*luz_amb.x, cor_esf.y*luz_amb.y, cor_esf.z*luz_amb.z);

float SphereIntersect(Ponto3d origin, Ponto3d centro, float raio, Vector3 dir) {
    Vector3 w(origin.x-centro.x, origin.y-centro.y, origin.z-centro.z);
    float b = 2.0f*dir.dot(w);
    float c = w.dot(w) - raio*raio;

    float delta = b*b - 4*c;
    if(delta < 0) return -1;

    float t1 = (-b + sqrt(delta))/2;
    float t2 = (-b - sqrt(delta))/2;

    return (t1 > t2) ? t2 : t1;
}

int main(){
    std::ofstream image("imagem.ppm");
    const Ponto3d pos_obs = {0, 0, 0};
    Ponto3d c_esf = {0, 0, -dist};
    const float r_esf = 10;

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
                dr.normalize();
                float disc = SphereIntersect(pos_obs, c_esf, r_esf, dr);
                if(disc != -1) {
                    Vector3 origin(pos_obs.x, pos_obs.y, pos_obs.z);
                    Vector3 p_int(origin + dr*disc);

                    Vector3 normal(p_int.x-c_esf.x, p_int.y-c_esf.y,p_int.z-c_esf.z);
                    normal.normalize();
                    Vector3 dir_luz(fp.x-p_int.x,fp.y-p_int.y,fp.z-p_int.z);
                    dir_luz.normalize();
                    Vector3 dr_inv(-dr.x,-dr.y,-dr.z);
                    //dr_inv.normalize();

                    // Intensidade difusa
                    Vector3 luz_dif(.7, .7, .7);
                    Vector3 mat_dif(.7, .1, .7);
                    float i_dif = std::max(0.0f, normal.dot(dir_luz));
                    // Vector3 cor_dif(
                    //     luz_dif.x*mat_dif.x*i_dif,
                    //     luz_dif.y*mat_dif.y*i_dif,
                    //     luz_dif.z*mat_dif.z*i_dif
                    // );
                    Vector3 cor_dif = mat_dif * i_dif;

                    // Intensidade especular
                    Vector3 luz_esp(.8, .8, .8);
                    Vector3 mat_esp(.4, .4, .4);
                    int brightness = 2;
                    Vector3 reflection(dir_luz.reflect(dir_luz, normal));
                    float i_esp = pow(std::max(0.0f, reflection.dot(dr_inv)), brightness);
                    Vector3 cor_esp(
                        luz_esp.x*mat_esp.x*i_esp,
                        luz_esp.y*mat_esp.y*i_esp,
                        luz_esp.z*mat_esp.z*i_esp
                    );
                    Vector3 cor_final(cor_amb+cor_dif+cor_esp);
                    cor_final.clamp();

                    image << (int)(cor_final.x * 255.999) << " "
                          << (int)(cor_final.y * 255.999) << " "
                          << (int)(cor_final.z * 255.999) << " ";
                    //image << "145 60 200 ";
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