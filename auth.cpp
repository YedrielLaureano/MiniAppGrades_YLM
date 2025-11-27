#include "auth.h"
#include "utils.h"
#include "menu.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

bool validarUsername(const string& username) {
    if (username.empty()) return false;
    if (username.length() < 5 || username.length() > 14) return false;

    for (char c : username) {
        if (!isalnum(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

bool validarPassword(const string& password) {
    if (password.empty()) return false;
    if (password.length() < 8 || password.length() > 12) return false;

    bool tieneLetra = false, tieneNumero = false;
    for (char c : password) {
        if (isspace(static_cast<unsigned char>(c))) return false;
        if (!isalnum(static_cast<unsigned char>(c))) return false;
        if (isalpha(static_cast<unsigned char>(c))) tieneLetra = true;
        if (isdigit(static_cast<unsigned char>(c))) tieneNumero = true;
    }
    return (tieneLetra && tieneNumero);
}

bool validarTexto(const string& texto, int minLen = 2, int maxLen = 50) {
    if (texto.empty()) return false;

    if ((int)texto.length() < minLen || (int)texto.length() > maxLen) return false;

    for (char c : texto) {
        unsigned char uc = static_cast<unsigned char>(c);

        if (!(isalnum(uc) || isspace(uc))) {
            return false;
        }
    }

    return true;
}

// Register
void registrarUsuario() {
    string username, password, tipo, completeName;

    cout << "\n--- Registro de usuario ---\n";

    // Validacion de username
    while (true) {
        cout << "Username (minimo 5 caracteres): ";
        cin >> username;

        if (!validarUsername(username)) {
            cout << "Entrada invalida. El username debe tener entre 5 y 14 caracteres y solo letras/numeros.\n";
            continue;
        }

        if (usuarioExiste(username)) {
            cout << "Ese nombre de usuario ya existe. Intenta con otro.\n";
        }
        else break;
    }

    // Validacion de password
    while (true) {
        cout << "Password (minimo 8 caracteres, debe incluir letras y numeros, sin espacios): ";
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> password;

        if (!validarPassword(password)) {
            cout << "Contrasena invalida. Debe tener entre 8 y 12 caracteres, incluir letras y numeros, y no contener espacios.\n";
            continue;
        }

        break;
    }

    // Validacion de tipo de usuario
    do {
        cout << "Eres estudiante o maestro? ";
        cin >> tipo;

        for (char& c : tipo) {
            c = tolower(c);
        }

        if (tipo != "estudiante" && tipo != "maestro") {
            cout << "Tipo invalido. Escribe 'estudiante' o 'maestro'.\n";
        }

    } while (tipo != "estudiante" && tipo != "maestro");

    cin.ignore();
    cout << "Nombre completo con apellidos: ";
    getline(cin, completeName);

   // Validacion de nombre completo
    if (!validarTexto(completeName, 5, 40)) {
        cout << "Nombre invalido. Debe tener entre 5 y 40 caracteres.\n";
        return;
    }

    string hashedPassword = SHA1(password);
    ofstream MyFile(FILENAME, ios::app);
    MyFile << username << " " << hashedPassword << " " << tipo << " " << completeName << endl;
    MyFile.close();

    registrarLog(username, "creo cuenta");
    cout << "Registro exitoso.\n";
}

// Login
void loginUsuario() {
    string username, password;
    cout << "\n--- Login de usuario ---\n";
    cout << "Username: ";
    cin >> username;

    // Validar username
    if(!validarUsername(username)) {
        cout << "Username invalido.\n";
        return;
    }
    cout << "Password: ";
    cin >> password;

    //Validar password
    if (!validarPassword(password)) {
        cout << "Credenciales incorrectas\n";
        return;
    }

    string hashedInput = SHA1(password);
    ifstream MyFile(FILENAME);
    string linea;
    bool encontrado = false;

    while (getline(MyFile, linea)) {
        stringstream ss(linea);
        string u, p, tipo;
        string completeName;
        ss >> u >> p >> tipo;
        getline(ss >> std::ws, completeName);


        if (u == username && p == hashedInput) {
            encontrado = true;
            registrarLog(username, "inicio sesion");
            cout << "Login exitoso.\n";
            if (tipo == "estudiante") {
                menuEstudiante(completeName, username);
            }
            else if (tipo == "maestro") {
                menuMaestro(completeName, username);
            }
            else if (tipo == "admin") {
                menuAdministrador(username);
            }
            else {
                cout << "Tipo de usuario desconocido.\n";
            }
            break;
        }
    }

    MyFile.close();
    if (!encontrado) {
        cout << "Credenciales incorrectas o usuario no encontrado.\n";
    }
}
