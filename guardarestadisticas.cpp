#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const string ARCHIVO_ESTADISTICAS = "estadisticas.txt";

struct EstadisticaJugador {
    string nombre;
    int carreras = 0;
    int ganes = 0;
    int empates = 0;
    int perdidas = 0;
};

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
        cout << "Presiona Enter..."; 
        cin.ignore(); 
        cin.get();
        return;
    }

    EstadisticaJugador lista[100];
    int total = 0;

    string nombreLeido;
    char resultadoLeido;

    while (archivo >> nombreLeido >> resultadoLeido) {
        if (resultadoLeido != 'G' && resultadoLeido != 'E' && resultadoLeido != 'P')
            continue;

        bool encontrado = false;
        for (int i = 0; i < total; i++) {
            if (lista[i].nombre == nombreLeido) {
                lista[i].carreras++;
                if (resultadoLeido == 'G') lista[i].ganes++;
                if (resultadoLeido == 'E') lista[i].empates++;
                if (resultadoLeido == 'P') lista[i].perdidas++;
                encontrado = true;
                break;
            }
        }

        if (!encontrado && total < 100) {
            lista[total].nombre = nombreLeido;
            lista[total].carreras = 1;
            if (resultadoLeido == 'G') lista[total].ganes = 1;
            if (resultadoLeido == 'E') lista[total].empates = 1;
            if (resultadoLeido == 'P') lista[total].perdidas = 1;
            total++;
        }
    }
    archivo.close();

    limpiarPantalla();//llamaremos la funcion despues
    cout << "=== ESTADISTICAS GENERALES ===\n";
    cout << "Jugador  Carreras  Ganes  Empates  Perdidas\n";

    for (int i = 0; i < total; i++) {
        cout << lista[i].nombre << "   " << lista[i].carreras << "   "
             << lista[i].ganes << "   " << lista[i].empates << "   " << lista[i].perdidas << endl;
    }

    for (int i = 0; i < total - 1; i++)
        for (int j = 0; j < total - i - 1; j++)
            if (lista[j].ganes < lista[j + 1].ganes) {
                EstadisticaJugador temp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }

    int tope = total < 3 ? total : 3;
    cout << "\n=== TOP 3 MEJORES CORREDORES ===\n";
    for (int i = 0; i < tope; i++)
        cout << (i + 1) << ". " << lista[i].nombre << " (" << lista[i].ganes << " victorias)\n";

    cout << "\nPresiona Enter...";
    cin.ignore();
    cin.get();
}
