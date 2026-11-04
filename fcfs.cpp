#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// Estructura para representar las propiedades y métricas de cada proceso
struct Proceso {
    string id;
    int llegada;
    int servicio;
    int comienzo;
    int finalizacion;
    int espera;
    int estancia;
    double estanciaNormalizada;
    int ordenOriginal; // Para desempatar respetando el orden de ingreso
};

int main() {
    int n;
    cout << "=== SIMULADOR DE PLANIFICACION FCFS ===" << endl;
    cout << "Ingrese el numero de procesos (entre 3 y 5): ";
    cin >> n;

    while (n < 3 || n > 5) {
        cout << "Numero invalido. Ingrese un valor entre 3 y 5: ";
        cin >> n;
    }

    vector<Proceso> procesos(n);

    // Lectura de datos de entrada
    for (int i = 0; i < n; ++i) {
        procesos[i].id = "P" + to_string(i + 1);
        procesos[i].ordenOriginal = i;
        
        cout << "\n--- Proceso " << (i + 1) << " ---" << endl;
        cout << "Identificador (ej. A, B, P1): ";
        cin >> procesos[i].id;
        
        do {
            cout << "Tiempo de llegada (TL >= 0): ";
            cin >> procesos[i].llegada;
        } while (procesos[i].llegada < 0);

        do {
            cout << "Tiempo de servicio (Ts > 0): ";
            cin >> procesos[i].servicio;
        } while (procesos[i].servicio <= 0);
    }

    // Ordenamiento estable por tiempo de llegada (TL). 
    // En caso de empate, se respeta el orden original de ingreso.
    stable_sort(procesos.begin(), procesos.end(), [](const Proceso& a, const Proceso& b) {
        if (a.llegada != b.llegada) {
            return a.llegada < b.llegada;
        }
        return a.ordenOriginal < b.ordenOriginal;
    });

    // Simulación de ejecución del algoritmo FCFS
    int tiempoActual = 0;
    double sumaEspera = 0;
    double sumaEstancia = 0;
    double sumaEstanciaNorm = 0;

    cout << "\n=== TRAZA DE EJECUCION DE LA CPU ===" << endl;
    for (int i = 0; i < n; ++i) {
        // Detectar intervalo de inactividad de la CPU
        if (tiempoActual < procesos[i].llegada) {
            cout << "[CPU OCIOSA] Desde t=" << tiempoActual 
                 << " hasta t=" << procesos[i].llegada << endl;
            tiempoActual = procesos[i].llegada;
        }

        procesos[i].comienzo = tiempoActual;
        procesos[i].finalizacion = procesos[i].comienzo + procesos[i].servicio;
        procesos[i].espera = procesos[i].comienzo - procesos[i].llegada;
        procesos[i].estancia = procesos[i].finalizacion - procesos[i].llegada;
        procesos[i].estanciaNormalizada = (double)procesos[i].estancia / procesos[i].servicio;

        tiempoActual = procesos[i].finalizacion;

        sumaEspera += procesos[i].espera;
        sumaEstancia += procesos[i].estancia;
        sumaEstanciaNorm += procesos[i].estanciaNormalizada;
    }

    // Presentación de la tabla de resultados
    cout << "\n" << string(60, '=') << endl;
    cout << left << setw(10) << "Proceso"
         << setw(7)  << "TL"
         << setw(7)  << "Ts"
         << setw(7)  << "TC"
         << setw(7)  << "TF"
         << setw(7)  << "TE"
         << setw(7)  << "Tr"
         << setw(8)  << "Tr/Ts" << endl;
    cout << string(60, '-') << endl;

    for (const auto& p : procesos) {
        cout << left << setw(10) << p.id
             << setw(7)  << p.llegada
             << setw(7)  << p.servicio
             << setw(7)  << p.comienzo
             << setw(7)  << p.finalizacion
             << setw(7)  << p.espera
             << setw(7)  << p.estancia
             << fixed << setprecision(2) << setw(8) << p.estanciaNormalizada << endl;
    }
    cout << string(60, '=') << endl;

    // Métricas promedio
    cout << "\nPROMEDIOS DEL SISTEMA:" << endl;
    cout << "  - Tiempo de Espera Promedio (TE):   " << fixed << setprecision(2) << (sumaEspera / n) << endl;
    cout << "  - Tiempo de Estancia Promedio (Tr): " << fixed << setprecision(2) << (sumaEstancia / n) << endl;
    cout << "  - Estancia Normalizada Promedio:    " << fixed << setprecision(2) << (sumaEstanciaNorm / n) << endl;

    return 0;
}
