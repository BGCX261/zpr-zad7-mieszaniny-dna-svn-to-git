/**
 * Autor : Tomasz Lechowicz
 *
 * Obsluga wielowatkowych obliczen, oparta na aktywnych obiektach z biblioteki faif
 */
#ifndef __THREAD_MANAGER_H__
#define __THREAD_MANAGER_H__

#include <boost/thread/thread.hpp>
#include <faif/utils/actobj/Scheduler.h>
#include <faif/utils/actobj/CommandHistory.h>
#include <faif/utils/actobj/OstreamCommandObserver.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include <MarkerAnalyzer.h>

using namespace boost;
using namespace faif::actobj;

/**
 * Wzorzec projektowy komendy, oparty na aktywnym obiekcie z biblioteki faif
 */

typedef std::vector< std::pair<std::string, char> > PersonParam;

class DummyCmd : public Command {
public:
	/**
	 * Konstruktur
	 *		_person_number - dla ilu osob maja zostac wykonane obliczenia
	 *		_param - mieszanka markerow
	 *		_person_param - mieszanka markerow jednej konretnej osoby, dla ktorej
	 *			beda obliczane osoby spelniajace mieszanke
	 */	
	DummyCmd(){}
	DummyCmd(PersonParam s, int pn);
    DummyCmd(PersonParam s, int pn, PersonParam pp);
	virtual ~DummyCmd() {};

    /**
	 * Operator() wywoluje obliczenia dla poszczegolnych markerow, wykorzystuje obiekt MarkerAnalyzer
	 */
	virtual void operator()(Progress& progress);

	/**
	 * Metoda wypisujaca na ekran wynik obliczen dla danej mieszanki i danej osoby
	 */
	void show_result();

private:
	int _person_number;
	PersonParam _param;
	PersonParam _person_param;
	std::vector< std::vector<std::string> > _result;
};

/**
 * Klasa obslugujaca i zarzadzajaca watkami obliczeniowymi programu
 */

class TMaster {
public:
	/**
	 * Konstruktor klasy pobiera obiekt klasy Scheduler z biblioteki faif bedacy singletonem
	 */
	TMaster():_scheduler(Scheduler::getInstance()) {}
	/**
	 * Destruktor zapewniajacy przerwanie wszystkich bierzacych watkow w razie zakonczenia programu
	 */
	~TMaster(){
		this->_scheduler.finishAll();
	}

	/**
	 * Metoda wywolujaca obliczenia w oddzielnym watku w przypadku obliczenia wszystkich mozliwosci
	 * dla danej mieszanki
	 */
	void run_asyn_tslave(CommandID _cid, PersonParam mix_param, int per_num);
	
	/**
	 * Przeciazona metoda wywolujaca obliczenia w oddzielnym watku w przypadku obliczenia mieszanki
	 * dla konkretnej osoby
	 */
	void run_asyn_tslave(CommandID _cid, PersonParam mix_param, int per_num, PersonParam person_param);

	void check_progress(CommandID _cid);

	/**
	 * Metoda wypisujaca wynik obliczen
	 */
	void cmd_result(CommandID _cid);

private:
	Scheduler& _scheduler;
	CommandHistory _cmd_hist;

	/**
	 * Metoda wypisujaca na standardowe wyjscie informacje o stanie obliczen
	 * todo: wprowadzic procentowe okreslanie zaawansowania obliczen
	 */
	void show_progress(PCommand _cmd);
};

#endif
