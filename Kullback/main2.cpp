#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include "../Common/strprocess.hpp"

std::unordered_map<std::string, double> lang_phi_p {
	{"EN", 0.068},
	{"ES", 0.0775}
};
std::string lang;
const double PHI_R = 0.038; // Valor para texto completamente aleatorio

// Función para calcular la coincidencia observada en un texto dado
double calcular_phi_observada(const std::string &texto) {
	std::unordered_map<char, int> frecuencia;
	int N = 0;

	// Contar la frecuencia de cada letra
	for (char c : texto) {
		if (isalpha(c)) {
			char letra = toupper(c);
			frecuencia[letra]++;
			N++;
		}
	}

	// Calcular la coincidencia observada (phi_observada)
	double phi_observada = 0.0;
	for (const auto &entry : frecuencia) {
		int fi = entry.second;
		phi_observada += fi * (fi - 1);
	}
	phi_observada /= (N * (N - 1));

	return phi_observada;
}

// Función para dividir el texto en columnas según la longitud de clave
std::vector<std::string> dividir_en_columnas(const std::string &mensaje, int longitud_clave) {
	std::vector<std::string> columnas(longitud_clave);
	for (size_t i = 0; i < mensaje.size(); ++i) {
		if (isalpha(mensaje[i])) {
			columnas[i % longitud_clave] += toupper(mensaje[i]);
		}
	}
	return columnas;
}

// Función que aplica el método para determinar la longitud de clave probable
void determinar_longitud_clave(const std::string &mensaje, int longitud_maxima_clave) {
	std::cout << "Estimación de longitud de clave usando coincidencia teórica y observada:\n";
	
	for (int longitud_clave = 1; longitud_clave <= longitud_maxima_clave; ++longitud_clave) {
		auto columnas = dividir_en_columnas(mensaje, longitud_clave);

		// Calcular phi_observada promedio para las columnas
		double phi_observada_total = 0.0;
		int columnas_validas = 0;
		for (const auto &columna : columnas) {
			if (columna.size() > 1) { // Evitar divisiones por cero
				phi_observada_total += calcular_phi_observada(columna);
				columnas_validas++;
			}
		}
		double phi_observada_promedio = (columnas_validas > 0) ? (phi_observada_total / columnas_validas) : 0;

		// Comparación de phi_observada con phi_p y phi_r
		double diferencia_p = std::abs(phi_observada_promedio - lang_phi_p[lang]);
		double diferencia_r = std::abs(phi_observada_promedio - PHI_R);

		std::cout << "Key length " << longitud_clave << ":\tmean phi_obs = " << phi_observada_promedio
				  << ",\t|phi_obs - phi_p| = " << diferencia_p
				  << ",\t|phi_obs - phi_r| = " << diferencia_r << std::endl;
	}
}

int main(int argc, char **argv) {
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " [msg] [max_key_length] [lang]\n";
		return 1;
	}

	std::string mensaje = remove_delims(argv[1]);
	int longitud_maxima_clave = std::stoi(argv[2]); // Longitud máxima de clave a evaluar
	lang = remove_delims(argv[3]);

	// Aplicar el método de coincidencia
	determinar_longitud_clave(mensaje, longitud_maxima_clave);

	return 0;
}
