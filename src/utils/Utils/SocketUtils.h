#ifndef _SOCKET_STREAM_h__
#define _SOCKET_STREAM_h__

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

namespace Utils_ns {


class SocketClient {

  public:
    SocketClient(std::string path,bool isBlocking=false);
    ~SocketClient();

		enum SocketStatus {
    	eConnectionClosed = 0,
      eConnectionReset = -1,
      eConnectionTimedOut = -2
    };

	public:
    int Init();
		int Close();
		int Reset();
    ssize_t Write(const char* buffer, size_t bufsize);
		ssize_t Read(char* buffer, size_t bufsize, int timeout);

	private:
		bool WaitForSocketReadEvent(int timeout);

	private:
		int m_sockfd;
		std::string m_sockpath;
		bool m_isBlocking;
  
};//close SocketClient()

}//close namespace

#endif
