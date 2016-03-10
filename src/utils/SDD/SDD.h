#ifndef SDD_H
#define SDD_H


#include <tango.h>
#include <MoniPoint.h>

#include <string>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <iomanip>

#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <map>

namespace SDD_ns
{

	class MoniPointBase;
	
	class SDD {

		public:
		
			// The class needs to be default constructible when you want to convert from msgpack::object to your_class                 
    	SDD(std::string filename);
			~SDD();

		public: 
			virtual int parse(SDD_ns::MoniPoints& mpCollection){return 0;}
			int readFile();

		protected:
			long GetAttrDataType(std::string dataTypeTag);
			Tango::AttrWriteType GetAttrRWType(std::string dataRWTag);
			int GetAttrDataFormat(std::string dataFormatTag);

			
		private:
		

		protected:
    	std::string fFileName;
			std::string fSDDString;		    
	};

}	//	End of namespace

#endif
