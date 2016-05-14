#ifndef ATTR_CALLBACK_H
#define ATTR_CALLBACK_H

#include <tango.h>
#include <BaseDevice.h>

using namespace Tango;

namespace BaseDevice_ns {
  
	class BaseDevice;

	class AttrCallBack : public Tango::CallBack, public Tango::LogAdapter
	{ 
		private:
			static BaseDevice* device;	
			static omni_mutex* mutex;

		public:

			AttrCallBack(BaseDevice* dev,omni_mutex* mutex); 
			~AttrCallBack();
		   
		public:
			void push_event(Tango::EventData*); 
		
		private: 
			int UpdateDeviceAttr(Tango::EventData* event);

		private:
			static log4tango::Logger* logger;
			
		friend class BaseDevice;

	};
}//close namespace

#endif
