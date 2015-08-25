#include <thread_manager.h>

using namespace boost;
using namespace faif::actobj;

typedef std::vector< std::pair<std::string, char> > PersonParam;

/////////////////////	DummyCmd	/////////////////////////

	/**
	 * Konstruktor dla komendy, ktora oblicza wszystkie mozliwe mieszanki
	 *  s - mieszanka
	 *  pn - ilosc osob
	 */ 
	DummyCmd::DummyCmd(PersonParam s, int pn): _person_number(pn), _param(s), _person_param(PersonParam()){}
	/**
	 * Konstruktor dla komendy, ktora oblicza mozliwe mieszanki dla podanego profilu osoby
	 *  s - mieszanka
	 *  pn - ilosc osob
	 *  pp - profil osoby do ktorej maja zostac dopasowane pozostale
	 */ 
    DummyCmd::DummyCmd(PersonParam s, int pn, PersonParam pp): _person_number(pn), _param(s), _person_param(pp){}

    /**
	 * Przeciazony operator wykonujacy obliczenia dla wszystkich markerow
	 */ 	
	void DummyCmd::operator()(Progress& progress){
		//Rozpocznij obliczenia
		progress.setProgress(0);
		
		//Petla wykonujaca obliczenia dla kazdego markera oddzielnie
		for(PersonParam::iterator it=_param.begin(); it!=_param.end(); ++it){
			std::string _tmp_person_param("");

			if(!_person_param.empty()){
				//Wyszukaj czy dla danego markera zostal podany profil osoby
				for(PersonParam::iterator it_pp=_person_param.begin(); it_pp!=_person_param.end(); ++it_pp){
					if((*it_pp).second == (*it).second)
						_tmp_person_param = (*it_pp).first;
				}
			}
			try{
				//Wykonaj obliczenia dla markera
				_result.push_back(analyzer::MarkerAnalyzer( (*it).first, _person_number).getProfile(_tmp_person_param) );
			} catch (analyzer::AnalyzerException& ae) {
				//Blad w definicji parametrow obliczen
				// mozliwe bledy
				// - zla definicja mieszaniny
				// - zla ilosc osob
				// - nieprawidlowy profil osoby
				std::cout << ae << std::endl;
			}
		}

		//Obliczenia zakonczone
		progress.setProgress(1);
    }

	/**
	 * Wypisz na ekran wynik obliczen
	 */ 
	void DummyCmd::show_result(){
		//Pobierz marker
		for(int i=0; i<_param.size(); ++i){
			std::cout << "Warianty dla " << _person_number << " osob, dla markera " << _param[i].second << std::endl;
			if(!_person_param.empty()){
				//Wyszukaj czy dla danego markera zostal podany profil osoby
				for(PersonParam::iterator it_pp=_person_param.begin(); it_pp!=_person_param.end(); ++it_pp){
					if((*it_pp).second == _param[i].second)
						std::cout << "Osoba znana : " << _param[i].second << (*it_pp).first[0] << _param[i].second << (*it_pp).first[1] << std::endl;
				}
			}
			//Wypisz wszystkie obliczone kombinacje po kolei dla danego markera
			for(std::vector<std::string>::iterator it=_result[i].begin(); it!=_result[i].end(); ++it){
				for(int j=0; j<(*it).size(); ++j){
					std::cout << _param[i].second << (*it)[j];
					std::cout << _param[i].second << (*it)[++j] << " ";
				}
				std::cout << std::endl;
			}
		}
	}

/////////////////////	TMaster		/////////////////////////

	/**
	 * Metoda puszczajaca watek obliczeniowy dla wszystkich mozliwych kombinacji osob w mieszance
	 *  _cid - identyfikator procesu
	 *  mix_param - mieszanina
	 *  per_num - ilosc osob
	 */ 
	void TMaster::run_asyn_tslave(CommandID _cid, PersonParam mix_param, int per_num){
		PCommand cmd(new DummyCmd(mix_param, per_num));
		_scheduler.executeAsynchronously( cmd );
		this->_cmd_hist.insert(_cid, cmd);
	}

	/**
	 * Metoda puszczajaca watek obliczeniowy dla wszystkich mozliwych kombinacji osob w mieszance pasujacych do danej osoby
	 *  _cid - identyfikator procesu
	 *  mix_param - mieszanina
	 *  per_num - ilosc osob
	 *  person_param - profil osoby
	 */ 
	void TMaster::run_asyn_tslave(CommandID _cid, PersonParam mix_param, int per_num, PersonParam person_param){
		PCommand cmd(new DummyCmd(mix_param, per_num, person_param));
		_scheduler.executeAsynchronously( cmd );
		this->_cmd_hist.insert(_cid, cmd);
	}

	/**
	 * Sprawdzanie zaawansowania obliczen
	 */ 
	void TMaster::check_progress(CommandID _cid){
		this->show_progress( this->_cmd_hist.find(_cid) );
	}

	/**
	 * Wypisywanie wynikow obliczen na ekran
	 *  _cid - identyfikator komendy
	 */ 
	void TMaster::cmd_result(CommandID _cid){
		try {
			//Sprawdz czy komenda istnieje i wywolaj metode wypisujaca wynik
			if(this->_cmd_hist.find(_cid)->getDescriptor().progress_ == 1)
				dynamic_cast<DummyCmd*>(&(*this->_cmd_hist.find(_cid)))->show_result();
		} catch(std::exception& e) {
			std::cout << "Nie ma takiego procesu!" << std::endl;
		}
	}

	/**
	 * Wypisywanie stanu obliczen na ekran
	 *  _cmd - wskaznik na komende, ktorej stan ma zostac wypisany
	 */ 
	void TMaster::show_progress(PCommand _cmd){
		if(_cmd->getDescriptor().progress_)
			std::cout << "wykonane" << std::endl;
		else
			std::cout << "w trakcie obliczen" << std::endl;
	}
