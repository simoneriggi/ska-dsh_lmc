#ifndef TaskManager_H
#define TaskManager_H

#include <Scheduler.h>
#include <CodeUtils.h>

#include <tango.h>


//# JSON CPP
#include <json/json.h>

#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>


namespace Utils_ns {
	class CodeUtils;
}


namespace Scheduler_ns {

class TaskCallBack;
class Scheduler;

enum TaskExecStatus {
	eINIT= 0,
	eIDLE= 1,
	eRUNNING= 2,
	eCOMPLETED= 3
};
enum TaskStatus {
	eUNKNOWN= 0,
	eSUCCESS= 1,
	eFAILED= 2,
	eABORTED= 3,
	eEXPIRED= 4
};

typedef std::chrono::system_clock::time_point Timestamp;


class Task {

	public :

		Task(){
			status= eUNKNOWN;	
			execStatus= eINIT;
		};

		Task(std::string& m_id,std::string& m_name,std::string& m_deviceName,Timestamp& m_tstart)
			: id(m_id), name(m_name),	deviceName(m_deviceName), startTime(m_tstart)	
		{
			status= eUNKNOWN;	
			execStatus= eINIT;
			hasEndTime= false;
			endTime= startTime;
			hasArgin= false; 
		}//close constructor 

		Task(std::string& m_id,std::string& m_name,std::string& m_deviceName,Timestamp& m_tstart,Timestamp& m_tend)
			: id(m_id), name(m_name),	deviceName(m_deviceName), startTime(m_tstart), endTime(m_tend)	
		{
			status= eUNKNOWN;	
			execStatus= eINIT;
			hasEndTime= true;	
			hasArgin= false; 
		}//close constructor 

    Task(std::string& m_id,std::string& m_name,std::string& m_deviceName,Timestamp& m_tstart,Tango::DeviceData& m_argin)
			: id(m_id), name(m_name),	deviceName(m_deviceName), startTime(m_tstart), argin(m_argin)
		{
			status= eUNKNOWN;	
			execStatus= eINIT;
			hasEndTime= false;
			endTime= startTime;
			hasArgin= true; 
		}//close constructor 

		Task(std::string& m_id,std::string& m_name,std::string& m_deviceName,Timestamp& m_tstart,Timestamp& m_tend,Tango::DeviceData& m_argin)
			: id(m_id), name(m_name),	deviceName(m_deviceName), startTime(m_tstart), endTime(m_tend), argin(m_argin)
		{
			status= eUNKNOWN;	
			execStatus= eINIT;
			hasEndTime= true;	
			hasArgin= true; 
		}//close constructor 

    ~Task(){
			
		};//close destructor 

		/*
		bool operator<(const Task& aTask){
    	return ( this->startTime < aTask.startTime );
    }
		bool operator==(const Task& task){
			return (this->id==task.id && this->name==task.name);
		}
		*/

	public:
		/** 
		\brief Print task
 		*/
		void Print(){
			auto now = std::chrono::system_clock::now();
			auto tdiff= std::chrono::duration<double,std::milli>(startTime-now);
			cout<<"Task id: "<<id<<", name: "<<name<<" exec_status="<<execStatus<<", devName="<<deviceName<<", tdiff(s)="<<tdiff.count()/1000.<<endl;
		}

		/** 
		\brief Get printable string
 		*/
		std::string GetPrintableString(){
			std::stringstream sstream;
			auto epoch= std::chrono::duration_cast<std::chrono::seconds>(startTime.time_since_epoch()).count();
			sstream<<"Task id: "<<id<<", name: "<<name<<", device_name="<<deviceName<<", epoch_start(s)="<<epoch;
			return sstream.str();
		}

		/** 
		\brief Get Tango pipe from task
 		*/
		int GetPipeBlob(Tango::DevicePipeBlob& blob){
			
			long int tstart= std::chrono::duration_cast<std::chrono::seconds>(startTime.time_since_epoch()).count();
			long int tstop= tstart;
			if(hasEndTime) tstop= std::chrono::duration_cast<std::chrono::seconds>(endTime.time_since_epoch()).count();
	
			blob.set_name(id);
			std::vector<std::string> field_names {"id","name","device_name","status","exec_status","tstart","tend","hasEndTime","hasArgin"};
				
			try {
				blob.set_data_elt_names(field_names);
				
				blob["id"] << id;
				blob["name"] << name;
				blob["device_name"] << deviceName;
				blob["status"] << status;
				blob["exec_status"] << execStatus;
				blob["tstart"] << tstart;
				blob["tend"] << tstop;
				blob["hasEndTime"] << hasEndTime;
				blob["hasArgin"] << hasArgin;
			}
			catch(Tango::DevFailed& e){
				Tango::Except::print_exception(e);
				cerr<<"ERROR: Exception occurred while creating and filling task pipe!"<<endl;
				return -1;
			}
			catch(...){
				cerr<<"ERROR: Unknown exception occurred while creating and filling task pipe!"<<endl;
				return -1;
			}
			return 0;
			
		}//close GetPipeBlob()

