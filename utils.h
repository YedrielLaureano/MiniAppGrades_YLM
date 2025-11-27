#pragma once

#include <string>

extern const std::string FILENAME;
extern const std::string NOTASFILE;
extern const std::string LOGFILE;

std::string SHA1(const std::string& password);

std::string obtenerFechaHora();

void registrarLog(const std::string& usuario, const std::string& accion);

bool usuarioExiste(const std::string& username);
