/**
 * Autor: Lukasz Danko, N1-IBERE2
 * 
 * Definicje metod wyjatkow
 */

#include "AnalyzerExceptions.h"

using namespace std;

namespace analyzer {

	ostream& AnalyzerException::print(ostream& os) const throw() {
		os << what() << endl;
		return os;
	}

	BadProfileException::BadProfileException(const string &profile) : _profile(profile) {};

	ostream& BadProfileException::print(ostream& os) const throw() {
		os << "Bad known people profile: " << _profile << endl;
		return os;
	}

	BadPeoplesCountException::BadPeoplesCountException(int peoples) : _peoples(peoples) {};

	ostream& BadPeoplesCountException::print(ostream& os) const throw() {
		os << "Bad peoples count: " << _peoples << endl;
		return os;
	}

	BadMarkerException::BadMarkerException(const string &marker) : _marker(marker) {};

	ostream& BadMarkerException::print(ostream& os) const throw() {
		os << "Bad marker mixture: " << _marker << endl;
		return os;
	}

 	std::ostream& operator<<(std::ostream& os, const AnalyzerException& ex) {
		ex.print(os);
		return os;
	}

} // namespace analyzer