		std::shared_ptr<Tango::DevicePipeBlob> GetPipeBlobPtr(){
			
			long int tstart= std::chrono::duration_cast<std::chrono::seconds>(startTime.time_since_epoch()).count();
			long int tstop= tstart;
			if(hasEndTime) tstop= std::chrono::duration_cast<std::chrono::seconds>(endTime.time_since_epoch()).count();
	
			auto blob= std::make_shared<Tango::DevicePipeBlob>();
			blob->set_name(id);
			std::vector<std::string> field_names {"id","name","device_name","status","exec_status","tstart","tend","hasEndTime","hasArgin"};
				
			try {
				blob->set_data_elt_names(field_names);
				
				(*blob)["id"] << id;
				(*blob)["name"] << name;
				(*blob)["device_name"] << deviceName;
				(*blob)["status"] << status;
				(*blob)["exec_status"] << execStatus;
				(*blob)["tstart"] << tstart;
				(*blob)["tend"] << tstop;
				(*blob)["hasEndTime"] << hasEndTime;
				(*blob)["hasArgin"] << hasArgin;
			}
			catch(Tango::DevFailed& e){
				Tango::Except::print_exception(e);
				cerr<<"ERROR: Exception occurred while creating and filling task pipe!"<<endl;
				return 0;
			}
			catch(...){
				cerr<<"ERROR: Unknown exception occurred while creating and filling task pipe!"<<endl;
				return 0;
			}
			return blob;
			
		}//close GetPipeBlobPtr()


	private:
		
	public:
	
		
		//Cmd info
		std::string id;
		std::string name;
		std::string deviceName;
		int status;
		int execStatus;
		
		//Timestamp info
		Timestamp startTime;
		Timestamp endTime;	
		bool hasEndTime;
		Tango::DeviceData argin;
		bool hasArgin;
		
};//close class

typedef std::shared_ptr<Task> TaskPtr;
struct CompareTaskByActivationTime {
	bool operator()( const Task& t1, const Task& t2 ) const {
  	return t1.startTime < t2.startTime;
  }
};
struct CompareTaskByExpirationTime {
	bool operator()( const Task& t1, const Task& t2 ) const {
  	return t1.endTime < t2.endTime;
  }
};

struct CompareTaskPtr {
	bool operator()( const TaskPtr t1, const TaskPtr t2 ) const {
  	//return t1->startTime > t2->startTime;
  	return t1->startTime < t2->startTime;
  }
};

struct MatchTaskById {
	MatchTaskById(const std::string& id) : m_id(id) {}
 	bool operator()(const Task& obj) const {
  	return (obj.id==m_id);
 	}
 	private:
  	const std::string& m_id;
};

struct MatchTask {
	MatchTask(const std::string& id,const std::string& name) : m_id(id), m_name(name) {}
 	bool operator()(const Task& obj) const {
  	return (obj.id==m_id && obj.name==m_name);
 	}
 	private:
  	const std::string& m_id;
		const std::string& m_name;
};

struct MatchTaskPtr {
	MatchTaskPtr(const std::string& id,const std::string& name) : m_id(id), m_name(name) {}
 	bool operator()(const TaskPtr obj) const {
  	return (obj->id==m_id && obj->name==m_name);
 	}
 	private:
  	const std::string& m_id;
		const std::string& m_name;
};

struct MatchExecStatus {
	MatchExecStatus(const int& exec_status) : m_exec_status(exec_status) {}
 	bool operator()(const Task& obj) const {
  	return (obj.execStatus==m_exec_status);
 	}
 	private:
  	const int& m_exec_status;
};

struct MatchOldTask {
	MatchOldTask(const int& filter_exec_status,const Timestamp& current_time,const double& historyTimeThr) 
		: m_filter_exec_status(filter_exec_status), m_currentTime(current_time), m_historyTimeThr(historyTimeThr) 
	{}
 	bool operator()(const Task& obj) const {
		double tdiff= std::chrono::duration<double,std::milli>(m_currentTime-obj.startTime).count()*1.e-3;
		bool isOld= (tdiff>=m_historyTimeThr) && (m_filter_exec_status==-1 || obj.execStatus!=m_filter_exec_status);
  	return isOld;
 	}
 	private:
  	const int& m_filter_exec_status;
		const Timestamp& m_currentTime;
		const double& m_historyTimeThr;
};

struct MatchPtrExecStatus {
	MatchPtrExecStatus(const int& exec_status) : m_exec_status(exec_status) {}
 	bool operator()(const TaskPtr obj) const {
  	return (obj->execStatus==m_exec_status);
 	}
 	private:
  	const int& m_exec_status;
};


typedef std::set<Task,CompareTaskByActivationTime> TaskQueue;
typedef std::set<Task,CompareTaskByExpirationTime> TaskMonitorQueue;
typedef std::vector<Task> Tasks;


typedef std::priority_queue<TaskPtr, std::vector<TaskPtr>, CompareTaskPtr> TaskPtrQueue;
typedef std::vector<TaskPtr> TaskPtrs;


class TaskCollection {
	public:
		TaskCollection(){};
		~TaskCollection(){};

