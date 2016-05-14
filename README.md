##**About**  
SKA Dish LMC Control System Prototype

##**Installation**  
Install the project dependencies:  
* omniORB [http://omniorb.sourceforge.net/]  
* zmq [http://zeromq.org/]  
* Tango Control System [http://www.tango-controls.org/]  
* jsoncpp  [https://github.com/open-source-parsers/jsoncpp]  
* log4cxx [https://logging.apache.org/log4cxx/]
* boost [http://www.boost.org/]
* pugixml [http://pugixml.org/]  
* cmake (>=2.8) [https://cmake.org]  
  
Make sure you have set the following environment variables to the external library installation dirs 
* TANGO_ROOT  
* OMNIORB_ROOT  
* ZMQ_ROOT  
* JSONCPP_ROOT
* BOOST_ROOT
* LOG4CXX_ROOT

Add also the following paths to the PKG_CONFIG_PATH environment var: 
* $TANGO_ROOT/lib/pkgconfig  
* $OMNIORB_ROOT/lib/pkgconfig  
* $ZMQ_ROOT/lib/pkgconfig  
* $LOG4CXX_ROOT/lib/pkgconfig  

cmake should find all needed include dirs and libraries used to build the project.

To build and install the project:

* Clone this repository into your local $SOURCE_DIR  
  ```git clone https://github.com/simoneriggi/ska-dsh_lmc.git $SOURCE_DIR```
* Create the build and install directories: $BUILD_DIR, $INSTALL_DIR  
* In the build directory:  
  ```cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR $SOURCE_DIR```  
  ```make```  
  ```make install```  
  
