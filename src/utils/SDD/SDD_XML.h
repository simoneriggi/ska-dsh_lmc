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
		        
			SDD_XML();
    	SDD_XML(std::string filename);
			~SDD_XML();

		public: 
			int ReadFromFile(pugi::xml_document& doc,std::string& filename);
			int ReadFromString(pugi::xml_document& doc,std::string& filename);
			int Parse(std::vector<SDD_ns::DeviceAttr*>& attrCollection,std::string& input,bool readFromFile);
			//int Parse(std::vector<Tango::Attribute*>& attrCollection,std::string& input,bool readFromFile,std::string dev_name);

			int parse(SDD_ns::MoniPoints& mpCollection,bool filter=false,std::string component_filter="",int componentId_filter=1);
		

		private:
			int readFile();

			DeviceAttr* ParseAttrNode(pugi::xml_node& node);
			//Tango::Attribute* ParseAttrNode(pugi::xml_node& node,std::string dev_name);
			
			template<typename T>
				int ParseMultiAttrProp(Tango::MultiAttrProp<T>* prop,pugi::xml_node& node);
				//int ParseMultiAttrProp(Tango::MultiAttrProp<T>& prop,pugi::xml_node& node);

		private:
			pugi::xml_document fDoc;
			
	};

}	//	End of namespace

#endif

