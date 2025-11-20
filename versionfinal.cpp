#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <chrono>
#include <cstdlib> 
#include <fstream>
#include <algorithm>

using namespace std;

const int META = 70; 
const string ARCHIVO_ESTADISTICAS = "estadisticas.txt"; 

struct EstadisticaJugador {
    string nombre;
    int carreras = 0;
    int ganes = 0;
    int empates = 0;
    int perdidas = 0;
};

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void mostrarMenu() {
    cout << "    === Bienvenido al juego ===    " << endl;
    cout << "    *** CARRERA DE CABALLOS *** " << endl;
    cout << "1. Iniciar Juego" << endl;
    cout << "2. Reglas de juego" << endl;
    cout << "3. Ver Reportes y Estadisticas" << endl; 
    cout << "4. Salir" << endl;
}

void guardarEstadisticaSimple(string nombre, char resultado) {
    ofstream archivo(ARCHIVO_ESTADISTICAS, ios::app);
    if (archivo.is_open()) {
        archivo << nombre << " " << resultado << endl;
        archivo.close();
    }
}

void generarReportes() {
    ifstream archivo(ARCHIVO_ESTADISTICAS);
    if (!archivo.is_open()) {
        cout << "No hay juegos registrados aun. Juega primero.\n";
        cout << "Presiona Enter..."; cin.ignore(); cin.get();
        return;
    }

    EstadisticaJugador lista[100];
    int total = 0;

    string nombreLeido;
    char resultadoLeido;

    while (archivo >> nombreLeido >> resultadoLeido) {
        
        // --- CORRECCIÓN: FILTRO DE SEGURIDAD ---
        // Si el resultado no es G (Gane), E (Empate) o P (Perdida),
        // significa que el archivo tiene basura. Saltamos esta línea.
        if (resultadoLeido != 'G' && resultadoLeido != 'E' && resultadoLeido != 'P') {
            continue; 
        }
        // ---------------------------------------

        bool encontrado = false;
        for (int i = 0; i < total; i++) {
            if (lista[i].nombre == nombreLeido) {
                lista[i].carreras++;
                if (resultadoLeido == 'G') lista[i].ganes++;
                else if (resultadoLeido == 'E') lista[i].empates++;
                else if (resultadoLeido == 'P') lista[i].perdidas++;
                encontrado = true;
                break;
            }
        }

        if (!encontrado && total < 100) {
            lista[total].nombre = nombreLeido;
            lista[total].carreras = 1;
            lista[total].ganes = 0;
            lista[total].empates = 0;
            lista[total].perdidas = 0;

            if (resultadoLeido == 'G') lista[total].ganes = 1;
            else if (resultadoLeido == 'E') lista[total].empates = 1;
            else if (resultadoLeido == 'P') lista[total].perdidas = 1;
            
            total++;
        }
    }
    archivo.close();

    limpiarPantalla();
    cout << "=== ESTADISTICAS GENERALES ===\n";
    cout << "Jugador\t\tCarreras | Ganes | Empates | Perdidas\n";
    cout << "--------------------------------------------------------\n";
    for (int i = 0; i < total; i++) {
        cout << lista[i].nombre << "\t\t   " << lista[i].carreras << "\t   " << lista[i].ganes << "\t    " << lista[i].empates << "\t      " << lista[i].perdidas << endl;
    }

    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (lista[j].ganes < lista[j + 1].ganes) {
                EstadisticaJugador temp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }

    cout << "\n=== TOP 3 MEJORES CORREDORES ===\n";
    int tope = 3;
    if (total < 3) tope = total;

    for (int i = 0; i < tope; i++) {
        cout << (i + 1) << ". " << lista[i].nombre << " (" << lista[i].ganes << " victorias)" << endl;
    }

    cout << "\nPresiona Enter para volver...";
    cin.ignore(); cin.get();
}

