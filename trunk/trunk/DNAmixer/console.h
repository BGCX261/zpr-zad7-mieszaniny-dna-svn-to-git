/**
 * Autor : Tomasz Lechowicz
 *
 * Interfejs uzytkownika w formie konsoli
 */
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <exception>
#include <thread_manager.h>
#include <vector>
#include <map>
#include <utility>

using namespace boost::program_options;

/**
 * Klasa wyjatkow do obslugi nieprawidlowo podanych danych wejsciowych
 */
class BadMixException: std::exception{};

/**
 * Klasa kontenera przechowujacego informacje o wykonanych obliczeniach
 */
class ProcessContainer{
public:
	ProcessContainer();
	ProcessContainer(long pid, std::string name, int pnumber);

	long get_pid();
	
	/**
	 * Metoda wypisujaca na standardowe wyjscie informacje o watku obliczeniowym
	 */
	void show_info();

private:
	long _pid;
	std::string _name;
	int _pnumber;
};

/**
 * Klasa emulujaca konsole, wykorzystujaca biblioteke boost::program_options
 */
class Console{
public:
	/**
	 * Konstruktor konsoli, zapisujacy informacje o dozwolonych komendach
	 */
	Console();

	/**
	 * Metoda parsujaca strumien wejsciowy i wywolujaca podane komendy
	 */
	void parse_cmd_line(std::istream& in);

	/**
	 * Metoda zwracajaca informacje czy uzytkownik wywolal komende zamykajaca program
	 */
	bool get_stop_flag();

private:
	variables_map _vm;
	options_description _desc;
	bool _stop_flag;
	TMaster _tmaster;
	long _id;
	std::vector<ProcessContainer> _proc_vector;

	/**
	 * Metoda bezpoœrednio wywolujaca wszystkie komendy uzytkownika
	 */
	void execute();

	/**
	 * Metoda przygotowujaca podana mieszanine do obrobki algorytmicznej
	 */
	std::vector< std::pair<std::string, char> > split_mix(std::string mix);

	/**
	 * Czyszczenie wczesniejszych komend uzytkownika
	 */
	void clear_vm();
};

#endif
