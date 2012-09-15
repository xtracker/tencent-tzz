#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string>

const int MAX_BUFF_SIZE = 1024;

bool init_socket_server(int *server_sockfd) {
	int &sockfd = *server_sockfd;
	struct sockaddr_in my_addr;
	//struct sockaddr_in remote_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("...");
        return false;
	}
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(3111);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero), 8);
	if (bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("....bind error");
        return false;
    }
	if (listen(sockfd, 10) == -1) {
		perror(".....");
        return false;
	}
    printf("Init Server Ok...!\nwait for client...\n");
    return true;
}

void* accept_client(void *p_sockfd) {
    int sockfd = *(int*)p_sockfd;
    //printf("doing somthing stuff...\n");
    char buff[MAX_BUFF_SIZE];
    int recvbytes;
    recvbytes = recv(sockfd, buff, MAX_BUFF_SIZE, 0);
    buff[recvbytes] = '\0';
    printf("%s\n", buff);
    
    std::string mock_json = "{\"title\":\"test_seed\"}";
    send(sockfd, mock_json.c_str(), mock_json.size(), 0);
    close(sockfd);
}

int main() {
    int sockfd;
    init_socket_server(&sockfd);
    while (1) {
        int client_fd;
        if ((client_fd = accept(sockfd, NULL, NULL)) == -1) {
            perror("accept error...\n"); 
            continue;
        }
        pthread_t c_thread;
        if (pthread_create(&c_thread, NULL, accept_client, &client_fd) != 0) {
            perror("pthread_create");
        }
        //close(client_fd);
    }
}
