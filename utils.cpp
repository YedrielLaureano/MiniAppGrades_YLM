#include "utils.h"
#include "notas.h"
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
//#include <functional>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>

using namespace std;

const string FILENAME = "usuarios.txt";
const string NOTASFILE = "notas.txt";
const string LOGFILE = "logfile.txt";

/*string hashContrasena(const string& password) {
    hash<string> hasher;
    size_t hashed = hasher(password);
    return to_string(hashed);
}*/

//*************************************************************************************************************************
// --- Implementacion SHA-1 (funcion sha1) ---
static inline uint32_t rol(uint32_t value, unsigned int bits) {
    return (value << bits) | (value >> (32 - bits));
}

string sha1(const string& input) {
    // Inicializacion de constantes
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    // Preprocesamiento: padding
    vector<uint8_t> bytes(input.begin(), input.end());
    uint64_t originalBitsLen = static_cast<uint64_t>(bytes.size()) * 8ULL;

    // append '1' bit then pad with zeros until length ≡ 448 (mod 512)
    bytes.push_back(0x80);
    while ((bytes.size() % 64) != 56) {
        bytes.push_back(0x00);
    }

    // append original length in bits as 64-bit big-endian
    for (int i = 7; i >= 0; --i) {
        bytes.push_back(static_cast<uint8_t>((originalBitsLen >> (8 * i)) & 0xFF));
    }

    // Procesar en bloques de 512 bits (64 bytes)
    for (size_t offset = 0; offset < bytes.size(); offset += 64) {
        uint32_t w[80];

        // llenar w[0..15] con el bloque
        for (int t = 0; t < 16; ++t) {
            size_t i = offset + t * 4;
            w[t] = (static_cast<uint32_t>(bytes[i]) << 24)
                | (static_cast<uint32_t>(bytes[i + 1]) << 16)
                | (static_cast<uint32_t>(bytes[i + 2]) << 8)
                | (static_cast<uint32_t>(bytes[i + 3]));
        }

        // extender w[16..79]
        for (int t = 16; t < 80; ++t) {
            w[t] = rol(w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16], 1);
        }

        // Inicializar variables temporales
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;

        for (int t = 0; t < 80; ++t) {
            uint32_t f, k;
            if (t < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if (t < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (t < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            uint32_t temp = rol(a, 5) + f + e + k + w[t];
            e = d;
            d = c;
            c = rol(b, 30);
            b = a;
            a = temp;
        }

        // Anadir este bloque a los hash intermedios
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    // Convertir a hex string (minusculas, 2 caracteres por byte)
    ostringstream oss;
    oss << hex << setfill('0') << nouppercase;
    oss << setw(8) << (h0)
        << setw(8) << (h1)
        << setw(8) << (h2)
        << setw(8) << (h3)
        << setw(8) << (h4);

    return oss.str();
}
//*************************************************************************************************************************

// Convertir password en cifrado (SHA1)
string SHA1(const string& password) {
    return sha1(password);
}

string obtenerFechaHora() {
    time_t ahora = time(nullptr);
    tm tiempoLocal;
    localtime_s(&tiempoLocal, &ahora);

    // Year-Month-Day Hour:Minute:Secund
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tiempoLocal);
    return string(buffer);
}

// Registra accion en logfile.txt 
void registrarLog(const string& usuario, const string& accion) {
    ofstream log(LOGFILE, ios::app);
    if (!log) {
        cerr << "Error al abrir logfile.txt\n";
        return;
    }
    // Fecha | usuario | accion
    log << obtenerFechaHora() << " | " << usuario << " | " << accion << endl;
    log.close();
}

// Verifica si el usuario ya existe
bool usuarioExiste(const string& username) {
    ifstream MyFile(FILENAME);
    string linea;
    while (getline(MyFile, linea)) {
        stringstream ss(linea);
        string u, p, t;
        ss >> u >> p >> t;
        if (u == username) return true;
    }
    return false;
}
