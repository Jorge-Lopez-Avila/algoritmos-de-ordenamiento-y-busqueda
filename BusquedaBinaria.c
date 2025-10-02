#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

// Función de búsqueda binaria
int binarySearch(int arr[], int n, int x) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        
        // Check if x is present at mid
        if (arr[mid] == x)
            return mid;
        
        // If x greater, ignore left half
        if (arr[mid] < x)
            low = mid + 1;
        
        // If x is smaller, ignore right half
        else
            high = mid - 1;
    }
    
    // If we reach here, then element was not present
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

// Función para generar un arreglo ORDENADO (requisito para búsqueda binaria)
void generate_sorted_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i; // Arreglo ordenado: 0, 1, 2, 3, ..., n-1
    }
}

// Función de comparación para qsort (alternativa)
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int main(void) {
    // Tamaños de arreglo a probar: 10^3, 10^4, 10^5
    int sizes[] = {1000, 10000, 100000};
    int num_sizes = 3;
    
    printf("=== BUSQUEDA BINARIA ===\n\n");
    printf("%-15s %-20s %-20s %-15s\n", "Tamano (n)", "Tiempo (segundos)", "Memoria (KB)", "Resultado");
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
        
        // Generar arreglo ORDENADO (CRÍTICO para búsqueda binaria)
        generate_sorted_array(arr, n);
        
        // Elemento a buscar (peor caso: último elemento)
        int x = n - 1; // Buscar el último elemento del arreglo ordenado
        
        // Medir memoria antes
        long mem_before = get_memory_usage();
        
        // Medir tiempo de ejecución
        double start_time = get_time();
        int result = binarySearch(arr, n, x);
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
    
    // Prueba comparativa de peor caso
    printf("\n=== COMPARACION: PEOR CASO vs MEJOR CASO ===\n");
    int test_n = 100000;
    int *test_arr = (int *)malloc(test_n * sizeof(int));
    generate_sorted_array(test_arr, test_n);
    
    // Mejor caso: elemento en el medio
    double start = get_time();
    int result_best = binarySearch(test_arr, test_n, test_n / 2);
    double time_best = get_time() - start;
    
    // Peor caso: elemento al final
    start = get_time();
    int result_worst = binarySearch(test_arr, test_n, test_n - 1);
    double time_worst = get_time() - start;
    
    printf("Mejor caso (medio): %.9f segundos (indice: %d)\n", time_best, result_best);
    printf("Peor caso (final):  %.9f segundos (indice: %d)\n", time_worst, result_worst);
    
    free(test_arr);
    
    return 0;
}