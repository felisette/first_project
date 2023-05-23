#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void preparation (unsigned char *pic, int x, int y){
  int i, j;
    for (i=0; i<x; i++)
        for (j=0; j<y; j++){
            if(pic[y*i+j]<100)
                pic[y*i+j]=0;
            if (pic[y*i+j]>160)
                pic[y*i+j]=255;
        }
    return;
}

char* load(const char *f, int *w, int *h) {
    unsigned char* image=NULL;
    int er=lodepng_decode32_file(&image, w, h, f);
    if(er){
        printf("error %u: %s\n", er, lodepng_error_text(er));
    }
    return (image);
}
 
void writePng(const char* f, const unsigned char* image, unsigned w, unsigned h){
    unsigned er;
    unsigned char* ph;
    size_t size;
    LodePNGState state;
    lodepng_state_init(&state);
    er=lodepng_encode(&ph, &size, image, w, h, &state);
    if(!er) lodepng_save_file(ph, size, f);
    else printf("error %u: %s\n", er, lodepng_error_text(er));
  
    lodepng_state_cleanup(&state);
    free(ph);
}

void fGause(unsigned char* p, unsigned char* l, int h, int w) {
  int i, j;
  int k;
    for (i=2; i<h-1; i++)
        for (j=2; j<w-1; j++)
          for (k=0; k<=3; k++)
            l[4*i*w+4*j+k]=0.09*p[4*(i-1)*w+4*(j-1)+k]+0.12*p[4*(i-1)*w+4*j+k]+0.09*p[4*(i-1)*w+4*(j+1)+k]+0.12*p[4*i*w+4*(j-1)+k]+0.15*p[4*i*w+4*j+k]+0.12*p[4*i*w+4*(j+1)+k]+0.09*p[4*(i+1)*w+4*(j-1)+k]+0.12*p[4*(i+1)*w+4*j+k]+0.09*p[4*(i+1)*w+4*(j+1)+k];
    return;
}
 
void coloring (unsigned char *dc, unsigned char *mcod, int ih, int iw, int n){
  int i;
    for(i=1;i<iw*ih; i++) {
        mcod[i*n]=60+dc[i]+0.3*dc[i-1];
        mcod[i*n+1]=70+dc[i];
        mcod[i*n+2]=120+dc[i];
        mcod[i*n+3]=255;
    }
    return;
}
 
int main() {
    char* start_file="skull.png";
    int x, y, n=4, i, j, k=0;
    char* a=load(start_file, &x, &y);
    if (a==NULL) {
        printf("ERROR: can't read file %s\n", start_file);
        return -1;
}
     unsigned char* o=(unsigned char*)malloc(x*y*sizeof(unsigned char));
    unsigned char* copy=(unsigned char*)malloc(x*y*sizeof(unsigned char));
    unsigned char* mco=(unsigned char*)malloc(x*y*n*sizeof(unsigned char));
    for (i=0; i<y*x; i++) {
        o[k]=0.3*a[4*i]+0.5*a[4*i+1]+0.111*a[4*i+2];
        k++;
    }
    preparation(o, y, x);
    fGause(o, copy, y, x);
    coloring(copy, mco, y, x, n);
    char* finish_file="output.png";
    writePng(finish_file, mco, x, y);
    free(a);
    free(o);
    free(copy);
    free(mco);
    return 0;
}
