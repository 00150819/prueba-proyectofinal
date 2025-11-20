#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <chrono>
using namespace std;

extern const int META;
extern void limpiarPantalla();
extern void guardarEstadisticaSimple(string, char);

void mostrarCarrera(char caballos[], int cantidad, int posiciones[], int dados[], int ranking[], int turno, string nombres[]) {
    cout << "\n--- TURNO " << turno << " ---\n";

    for (int i = 0; i < cantidad; i++) {
        cout << nombres[i] << " saco " << dados[i] << " -> posicion " << posiciones[i];
        if (posiciones[i] >= META) cout << " LLEGO (" << ranking[i] << ")";
        cout << endl;
    }

    for (int i = 0; i < cantidad; i++) {
        cout << caballos[i] << " |";
        for (int k = 0; k < META; k++) {
            if (k == posiciones[i]) cout << caballos[i];
            else if (k < posiciones[i]) cout << " ";
            else cout << ".";
        }
        cout << "|\n";
    }
}

int registrarJugadores(char caballos[], string nombres[]) {
    int cantidad;
    do {
        cout << "Ingrese la cantidad de jugadores (2-5): ";
        cin >> cantidad;
    } while (cantidad < 2 || cantidad > 5);

    for (int i = 0; i < cantidad; i++) {
        cout << "Nombre del jugador " << i + 1 << ": ";
        cin >> nombres[i];
        caballos[i] = nombres[i][0];
    }
    return cantidad;
}

void iniciarCarrera(char caballos[], int cantidad, string nombres[]) {
    limpiarPantalla();
    cout << "PREPARADOS... LISTOS... YA!\n";

    int posiciones[5] = {0};
    int dados[5] = {0};
    int ranking[5] = {0};
    bool terminados[5] = {false};

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dado(1, 6);

    int enMeta = 0;
    int turno = 1;
    int lugar = 1;

    while (enMeta < cantidad) {
        for (int i = 0; i < cantidad; i++) {
            if (!terminados[i]) {
                int pasos = dado(gen);
                dados[i] = pasos;
                posiciones[i] += pasos;

                if (posiciones[i] >= META) {
                    posiciones[i] = META;
                    terminados[i] = true;
                    ranking[i] = lugar;
                    enMeta++;
                }
            } else {
                dados[i] = 0;
            }
        }

        lugar = enMeta + 1;
        mostrarCarrera(caballos, cantidad, posiciones, dados, ranking, turno, nombres);
        this_thread::sleep_for(chrono::milliseconds(500));
        turno++;
    }

    cout << "\n=== RESULTADOS ===\n";

    for (int i = 0; i < cantidad; i++) {
        cout << nombres[i] << " llego en la posicion " << ranking[i] << endl;
        char r = (ranking[i] == 1 ? 'G' : 'P');
        guardarEstadisticaSimple(nombres[i], r);
    }

    cout << "Presiona Enter...";
    string pausa;
    cin >> pausa;
}