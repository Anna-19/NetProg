#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) {
struct sockaddr_in {
 short sin_family;
 unsigned short sin_port;
 struct in_addr sin_addr;
 char sin_zero[8];
};
struct in_addr {
 unsigned long s_addr;
};

sockaddr_in * addr = new (sockaddr_in);
addr->sin_family = AF_INET; // интернет протокол IPv4
addr->sin_port = htons(44212); // порт 44214
addr->sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

int s = socket(AF_INET, SOCK_STREAM, 0); // TCP

sockaddr_in * self_addr = new (sockaddr_in);
self_addr->sin_family = AF_INET; // интернет протокол IPv4
self_addr->sin_port = htons(44212);
self_addr->sin_addr.s_addr = inet_addr("127.0.0.1");

sockaddr_in * srv_addr = new (sockaddr_in);
srv_addr->sin_family = AF_INET; // интернет протокол IPv4
srv_addr->sin_port = htons(44212);
srv_addr->sin_addr.s_addr = inet_addr("127.0.0.1");

int rc = connect(s,(const sockaddr*)srv_addr, sizeof(sockaddr_in));
if (rc == -1) {
    cout << "Error: failed connect to server.\n";
    return 1;
}
else {
    char msg[50] = "Kalgudaeva";
    send(s, msg, sizeof(msg), 0);
    cout << "Отправлено: " << '"' << msg << "\n";
    char returned_msg[50];
    recv(s, returned_msg, sizeof(returned_msg), 0);
    cout << "Получено: " << '"' << returned_msg << "\n";
}
close(s);
return 0;
}
