#include<cstdlib>
#include<omp.h>
#include<gd.h>
#include<string>
#include<iostream>

using namespace std;

int main(int argc,char*argv[]){
	if(argc!=3){
		cout<<"Invalid"<<endl;
		exit(1);
	}
	FILE* ifp=fopen(argv[1],"rb");
	FILE* ofp=fopen(argv[1],"wb");

	double t=omp_get_wtime();

	gdImagePtr img=gdImageCreateFromPng(ifp);
	int w=gdImageSX(img);
	int h=gdImageSY(img);

	#pragma omp parallel for
	for(int x=0;x<w;x++){
		for(int y=0;y<h;y++){
			int color=gdImageGetPixel(img,x,y);
			int avgColor=(gdImageRed(img,color)+gdImageGreen(img,color)+gdImageBlue(img,color))/3;
			color=gdImageColorAllocate(img,avgColor,avgColor,avgColor);
			#pragma omp critical
			gdImageSetPixel(img,x,y,color);
		}
	}
	gdImagePng(img,ofp);
	gdImageDestroy(img);

	fclose(ifp);
	fclose(ofp);

	t=omp_get_wtime()-t;
    cout<<"Time: "<<t<<endl;
}