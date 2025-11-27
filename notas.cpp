#include "notas.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>


using namespace std;

bool validar_Texto(const string& texto, int minLen = 2, int maxLen = 50) {
    if (texto.empty()) return false;

    for (char c : texto) {
        unsigned char uc = static_cast<unsigned char>(c);

        if (!(isalnum(uc) || isspace(uc))) {
            return false;
        }
    }

    if ((int)texto.length() < minLen || (int)texto.length() > maxLen) return false;

    return true;
}

// Create (nota -> maestro)
void crearNota(const string& username) {
    string estudiante, materia;
    float nota;

    cout << "Estudiante: ";
    cin.ignore();
    getline(cin, estudiante);

    // Validar estudiante
    if (!validar_Texto(estudiante, 5, 40)) {
        cout << "Nombre invalido. Debe tener entre 5 y 40 caracteres.\n";
        return;
    }

    cout << "Materia: ";
    getline(cin, materia);

    // Validar materia
    if (!validar_Texto(materia, 3, 20)) {
        cout << "Materia invalida. Debe tener entre 3 y 20 caracteres.\n";
        return;
    }

    cout << "Nota: ";

    // Validar nota
    while (true) {
        if (cin >> nota) {
            if (nota >= 0 && nota <= 100) {
                break;
            }
            else {
                cout << "La nota debe estar entre 0 y 100. Intenta de nuevo: ";
            }
        }
        else {
            cout << "Entrada invalida. Ingresa numeros solamente: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    ofstream notas(NOTASFILE, ios::app);

    // estudiante|materia|nota|profesor
    notas << estudiante << "|" << materia << "|" << nota << "|" << username << endl;
    notas.close();
    registrarLog(username, "creo nota para " + estudiante + " en " + materia);
    cout << "Nota registrada.\n";
}

// Read (nota -> estudiante)
void verNotasEstudiante(const string& completeName, const string& username) {
    ifstream notas(NOTASFILE);
    string linea;
    bool hayNotas = false;
    while (getline(notas, linea)) {
        stringstream ss(linea);
        string est, materia, prof, notaStr;
        float nota;

        getline(ss, est, '|');
        getline(ss, materia, '|');
        getline(ss, notaStr, '|');
        getline(ss, prof, '|');

        // evitar errores si la linea esta incompleta
        if (est.empty() || materia.empty() || notaStr.empty() || prof.empty()) continue;

        // convertir nota a numero
        try {
            nota = stof(notaStr);
        }
        catch (...) {
            continue;  // linea danada -> ignorar
        }

        if (est == completeName) {
            cout << "Materia: " << materia << " | Nota: " << nota << endl;
            hayNotas = true;
        }
    }
    if (!hayNotas) cout << "No tienes notas registradas.\n";
}

// Read (nota -> maestro)
void verNotasMaestro(const string& username) {
    ifstream notas(NOTASFILE);
    string linea;
    bool hayNotas = false;
    while (getline(notas, linea)) {
        stringstream ss(linea);
        string est, mat, prof;
        float nota;

        getline(ss, est, '|');    
        getline(ss, mat, '|');    
        ss >> nota;                
        ss.ignore();               
        getline(ss, prof);         

        if (!est.empty() && est[0] == ' ') est.erase(0, 1);
        if (!mat.empty() && mat[0] == ' ') mat.erase(0, 1);
        if (!prof.empty() && prof[0] == ' ') prof.erase(0, 1);

        if (prof == username) {
            cout << "Estudiante: " << est << " | Materia: " << mat << " | Nota: " << nota << endl;
            hayNotas = true;
        }
    }
    if (!hayNotas) {
        cout << "No has registrado notas aun.\n";
    }
}

// Upadate (notas -> maestro)
void editarNota(const string& username) {
    string estudiante, materia;
    float nuevaNota;

    cin.ignore();
    cout << "Estudiante: ";
    getline(cin, estudiante);

    // Validar estudiante
    if (!validar_Texto(estudiante, 5, 40)) {
        cout << "Nombre invalido. Debe tener entre 5 y 40 caracteres.\n";
        return;
    }

    cout << "Materia: ";
    getline(cin, materia);

    // Validar materia
    if (!validar_Texto(materia, 3, 20)) {
        cout << "Materia invalida. Debe tener entre 3 y 20 caracteres.\n";
        return;
    }
    
     cout << "Nueva nota: ";

    // Validar nueva nota
    while (true) {
        if (cin >> nuevaNota) {
            if (nuevaNota >= 0 && nuevaNota <= 100) {
                break;
            }
            else {
                cout << "La nota debe estar entre 0 y 100. Intenta de nuevo: ";
            }
        }
        else {
            cout << "Entrada invalida. Ingresa numeros solamente: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    ifstream notasIn(NOTASFILE);
    ofstream notasOut("temp.txt");
    string linea;
    bool editado = false;

    while (getline(notasIn, linea)) {
        stringstream ss(linea);
        string est, mat, prof;
        float nota;

        getline(ss, est, '|');
        getline(ss, mat, '|');
        ss >> nota;
        ss.ignore();
        getline(ss, prof);

        if (est == estudiante && mat == materia && prof == username) {
            notasOut << est << "|" << mat << "|" << nuevaNota << "|" << prof << endl;
            editado = true;
        }
        else {
            notasOut << linea << endl;
        }
    }

    notasIn.close();
    notasOut.close();
    remove(NOTASFILE.c_str());
    rename("temp.txt", NOTASFILE.c_str());

    if (editado) registrarLog(username, "edito nota de " + estudiante + " en " + materia);
    cout << (editado ? "Nota editada.\n" : "No se encontro la nota para editar.\n");
}

// Delete (nota -> maestro)
void eliminarNota(const string& username) {
    string estudiante, materia;

    cin.ignore();
    cout << "Estudiante: ";
    getline(cin, estudiante);
    cout << "Materia: ";
    getline(cin, materia);

    ifstream notasIn(NOTASFILE);
    ofstream notasOut("temp.txt");
    string linea;
    bool eliminado = false;

    while (getline(notasIn, linea)) {
        stringstream ss(linea);
        string est, mat, prof;
        float nota;

        getline(ss, est, '|');
        getline(ss, mat, '|');
        ss >> nota;
        ss.ignore();
        getline(ss, prof);

        if (est == estudiante && mat == materia && prof == username) {
            eliminado = true;
            continue; 
        }
        notasOut << linea << endl;
    }

    notasIn.close();
    notasOut.close();
    remove(NOTASFILE.c_str());
    rename("temp.txt", NOTASFILE.c_str());

    if (eliminado) registrarLog(username, "elimino nota de " + estudiante + " en " + materia);
    cout << (eliminado ? "Nota eliminada.\n" : "No se encontro la nota para eliminar.\n");
}

// Read (usuarios -> administrador)
void verUsuarios() {
    ifstream MyFile(FILENAME);
    if (!MyFile.is_open()) {
        cout << "No se pudo abrir el archivo de usuarios.\n";
        registrarLog("administrador", "Error al abrir el archivo de usuarios.");
        return;
    }

    cout << "\n--- Lista de usuarios registrados ---\n";
    string linea;
    while (getline(MyFile, linea)) {
        stringstream ss(linea);
        string username, hashedPassword, tipo;
        string completeName;

        ss >> username >> hashedPassword >> tipo;
        getline(ss >> std::ws, completeName);

        cout << "Usuario: " << username << " | Tipo: " << tipo << " | Nombre completo: " << completeName << "\n";
    }

    MyFile.close();
}

// Read (log file -> administrador)
void verLogFile() {
    ifstream logFile(LOGFILE);
    if (!logFile.is_open()) {
        cout << "No se pudo abrir el archivo de log.\n";
        registrarLog("administrador", "Error al abrir el archivo log file.");
        return;
    }

    cout << "\n--- Registro de acciones ---\n";
    string linea;
    while (getline(logFile, linea)) {
        cout << linea << "\n";
    }

    logFile.close();
}
