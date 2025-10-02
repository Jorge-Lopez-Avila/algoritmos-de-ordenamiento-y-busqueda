#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

// Función de búsqueda lineal
int search(int arr[], int n, int x) {
    for (int i = 0; i < n; i++)
        if (arr[i] == x)
            return i;
    return -1;
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

// Función para generar un arreglo aleatorio
void generate_random_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }
}

int main(void) {
    // Tamaños de arreglo a probar: 10^3, 10^4, 10^5
    int sizes[] = {1000, 10000, 100000};
    int num_sizes = 3;
    
    printf("=== BÚSQUEDA LINEAL ===\n\n");
    printf("%-15s %-20s %-20s %-15s\n", "Tamaño (n)", "Tiempo (segundos)", "Memoria (KB)", "Resultado");
    printf("-----------------------------------------------------------------------\n");
    
    srand(time(NULL)); // Inicializar semilla para números aleatorios
    
    for (int i = 0; i < num_sizes; i++) {
        int n = sizes[i];
        
        // Asignar memoria para el arreglo
        int *arr = (int *)malloc(n * sizeof(int));
        if (arr == NULL) {
            printf("Error: No se pudo asignar memoria para n=%d\n", n);
            continue;
        }
        
        // Generar arreglo aleatorio
        generate_random_array(arr, n);
        
        // Elemento a buscar (peor caso: último elemento)
        int x = arr[n - 1]; // Buscar el último elemento (peor caso)
        
        // Medir memoria antes
        long mem_before = get_memory_usage();
        
        // Medir tiempo de ejecución
        double start_time = get_time();
        int result = search(arr, n, x);
        double end_time = get_time();
        
        // Medir memoria después
        long mem_after = get_memory_usage();
        
        // Calcular diferencias
        double exec_time = end_time - start_time;
        
        // Imprimir resultados
        printf("%-15d %-20.6f %-20ld %-15d\n", 
               n, exec_time, mem_after, result);
        
        // Liberar memoria
        free(arr);
    }
    
    return 0;
}