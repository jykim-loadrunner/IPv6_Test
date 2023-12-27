#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

int main(int argc, char const *argv[]) {

    //const uint16_t SERVER_PORT = 12345;
    int clientSocket = -1;
    struct sockaddr_in6 serverAddr;

    if (argc < 3) {
        std::cerr << "Argument count should be greater than 3";
        return -1;
    }

    // IPv6 TCP 소켓 생성
    clientSocket = socket(AF_INET6, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating client socket\n";
        return -1;
    }

    // 서버 주소 설정
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, argv[1], &(serverAddr.sin6_addr));
    serverAddr.sin6_port = htons(std::stoi(argv[2]));

    // 서버에 연결
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to the server\n";
        close(clientSocket);
        return -1;
    }

    std::cout << "Connected to the server\n";

    // 서버에 문자 전송
    char buffer[1024];
    int counter = 0;
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%d", counter);
        send(clientSocket, buffer, sizeof(buffer), 0);

        counter = (counter == 9) ? 0 : counter + 1;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "Connection closed\n";

    close(clientSocket);

    return 0;
}