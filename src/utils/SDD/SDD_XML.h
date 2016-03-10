#ifndef SDD_XML_H
#define SDD_XML_H

#include <MoniPoint.h>

#include <SDD.h>
#include <tango.h>

//# XML parser
#include <pugixml.h>

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
	
	class SDD_XML : public SDD {

		public:
		        
    	SDD_XML(std::string filename);
			~SDD_XML();

		public: 
			int parse(SDD_ns::MoniPoints& mpCollection,bool filter=false,std::string component_filter="",int componentId_filter=1);
		
		private:
			int readFile();

		private:
			pugi::xml_document fDoc;
			
	};

}	//	End of namespace

#endif

