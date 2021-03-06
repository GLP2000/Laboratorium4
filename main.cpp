#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "winbgi2.h"
#include "rk4.h"

double lambda = 1.0;
double funkcja(double t, double y);
double y_anl(double t, double y_0, double t_0);
int round(double number);

int main(){
	
	FILE *p;
	p=fopen("lab4.txt","w");
	if(p==NULL)
	printf("BLAD");

	double h, y_0,t_0,t_max;

	printf("Szukamy rozwiazania rownania rozniczkowego y' = f(y, t), gdzie y = y(t)\nWarunek cauchiego: y(t_0) = y_0\n");
	printf("podaj t_max\n");
	scanf("%lf",&t_max);
	printf("podaj y_0\n");
	scanf("%lf",&y_0);
	while(y_0==0)
	{
		printf("podaj nowe y_0, warunek y_0!=0\n");
		scanf("%lf",&y_0);
	}

	printf("podaj t_0\n");
	scanf("%lf",&t_0);

	while(t_0>=t_max)
	{
		printf("podaj nowe t_0, warunek t_0<t_max\n");
		scanf("%lf",&t_0);
	}

	graphics(800, 600);
	scale(0, 0, (t_max-t_0), 2*t_max);
	title("Blad Eulera (Niebielski) i RK4 (Pomaranczy)", "", "");

	double *ye, *yrk4;

	ye = (double*)malloc(64* sizeof(double));
	yrk4 = (double*)malloc(64 * sizeof(double));

	fprintf(p,"Liczby krokow\tKrok calkowania\tblad_eu\t\tblad_rk4\n");

	double t, y,y_rk4,N, blad_eu=0, blad_rk4=0, rzad_e=0, rzad_rk4=0;
	for(int g=0;g<7;g++)
	{	
		int f=0;
		N=pow(2.0,g);
		h=(t_max-t_0)/N;
		t=t_0;
		y=y_0;
		y_rk4=y_0;

		while((t_max-t)>h/2)
		{
			t=t+h;
			y=(y+h*funkcja(t,y));
			blad_eu=(fabs((y - y_anl(t,y_0,t_0)))/(fabs((y_anl(t,y_0,t_0)))));
			printf("Metoda eulera=%lf\t\tBlad eulera=%lf\n",y,blad_eu);
			setcolor(0.3);
			point(h,blad_eu);
			ye[f]=y;
			

			y_rk4=rk4(t,y_rk4, h, funkcja);
			blad_rk4=(fabs((y_rk4 - y_anl(t,y_0,t_0)))/(fabs((y_anl(t,y_0,t_0)))));
			printf("Metoda RK4=%lf\t\tBlad RK4=%lf\n",y_rk4,blad_rk4);
			setcolor(0.9);
			point(h,blad_rk4);
			yrk4[f]=y_rk4;

			f++;

			
			fprintf(p,"%lf\t%lf\t%lf\t%lf\n",N,h,blad_eu,blad_rk4);
		}

		rzad_e = round(log(fabs((y-y_anl(t,y_0,t_0))))/log(fabs(ye[f-1]-y_anl(t,y_0,t_0))));
		rzad_rk4 = round(log(fabs((y_rk4- y_anl(t,y_0,t_0))))/log(fabs(yrk4[f-1]-y_anl(t,y_0,t_0))));			
		printf("\n Rzad zbliezonosci Eulera = %lf\tRzad zbliezonosci  RK4 = %lf \n\n",rzad_e,rzad_rk4);
	
	}

	free(ye);
	free(yrk4);
	fclose(p);
	wait();
}
double funkcja(double t, double y )
{
	return lambda*y;
}


double y_anl(double t, double y_0, double t_0)
{
	return (y_0*exp(lambda*(t-t_0)));
}

int round(double number)
{
    return (number >= 0) ? (int)(number + 0.5) : (int)(number - 0.5);
}
