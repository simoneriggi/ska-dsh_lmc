/**
* @file CodeUtils.h
* @class CodeUtils
* @brief Utility functions for programming shortcut tasks
*
* Utility functions for programming shortcut tasks
* @author S. Riggi
* @date 15/01/2016
*/


#ifndef CodeUtils_h
#define CodeUtils_h 1

//# JSON CPP
#include <json/json.h>

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
#include <string>
#include <time.h>
#include <ctime>


using namespace std;

namespace Utils_ns {

class CodeUtils {

  public:
		
		/** 
		\brief Class constructor: initialize structures.
 		*/
    CodeUtils();
		/**
		* \brief Class destructor: free allocated memory
		*/
   	virtual ~CodeUtils();

		
	public:
		
		static int ParseJsonString(Json::Value& root,std::string& jsonString){
			Json::Reader reader;
			try {
				reader.parse(jsonString,root);
			}
			catch(...){
				cerr<<"CodeUtils::ParseJsonString(): ERROR: Failed to parse input string!"<<endl;
				return -1;
			}

			if(root.isNull()) return -1;
			return 0;
		}//close ParseJsonString()


		static int GetJsonString(std::string& jsonString,Json::Value const& jsonObj,bool isMinified=true) {
			try {
				if(isMinified){// write in a minified way
					Json::FastWriter fastWriter;
					jsonString= fastWriter.write(jsonObj);
				}
				else{	// write in a nice readible way
					Json::StyledWriter formattedWriter;
					jsonString= formattedWriter.write(jsonObj);
				}
			}
			catch(...){
				cerr<<"GetJsonString(): ERROR: Failed to encode argument to json string!"<<endl;
				return -1;
			}
			return 0;
		};


		/**
		* \brief Find item in a vector and returns item position
		*/
		template<class T>
			static bool FindItem(std::vector<T>& v, T itemValue,int& pos) {
				if(v.size()<=0){
					pos= -1;
					return false;
				}	
				typename std::vector<T>::iterator it= std::find(v.begin(),v.end(),itemValue);
				if(it==v.end()){
					pos= -1;
					return false;
				}	
				pos= (int)(it-v.begin());
				return true;
			}//close FindItem()


		/**
		* \brief Delete selected items from a vector
		*/
		template<class T>
			static void DeleteItems(std::vector<T>& data, const std::vector<int>& deleteIndices) {
    	std::vector<bool> markedElements(data.size(), false);
    	std::vector<T> tempBuffer;
    	tempBuffer.reserve(data.size()-deleteIndices.size());

   	 	for (std::vector<int>::const_iterator itDel = deleteIndices.begin(); itDel != deleteIndices.end(); itDel++)
      	markedElements[*itDel] = true;

    	for (size_t i=0; i<data.size(); i++) {
				if (markedElements[i]) {//Free memory!
					if(data[i]){
						delete data[i];
						data[i]= 0;
					}			
				}
				else{
					tempBuffer.push_back(data[i]);
				}
				/*
      	if (!markedElements[i]) {
					tempBuffer.push_back(data[i]);
				}
				*/
    	}
    	data = tempBuffer;
		}//close DeleteItems()


		/**
		* \brief Order vectors and get ordering index
		*/
		template<class T> struct index_cmp{

  		index_cmp(const T arr) : arr(arr) {}
  		bool operator()(const size_t a, const size_t b) const
 			{
    		return arr[a] < arr[b];
  		}
  		const T arr;
		};
		template<class T> struct descending_index_cmp{

  		descending_index_cmp(const T arr) : arr(arr) {}
  		bool operator()(const size_t a, const size_t b) const
 			{
    		return arr[a] > arr[b];
  		}
  		const T arr;
		};

		template< class T >
			static void reorder(std::vector<T> & unordered,std::vector<size_t> const & index_map,std::vector<T> & ordered){
  			// copy for the reorder according to index_map, because unsorted may also be
  			// sorted
  			std::vector<T> copy = unordered;
  			ordered.resize(index_map.size());
  			for(int i = 0; i<index_map.size();i++)
					ordered[i] = copy[index_map[i]];
			}

		template <class T>
			static void sort(std::vector<T> & unsorted,std::vector<T> & sorted,std::vector<size_t> & index_map){
  			// Original unsorted index map
  			index_map.resize(unsorted.size());
 				for(size_t i=0;i<unsorted.size();i++)
					index_map[i] = i;
  
  			// Sort the index map, using unsorted for comparison
  			std::sort(index_map.begin(),index_map.end(),index_cmp<std::vector<T>& >(unsorted));
  			sorted.resize(unsorted.size());
  			reorder(unsorted,index_map,sorted);
			}
	
		template <class T>
			static void sort_descending(std::vector<T> & unsorted,std::vector<T> & sorted,std::vector<size_t> & index_map){
  			// Original unsorted index map
  			index_map.resize(unsorted.size());
 				for(size_t i=0;i<unsorted.size();i++)
					index_map[i] = i;
  
  			// Sort the index map, using unsorted for comparison
  			std::sort(index_map.begin(),index_map.end(),descending_index_cmp<std::vector<T>& >(unsorted));
  			sorted.resize(unsorted.size());
  			reorder(unsorted,index_map,sorted);
			}


		static void StringFindAndReplace(std::string& str, const std::string& oldstr, const std::string& newstr){
  		size_t pos = 0;
  		while((pos = str.find(oldstr, pos)) != std::string::npos){
    		str.replace(pos, oldstr.length(), newstr);
     		pos += newstr.length();
  		}	
		}//close StringFindAndReplace()

		// return an evenly spaced 1-d grid of doubles.
		template <typename T>
			static std::vector<T> linspace(T first,T last,int len){
				std::vector<T> result(len);
  			double step = (last-first) / (len - 1);
  			for (int i=0; i<len; i++) { result[i] = first + i*step; }
  			return result;
			}

		/**
		* \brief Extract a scalar value from string
		*/
		//template<typename T>
		//	static T getValueFromString(std::string& input);

		template<typename T>
			static T getValueFromString(std::string& input){
				T value;
				std::stringstream sstream;
				sstream.str(input);
				sstream>>value;
				return value;
			}

	private:

};



}//close namespace


#endif 
