/**
 * Autor: Lukasz Danko, N1-IBERE2
 * 
 * Klasa wyznacza profile osób, dla zadanego profilu mieszaniny markerów
 */

#ifndef __MARKER_ANALYZER_H__
#define __MARKER_ANALYZER_H__

#include <iostream>
#include <string>
#include <vector>
#include <boost/regex.hpp>

/**
 * Biblioteka kombinatoryczna (next_combination_counts)
 * Dostepna pod adresem:
 *				http://photon.poly.edu/~hbr/boost/combinations.html
 */
#include "combination.hpp"
#include "AnalyzerExceptions.h"

using namespace std;

namespace analyzer {

	class MarkerAnalyzer {
	public:
		/**
		 * Konstruktor,
		 *		marker	-	profil mieszaniny
		 *		persons	-	liczba osób
		 */
		MarkerAnalyzer(const string &marker, int persons);
		virtual ~MarkerAnalyzer(void) {}
		
		/**
		 * Wyznacza profile osob
		 *		person - profil danej osoby
		 */
		vector<std::string> getProfile(const string &person = "") const;
		/**
		 * Wyznacza profile osob, dodatkowe formatowanie wyjscia
		 *		person - profil danej osoby
		 */
		vector<std::string> getFormattedProfile(const string &person = "") const;

	private:
		/**
		 * Liczba wariantow dla danego markera dla pojedynczej osoby
		 */
		static const int VARIANT_COUNT = 2;

		/**
		 * Minimalna ilosc osob w mieszaninie
		 */

		static const int MIN_PERSONS = 2;
		/**
		 * Maksymalna ilosc osob w mieszaninie
		 */
		static const int MAX_PERSONS = 3;

		string _marker;
		boost::regex _pattern;
		int _persons;

		/**
		 * wyznacza wszystkie kombinacja wariantow danego markera
		 */
		vector<string> variantsCombinations() const;

		/**
		 * na podstawie kombinacji wariantów, wyznacza wszystkie kombinacje osob
		 *		vc	- kombinacje wariantow
		 */
		vector<string> personsCombinations(const vector<string> &vc) const;

		/**
		 * Wycina dane znanej osoby z wynikow
		 *		pc			- kombinacja profili
		 *		person	- profil danej osoby
		 */
		vector<string> extractPerson(const vector<string> &pc, const string &person) const;
	};

} // namespace analyzer

#endif //__MARKER_ANALYZER_H__