	private:
		
		
		static bool compareByStartTime(const Task& a, const Task& b) {
   		return ( (a.startTime) < (b.startTime) );
		}
		static bool comparePtrByStartTime(const TaskPtr a, const TaskPtr b) {
   		return ( (a->startTime) < (b->startTime) );
		}

	public:
		int GetN(){return (int)(m_tasks.size());}
		bool HasTasks(){return (GetN()>0);}

		int GetNIdleTasks(){
			return std::count_if(m_tasks.begin(), m_tasks.end(),MatchExecStatus(eIDLE));
		}
		int GetRunningTasks(){
			return std::count_if(m_tasks.begin(), m_tasks.end(),MatchExecStatus(eRUNNING));
		}

		int RemoveIdleTasks(){
			if(m_tasks.empty()) return 0;
			try {
				m_tasks.erase( std::remove_if(m_tasks.begin(), m_tasks.end(), MatchExecStatus(eIDLE) ), m_tasks.end() );
			}
			catch(...){
				cerr<<"TaskCollection::RemoveIdleTasks(): ERROR: Exception while deleting task from container!"<<endl;
				return -1;
			}
			return 0;
		}//close if RemoveIdleTasks()


		void RemoveTask(int index){
			m_tasks.erase (m_tasks.begin()+index);		
		}
		int RemoveTask(std::string id,std::string name){
			int index= -1;
			if(!FindTask(index,id,name)) return -1;
			RemoveTask(index);
			return 0;
		}

		int RemoveTask(std::string id){
			if(m_tasks.empty()) return 0;
			std::vector<Task>::iterator it = std::find_if(m_tasks.begin(), m_tasks.end(), MatchTaskById(id));
			if(it==m_tasks.end()) return -1;
			try {
				m_tasks.erase(it);
			}
			catch(...){
				cerr<<"TaskCollection::RemoveTask(): ERROR: Exception while deleting task from container!"<<endl;
				return -1;
			}
			return 0;
		}

		int AddTask(Task& aTask){
			aTask.execStatus= eIDLE;
			try {
				m_tasks.push_back(aTask);	
			}
			catch(...){
				cerr<<"TaskCollection::AddTask(): ERROR: Exception occurred while adding task in collection..."<<endl;
				return -1;
			}
			return 0;
		}//close Add()
		
		Tasks& GetTasks() {return m_tasks;}

		Task* GetTask(int index){
			int nTasks= GetN();
			if(nTasks<=0 || index<0 || index>=GetN() ) return 0;
			return &m_tasks[index];
		}

		Task* FindTask(int& index,std::string id,std::string name){
			int nTasks= GetN();
			if(nTasks<=0) return 0;
			std::vector<Task>::iterator it = std::find_if(m_tasks.begin(), m_tasks.end(), MatchTask(id,name));
			if (it==m_tasks.end()) return 0;//not found in collection
			size_t pos = it-m_tasks.begin();
			index= pos;
			return GetTask(index);
		}//close FindTask()

		TaskPtr GetTaskPtr(int index){
			int nTasks= GetN();
			if(nTasks<=0 || index<0 || index>=GetN() ) return 0;
			return TaskPtr(std::make_shared<Task>(m_tasks[index]));
		}

