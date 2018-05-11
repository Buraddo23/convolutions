#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define W 1600
#define H 1200

typedef char  __attribute__((ext_vector_type(3))) char3;
typedef short __attribute__((ext_vector_type(3))) short3;

#define convert_short3(x) __builtin_convertvector(x, short3)

struct yuv420p {
	uint8_t y[H][W];
	uint8_t u[H/2][W/2];
	uint8_t v[H/2][W/2];
} in, out;

void process();

int main() {
  FILE *f = fopen("sea_1600x1200_P420.yuv", "rb");
  if ( !f )
  	return printf("Cannot open input file\n");
  if ( 1 != fread (&in, W*H*3/2, 1, f ))
  	return printf("Too few data\n");
  fclose(f);
  
  process();
  
  f = fopen("out_sea_1600x1200_P420.yuv", "wb");
  fwrite(&out, W*H*3/2, 1, f);
  fclose(f);
  
  return 0;
}

short edge_y[3][3] = {
	{0,   7, 0},
	{7, -28, 7},
	{0,   7, 0}
};

static inline
short hsum_v3s (short3 s) { return s.x + s.y + s.z; }

static inline
short clampab_s (short s, short a, short b) { return (s<a)?a:(s>b)?b:s; }

void process() {
	memset(out.y, 0xFF, sizeof(in.y));
	int y, x;
	
	const short3 k0 = *(short3*)&edge_y[0][0];
	const short3 k1 = *(short3*)&edge_y[1][0];
	const short3 k2 = *(short3*)&edge_y[2][0];
			
	for(y = 1; y < H-1; ++y) {
		for(x = 1; x < W-1; ++x) {
			char3 y0 = *(char3*)&in.y[y-1][x-1];
			char3 y1 = *(char3*)&in.y[ y ][x-1];
			char3 y2 = *(char3*)&in.y[y+1][x-1];
			
			out.y[y][x] = (uint8_t) clampab_s(hsum_v3s(k0*convert_short3(y0) + k1*convert_short3(y1) + k2*convert_short3(y2)), 0, 255);
		}
	}
	
	memset(out.u, 0x80, sizeof(out.u));
	memset(out.v, 0x80, sizeof(out.v));

}