void mostrarCarrera(char caballos[], int cantidad, int posiciones[], int dados[], int ranking[], int turno, string nombres[]) {
    
    cout << "\n-----------------------------------------------------------\n";
    cout << " >>> TURNO " << turno << " <<<\n";
    cout << "-----------------------------------------------------------\n";

    for (int i = 0; i < cantidad; i++) {
        cout << " - " << nombres[i] << " (" << caballos[i] << "): ";
        if (posiciones[i] >= META && dados[i] == 0) {
            cout << "YA LLEGO a la meta.";
        } else {
            cout << "Saco un " << dados[i] << " y avanza a la posicion " << posiciones[i];
        }
        cout << endl;
    }
    cout << endl;

    cout << "   +"; for(int k=0; k<META; k++) cout << "-"; cout << "+ META\n";

    for (int i = 0; i < cantidad; i++) {
        cout << " " << caballos[i] << " |";
        
        for (int k = 0; k < META; k++) {
            if (k == posiciones[i]) cout << caballos[i]; 
            else if (k < posiciones[i]) cout << " "; 
            else cout << "."; 
        }
        cout << "| ";

        if (posiciones[i] >= META) {
            cout << "LLEGO (" << ranking[i] << " lugar)";
        }
        cout << endl;
    }
    cout << "   +"; for(int k=0; k<META; k++) cout << "-"; cout << "+ META\n";
}

int registrarJugadores(char caballos[], string nombres[]) {
    int cantidad;
    do {
        cout << "\nIngrese la cantidad de jugadores (2-5): ";
        if (!(cin >> cantidad)) {
            cin.clear(); cin.ignore(10000, '\n');
            cantidad = 0;
        }
        if (cantidad < 2 || cantidad > 5) {
            cout << "Por favor introduce entre 2 a 5 jugadores\n";
        }
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

    int caballosEnMeta = 0;
    int lugarActual = 1; 
    int turno = 1; 

    while (caballosEnMeta < cantidad) {
        
        for (int i = 0; i < cantidad; i++) {
            
            if (!terminados[i]) {
                int pasos = dado(gen);
                dados[i] = pasos; 
                posiciones[i] += pasos;

                if (posiciones[i] >= META) {
                    posiciones[i] = META; 
                    terminados[i] = true;
                    ranking[i] = lugarActual; 
                    caballosEnMeta++;         
                }
            } else {
                dados[i] = 0; 
            }
        }
        
        lugarActual = caballosEnMeta + 1; 

        mostrarCarrera(caballos, cantidad, posiciones, dados, ranking, turno, nombres);
        this_thread::sleep_for(chrono::milliseconds(500)); 
        turno++;
    }

    cout << "\n=== RESULTADOS FINALES (Orden de Llegada) ===\n";

    for (int i = 0; i < cantidad; i++) {
        cout << nombres[i] << " llego en la posicion: " << ranking[i] << endl;
        
        char resultado;
        if (ranking[i] == 1) {
             int contPrimeros = 0;
             for(int j=0; j<cantidad; j++) if(ranking[j] == 1) contPrimeros++;
             
             if (contPrimeros > 1) resultado = 'E';
             else resultado = 'G';
             
             cout << "-> GANADOR!" << endl;
        } else {
             resultado = 'P';
        }
        guardarEstadisticaSimple(nombres[i], resultado);
    }
    
    cout << "\nResultados guardados. Presiona una tecla y Enter...";
    string pausa; cin >> pausa;
}

int main() {
    int opcion;
    do {
        mostrarMenu();
        cout << "\nElige una opcion: ";
        if (!(cin >> opcion)) {
            cin.clear(); cin.ignore(10000, '\n');
            continue;
        }

        switch (opcion) {
            case 1: {
                char caballos[5];
                string nombres[5];
                int cantidad = registrarJugadores(caballos, nombres);
                iniciarCarrera(caballos, cantidad, nombres);
                break;
            }
            case 2:
                limpiarPantalla();
                cout << "\n=== REGLAS DE JUEGO ===\n";
                cout << "1. 2 jugadores minimo, 5 maximo.\n";
                cout << "2. La letra inicial de tu nombre sera tu caballo.\n";
                cout << "3. La carrera termina cuando TODOS cruzan la meta (" << META << " pasos).\n";
                cout << "\nPresiona Enter para volver...";
                cin.ignore(); cin.get();
                break;
            case 3:
                generarReportes(); 
                break;
            case 4:
                cout << "\nSaliendo...\n";
                break;
            default:
                cout << "Opción no válida.\n";
        }
        limpiarPantalla();
    } while (opcion != 4);
    return 0;
}