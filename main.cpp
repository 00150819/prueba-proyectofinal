#include <iostream>
#include <string>
using namespace std;

const int META = 70;

void mostrarMenu();
void limpiarPantalla();
void generarReportes();
int registrarJugadores(char[], string[]);
void iniciarCarrera(char[], int, string[]);

int main() {
    int opcion;

    do {
        mostrarMenu();
        cout << "Elige una opcion: ";
        cin >> opcion;

        limpiarPantalla();

        if (opcion == 1) {
            char caballos[5];
            string nombres[5];
            int cantidad = registrarJugadores(caballos, nombres);
            iniciarCarrera(caballos, cantidad, nombres);
        } 
        else if (opcion == 2) {
            cout << "=== REGLAS ===\n";
            cout << "2 a 5 jugadores\n";
            cout << "Inicial del nombre representa al caballo\n";
            cout << "META = " << META << " pasos\n";
            cin.ignore();
            cin.get();
        }
        else if (opcion == 3) {
            generarReportes();
        }

        limpiarPantalla();

    } while (opcion != 4);

    return 0;
}