		TaskPtr FindTaskPtr(int& index,std::string id,std::string name){
			int nTasks= GetN();
			if(nTasks<=0) return 0;
			std::vector<Task>::iterator it = std::find_if(m_tasks.begin(), m_tasks.end(), MatchTask(id,name));
			if (it==m_tasks.end()) return 0;//not found in collection
			size_t pos = it-m_tasks.begin();
			index= pos;
			return GetTaskPtr(index);
		}//close FindTaskPtr()	

		int SetTaskExecStatus(int index,int status,bool check=true){
			if(check && !GetTask(index)){
				return -1;
			}
			m_tasks[index].execStatus= status;
			return 0;
		}

		int SetTaskStatus(int index,int exec_status,int status,bool check=true){
			if(check && !GetTask(index)){
				return -1;
			}
			m_tasks[index].execStatus= exec_status;
			m_tasks[index].status= status;
			return 0;
		}

		Task* FindTopTask(int& index){
			int nTasks= GetN();
			if(nTasks<=0) return 0;
			std::vector<Task>::iterator it= std::min_element(m_tasks.begin(),m_tasks.end(),compareByStartTime);
			size_t pos = it-m_tasks.begin();
			index= pos;
			return GetTask(index);
		}

		TaskPtr FindTopTaskPtr(int& index){
			int nTasks= GetN();
			if(nTasks<=0) return 0;
			std::vector<Task>::iterator it= std::min_element(m_tasks.begin(),m_tasks.end(),compareByStartTime);
			size_t pos = it-m_tasks.begin();
			index= pos;
			return GetTaskPtr(index);
		}

		void SortTasks(){
			std::sort(m_tasks.begin(),m_tasks.end(), compareByStartTime);
		};


		/** 
		\brief Get Tango pipe from task
 		*/
		int GetPipe(Tango::Pipe& pipe){
			//Init pipe
			pipe.set_root_blob_name("tasks");
			std::vector<std::string> field_names;
			for(int i=0;i<GetN();i++){
				std::string cmd_id= m_tasks[i].id;
				field_names.push_back(cmd_id);
			}//end loop tasks			
			pipe.set_data_elt_names(field_names);
				
			//Fill pipe
			//Tango::DevicePipeBlob pipe_blob;
			//std::vector<Tango::DevicePipeBlob> pipe_blobs;
			try{					
				for(int i=0;i<GetN();i++){
					
					//int status= m_tasks[i].GetPipeBlob(pipe_blob);
					auto pipe_blob= m_tasks[i].GetPipeBlobPtr();
					
					//if(status<0){
					if(!pipe_blob){
						throw std::runtime_error("Failed to get pipe blob from task");
					}
					//pipe_blobs.push_back(*pipe_blob);
					
					//pipe << pipe_blobs[i];
					pipe << *pipe_blob;
					
				}//end loop tasks
				
			}//close try
			catch(...){
				cerr<<"TaskCollection::GetPipeBlob(): ERROR: Exception occurred while filling task collection pipe!"<<endl;
				return -1;
			}
			return 0;
		}//close GetPipeBlob()


		int ClearTasks(double historyTimeThr,bool clearRunningTasks){
			if(m_tasks.empty()) return 0;
			auto now = std::chrono::system_clock::now();
			try {
				if(clearRunningTasks) m_tasks.erase( std::remove_if(m_tasks.begin(), m_tasks.end(), MatchOldTask(-1,now,historyTimeThr) ), m_tasks.end() );
				else m_tasks.erase( std::remove_if(m_tasks.begin(), m_tasks.end(), MatchOldTask(eRUNNING,now,historyTimeThr) ), m_tasks.end() );
			}
			catch(...){
				cerr<<"TaskCollection::ClearTasks(): ERROR: Exception while deleting task from container!"<<endl;
				return -1;
			}
			return 0;
		}//close ClearTasks()
	
	private:
		Tasks m_tasks;
		
};//close class TaskCollection


class TaskManager : public Tango::LogAdapter {

	public:
		TaskManager(Scheduler* dev,TaskCallBack* cb);
		~TaskManager();

		//TaskManager()=default;
  	//TaskManager(const Queue&) = delete;            // disable copying
  	//TaskManager& operator=(const Queue&) = delete; // disable assignment

	public:

		/** 
		\brief Add task to queue & list
 		*/
		int AddTask(Task& task);
		/** 
		\brief Pop task from queue
 		*/
		int PopTask(Task&);
		/** 
		\brief Pop task from monitor queue
 		*/
		int PopMonitorTask(Task&);

