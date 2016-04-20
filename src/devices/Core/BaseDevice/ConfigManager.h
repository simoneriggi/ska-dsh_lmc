#ifndef ConfigManager_H
#define ConfigManager_H

#include <BaseDevice.h>
#include <CodeUtils.h>

#include <tango.h>

#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>


namespace Utils_ns {
	class CodeUtils;
}


namespace BaseDevice_ns {


class ConfigManager : public Tango::LogAdapter {

	public:
		ConfigManager(Tango::DeviceImpl* device);
		~ConfigManager();

	private:
		mutable std::mutex m_mutex;

		//Tango-related vars
		log4tango::Logger* m_logger;

};//close ConfigManager class


}//close namespace

#endif
