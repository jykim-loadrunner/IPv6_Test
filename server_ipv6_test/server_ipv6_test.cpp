#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {

    const uint16_t SERVER_PORT = 12345;

    int serverSocket, clientSocket;
    struct sockaddr_in6 serverAddr, clientAddr;
    char serverAddr6_str[40], clientAddr6_str[40];
    socklen_t clientAddrLen = sizeof(clientAddr);

    // IPv6 TCP 소켓 설정
    serverSocket = socket(AF_INET6, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating server socket\n";
        return -1;
    }

    // 서버 주소 설정
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(SERVER_PORT);
    serverAddr.sin6_addr = in6addr_any;

    // 소켓과 주소를 결합
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding server socket\n";
        close(serverSocket);
        return -1;
    }

    // 연결 수신 대기
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening on server socket\n";
        close(serverSocket);
        return -1;
    }

    inet_ntop(AF_INET6, (void *)&serverAddr.sin6_addr, serverAddr6_str, sizeof(serverAddr6_str));
    std::cout << "Server IPv6 : " << serverAddr6_str << std::endl;
    std::cout << "Server waiting for connections...\n";

    // 연결 수락
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
        std::cerr << "Error accepting connection\n";
        close(serverSocket);
        return -1;
    }

    //std::cout << "Connection accepted from: " << inet_ntoa() << std::endl;    
    inet_ntop(AF_INET6, (void *)&clientAddr.sin6_addr, clientAddr6_str, sizeof(clientAddr6_str));
    std::cout << "Connection accepted from: " << clientAddr6_str << std::endl;

    // Client로 데이터 전송
    ;

    // 연결 종료
    close(clientSocket);
    close(serverSocket);

    return 0;
}