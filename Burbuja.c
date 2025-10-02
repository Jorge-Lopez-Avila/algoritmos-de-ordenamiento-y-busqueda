#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>

// Función para intercambiar dos elementos
void swap(int* xp, int* yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Versión optimizada de Bubble Sort
void bubbleSort(int arr[], int n) {
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = false;
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }
        // Si no se hicieron intercambios, el arreglo ya está ordenado
        if (swapped == false)
            break;
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

// Generar arreglo ordenado (mejor caso)
void generate_sorted_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
}

// Generar arreglo en orden inverso (peor caso)
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
    // NOTA: 10^5 puede tardar varios segundos con bubble sort
    int sizes[] = {1000, 10000, 100000};
    int num_sizes = 3;
    
    printf("=== ORDENAMIENTO POR BURBUJA ===\n\n");
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
        bubbleSort(arr, n);
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
    
    return 0;
}