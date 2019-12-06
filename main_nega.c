#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "BmpIoLib.h"

int main( int argc, char *argv[] )
{
	IBMP *pBmp;
	FILE *infile,*outfile;
	int i, j, idx;
	int w, h;
	int vr,vg,vb;

	if ( argc < 3 ) {
		printf("Input/Output File Name\n");
		return -1;
	}
	infile = fopen( argv[1], "rb" );
	if ( NULL == infile ) {
		printf("cannot open input file\n");
		return -1;
	}
	pBmp = BmpIO_Load( infile );
	fclose( infile );
	if ( NULL == pBmp ) {
		printf("cannot read input file\n");
		return -1;
	}
	w = BmpIO_GetWidth( pBmp );
	h = BmpIO_GetHeight( pBmp );
	printf("Input Bmp file is %d bit\n",pBmp->BitPerPix);
	printf("Input Bmp width and height are %d, %d\n",w,h);
	if(pBmp->BitPerPix != 24) {
		printf("only for 24 bit bmp file, stop\n");
		return -1;
	}
        /* Reading bmp was completed */

	int flag[w][h][4];
	for ( j = 0; j < h; j++ ){
	for ( i = 0; i < w; i++ ){
		idx = j*w+i;
		vr = pBmp->pColor[ idx ].r;
		vg = pBmp->pColor[ idx ].g;
		vb = pBmp->pColor[ idx ].b;
		/* 0,0,0=black, 255,255,255=white */
		flag[i][j][1]=vr;
		flag[i][j][2]=vg;
		flag[i][j][3]=vb;
	}
	}
        int cor=2;
        int ic,is,ie,jc,js,je;
        int flagc[(w-1)/cor][(h-1)/cor][4];
        
        for ( j = 0; j < h; j++){
	for ( i = 0; i < w; i++){
	   ic = i/cor;
	   is = ic*cor;
	   ie = is + (cor -1);
	   if (ie>w-1){
	      ie=w-1;
	   }
	   jc = j/cor;
	   js = jc*cor;
           je = js + (cor -1);
           if (je>w-1){
	      je=w-1;
	   }
   
             int ii,jj;
	     int vrave = 0,vgave = 0,vbave = 0;
	     for(ii=is;ii<=ie;ii++){
	     for(jj=js;jj<=je;jj++){
		vrave += flag[ii][jj][1];
		vgave += flag[ii][jj][2];
		vbave += flag[ii][jj][3];
		 
	     }
	     }
	   vrave = vrave/((ie-is+1)*(je-js+1));
	   vgave = vgave/((ie-is+1)*(je-js+1));
	   vbave = vbave/((ie-is+1)*(je-js+1));
	     
		idx = j*w+i;
		pBmp->pColor[ idx ].r = vrave;
		pBmp->pColor[ idx ].g = vgave;
		pBmp->pColor[ idx ].b = vbave;
	        flagc[ic][jc][1] = vrave;
	        flagc[ic][jc][2] = vgave;
	        flagc[ic][jc][3] = vbave;
	}
	}

        /* Output bmp is started */
	outfile = fopen( argv[2], "wb" );
	if ( NULL == outfile ) {
		printf("cannot open output file\n");
		return -1;
	}
	BmpIO_Save(outfile,pBmp);
	fclose( outfile );
	BmpIO_DeleteBitmap( pBmp );
	printf("Output Bmp was complated\n");
	return 0;
}
