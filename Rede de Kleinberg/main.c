#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mtwister.h"
#define d 2
#define V 2*d + 1
int L,N;
int* link;
int seed;
int X(int i){
    return i%L;
}
int Y(int i){
    return i/L;
}
int find_distance(int a,int b){
    return abs(X(b) - X(a)) + abs(Y(b) - Y(a));
}
int sinal(int i){
    switch(genrand64_int63()%2){
        case 0:
            return i;
        case 1:
            return -i;
    }
}
int power_law(int R,double alpha){
	double rand = genrand64_real1();
	double CN = pow(R,1 -alpha) - pow(2,1 -alpha);
	if(alpha==1)return 2*pow(R/2,rand);
    else return pow(rand*CN + pow(2,1 -alpha),1/(1-alpha));
}
int long_link(int site,double alpha){
    double ALPHA = alpha - (d-1);
    int r = power_law(L-1,ALPHA),x,y;
    x = sinal(genrand64_real1()*r);
    if(X(site)+x<0) x +=L;
    if(X(site)+x>L-1) x-=L;
    y = sinal(r - abs(x));
    if(Y(site)+y<0) y +=L;
    if(Y(site)+y>L-1) y -= L;
    return site + x + y*L;
}
int vizinho(int i, int y){
    switch(y){
        case 0: //UP//
            if (Y(i)!=L-1)return i+L;
            else return X(i);
        case 1: //Down//
            if(Y(i)!=0) return i-L;
            else return (X(i)+L*(L-1));
        case 2: //Right//
            if(X(i)!=L-1) return i+1;
            else return abs(i-(L-1));
        case 3: //Left//
            if (X(i)!=0) return i-1;
            else return i+L-1;
        case 4:
            return link[i];
    }
}
double greedy(int alvo,int* rede_ida,int* rede_volta){
	int size = 0,i,central;
	double T_medio = 0;
    int shell,d_menor,D,t,viz;
	int* ida = (int*) malloc(0*sizeof(int));
	for(central=0;central<N;central++){
		t = 0;
		ida = (int*) realloc(ida,(size+1)*sizeof(int));
		ida[size] = central;
		shell = central;
		rede_volta[central] = 0;
		while(shell!=alvo){
			if(rede_ida[shell]!=0){
				t += rede_ida[shell];
				shell = alvo;
				break;
			}
			d_menor = find_distance(shell,alvo);
    		for(i=0;i<V;i++){
        		D = find_distance(vizinho(shell,i),alvo);
        		if(d_menor>D){
					d_menor = D;
					viz = vizinho(shell,i);
        		}
    		}
			size++;
    		shell = viz;
			ida = (int*) realloc(ida,(size+1)*sizeof(int));
			t++;
			ida[size] = viz;
		}
		T_medio += t;
		for(i=0;i<size+1;i++){
			rede_ida[ida[i]] = t;
			t--;
		}
		size = 0;
	}
	T_medio /= (N-1);
	free(ida);
	return T_medio;
}
int main(int argc,char *argv[ ]){
    L = atoi(argv[1]);
	double alpha = (atof(argv[2]))/100;
	seed = atoi(argv[3]);
	N = L*L;
    int i;
	int* vetor = (int*) malloc(N*sizeof(int));
	int* v = (int*) malloc(N*sizeof(int));
	double T_medio = 0;
    link = (int*) malloc(N*sizeof(int));
	init_genrand64(seed);
    for(i=0;i<N;i++) link[i] = long_link(i,alpha);
	for(i=0;i<N;i++){
		if((i+1)%2!=0) T_medio += greedy(i,vetor,v);
		else T_medio += greedy(i,v,vetor);
	}
	T_medio /= N;
	free(vetor);
	free(v);
	printf("%f\t%f\n",alpha,T_medio);
    return 0;
}