		/** 
		\brief Find task 
 		*/
		Task* FindTask(int& index,std::string id,std::string name);
		/** 
		\brief Find task ptr
 		*/
		TaskPtr FindTaskPtr(int& index,std::string id,std::string name);
		/** 
		\brief Set task exec status
 		*/
		int SetTaskStatus(int index,int exec_status,int status,bool check=true);
		/** 
		\brief Set task exec status
 		*/
		int SetTaskStatus(Task& task,int exec_status,int status);

		/** 
		\brief Clear task list (remove old tasks)
 		*/
		int ClearTasks(double historyTimeThr,bool clearRunningTasks);

	 	/** 
		\brief Wake up signal
 		*/		
		void WakeUp();
		/** 
		\brief End signal
 		*/		
		void End();
		/** 
		\brief Check if queue is empty
 		*/
		bool IsEmptyQueue() const;
		/** 
		\brief Get number of tasks in queue
 		*/
		int GetNTasksInQueue(){
			std::unique_lock<std::mutex> mlock(m_mutex);
			return static_cast<int>(m_taskQueue.size());
		}
		/** 
		\brief Get number of tasks in monitor queue
 		*/
		int GetNTasksInMonitorQueue(){
			std::unique_lock<std::mutex> mlock(m_mutex);
			return static_cast<int>(m_monitorQueue.size());
		}

		/** 
		\brief Get number of tasks in running status
 		*/
		int GetNRunningTasks(){
			std::unique_lock<std::mutex> mlock(m_mutex);
			return m_tasks.GetRunningTasks();
		}

		/** 
		\brief Remove task 
 		*/
		int RemoveTask(std::string cmd_id);

		/** 
		\brief Flush queue
 		*/
		int FlushQueue();

		/** 
		\brief Get number of tasks in collection
 		*/
		int GetNTasksInCollection(){
			std::unique_lock<std::mutex> mlock(m_mutex);
			return m_tasks.GetN();
		}
		/** 
		\brief Print tasks in queue
 		*/
		void PrintQueueTasks();
		/** 
		\brief Print tasks in list
 		*/
		void PrintTasks(){
			//std::unique_lock<std::mutex> mlock(m_mutex);
			std::lock_guard<std::mutex> lock(m_mutex);
			Tasks& task_list= m_tasks.GetTasks();
			INFO_STREAM<<"TaskManager::PrintTasks(): INFO: "<<task_list.size()<<" tasks present in collection"<<endl;
			for(unsigned int i=0;i<task_list.size();i++){
				std::string thisTaskString= task_list[i].GetPrintableString();
				INFO_STREAM<<"TaskManager::PrintTasks(): INFO: "<<thisTaskString<<endl;
			}
			return;
		}

		/** 
		\brief Get task history info
 		*/
		int GetTaskHistoryInfo(std::vector<std::string>& info){
			std::lock_guard<std::mutex> lock(m_mutex);
			Tasks& task_list= m_tasks.GetTasks();
			for(unsigned int i=0;i<task_list.size();i++){
				std::string thisTaskString= task_list[i].GetPrintableString();
				info.push_back(thisTaskString);
			}
			return 0;
		}

		/** 
		\brief Get task info
 		*/
		int GetTaskInfo(std::vector<std::string>& info){
			std::lock_guard<std::mutex> lock(m_mutex);
			for(auto task_iter : m_taskQueue) {
 				std::string thisTaskString= task_iter.GetPrintableString();
				info.push_back(thisTaskString);
			}//end loop tasks in queue 

			return 0;
		}

		/** 
		\brief Set maximum number of cached device proxies
 		*/
		void SetMaxCachedDevProxies(int val){
			std::unique_lock<std::mutex> mlock(m_mutex);
			m_maxCachedDevProxies= val;
		}
		/*
		\brief Get current number of cached device proxies
 		*/
		int GetNCachedDevProxies(){
			std::unique_lock<std::mutex> mlock(m_mutex);
			return m_cachedDeviceProxies.size();
		}
		/** 
		\brief Set maximum number of tasks in the queue
 		*/
		void SetMaxNTasksInQueue(int val){
			std::unique_lock<std::mutex> mlock(m_mutex);
			m_maxTasksInQueue= val;
		}
		/** 
		\brief Set maximum number of tasks in the collection
 		*/
		void SetMaxNTasksInCollection(int val){
			std::unique_lock<std::mutex> mlock(m_mutex);
			m_maxTasksInCollection= val;
		}

