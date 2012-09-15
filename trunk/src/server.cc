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

bool parser_http_header_getter(const char *header, size_t len,
                                std::string *x, std::string *y, std::string *t) {
    const char *pos_start, *pos_end;
    for (pos_start = header; pos_start < header + len - 2; pos_start++) {
        if (*pos_start == 'G' && *(pos_start + 1) == 'E' &&
            *(pos_start + 2) == 'T') {
            break;
        }
    }
    pos_start += 4;
    for (pos_end = pos_start; pos_end < header + len; pos_end++) {
        if (*pos_end == ' ')
            break;
    }

    if (pos_end == header + len) {
        return false;
    }
    
    const char *ptr = pos_start + 1;
    int flag = 0;
    while (1) {
        printf("....%c\n", *ptr);
        if (ptr == pos_end) break;
        if (flag == 0) {
            if (*ptr == 'x') {
                flag = 1;
            } else if (*ptr == 'y') {
                flag = 2;
            } else if (*ptr == 't') {
                flag = 3;
            }
            ptr += 1;
        } else {
            if (*ptr == '&') {
                flag = 0;
            } else {
                printf("....%c\n", *ptr);
                if (flag == 1) x->push_back(*ptr);  
                else if (flag == 2) y->push_back(*ptr);
                else if (flag == 3) t->push_back(*ptr);
            }
        }
        ptr++;
    }
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
    
    std::string x,y,t;
    parser_http_header_getter(buff, recvbytes, &x, &y, &t);

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
