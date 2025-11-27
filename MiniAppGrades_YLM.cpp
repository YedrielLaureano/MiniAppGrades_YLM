//******************************************************************************//
// Yedriel Laureano Marrero | Y00656197                            11/26/2025   //
//                                                                              //
//          Proyecto: “Mini-App Segura de Autenticación y Datos”                //
//                         (Sistema de Notas)                                   //
//                                                                              //
// COMP2700.CYBERSECURITY                                                       //
// Prof. H. Cardona                                                             //
//******************************************************************************//


#include "auth.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

int main() {
    int opcion;
    do {
        this_thread::sleep_for(chrono::seconds(5));
        system("cls");
        cout << "====================================================\n";
        cout << "========== Bienvenido al sistema de notas ==========\n";
        cout << "====================================================\n";
        cout << "\n1. Registrarse\n";
        cout << "2. Login\n";
        cout << "3. Salir\n";
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
        case 1:
            registrarUsuario();
            break;
        case 2:
            loginUsuario();
            break;
        case 3:
            cout << "Vuelva pronto.\n";
            break;
        default:
            cout << "Opcion invalida.\n";
        }
    } while (opcion != 3);

    return 0;
}