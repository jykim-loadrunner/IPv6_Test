#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {

    const uint16_t SERVER_PORT = 8090;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    // 주소 설정
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(SERVER_PORT);

    // 소켓을 주소에 바인드
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket to address\n";
        close(serverSocket);
        return -1;
    }

    // 연결 대기
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening for connections\n";
        close(serverSocket);
        return -1;
    }

    std::cout << "Server listening on port " << SERVER_PORT << "...\n";
    
    // 클라이언트 연결 수락
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        std::cerr << "Error accepting for connections\n";
        close(serverSocket);
        return -1;
    }

    // Client에서 보낸 데이터 출력
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    buffer[bytesRead] = '\0';
    std::cout << "Received : " << buffer << std::endl;

    char response[1024];
    sprintf(response, "Hello from server! Your data : %s", buffer);
    if (send(clientSocket, response, strlen(response), 0) == -1) {
        std::cerr << "Error sending response\n";
    }

    // char buffer[1024];
    // int bytesRead;
    // while (true) {
    //     memset(buffer, 0, sizeof(buffer));
    //     while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
    //         buffer[bytesRead] = '\0';
    //         std::cout << "Received : " << buffer << std::endl;
    //     }
    // }

    std::cout << "Connection closed\n" << std::endl;

    // 연결 종료
    close(clientSocket);
    close(serverSocket);

    return 0;
}