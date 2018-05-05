#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define W 1600
#define H 1200

struct yuv420p {
	uint8_t y[H][W];
	uint8_t u[H/2][W/2];
	uint8_t v[H/2][W/2];
} in, out;

void process();

int main() {
  FILE *f = fopen("sea_1600x1200_P420.yuv", "rb");
  if (!f )
  	return printf("Cannot open input file\n");
  if ( 1 != fread (&in, W*H*3/2, 1, f))
  	return printf("Too few data\n");
  fclose(f);
  
  process();
  
  f = fopen("out_sea_1600x1200_P420.yuv", "wb");
  fwrite(&out, W*H*3/2, 1, f);
  fclose(f);
  
  return 0;
}

float sharp_y[3][3] = {
	{0, -2, 0},
	{-2, 10, -2},
	{0, -2, 0}
};

float sharp_uv[3][3] = {
	{0, -3, 0},
	{-3, 15, -3},
	{0, -3, 0}
};

void process() {
	memset(out.y, 0xFF, sizeof(in.y));
	int y, x;
	for(y = 1; y < H-1; ++y){
		for(x = 1; x < W-1; ++x){
			int i, j;
			float p = 0;
			for(i = 0; i < 3; ++i){
				for(j = 0; j < 3; ++j){
					p += in.y[y-1+i][x-1+j] * sharp_y[i][j];
				}
			}
			p /= 9;
			if (p<0) p=0;
			else if(p>255) p=255;
			out.y[y][x] = (uint8_t) p;
		}
	}

	memset(out.u, 0x80, sizeof(in.u));
	memset(out.v, 0x80, sizeof(in.v));
	for(y = 1; y < H/2-1; ++y){
		for(x = 1; x < W/2-1; ++x){
			int i, j;
			float p = 0, q = 0;
			for(i = 0; i < 3; ++i){
				for(j = 0; j < 3; ++j){
					p += (in.u[y-1+i][x-1+j]+128) * sharp_uv[i][j];
					q += (in.v[y-1+i][x-1+j]+128) * sharp_uv[i][j];
				}
			}
			p /= 9; p -= 128;
			q /= 9; q -= 128;
			if (p<0) p=0;
			else if(p>255) p=255;
			if (q<0) q=0;
			else if(q>255) q=255;
			out.u[y][x] = (uint8_t) p;
			out.v[y][x] = (uint8_t) q;
		}
	}
}