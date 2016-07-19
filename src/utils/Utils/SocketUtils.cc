
#include <SocketUtils.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h> 

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/un.h>

#include <iostream>
using namespace std;

namespace Utils_ns {


SocketClient::SocketClient(std::string path,bool isBlocking)
	: m_sockpath(path), m_isBlocking(isBlocking)
{
	m_sockfd= 0;
}//close constructor


SocketClient::~SocketClient(){

	//Closing socket
	Close();

}//close destructor

int SocketClient::Close(){
	//## Close socket if open
	if (m_sockfd > 0){
    close(m_sockfd);
  } 
	return 0;
}//close Close()

int SocketClient::Reset(){

	Close();
	return Init();

}//close Reset()


int SocketClient::Init(){

	/*
	//## Check is socket is already open
	if(m_sockfd>0){
		cout << "SocketClient::Init(): INFO: Socket already init and connected..." << endl;
 		return 0;
	}
	*/

	//## Define socket vars
  struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0; 	

  struct sockaddr_un remote_socket_name;
 	int len;
  long arg; 
  int retval;
  fd_set fdset;

	//## Open socket
	m_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if(m_sockfd<0) {
  	cerr << "SocketClient::Init(): ERROR: Failed to open socket!"<< endl;
    return -1;
  }

  //## Set non-blocking socket 
	memset(&remote_socket_name , 0 , sizeof ( struct sockaddr_un ) ) ;
	remote_socket_name.sun_family = AF_UNIX;
  strncpy(remote_socket_name.sun_path, m_sockpath.c_str(),sizeof(remote_socket_name.sun_path)-1);
  len= sizeof(struct sockaddr_un);

	if(!m_isBlocking){
  	arg = fcntl(m_sockfd, F_GETFL, NULL);
  	arg |= O_NONBLOCK; 
  	fcntl(m_sockfd, F_SETFL, arg); 
  }

	//## Connect socket
	retval = connect( m_sockfd, (struct sockaddr*)&remote_socket_name,len );

  if( errno == EINPROGRESS){
		cerr << "SocketClient::Init(): INFO: Socket connect in EINPROGRESS status..."<< endl;
  	
		FD_ZERO(&fdset);      
		FD_SET(m_sockfd, &fdset);

		if( select( m_sockfd+1, 0, &fdset, 0, &timeout) ){

			int result;
			socklen_t result_len = sizeof(result);
			if (getsockopt(m_sockfd, SOL_SOCKET, SO_ERROR, &result, &result_len) < 0) {
    		cerr << "SocketClient::Init(): Socket connection failed!" << endl;
				return -1;
			}

			if (result != 0) { // connection failed; error code is in 'result'
    		cerr << "SocketClient::Init(): Socket connection failed!" << endl;
				return -1;
			}
				
			cout << "SocketClient::Init(): Socket connection opened with success" << endl; 
		}//close if select
		else{
			cerr << "SocketClient::Init(): Socket connection failed!" << endl;
			return -1;
		}
  }//close if errno == EINPROGRESS 
  else if (retval < 0) {
		cerr << "SocketClient::Init(): Socket connect() returned error: " <<strerror(errno)<<" !"<< endl; 
		return -1;
  }//close else if
  else {
		cout << "SocketClient::Init(): Connection open with success"<< endl; 
  }//close else

 	return 0;

}//close Init()

ssize_t SocketClient::Write(const char* buffer, size_t bufsize) {
	if(bufsize<=0) return 0;
  return write(m_sockfd, buffer, bufsize);
}//close Write()

ssize_t SocketClient::Read(char* buffer, size_t bufsize, int timeout) {
	
	//Indefinite wait read
  if (timeout <= 0) {
    ssize_t n= read(m_sockfd, buffer, bufsize);
    return n;
  }
    
	//Read with timeout
  if (WaitForSocketReadEvent(timeout) == true) {
    return read(m_sockfd, buffer, bufsize);
  }
  return eConnectionTimedOut;

}//close Read()


bool SocketClient::WaitForSocketReadEvent(int timeout) {
    
  fd_set sdset;
  struct timeval tv;

  tv.tv_sec = timeout;
  tv.tv_usec = 0;
  FD_ZERO(&sdset);
  FD_SET(m_sockfd, &sdset);
  if (select(m_sockfd+1, &sdset, NULL, NULL, &tv) > 0) {
    return true;
  }
  return false;
}

}//close namespace

