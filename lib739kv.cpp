#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define NUM_SERVER 3;
#define BUFFER_SIZE 1024;

char* port;
char* host;
int sockets[NUM_SERVER];
struct sockaddr_in server_addrs[NUM_SERVER];
char buffer[1024] = {0};

int kv739_init(char ** server_list)
{
	// go through server list
	size_t i;
	// init all servers with name host:post
	while (server_list[i] != NULL) {
		// extract host & port
		char* server_name = strtok(server_list[i], ":");			
	}
	return 0;
}

int init_single_server(int server_id, char* host, char* port)
{
	int sock = sockets[server_id];
	struct sockaddr_in server_addr = server_addrs[server_id];

	// start socket
	if ((socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "Socket creation error" << std::endl;
		return -1
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	// convert IPv4 addr from text to binary
	if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0)
        {
		return -1;
	}

	// establish connection
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		return -1;
	}

}

int kv739_put(char * key, char * value, char * old_value)
{

/*
	char msg[strlen(key) + strlen(value) + strlen(old_value) + 2];
	strcpy(msg, key);
	strcat(msg, ",");
	strcat(msg, value);
	strcat(msg, ",");
	strcat(msg, old_value);
*/
	
	send_msg(0, msg);
	return 0;
}

int kv739_get(char * key, char * value)
{
	string url = "http//\{" + host + "\}/kv/?k=\{" + key + "\}";
	char* valread[BUFFER_SIZE];
	send_msg(0, msg, valread);
	return 0;
}

void send_msg(int server_id, char* msg, char* valread)
{
	send(sockets[server_id], msg, strlen(msg));
	strcpy(valraed, read(sockets[server_id], buffer, 1024));
	
}