		/** 
		\brief Execute task
 		*/
		int ExecuteTask(Task&);

		/** 
		\brief Get task collection pipe 
 		*/
		int GetTaskPipe(Tango::Pipe&);

	private:		
		/** 
		\brief Is full device proxy cache
 		*/
		bool IsFullDeviceProxyCache(){
			int cache_size= (int)m_cachedDeviceProxies.size();
			return (cache_size>=m_maxCachedDevProxies && cache_size>0);
		}
		
		/** 
		\brief Add device proxy to cached
 		*/
		int AddDeviceToCached(std::string device_name,Tango::DeviceProxy* device,bool check=true){
			if(!device) return -1;
			if(IsFullDeviceProxyCache()) return -1;
			
			if(check){
				std::map<std::string,Tango::DeviceProxy*>::iterator it= m_cachedDeviceProxies.find(device_name);
				if(it==m_cachedDeviceProxies.end() || m_cachedDeviceProxies.empty()){
					m_cachedDeviceProxies.insert( std::pair<std::string,Tango::DeviceProxy*>(device_name,device) );
				}
			}	
			else{
				m_cachedDeviceProxies.insert( std::pair<std::string,Tango::DeviceProxy*>(device_name,device) );
			}
			return 0;
		}//close AddDeviceToCached()

		/** 
		\brief Get cached device proxy 
 		*/
		Tango::DeviceProxy* GetCachedDeviceProxy(std::string device_name){
			if(m_cachedDeviceProxies.empty()) return 0;
			std::map<std::string,Tango::DeviceProxy*>::iterator it= m_cachedDeviceProxies.find(device_name);
			if(it==m_cachedDeviceProxies.end()) return 0;					
			return it->second;
		}

		/** 
		\brief Find task in queue
 		*/
		const Task* FindTaskInQueue(std::string cmd_id){
			if(m_taskQueue.empty()) return 0;
			std::set<Task>::iterator it= std::find_if(m_taskQueue.begin(),m_taskQueue.end(),MatchTaskById(cmd_id));
			if(it==m_taskQueue.end()) return 0; 
			return &(*it);
		}
		/** 
		\brief Remove task from queue
 		*/
		int RemoveTaskFromQueue(std::string cmd_id){
			if(m_taskQueue.empty() ) return 0;		
			std::set<Task>::iterator it= std::find_if(m_taskQueue.begin(),m_taskQueue.end(),MatchTaskById(cmd_id));
			if(it==m_taskQueue.end()) return -1;			
			try {
				m_taskQueue.erase(it);
			}
			catch(...){
				WARN_STREAM<<"TaskManager::RemoveTaskFromQueue(): ERROR: Exception while deleting task from queue!"<<endl;
				return -1;
			}
			return 0;
		}

		/** 
		\brief Remove task from monitor queue
 		*/
		int RemoveTaskFromMonitorQueue(std::string cmd_id){
			if(m_monitorQueue.empty() ) return 0;		
			std::set<Task>::iterator it= std::find_if(m_monitorQueue.begin(),m_monitorQueue.end(),MatchTaskById(cmd_id));
			if(it==m_monitorQueue.end()) {
				WARN_STREAM<<"TaskManager::RemoveTaskFromMonitorQueue(): WARN: Cannot find task id "<<cmd_id<<" in monitor queue!"<<endl;
				return -1;			
			}
			try {
				m_monitorQueue.erase(it);
			}
			catch(...){
				WARN_STREAM<<"TaskManager::RemoveTaskFromMonitorQueue(): ERROR: Exception while deleting task from monitor queue!"<<endl;
				return -1;
			}
			return 0;
		}

		

	private:		
		//Thread mutex variables
		static bool m_wakeUpSignal;
		static bool m_endSignal;	
		mutable std::mutex m_mutex;
		std::condition_variable m_cond;

		//Tango-related vars
		log4tango::Logger* m_logger;
		TaskCallBack* m_taskCallBack;
		int m_maxCachedDevProxies;
		Tango::DeviceProxy* m_deviceProxy;
		std::map<std::string,Tango::DeviceProxy*> m_cachedDeviceProxies;
		
		//Queue & task tracker vars
		TaskCollection m_tasks;
		TaskQueue m_taskQueue;
		TaskMonitorQueue m_monitorQueue;
		
		//Options
		int m_maxTasksInQueue;
		int m_maxTasksInCollection;

};//close class




}//close namespace

#endif
