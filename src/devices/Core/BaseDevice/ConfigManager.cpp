#include <ConfigManager.h>

#include <tango.h>


//## Standard headers
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctime>
#include <stdexcept>
#include <unistd.h>
#include <getopt.h>
#include <math.h>
#include <time.h>

#include <map>
#include <vector>
#include <thread>
#include <memory>
#include <functional>
#include <condition_variable>
#include <mutex>


using namespace std;


namespace BaseDevice_ns {


ConfigManager::ConfigManager(Tango::DeviceImpl* dev) 
	: Tango::LogAdapter(dev)
{

	m_logger= 0;
	if(dev) m_logger= Tango::LogAdapter(dev).get_logger();

}//close constructor
  

ConfigManager::~ConfigManager(){


}//close destructor


}//close namespace 

