#include "console.h"

int main(int argc, char* argv[]){
	Console _this_console;

	while(!_this_console.get_stop_flag()){
		std::cout << "> ";
		_this_console.parse_cmd_line(std::cin);
	}

	return 0;
}
