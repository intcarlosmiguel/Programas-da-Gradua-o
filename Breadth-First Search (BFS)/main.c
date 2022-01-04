#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define L 8
#define N (L*L)
#define infinity L+1
int X(int i){
    return i%L;
}
int Y(int i){
    return i/L;
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
    }
}
void print_lattice(int* connection){
    int i;
    for(i=N-1;i>=0;i--){
        if(i%L!=0) printf("%d\t",connection[i]);
        else printf("%d\n",connection[i]);
    }
}
void begin(int central){
    double l = 0;
    int i,j,size=1,site,shell,connect;
    int* order = (int*)malloc(size*sizeof(int));
    int connection[N];
    for(i=0;i<N;i++) connection[i] = infinity;
    connection[central] = 0;
    order[0] = central;
    for(i=0;i<N;i++){
        if(size==N) break;
    	site = order[i];
    	connect = connection[site];
    	for(j=0;j<4;j++){
    	    shell = vizinho(site,j);
    	    if(connection[shell]>connect+1){
    	        order = (int*)realloc(order,(size+1)*sizeof(int));
    	        order[size] = shell;
    	        connection[shell] = connect+1;
    	        l += connection[shell];
    	        size++;
    	    }
    	}
    }
    free(order);
    printf("%f\n",l/(N-1));
    print_lattice(connection);
}
int main(){
    begin(8);
    return 0;
}
