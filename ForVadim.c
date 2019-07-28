#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>

typedef struct s1{
	double x;
	double y;
}dpoint;

int GlobalCnt = 0;
/*
	Generating coordinates for n points on a circle of radius 1
*/
void
GenMas(dpoint* mas, int n)
{
	for(int i = 0; i < n; i++){
		mas[i].x = cos(2*M_PI*i/n); 
		mas[i].y = sin(2*M_PI*i/n);
	}
}

/*
	Generating lines between dots:
	if (k-dot * mult)%n eqv m-dot then build line
*/
void 
GenGNUPlotData(dpoint* mas, int n, double mult, int frames)
{
	FILE* out = fopen("data.txt","w");
	for(int i = 0; i < n; i++){
		int j = ((int)(mult*i*frames))%(n*frames);
		fprintf(out,"%lf %lf\n",mas[i*frames].x,mas[i*frames].y);
		fprintf(out,"%lf %lf\n\n",mas[j].x,mas[j].y);
	}
	fclose(out);
}

/*
	Build script for GNUPlot
	Used data from GenGNUPlotData
	output - *.svg; size: 1080*1080
	folder: ./VectPack<n>/*.svg
*/
void
GenGNUPlotScr(int n, int frames)
{
	FILE* out = fopen("scr.txt","w");
	fprintf(out, "set terminal png size 1080,1080 background \"black\"\n");
	fprintf(out, "set output './Png%dx%dframes/%07d.png\n",n,frames,GlobalCnt);
	fprintf(out, "set xrange [-1:1]\n");
	fprintf(out, "set yrange [-1:1]\n");
	fprintf(out, "set border 0\n");
	fprintf(out, "set xtics nomirror\n");
	fprintf(out, "set ytics nomirror\n");
	fprintf(out, "set noxtics\n");
	fprintf(out, "set noytics\n");
	fprintf(out, "set nokey\n");
	fprintf(out, "set size square\n");
	fprintf(out, "plot \"data.txt\" w lp lw 1 pt 7 ps 1\n");
	fclose(out);
	GlobalCnt++;
}

int
main(void)
{
	int n;
	int nbig;
	int frames;
	printf("Enter count n: \n");
	scanf("%d",&n);
	printf("Enter frames param:\n");
	scanf("%d",&frames);
	nbig = n*frames;
	dpoint mas[nbig];
	GenMas(mas,nbig);
	char a[200];
	sprintf(a,"mkdir Png%dx%dframes",n,frames);
	system(a);
	for(int i = 0; i < nbig+1; i++){
		GenGNUPlotData(mas,n,(double)i/frames,frames);
		GenGNUPlotScr(n,frames);
		system("gnuplot scr.txt");
		printf("%g% [%g/%d]\n",100.0*((double)(i+1))/nbig,(double)(i+1)/frames,n);
	}
	return 0;
}