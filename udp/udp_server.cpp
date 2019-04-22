// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <iostream>


class UDPServer{
private:
	std::string _addr;
	int _port;
	struct sockaddr_in _servaddr;
	struct sockaddr_in _cliaddr;
	socklen_t _cliaddrlen;
	int _sockfd;
	const int _maxlen = 1024;
	bool _connected = false;

public:
	UDPServer(std::string addr, int port);
	std::string GetAddress();
	int GetPort();
	bool IsConnected() const;
	void ConnectClient();
	template<typename T>
	void Send(T* data, int ndata);
};

UDPServer::UDPServer(std::string addr, int port): _port(port), _addr(addr){

	// Initialize socket
	_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (_sockfd < 0){
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Reset and fill server address structs
	memset(&_servaddr, 0, sizeof(_servaddr));
	memset(&_cliaddr, 0, sizeof(_cliaddr));

	_servaddr.sin_family = AF_INET;
	_servaddr.sin_addr.s_addr = INADDR_ANY;
	_servaddr.sin_port = htons(port);

	// Bind the socket with the server address
	if (bind(_sockfd, (const struct sockaddr *)&_servaddr,sizeof(_servaddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}

std::string UDPServer::GetAddress(){
	return _addr;
}

int UDPServer::GetPort(){
	return _port;
}

bool UDPServer::IsConnected() const{
	return _connected;
}

void UDPServer::ConnectClient(){
	char buffer[_maxlen];
	int n;
	n = recvfrom(_sockfd, (char *)buffer, _maxlen,
							 MSG_WAITALL, ( struct sockaddr *) &_cliaddr, &_cliaddrlen);
	buffer[n] = '\0';
	printf("Client: %s\n", buffer);
	printf("Client Connected");
}

template<typename T>
void UDPServer::Send(T* data, int ndata){
	if (_connected){
		sendto(_sockfd, (const T *) data, sizeof(data)*ndata,
  				 MSG_DONTWAIT, (const struct sockaddr *) &_cliaddr, _cliaddrlen);
	}
}

// Driver code
int main() {

	std::string addr;
	int port;

	std::cout << "Enter IP Address" << std::endl;
	std::cin >> addr;

	std::cout << "Enter Port" << std::endl;
	std::cin >> port;

	double data[2] = {2.87, 3.14};
	UDPServer server(addr, port);
	server.ConnectClient();
	server.Send(data, 2);

	return 0;
}
