#include <console.h>

using namespace boost::program_options;

/////////////////////	ProcessContainer	/////////////////////////

	/**
	 * Konstruktor pustego procesu
	 */
	ProcessContainer::ProcessContainer(): _pid(0), _name(""), _pnumber(0){}
	/**
	 * Konstruktor procesu
	 *  pid - identyfikator watku obliczeniowego
	 *  name - nazwa watku (mieszanina dla ktorej prowadzone sa obliczenia
	 *  pnumber - ilosc osob dla ktorych prowadzone sa obliczenia
	 */
	ProcessContainer::ProcessContainer(long pid, std::string name, int pnumber): _pid(pid), _name(name), _pnumber(pnumber){}

	/**
	 * Metoda podajaca identyfikator procesu zapisanego w obiekcie
	 */
	long ProcessContainer::get_pid(){
		return this->_pid;
	}
	
	/**
	 * Wypisywanie na ekran informacji o procesie zapisanym w obiekcie
	 */
	void ProcessContainer::show_info(){
		std::cout << "ID procesu : " << this->_pid  << "\n\tmikstura: " << this->_name << "\n\tilosc osob: " << this->_pnumber << "\n\tstan: ";
	}

/////////////////////		Console			/////////////////////////

	/**
	 * Konstruktor konsoli
	 */
	Console::Console(): _desc(options_description("Dostêpne opcje")), _stop_flag(false), _id(1){
		//Dodaj wszystkie dostepne komendy konsoli
		this->_desc.add_options()
			("help", "opis dostepnych opcji programu")
			("exit", "bezpiecznie konczy program")
			("mix", value<std::vector<std::string> >(), "wypisuje mo¿liwe mieszaniny dla podanej grupy osob")
			("ls", "wypisz wszystkie dostepne procesy")
			("show", value<int>()->default_value(0), "wypisz na ekran wynik podanego procesu")
		;

		//Wyrysuj logo programu
		std::cout << "`-:-.   ,-;\"`-:-.   ,-;\"`-:-.   ,-;\"`-:-.   ,-;\"" << std::endl;
		std::cout << "   `=`,'=/     `=`,'=/     `=`,'=/     `=`,'=/      " << std::endl;
		std::cout << "     y==/        y==/        y==/        y==/       " << std::endl;
		std::cout << "   ,=,-<=`.    ,=,-<=`.    ,=,-<=`.    ,=,-<=`.     " << std::endl;
		std::cout << ",-'-'   `-=_,-'-'   `-=_,-'-'   `-=_,-'-'   `-=_    " << std::endl;
		std::cout << std::endl << "\t\t\tDNA MIXER" << std::endl << "\tTomasz Lechowicz & Lukasz Danko presents" << std::endl;
	}

	/**
	 * Metoda odczytujaca z wejscia komende i wywolujaca odpowiednie funkcje
	 */
	void Console::parse_cmd_line(std::istream& in){
		std::string sin;
		char buffer[255];
		char** av;
		int ac;

		//Pobierz wiersz
		in.getline(buffer, 255);
		sin = buffer;

		//Podziel wiersz, gdzie znakiem podzia³u pomiêdzy atomami leksykalnymi s¹ spacje
		std::vector<std::string> SplitVector;
		boost::split(SplitVector, sin, boost::algorithm::is_space());

		//Przygotuj zmienne pomocnicze emulujace parametry wywolania programu (argc i argv**)
		if(SplitVector.size() == 1)
			//Jesli komenda bezparametrowa
			ac = 2;
		else
			//Jesli komenda posiada parametry wywolania
			ac = 1+SplitVector.size()+SplitVector.size()-2;

		av = new char*[ac];

		av[0] = "DNAmixer";
		SplitVector[0] = "--" + SplitVector[0];

		//Wpisz do tablicy "argv**" kolejne parametry komendy
		//Wielokrotne wpisanie samej nazwy komendy (SplitVector[0].data()) wymagane
		// jest przez biblioteke boost::program_options
		for(int i(1), j(1); i<ac; ++i){
			av[i++] = const_cast<char*>(SplitVector[0].data());
			if(ac>2)
				av[i] = const_cast<char*>(SplitVector[j++].data());
		}
		try{
			//Wpisz komende wraz z parametrami do obiektu przechowujacego wszelkie
			// informacje o wywolywanych parametrach
			store(parse_command_line(ac, av, this->_desc), this->_vm);
			notify(this->_vm);
		} catch (std::exception& e){
			//Cos zostalo wpisane zle
			std::cout << "Wpisales nieznana komende lub podales nieprawidlowe parametry. Wpisz komende help aby otrzymac liste wszystkich dostepnych komend" << std::endl;
		}

		//Wykonaj wpisana komende
		this->execute();
	}

	bool Console::get_stop_flag(){
		return this->_stop_flag;
	}

	void Console::execute(){
		//Wypisywanie na ekran dostepnych komendy i sposobu ich wywolania
		if(this->_vm.count("help")){

			std::cout << std::endl << "help - opis dostepnych opcji programu" << std::endl;
			std::cout << std::endl << "exit - bezpiecznie konczy program" << std::endl;
			std::cout << std::endl << "mix - wypisuje mozliwe mieszaniny dla podanej grupy osob" << std::endl;
			std::cout << "\tParametry:" << std::endl;
			std::cout << "\t 1 - mieszanina dla ktorej maja zostac przeprowadzone obliczenia (wymagany)" << std::endl;
			std::cout << "\t\t np A1A2B1B2C3 (musza byc duze litery)" << std::endl;
			std::cout << "\t 2 - liczba osob (wymagany, dopuszczalne wartosci 2 oraz 3)" << std::endl;
			std::cout << "\t 3 - mieszanina osoby do ktorej maja zostac dopasowane pozostale osoby (opcjonalny)" << std::endl;
			std::cout << "\t\t np A1A1B1B2 (musza byc duze litery, kazdy marker musi wystepowac dwa razy)" << std::endl;
			std::cout << std::endl << "ls - wypisz wszystkie dostepne procesy" << std::endl;
			std::cout << std::endl << "show - wypisz na ekran wynik podanego procesu" << std::endl;
			this->clear_vm();

		//Wyjscie z programu
		} else if(this->_vm.count("exit")){

			this->_stop_flag = true;
			this->clear_vm();
			return;

		//Utworzenie watku obliczeniowego
		}  else if(this->_vm.count("mix")){

			std::cout << "Mixing" << std::endl;
			try{
				//Obliczanie wszystkich mozliwych kombinacji dla mieszaniny
				if(this->_vm["mix"].as<std::vector<std::string> >().size() == 2){
					this->_tmaster.run_asyn_tslave(this->_id, this->split_mix(this->_vm["mix"].as<std::vector<std::string> >()[0]), boost::lexical_cast<int>(this->_vm["mix"].as<std::vector<std::string> >()[1]) );
					this->_proc_vector.push_back(ProcessContainer(this->_id++, this->_vm["mix"].as<std::vector<std::string> >()[0], boost::lexical_cast<int>(this->_vm["mix"].as<std::vector<std::string> >()[1])));
						
				//Dopasowanie osob do podanej osoby dla danej mieszanieny
				} else if(this->_vm["mix"].as<std::vector<std::string> >().size() == 3) {
					this->_tmaster.run_asyn_tslave(this->_id, this->split_mix(this->_vm["mix"].as<std::vector<std::string> >()[0]), boost::lexical_cast<int>(this->_vm["mix"].as<std::vector<std::string> >()[1]), this->split_mix(this->_vm["mix"].as<std::vector<std::string> >()[2]) );
					this->_proc_vector.push_back(ProcessContainer(this->_id++, this->_vm["mix"].as<std::vector<std::string> >()[0], boost::lexical_cast<int>(this->_vm["mix"].as<std::vector<std::string> >()[1])));

				//Zle sformulowana mieszaniena albo za mala liczba parametrow
				} else {
					throw BadMixException();	
				}

				this->clear_vm();
			} catch (BadMixException& e){
				std::cout << "Zle sformulowana mikstura" << std::endl;
				this->clear_vm();
			} catch (analyzer::AnalyzerException& ae){
				std::cout << ae << std::endl;
				this->clear_vm();
			}

		//Wypisz wszystkie watki obliczeniowe ktore wywolal uzytkownik
		} else if(this->_vm.count("ls")){
			for(std::vector<ProcessContainer>::iterator it=_proc_vector.begin(); it != _proc_vector.end(); ++it){
				(*it).show_info();
				this->_tmaster.check_progress((*it).get_pid());
			}
			this->clear_vm();

		//Wypisz na ekran wyniki obliczen danego watku
		} else if(this->_vm.count("show")){
			if(this->_vm["show"].as<int>() == 0){
			//Podany zostal identyfikator watku
			} else {
				bool _pid_exists(false);
				//Sprawdzanie czy taki watek istnieje
				for(std::vector<ProcessContainer>::iterator it=_proc_vector.begin(); it!=_proc_vector.end(); ++it){
					if((*it).get_pid() == this->_vm["show"].as<int>())
						_pid_exists=true;
				}
				if(_pid_exists)	
					//wypisz wynik dla podanego watku
					this->_tmaster.cmd_result(this->_vm["show"].as<int>());
				else
					std::cout << "Podanego procesu nie ma na liscie, wpisz komende ls aby wypisac wszystkie dostepne procesy" << std::endl;
			}
			this->clear_vm();
		}
	}

	/**
	 * Metoda przygotowujaca podana miksture do obliczen
	 *  mix - mikstura
	 */
	std::vector< std::pair<std::string, char> > Console::split_mix(std::string mix){
		std::vector< std::pair<std::string, char> > return_vector;
		std::map<char, std::string> marker_map;

		boost::regex proper_exp("([A-Z][0-9])*");

		//Sprawdz czy mikstura zostala poprawnie sformulowana
		if(boost::regex_match(mix, proper_exp)){
			//Dodaj cyfre do markera
			for(std::string::iterator it=mix.begin(); it < mix.end(); ++it){
				marker_map[*(it++)] += (*(it+1));
			}

			//Przygotuj wektor wyjsciowy
			for(std::map<char, std::string>::iterator it=marker_map.begin(); it != marker_map.end(); ++it){
				return_vector.push_back( std::make_pair( (*it).second,(*it).first ) );
			}
		
			return return_vector;

		} else {
			//Mikstura niepoprawnie sformulowana
			throw BadMixException();
		}	
	}

	/**
	 * Metoda czyszczaca historie wywolywanych komend
	 * Wymagana aby w program nie wywolywal podanej komendy za kazdym razem
	 *  nawet jesli wpisaloby sie inna komende
	 */
	void Console::clear_vm(){
		this->_vm = variables_map();
	}
