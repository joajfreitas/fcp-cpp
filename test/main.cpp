#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "fcp-cpp/fcp.hpp"


std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

int main(int argc, char *argv[])
{
	Fcp fcp;
	fcp.decompile_file("fst10e.json");
	FILE *fp = fopen(argv[1], "r");
	char buffer[1024];

	std::string delimiter = " ";

	float timestamp = 0;
	unsigned long sid = 0;
	unsigned long data = 0;
	char *data_str = (char *) malloc(1024*sizeof(char));
	char *bus = (char *) malloc(1024*sizeof(char));
	while (fgets(buffer, 1024, fp)) {
		sscanf(buffer, "(%f) %s %lx#%s\n", &timestamp, bus, &sid, data_str);
	
		std::string str = data_str;
		int dlc = str.size()/2;

		for (int i=0; i<8-dlc; i++) {
			str.append("00");
		}
		
		unsigned long data = std::stoul(str, NULL, 16);
		
		CANdata msg;
		msg.data[0] = (data >> 48) & 0xFFFF;
		msg.data[1] = (data >> 32) & 0xFFFF;
		msg.data[2] = (data >> 16) & 0xFFFF;
		msg.data[3] = (data >>  0) & 0xFFFF;

		msg.sid = sid;
		
		//std::cout << msg.sid << "#" << msg.data[0] << "," << msg.data[1] << "," << msg.data[2] << "," << msg.data[3] << std::endl;
		std::pair<std::string, std::map<std::string, double>> signals = fcp.decode_msg(msg);
		std::string name = signals.first;

		for (std::pair<std::string, double>  const& el: signals.second) {
			std::cout << timestamp << el.first << ": " << el.second << "\n";
		}	
	}

	return 0;
}
