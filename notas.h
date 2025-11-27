#pragma once

#include <string>


void verNotasEstudiante(const std::string& completeName, const std::string& username);

void verNotasMaestro(const std::string& username);

void crearNota(const std::string& maestro);

void editarNota(const std::string& maestro);

void eliminarNota(const std::string& maestro);

void verUsuarios();

void verLogFile();
