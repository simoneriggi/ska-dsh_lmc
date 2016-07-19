/**
* @file SysUtils.h
* @class SysUtils
* @brief Utility functions for system tasks
*
* Utility functions for system tasks
* @author S. Riggi
* @date 23/08/2010
*/


#ifndef SysUtils_h
#define SysUtils_h 1


#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdexcept>

#include <vector>
#include <algorithm>
#include <map>
#include <time.h>
#include <ctime>
#include <chrono>

using namespace std;


namespace Utils_ns {

class SysUtils {

  public:
		
		/** 
		\brief Class constructor: initialize structures.
 		*/
    SysUtils();
		/**
		* \brief Class destructor: free allocated memory
		*/
   	virtual ~SysUtils();

		
	public:

		
		
		static timespec TimeDiff(timespec start, timespec end) {
			timespec temp;
			if ((end.tv_nsec-start.tv_nsec)<0) {
				temp.tv_sec = end.tv_sec-start.tv_sec-1;
				temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
			} 
			else {
				temp.tv_sec = end.tv_sec-start.tv_sec;
				temp.tv_nsec = end.tv_nsec-start.tv_nsec;
			}
			return temp;
		}

		static timespec TimeSum (timespec time1, timespec time2) {
			timespec  result ;
			result.tv_sec = time1.tv_sec + time2.tv_sec ;
    	result.tv_nsec = time1.tv_nsec + time2.tv_nsec ;
    	if (result.tv_nsec >= 1000000000L) {
      	result.tv_sec++ ;  result.tv_nsec = result.tv_nsec - 1000000000L ;
    	}
    	return (result) ;
		}
		
		static double TimeToSec (timespec time){
    	return ((double)time.tv_sec + (time.tv_nsec/1.e+09)) ;
		}

		static double TimeToNSec (timespec time){
			return (time.tv_sec*1.e+09 + (double)time.tv_nsec) ;
		}

		static long long int getTimestamp() {
      auto now = std::chrono::high_resolution_clock::now();
      auto duration = now.time_since_epoch();
      return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    }//close getTimestamp()

    static long long int getTimestamp_usec() {
      auto now = std::chrono::high_resolution_clock::now();
      auto duration = now.time_since_epoch();
      return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    }//close getTimestamp_usec()

		static std::time_t UTCStringToEpoch(std::string& t_utc){
			std::tm tm_utc;
			strptime(t_utc.c_str(), "%Y-%m-%dT%T%z", &tm_utc);
			tm_utc.tm_isdst = -1;
			std::time_t epoch_utc = timegm(&tm_utc);//Assume tm is expressed in UTC (neglects any timezone setting)
			return epoch_utc;
		}

		static std::chrono::system_clock::time_point UTCStringToChronoTimeStamp(std::string& t_utc){
			std::time_t epoch_utc= UTCStringToEpoch(t_utc);
			return std::chrono::system_clock::from_time_t(epoch_utc);
		}

		static std::chrono::system_clock::time_point UTCStringToLocalChronoTimeStamp(std::string& t_utc){
			std::time_t epoch_utc= UTCStringToEpoch(t_utc);
			std::tm* tm_local= std::localtime(&epoch_utc);
			tm_local->tm_isdst = -1;
			std::time_t epoch_local= std::mktime(tm_local);
			return std::chrono::system_clock::from_time_t(epoch_local);
		}

		static double GetOffsetToUTC(){
			time_t currtime;
 		 	struct tm* timeinfo;
  		time ( &currtime );
  		timeinfo = gmtime ( &currtime );
  		time_t utc = mktime( timeinfo );
  		timeinfo = localtime ( &currtime );
  		time_t local = mktime( timeinfo );

  		// Get offset in hours from UTC
  		double offsetFromUTC = difftime(utc, local) / 3600.;

  		// Adjust for DST
			if (timeinfo->tm_isdst){
    		offsetFromUTC -= 1;
  		}
			return offsetFromUTC;
		}//close GetOffsetToUTC()

		static std::time_t TimeStringToUTCEpoch(std::string& stime) {
			int year, month, day;
			int hour, minute, second;
			int zone, zone_fract;
			sscanf(stime.c_str(), "%4u-%2u-%2uT%2u:%2u:%2u%3u:%2u", &year, &month, &day, &hour, &minute, &second, &zone, &zone_fract);

			long long int utc_offset= zone*3600 + zone_fract*60;	
			cout<<"TimeStringToUTCEpoch(): utc_offset="<<utc_offset<<endl;

			//Create a tm struct
			std::tm time_struct;
			time_struct.tm_year= year-1900;
			time_struct.tm_mon= month-1;
			time_struct.tm_mday= day;
			time_struct.tm_hour= hour;
			time_struct.tm_min= minute;
			time_struct.tm_sec= second;
			time_struct.tm_isdst = -1;
			time_struct.tm_gmtoff= utc_offset;

			std::time_t epoch = timegm(&time_struct);//Assume tm is expressed in UTC (neglects any timezone setting, internally the gmtoff is forced to zero)
			std::time_t epoch_utc = epoch - utc_offset;
			
			return epoch_utc;
		}
	
		static std::chrono::system_clock::time_point TimeStringToUTCChronoTimeStamp(std::string& stime) {
			std::time_t epoch_utc= TimeStringToUTCEpoch(stime);
			return std::chrono::system_clock::from_time_t(epoch_utc);
		}
		
		static std::time_t TimeStringToLocalEpoch(std::string& stime) {
			std::time_t epoch_utc= TimeStringToUTCEpoch(stime);
			std::tm* tm_utc= gmtime(&epoch_utc);
			tm_utc->tm_isdst = -1;
			std::time_t epoch_local= std::mktime (tm_utc);
			cout<<"TimeStringToLocalEpoch(): epoch_utc="<<epoch_utc<<", epoch_local="<<epoch_local<<endl;
			return epoch_local;
		}
		
		static std::chrono::system_clock::time_point TimeStringToLocalChronoTimeStamp(std::string& stime) {
			std::time_t epoch_local= TimeStringToLocalEpoch(stime);
			return std::chrono::system_clock::from_time_t(epoch_local);
		}

	private:
	
};//close Class()


}//close namespace


#endif 
