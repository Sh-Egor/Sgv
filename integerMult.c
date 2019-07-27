#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>

typedef struct s1{
	double x;
	double y;
}dpoint;

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
GenGNUPlotData(dpoint* mas, int n, int mult)
{
	FILE* out = fopen("data.txt","w");
	for(int i = 0; i < n; i++){
		int j = (i*mult)%n;
		fprintf(out,"%lf %lf\n",mas[i].x,mas[i].y);
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
GenGNUPlotScr(int num, int n)
{
	FILE* out = fopen("scr.txt","w");
	fprintf(out, "set terminal svg enhanced size 1080,1080\n");
	fprintf(out, "set output './VectPack%d/%03d.svg'\n",n,num);
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
}

int
main(void)
{
	int n;
	printf("Enter count n: \n");
	scanf("%d",&n);
	dpoint mas[n];
	GenMas(mas,n);
	char a[200];
	sprintf(a,"mkdir VectPack%d",n);
	system(a);
	for(int i = 0; i < n; i++){
		GenGNUPlotData(mas,n,i);
		GenGNUPlotScr(i,n);
		system("gnuplot scr.txt");
		printf("%g% [%d/%d]\n",100.0*(i+1)/n,i+1,n);
	}
	return 0;
}