/**
* @file CodeUtils.cc
* @class CodeUtils
* @brief Utility functions for programming shortcut tasks
*
* Utility functions for programming shortcut tasks
* @author S. Riggi
* @date 15/01/2016
*/

#include <CodeUtils.h>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <ctime>

using namespace std;

namespace Utils_ns {

CodeUtils::CodeUtils(){

}

CodeUtils::~CodeUtils(){

}



template<>
bool CodeUtils::getValueFromString(std::string& input){
	bool value;
	std::stringstream sstream;
	sstream.str(input);
	sstream>>std::boolalpha>>value;
	return value;
}



}//close namespace



