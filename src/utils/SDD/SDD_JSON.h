#ifndef SDD_JSON_H
#define SDD_JSON_H


#include <SDD.h>
#include <tango.h>

//# JSON CPP
#include <json/json.h>

#include <string>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <iomanip>

#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <map>

namespace SDD_ns {

	class MoniPointBase;
	class SDD;
	
	class SDD_JSON : public SDD {

		public:
		
			SDD_JSON(std::string filename);
			~SDD_JSON();

		public: 
			std::string getJSON(bool isMinified=true);
			int parse(SDD_ns::MoniPoints& mpCollection);
		
		private:
			

		private:
			Json::Value fSDDRoot;
	};

}	//	End of namespace

#endif

