#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>

// Función para mezclar dos subarreglos
// Primer subarreglo: arr[l..m]
// Segundo subarreglo: arr[m+1..r]
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    
    // Crear arreglos temporales
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));
    
    // Copiar datos a los arreglos temporales L[] y R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    
    // Mezclar los arreglos temporales de vuelta en arr[l..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copiar los elementos restantes de L[], si hay alguno
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copiar los elementos restantes de R[], si hay alguno
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    // Liberar memoria temporal
    free(L);
    free(R);
}

// l es el índice izquierdo y r es el índice derecho del subarreglo
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        
        // Ordenar primera y segunda mitad
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        
        // Mezclar las mitades ordenadas
        merge(arr, l, m, r);
    }
}

// Función para obtener el tiempo actual en segundos
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

// Función para obtener el uso de memoria en KB
long get_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // En KB en Linux
}

// Generar arreglo aleatorio
void generate_random_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
}

// Generar arreglo ordenado
void generate_sorted_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
}

// Generar arreglo en orden inverso
void generate_reverse_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

// Verificar si el arreglo está ordenado
bool is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1])
            return false;
    }
    return true;
}

int main(void) {
    // Tamaños de arreglo a probar: 10^3, 10^4, 10^5
    int sizes[] = {1000, 10000, 100000};
    int num_sizes = 3;
    
    printf("=== MERGE SORT ===\n\n");
    printf("%-15s %-20s %-20s %-15s\n", "Tamano (n)", "Tiempo (segundos)", "Memoria (KB)", "Ordenado?");
    printf("-----------------------------------------------------------------------\n");
    
    srand(time(NULL));
    
    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        
        // Asignar memoria para el arreglo
        int *arr = (int *)malloc(n * sizeof(int));
        if (arr == NULL) {
            printf("Error: No se pudo asignar memoria para n=%d\n", n);
            continue;
        }
        
        // Generar arreglo aleatorio (caso promedio)
        generate_random_array(arr, n);
        
        // Medir memoria antes
        long mem_before = get_memory_usage();
        
        // Medir tiempo de ejecución
        double start_time = get_time();
        mergeSort(arr, 0, n - 1);
        double end_time = get_time();
        
        // Medir memoria después
        long mem_after = get_memory_usage();
        
        // Verificar si está ordenado
        bool sorted = is_sorted(arr, n);
        
        // Calcular tiempo de ejecución
        double exec_time = end_time - start_time;
        
        // Imprimir resultados
        printf("%-15d %-20.6f %-20ld %-15s\n", 
               n, exec_time, mem_after, sorted ? "SI" : "NO");
        
        // Liberar memoria
        free(arr);
    }
    
    // Comparación de casos
    printf("\n=== COMPARACION: MEJOR vs PEOR vs PROMEDIO ===\n");
    int test_n = 100000;
    int *test_arr = (int *)malloc(test_n * sizeof(int));
    
    // Mejor caso: arreglo ordenado
    generate_sorted_array(test_arr, test_n);
    double start = get_time();
    mergeSort(test_arr, 0, test_n - 1);
    double time_best = get_time() - start;
    
    // Peor caso: arreglo invertido
    generate_reverse_array(test_arr, test_n);
    start = get_time();
    mergeSort(test_arr, 0, test_n - 1);
    double time_worst = get_time() - start;
    
    // Caso promedio: arreglo aleatorio
    generate_random_array(test_arr, test_n);
    start = get_time();
    mergeSort(test_arr, 0, test_n - 1);
    double time_avg = get_time() - start;
    
    printf("Tamano: %d elementos\n", test_n);
    printf("Mejor caso (ordenado):  %.6f segundos\n", time_best);
    printf("Caso promedio (random): %.6f segundos\n", time_avg);
    printf("Peor caso (invertido):  %.6f segundos\n", time_worst);
    
    free(test_arr);
    
    return 0;
}