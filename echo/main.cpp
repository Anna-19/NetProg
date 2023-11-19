#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0); 

    if (server_socket == -1) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return 1;
    }

    sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(7777); 

    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Ошибка при привязке сокета" << std::endl;
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 5) == -1) {
        std::cerr << "Ошибка при прослушивании сокета" << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Сервер службы echo запущен" << std::endl;

    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_len);
    if (client_socket == -1) {
        std::cerr << "Ошибка при принятии соединения" << std::endl;
        close(server_socket);
        return 1;
    }

    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

    if (bytes_received == -1) {
        std::cerr << "Ошибка при получении данных" << std::endl;
    } else {
        send(client_socket, buffer, bytes_received, 0);
        std::cout << "Сообщение отправлено клиенту " << std::endl;
    }

    close(client_socket);
    close(server_socket);

    return 0;
}

