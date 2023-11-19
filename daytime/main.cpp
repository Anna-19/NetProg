#include <iostream>
#include <ctime>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_socket == -1) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return 1;
    }

    sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(44214); 

    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Ошибка при привязке сокета" << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Сервер службы daytime запущен" << std::endl;

    char buffer[1024];
    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (sockaddr*)&client_address, &client_address_len);

    if (bytes_received == -1) {
        std::cerr << "Ошибка при получении данных" << std::endl;
    } else {
        time_t raw_time;
        time(&raw_time);
        std::string time_str = ctime(&raw_time);
        sendto(server_socket, time_str.c_str(), time_str.size(), 0, (sockaddr*)&client_address, client_address_len);
        std::cout << "Отправлено клиенту: " << time_str << std::endl;
    }

    close(server_socket);

    return 0;
}
