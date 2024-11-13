#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mtwister.h"
int L; // Largura da rede
int N; // Número de sítios
int EMPTY; // Número que representa sítios vazios
int n_clusters = 0,cluster = 0; // Número de clusters e maior cluster
int px,py; // Número de sítios ocupados quando percola em X e em Y
int* ptr; // Vetor que salva, sítios ocupados ou não ocupados, se são raízes ou não
int* order; // Vetor que guarda a ordem que os sítios vão ser ocupados
int* directional; // Vetor que aponta para qual sítio, o sítio i está ligado
int* Xroot; // Distância em X de um sítio i para a sua raiz
int* Yroot; // Distância em Y de um sítio i para a sua raiz
int X(int i){
    return i%L; // Retorna a coordenada x do sítio i
}
int Y(int i){
    return i/L;// Retorna a coordenada x do sítio i
}
void find_distance(int a,int j){ // Calcula a distância entre a e j
    int dx=0,dy =0;
    if(ptr[j]<0){// Caso j seja uma raiz
        int i = a;
        while(i!=j){
            dx = -X(i) + X(directional[i]);
            if(abs(dx)==L-1) dx = -dx/abs(dx);
            dy = -Y(i) + Y(directional[i]);
            if(abs(dy)==L-1) dy = -dy/abs(dy);
            Xroot[a] += dx;
            Yroot[a] += dy;
            i = directional[i];
        }
    }
    else{//Caso contrário
        dx = -X(a) + X(j);
        if(abs(dx)==L-1) dx = -dx/abs(dx);
        dy = -Y(a) + Y(j);
        if(abs(dy)==L-1) dy = -dy/abs(dy);
        Xroot[a] = Xroot[j] + dx;
        Yroot[a] = Yroot[j] + dy;
    }
}
int findroot(int i){// Encontra a raiz de i e atualiza o vetor directional e ptr quando houve uma junção de dois clusters
    if (ptr[i]<0) return i; // Caso i já seja a raiz dele mesmo
    if(ptr[ptr[i]]>0){ //Caso contrário
        int j = i;
        int root = i;
        int dx=0,dy=0;
        int u = ptr[root];
        u = ptr[root];
        u = findroot(u);
        while(ptr[root]!=j){
            findroot(directional[j]);
            find_distance(root,directional[j]);
            j = directional[j];
            if(ptr[ptr[j]]<0)break;
        }
    }
    return ptr[i] = findroot(ptr[i]);
}
void snake(int a,int b,int Ra,int Rb,int dir){// Atualização primária do vetor directional e da distância do sítio a sua raiz
    if(Ra!=a){ // Alterando a direção das ligações
        int u = Ra;
        int t = a;
        while(u!=a){
            while(directional[t]!=u) t = directional[t];
            directional[u] = t;
            if(t!=a) ptr[t] = a;
            u = t;
            t = a;
        }
    }
    int i=0,j=0;
    switch (dir){ // Cálculo da distância para a Raiz
        case -2:
        case 1:
            j = -1;
            break;
        case -1:
        case 2:
            j = 1;
            break;
        case -4:
        case 3:
            i = -1;
            break;
        case -3:
        case 4:
            i = 1;
            break;
    }
    Xroot[a] = Xroot[b] + i;
    Yroot[a] = Yroot[b] + j;
    directional[a] = b;
}
int vizinho(int x, int y){// Função que retorna o vizinho y do sitio x
    switch(y){
        case 1: //UP//
            if (x/L!=L-1)return x+L;
            else return x-L*(L-1);
        case 2: //Down//
            if(x>=L) return x-L;
            else return x+(L-1)*L;
        case 3: //Right//
            if((x+1)%L!=0) return x+1;
            else return x+1-L;
        case 4: //Left//
            if (x%L!=0) return x-1;
            else return x+L-1;
    }
}
void findwrapping(int a,int b, int No){//Função que identifica quando ocorreu a percolação em X e/ou Y
    int deltax = X(a) - X(b);
    int deltay = Y(a) - Y(b);
    int dx = abs(Xroot[a] - Xroot[b]);
    int dy = abs(Yroot[a] - Yroot[b]);
    if(deltay==0){
        if((dy!=0)&&(py==EMPTY)) py =  (No+2);
        if((dx!=1)&&(px==EMPTY)) px =  (No+2);
    }
    if(deltax==0){
        if((dx!=0)&&(px==EMPTY)) px = (No+2);
        if((dy!=1)&&(py==EMPTY)) py = (No+2);
    }
}
void link(int neigh,int site,int t,int dir){// Função que Realiza a ligação entre dois sitios
    int r1 = findroot(site); //Raiz do sítio
    int r2 = findroot(neigh);//Raiz do vizinho
    if (r1!=r2){ // Caso sejam de raízes diferentes
        if (ptr[r1]>ptr[r2]){// Se o cluster do vizinho tiver maior tamanho 
            snake(site,neigh,r1,r2,-dir);
            ptr[r2] += ptr[r1];// O tamanho dos clusters é somado
            ptr[r1] = r2;// A raiz r1 passa a apontar para r2
            if(r1!=site) find_distance(r1,r2);//Para esse caso, atualizamos a distância de uma raiz para a outra
            r1 = r2;
            ptr[site] = r2;// Sitio aponta para r2
        }
        else{// Se o cluster do sitio tiver maior tamanho 
            snake(neigh,site,r2,r1,dir);
            ptr[r1] += ptr[r2]; // O tamanho dos clusters é somado
            ptr[r2] = r1; // A raiz r2 passa a apontar para r1
            if(r2!=neigh) find_distance(r2,r1); //Para esse caso, atualizamos a distância de uma raiz para a outra
            ptr[neigh] = r1;//O vizinho aponta para r1
        }
        n_clusters--; // Se houve uma ligação, o número de clusters diminuiu
        if(-ptr[r1]>cluster) cluster = -ptr[r1]; // Aqui salvamos o maior cluster
    }
    else findwrapping(site,neigh,t); // Caso contrário ele vai checar se houve percolação
}
void begin(){// Aqui preenchemos o vetor order em ordem crescente e o directional de valores vazios
    int i;
    for(i=0;i<N;i++){
        order[i] = i;
        directional[i] = EMPTY;
    }
}
void permutation(int seed){// Essa função aleatoriza o vetor order
	int i;
	for(i=0;i<N;i++){
    	init_genrand64(seed+i); // Gerador de números aleatórios
		double rand =genrand64_real2();
        int j = i + (N-i)*rand;
        int u = order[i];
        order[i] = order[j];
        order[j] = u;
    }
}
void neighbors(double* resultados,double* maior_cluster){// Aqui começamos a ocupar os sítios
    int i,j,site;
    FILE* critical_point;
    for(i=0;i<N;i++) ptr[i] = EMPTY; // Inicialmente todos estão desocupados
    for(i=0;i<N;i++){
        site = order[i]; //Selecionamos um sitio aleatório
        //printf("site = %d\n",site);
        ptr[site] = -1;// Ocupamos ele
        directional[site] = site; // Ele aponta para ele mesmo
        Xroot[site] = 0;
        Yroot[site] = 0;
        n_clusters++;
        for(j = 1; j < 5; j++) if(ptr[vizinho(site,j)]!=EMPTY) link(vizinho(site,j),site,i,j);
        resultados[i] += (double)cluster/(i+1);
        if((px!=EMPTY)&&(py!=EMPTY)) break;
    }
    char filename[800];
    sprintf(filename, "./output/critical_%d.txt", L);
    critical_point = fopen(filename,"a");
    //printf("%d,%d\n",cluster,n_clusters);
    if(px>py) fprintf(critical_point,"%d\t%d\t%d\n",px,py,px);
    else fprintf(critical_point,"%d\t%d\t%d\n",px,py,py);
    FILE* file_clusters;
    FILE* bigcluster;
    sprintf(filename, "./output/clusters_%d.txt", L);
    file_clusters = fopen(filename,"a");

    for ( i = 0; i < N; i++) if((ptr[i] != EMPTY) && (ptr[i] < 0)) fprintf(file_clusters,"%d\n",-ptr[i]);
    fclose(file_clusters);
    *maior_cluster += cluster;
}
int main(int argc,char *argv[ ]){ // Começa Aqui

    int seed = atoi(argv[1]); // Definindo valor da Seed

    L = atoi(argv[2]); // Definindo o valor do Tamanho da rede
    int redes = atoi(argv[3]); // Definindo o valor do Tamanho da rede

    N = L*L; // Definindo o valor do Número de Sítios
    EMPTY = -(N+1); // Definindo o valor do Número de Vazio
    int i;
    double* resultados = malloc(N*sizeof(double));
    double maior_cluster = 0;
    for ( i = 0; i < redes; i++){
        ptr = (int*)malloc(N*sizeof(int));// Definindo o tamanho do vetor
        order = (int*)malloc(N*sizeof(int));// Definindo o tamanho do vetor
        directional = (int*)malloc(N*sizeof(int));// Definindo o tamanho do vetor
        Xroot = (int*)malloc(N*sizeof(int));// Definindo o tamanho do vetor
        Yroot = (int*)malloc(N*sizeof(int));// Definindo o tamanho do vetor

        px = py = EMPTY;// Definindo que por hora, px e py são vazios
        begin();
        permutation(seed);
        neighbors(resultados,&maior_cluster);
        px = py = EMPTY;
        free(ptr);// Liberação de memória
        free(order);// Liberação de memória
        free(directional);// Liberação de memória
        free(Xroot);// Liberação de memória
        free(Yroot);// Liberação de memória
        seed++;
        n_clusters = 0,cluster = 0;
        if((i+1)%1000 == 0)printf("Rodou: %d\n",i+1);
    }
    FILE* file; 
    char filename[800];
    sprintf(filename, "./output/prob_%d.txt", L);
    file = fopen(filename,"a");
    for ( i = 0; i < N; i++) if(resultados[i] > 0)fprintf(file,"%f\n",resultados[i]/redes);
    fclose(file);
    free(resultados);

    sprintf(filename, "./output/big_cluster.txt");
    file = fopen(filename,"a");
    fprintf(file,"%d %f\n",L,maior_cluster/redes);
    fclose(file);
}