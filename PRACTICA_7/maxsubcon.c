#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COND 40
#define INFINITO 999999999

typedef struct {
    int* sol;
    int tam_sol;
    int* rest;
    int tam_rest;
} resultado;


void llenar_arr(int*, int);
void print_arr(int*, int);
int merge_sort(int*, int, int);
int merge(int*, int, int, int);
int sumar_arreglo(int*, int); 
resultado maxsubcon(int*, int);


int main(){
    srand(time(NULL));
    int n;

    printf("Ingrese la longitud del arreglo: ");
    if(scanf("%d",&n) != 1 || n < 2){
        printf("\nError: Valor invalido. \nDebe ser un numero mayor o igual a 2\n");
        return 1;
    }


    int* datos = (int*)malloc(n * sizeof(int));
    if (datos == NULL) {
        printf("Error al asignar memoria\n");
        return 1;
    }

    llenar_arr(datos,n);
    printf("Arreglo generado\n");

    if(n <= COND)
        print_arr(datos,n);


    if(sumar_arreglo(datos,n) == 0){
        printf("\nNo existe solucion: todos los elementos son 0\n");
        free(datos);
        return 0;
    }

    if(merge_sort(datos,0,n-1)){
        printf("Error al asignar memoria\n");
        free(datos);
        return 1;
    }

    printf("Arreglo ordenado\n");

    if(n <= COND)
        print_arr(datos,n);
    else
        printf("\n");

    clock_t inicio = clock();
    resultado salida = maxsubcon(datos,n);
    clock_t fin = clock();
    float tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;

    if(salida.sol == NULL){
        printf("Error al asignar memoria.\n");
        free(datos);
        return 1;
    }


    if(salida.tam_sol <= COND){
        printf("\nSolucion: \n");
        print_arr(salida.sol,salida.tam_sol);
    }
    printf("Longitud del conjunto solucion: %d\n",salida.tam_sol);
    printf("Suma: %d\n\n",sumar_arreglo(salida.sol,salida.tam_sol));

    if(salida.tam_rest <= COND){
        printf("Elementos restantes: \n");
        print_arr(salida.rest,salida.tam_rest);
    }
    printf("Longitud del conjunto de restantes: %d\n",salida.tam_rest);
    printf("Suma: %d\n\n",sumar_arreglo(salida.rest,salida.tam_rest));

    printf("Tiempo para la obtencion de la solucion: %lf s\n", tiempo);

    free(datos);
    free(salida.sol);
    free(salida.rest);

    return 0;
}


void llenar_arr(int* arr, int n){
    int min = 0;
    int max = 20;

    for (int i = 0; i < n; i++) 
        arr[i] = min + rand() % (max - min + 1);
}


void print_arr(int* arr, int n){
    printf("{  ");
    for(int i = 0;i < n; i++)
        printf("%d  ",arr[i]);
    
    printf("}\n");
}


int merge_sort(int *A, int p, int r) {
    if (p < r) {
        int q = p + (r - p) / 2; //1
        if(merge_sort(A, p, q))
            return 1;
        if(merge_sort(A, q + 1, r))
            return 1;
        return merge(A, p, q, r);
    }
    return 0;
}


int merge(int *A, int p, int q, int r){
    int n1 = q - p + 1;
    int n2 = r - q;
    int *L = malloc((n1 + 1)*sizeof(int));
    int *R = malloc((n2 + 1)*sizeof(int));
    if(L == NULL || R == NULL){
        free(L);
        free(R);
        return 1;
    }
    int i = 0;
    int j = 0;
    for(i = 0; i < n1; ++i){
        *(L+i) = *(A + p + i);
    }
    for(j = 0; j < n2; ++j){
        *(R+j) = *(A + q + 1 + j);
    }
    
    *(L+i) = -INFINITO;
    *(R+j) = -INFINITO;
    i = 0;
    j = 0;

    for(int k = p; k <= r; ++k){
        if(*(L + i) >= *(R + j)){
            *(A + k) = *(L + i);
            i++;
        }else{
            *(A + k) = *(R + j);
            j++;
        }
    }
    free(L);
    free(R);
    return 0;
}


int sumar_arreglo(int* A, int n){
    int sum = 0;
    for(int i=0 ; i<n ; i++)
        sum += A[i];

    return sum;
}


resultado maxsubcon(int* datos, int n){
    int suma_datos = sumar_arreglo(datos,n);
    int suma_sol = 0;

    resultado salida;
    salida.sol = (int*)malloc(n * sizeof(int)); 
    salida.tam_sol = 0;
    salida.rest = (int*)malloc(n * sizeof(int));
    salida.tam_rest = n;

    if(salida.sol == NULL || salida.rest == NULL){
        free(salida.sol);
        free(salida.rest);
        salida.sol = NULL;
        salida.rest = NULL;
        return salida;
    }

    while(suma_datos >= suma_sol && salida.tam_sol < n){
        salida.sol[salida.tam_sol] = datos[salida.tam_sol];
        suma_sol += datos[salida.tam_sol];
        suma_datos -= datos[salida.tam_sol];
        salida.tam_sol ++;
        salida.tam_rest --;
    }
    
    for(int i = 0 ; i < salida.tam_rest ; i++)
        salida.rest[i] = datos[i+salida.tam_sol];
    
    return salida;
}