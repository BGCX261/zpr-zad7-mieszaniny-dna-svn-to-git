/**
 * Autor: Lukasz Danko, N1-IBERE2
 * 
 * Definicje metod klasy MerkerAnalyzer
 */

#include "MarkerAnalyzer.h"

using namespace std;
using namespace boost;

namespace analyzer {

	MarkerAnalyzer::MarkerAnalyzer(const string &marker, int persons) : _marker(marker), _persons(persons) {
		if(persons < MIN_PERSONS || persons > MAX_PERSONS)
			throw  BadPeoplesCountException(persons);
		// szukaj nie cyfr
		regex e1("[^1-9]");
		// szukaj powtarzaajacych sie markerow
		regex e2("([1-9]).*\\1");
		if(marker.length() > (unsigned)VARIANT_COUNT*persons || regex_search(marker, e1) || regex_search(marker, e2))
			throw BadMarkerException(marker);
		//if(regex_match(marker, regex("[1-9]{2}")))
		string res = "";
		size_t i;
		// sk³ada wyra¿enie: /(?=.marker[i].*)/, dla kolejnych wariantów
		for(i = 0; i < _marker.size() - 1; i++) {
			res += "(?=.*"; res += _marker[i]; res += ".*)";
		}
		res += ".*"; res += _marker[i]; res += ".*";
		_pattern = regex(res);	
	}

	vector<string> MarkerAnalyzer::getProfile(const string &person) const {
		vector<string> vc = variantsCombinations();
		vector<string> pc = personsCombinations(vc);
		if(person.length()) {
			return extractPerson(pc, person);
		}
		return pc;
	}

	vector<string> MarkerAnalyzer::getFormattedProfile(const string &person) const {
		vector<string> profile = getProfile(person);
		vector<string> result;
		string tmp;
		for(size_t i = 0; i < profile.size(); i++) {
			tmp = "";	// obudowuje wynik w nawiasy, rozdzielaj¹ce kolejne osoby (np. [12][34])
			for(size_t j = 0; j < profile[i].size(); j++) {
				tmp += (j%2?"":"["); tmp += profile[i][j]; tmp += (j%2?"]":"");
			}
			result.push_back(tmp);
		}
		return result;
	}

	vector<string> MarkerAnalyzer::variantsCombinations() const {
		vector<int> v(_marker.size(), 0);
		v.back() = VARIANT_COUNT;
		string var;
		vector<string> result;
		do {
			var = "";
			for(size_t i = 0; i < v.size(); i++) {
				for(int j = 0; j < v[i]; j++) {
					var += _marker[i];
				}
			}
			result.push_back(var);
		} while (next_combination_counts(v.begin(), v.end()));
		return result;
	}

	vector<string> MarkerAnalyzer::personsCombinations(const vector<string> &vc) const {
		vector<int> v(vc.size(), 0);
		v.back() = _persons;
		string per;
		vector<string> result;
		do {
			per = "";
			for(size_t i = 0; i < v.size(); i++)
				for(int j = 0; j < v[i]; j++)
					per += vc[i];
			if(regex_match(per, _pattern))
				result.push_back(per);
		} while (next_combination_counts(v.begin(), v.end()));
		return result;
	}

	vector<string> MarkerAnalyzer::extractPerson(const vector<string> &pc, const string &person) const {
		if(!regex_match(person, regex("[1-9]{2}")))
			throw BadProfileException(person);
		vector<string> result;
		regex e("((..)*)" + person + "((..)*)");
		smatch what;
		for(size_t i = 0; i < pc.size(); i++) {
			if(regex_match(pc[i], what, e)) {
				result.push_back(what[1] + what[3]);
			}
		}
		return result;
	}

} // namespace analyzer
