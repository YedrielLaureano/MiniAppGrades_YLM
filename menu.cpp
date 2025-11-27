#include "menu.h"
#include "notas.h"
#include "utils.h"
#include <string>
#include <iostream>

using namespace std;


// Menu estudiante
void menuEstudiante(const string& completeName, const string& username) {
    int opcion;
    do {
        cout << "\n--- Menu Estudiante ---\n";
        cout << "1. Ver mis notas\n";
        cout << "2. Cerrar sesion\n";
        cout << "Opcion: ";
        
        while (true) {
            if (cin >> opcion && opcion >= 1 && opcion <= 2) {
                break;
            }
            else {
                cout << "Entrada invalida. Solo numeros del 1 al 2.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }

        switch (opcion) {
        case 1: {
            verNotasEstudiante(completeName, username);
            break;
        }
        case 2:
            registrarLog(username, "cerro sesion");
            cout << "Sesion cerrada.\n";
            break;
        default:
            cout << "Opcion invalida.\n";
        }
    } while (opcion != 2);
}

// Menu maestro
void menuMaestro(const string& completeNmae, const string& username) {
    int opcion;
    do {
        cout << "\n--- Menu Maestro ---\n";
        cout << "1. Ver todas las notas\n";
        cout << "2. Crear nota\n";
        cout << "3. Editar nota\n";
        cout << "4. Eliminar nota\n";
        cout << "5. Cerrar sesion\n";
        cout << "Opcion: ";
        
        while (true) {
            if (cin >> opcion && opcion >= 1 && opcion <= 5) {
                break;
            }
            else {
                cout << "Entrada invalida. Solo numeros del 1 al 5.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }

        switch (opcion) {
        case 1: {
            verNotasMaestro(username);
            break;
        }
        case 2: {
            crearNota(username);
            break;
        }
        case 3: {
            editarNota(username);
            break;
        }
        case 4: {
            eliminarNota(username);
            break;
        }
        case 5:
            registrarLog(username, "cerro sesion");
            cout << "Sesion cerrada.\n";
            break;
        default:
            cout << "Opcion involida.\n";
        }
    } while (opcion != 5);
}

// Menu administrador
void menuAdministrador(const string& username) {
    int opcion;
    do {
        cout << "\n--- Menu Administrador ---\n";
        cout << "1. Ver usuarios\n";
        cout << "2. Ver log file\n";
        cout << "3. Cerrar sesion\n";
        cout << "Opcion: ";

        while (true) {
            if (cin >> opcion && opcion >= 1 && opcion <= 3) {
                break;
            }
            else {
                cout << "Entrada invalida. Solo numeros del 1 al 3.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }

        switch (opcion) {
        case 1: {
            verUsuarios();
            break;
        }
        case 2: {
            verLogFile();
            break;
        }
        case 3:
            registrarLog(username, "cerro sesion");
            cout << "Sesion cerrada.\n";
            break;
        default:
            cout << "Opcion invalida.\n";
        }
    } while (opcion != 3);
}