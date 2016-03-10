##**About**  
SKA Dish LMC Control System Prototype

##**Installation**  
Install the project dependencies:  
* omniORB [http://omniorb.sourceforge.net/]  
* zmq [http://zeromq.org/]  
* Tango Control System [http://www.tango-controls.org/]  
* jsoncpp  [https://github.com/open-source-parsers/jsoncpp]  
* pugixml [http://pugixml.org/]  
* cmake (>=2.8) [https://cmake.org]  
  
Make sure you have the following environment variables set to your dependency installation dirs and let cmake find the include dirs and libraries:  
* TANGO_ROOT  
* OMNIORB_ROOT  
* ZMQ_ROOT  
* JSONCPP_ROOT  

To install the project:

* Clone this repository into your local $SOURCE_DIR  
  ```git clone https://github.com/simoneriggi/ska-dsh_lmc.git $SOURCE_DIR```
* Create the build and install directories: $BUILD_DIR, $INSTALL_DIR  
* In the build directory:  
  ```cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR $SOURCE_DIR```  
  ```make```  
  ```make install```  
  
