#include <iostream>
#include <string>
#include <sstream>
#include "asio.hpp"
#include "lexical_cast.hpp"
#include <boost/exception/diagnostic_information.hpp>

std::string portPath;
uint32_t baudRate = 9600;

char c;
std::string result;

static void showHelp(std::string commandName) {
	std::cerr << "Usage: " << commandName << " <option(s)>\n"
		<< "Options:\n"
		<< "\t-h,--help\t\tShow this help message\n"
		<< "\t-p,--port PORT PATH\tSpecify the port path, i.e. /dev/tty.usbmodem1234\n"
		<< "\t-b,--baudrate BAUDRATE\tSpecify the baudrate, i.e. 9600, 115200, etc.\n"
		<< "\t\t\t\tdefault is 9600\n"
		<< std::endl;
}

int main(int argc, char** argv) {

	(void) argc;

	if (argc < 2) {
		std::cerr << "You need to pass options to the command. Call ./main -h for help" << std::endl;
		return 1;
	}

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help")) {
			showHelp(argv[0]);
			return 0;
		}
		else if ((arg == "-p") || (arg == "--port")) {
			if (i + 1 < argc) {
				portPath = argv[i + 1];
			}
			else {
				std::cerr << "--port option requires one argument." << std::endl;
				return 1;
			}
		}
		else if ((arg == "-b") || (arg == "--baudrate")) {
			if (i + 1 < argc) {
				baudRate = boost::lexical_cast<uint32_t>(argv[i + 1]);
			}
		}
	}

	boost::asio::io_service io;
	boost::asio::serial_port serial(io, portPath);

	try {
		serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
	}
	catch(boost::system::system_error& e) {
		std::cout<<"Error: " << e.what()<<std::endl;
		std::cout<<"Info: "  << boost::diagnostic_information(e) <<std::endl;
		return 1;
	}

	std::cout<<"Hello World!\n"
		<<"Serial port: " << portPath
		<<"\nBaud rate: " << baudRate
		<<std::endl;

	while (true) {
		while (true) {
			boost::asio::read(serial, boost::asio::buffer(&c, 1));

			switch (c) {
				case '\r':
					break;
				case '\n':
					std::cout << result << std::endl;
					result = "";
					break;
				default:
					result += c;
			}

			break;
		}
	}


}
