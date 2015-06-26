#include <stdio.h>
#include <stdlib.h>

#define HARDWARE_BW_ADDR_HEXACOLOR (int*)5242884
#define HARDWARE_BW_ADDR_RESULT (int*)5242892
#define MUTEX_TOKEN_ADDR (int*)5242896

#define MULT_PROC


#ifdef MULT_PROC
volatile int w, h;
volatile int count = 0;
volatile unsigned int *in;
volatile unsigned int *out;
volatile bool file_loaded = false;

int increment_count() {
  int res;
  
  while(*mutex);
  
  res = count;
  count = res + 1;
  
  *mutex = 0;
  
  return res;
}

int main(){
  int i,j,max;
  FILE *f;
  volatile int *mutex = MUTEX_TOKEN_ADDR;
  volatile int *bw_input = HARDWARE_BW_ADDR_HEXACOLOR;
  volatile int *bw_output = HARDWARE_BW_ADDR_RESULT;
  
  while(*mutex);
  if (!file_loaded) {
    scanf("%d %d", &w, &h );
    in = malloc(sizeof(unsigned int) * w * h);
    out = malloc(sizeof(unsigned int) * w * h);
    
    for (i=0;i<w*h;i++ ){
      scanf("%x", &in[i]);
    }
    
    file_loaded = true;
    *mutex = 0;
  }
  
  max = w*h;
  while ((i = increment_count()) < max) {
    *bw_input = in[i];
    out[i] = *bw_output;
  }
  
  if (i == max) {
    //BEGIN HEADER
    f = fopen("img.pgm", "w");
    if (f == NULL)
    {
        printf("Error opening file!n");
        exit(1);
    }
    fprintf(f, "P2\n");  
    
    fprintf(f, "%d ", w);
    fprintf(f, "%d ", h);
    fprintf(f, "255");
    fprintf(f, "\n");
    //HEADER END
    
    for (i=0;i<h;i++){
      for (j=0;j<w;j++){
        out[i*w+j] = out[i*w+j] >> 16;
        printf("%x ", out[i*w+j]);
        fprintf(f, "%d ", out[i*w+j]);
      }
      fprintf(f, "\n");
      printf("\n");
    }
    printf("========================");
    fclose(f);
  }
  
  return 0;
}
#else
unsigned int* calc(unsigned int* in, int w, int h){
  unsigned int * out = new unsigned int[w*h];
  int i=0;
  for (i=0;i<w*h;i++ ){
    out[i] = ((0xff & in[i]) + ((in[i] >> 8) & 0xff) + ((in[i] >> 16) & 0xff))/3  ;
    out[i] = out[i] | (out[i] << 8) | (out[i] << 16);
    //calcula a escala de cinza de cada pixel
  }
  return out;
}  

int main(){
  int i=0;
  int j=0;
  int w, h;
  scanf("%d %d", &w, &h );
  unsigned int in[w*h];
  unsigned int *out;
  FILE *f = fopen("img.pgm", "w");
  if (f == NULL)
  {
      printf("Error opening file!n");
      exit(1);
  }
  //BEGIN HEADER
  fprintf(f, "P2\n");  
  for (i=0;i<w*h;i++ ){
    scanf("%x", &in[i]);
  }
  fprintf(f, "%d ", w);
  fprintf(f, "%d ", h);
  fprintf(f, "255");
  fprintf(f, "\n");
  //HEADER END
  
  out = calc(in,w,h);
  for (i=0;i<h;i++){
    for (j=0;j<w;j++){
      out[i*w+j] = out[i*w+j] >> 16;
      printf("%x ", out[i*w+j]);
      fprintf(f, "%d ", out[i*w+j]);
    }
    fprintf(f, "\n");
    printf("\n");
  }
  printf("========================");
  fclose(f);
  
  return 0;
}
#endif