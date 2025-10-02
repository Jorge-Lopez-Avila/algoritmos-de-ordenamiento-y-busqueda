#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

// Contador global de llamadas recursivas
long long call_count = 0;

// Fibonacci recursivo (versión original - ineficiente)
int nthFibo(int n) {
    call_count++; // Contar cada llamada
    
    // Caso base
    if (n <= 1) {
        return n;
    }
    
    // Caso recursivo
    return nthFibo(n - 1) + nthFibo(n - 2);
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

int main(void) {
    // Valores de n a probar
    // NOTA: Valores > 45 pueden tardar MUCHO tiempo (minutos/horas)
    int values[] = {10, 20, 30, 35, 40};
    int num_values = 5;
    
    printf("=== FIBONACCI RECURSIVO - ANALISIS DE COMPLEJIDAD ===\n\n");
    printf("%-10s %-20s %-20s %-20s %-15s\n", 
           "n", "Tiempo (segundos)", "Memoria (KB)", "Llamadas", "Resultado");
    printf("-----------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < num_values; i++) {
        int n = values[i];
        
        // Reiniciar contador
        call_count = 0;
        
        // Medir memoria antes
        long mem_before = get_memory_usage();
        
        // Medir tiempo de ejecución
        double start_time = get_time();
        int result = nthFibo(n);
        double end_time = get_time();
        
        // Medir memoria después
        long mem_after = get_memory_usage();
        
        // Calcular tiempo de ejecución
        double exec_time = end_time - start_time;
        
        // Imprimir resultados
        printf("%-10d %-20.6f %-20ld %-20lld %-15d\n", 
               n, exec_time, mem_after, call_count, result);
    }
    
    // Demostración del crecimiento exponencial
    printf("\n=== CRECIMIENTO EXPONENCIAL ===\n");
    printf("%-10s %-20s %-25s\n", "n", "Llamadas", "Tiempo estimado");
    printf("-------------------------------------------------------\n");
    
    // Calcular tiempo por llamada promedio (usando n=30)
    call_count = 0;
    double start = get_time();
    nthFibo(30);
    double time_30 = get_time() - start;
    double time_per_call = time_30 / call_count;
    
    // Proyecciones
    long long calls_35 = 29860703LL;  // Llamadas para n=35
    long long calls_40 = 331160281LL; // Llamadas para n=40
    long long calls_45 = 3672623805LL; // Llamadas para n=45
    
    printf("%-10d %-20lld %-25.3f seg\n", 30, call_count, time_30);
    printf("%-10d %-20lld %-25.3f seg\n", 35, calls_35, calls_35 * time_per_call);
    printf("%-10d %-20lld %-25.3f seg\n", 40, calls_40, calls_40 * time_per_call);
    printf("%-10d %-20lld %-25.1f min\n", 45, calls_45, (calls_45 * time_per_call) / 60);
    
    
    printf("\n=== SOLUCION: PROGRAMACION DINAMICA ===\n");
    printf("Fibonacci iterativo o con memoizacion: O(n)\n");
    printf("Reduciria Fibonacci(40) de ~%.1f seg a ~0.000001 seg\n", 
           calls_40 * time_per_call);
    printf("Mejora: ~%.0fx mas rapido\n", (calls_40 * time_per_call) / 0.000001);
    
    return 0;
}