/**
 * Autor: Lukasz Danko, N1-IBERE2
 * 
 * Wyj¹tki zglaszane w klasie MarkerAnalyzer
 */

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <exception>
#include <string>
#include <iostream>

using namespace std;

namespace analyzer {

	/**
	 * Klasa bazowa dla wyjatkow rzucanych przez MarkerAnalyzer
	 */
	class AnalyzerException : public exception {
	public:
		AnalyzerException() {}
		virtual ~AnalyzerException() throw() {}

		virtual const char *what() const throw() {return "AnalyzerException";}
		virtual ostream& print(ostream& os) const throw();
	};

	/**
	 * Bledny profil danej osoby
	 */
	class BadProfileException : public AnalyzerException {
	public:
		BadProfileException(const string &profile);
		virtual ~BadProfileException() throw() {}

		virtual const char *what() const throw() {return "BadProfileException";}
		virtual ostream& print(ostream& os) const throw();
	private:
		string _profile;
	};

	/**
	 * Nieprawidlowa liczba osob w mieszaninie
	 */
	class BadPeoplesCountException : public AnalyzerException {
	public:
		BadPeoplesCountException(int peoples);
		virtual ~BadPeoplesCountException() throw() {}

		virtual const char *what() const throw() {return "BadPeoplesCountException";}
		virtual ostream& print(ostream& os) const throw();
	private:
		int _peoples;
	};

	/**
	 * Nieprawidlowa mieszanina
	 */
	class BadMarkerException : public AnalyzerException {
	public:
		BadMarkerException(const string &marker);
		~BadMarkerException() throw() {}

		virtual const char *what() const throw() {return "BadMarkerException";}
		virtual ostream& print(ostream& os) const throw();
	private:
		string _marker;
	};

	ostream& operator<<(ostream& os, const AnalyzerException& ex);

} // namespace analyzer

#endif // __EXCEPTIONS_H__