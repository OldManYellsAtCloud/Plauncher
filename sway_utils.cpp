//#include "logger.h"
#include <string>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <sys/time.h>
#include <unistd.h>

#include <QDebug>

#include <memory>
#include "sway_utils.h"

#include <iostream>

#define IPC_HEADER_SIZE  14

void get_sub_response(int sock_fd, std::function<void(std::string)> &callback){
    char data[IPC_HEADER_SIZE];

    memset(data, 0, IPC_HEADER_SIZE);

    uint32_t total;
    int32_t response_length;
    size_t size;

    std::unique_ptr<char[]> buffer;

    do {
        total = 0;

        while (total < IPC_HEADER_SIZE) {
            size = recv(sock_fd, data + total, IPC_HEADER_SIZE - total, 0);
            if (size < 0 ) {
                return;
            }
            total += size;
        }

        memcpy(&response_length, data + 6, sizeof(response_length));

        buffer = std::make_unique<char[]>(response_length + 1);
        //memset(buffer.get(), 0, response_length + 1);

        total = 0;
        while (total < response_length) {
            size = recv(sock_fd, buffer.get() + total, response_length - total, 0);
            if (size < 0 ) {
                return;
            }
            total += size;
        }

        qDebug() << buffer.get();

        callback(buffer.get());

    } while (true);
}


std::string get_ipc_response(int sock_fd){
    char data[IPC_HEADER_SIZE];

    memset(data, 0, IPC_HEADER_SIZE);

    uint32_t total;
    int32_t response_length;
    size_t size;

    std::unique_ptr<char[]> buffer;

    total = 0;

    while (total < IPC_HEADER_SIZE) {
        size = recv(sock_fd, data + total, IPC_HEADER_SIZE - total, 0);
        if (size < 0 ) {
            return NULL;
        }
        total += size;
    }

    memcpy(&response_length, data + 6, sizeof(response_length));

    buffer = std::make_unique<char[]>(response_length + 1);

    total = 0;
    while (total < response_length) {
        size = recv(sock_fd, buffer.get() + total, response_length - total, 0);
        if (size < 0 ) {
            return NULL;
        }
        total += size;
    }

    return std::string(buffer.get());
}


void send_ipc_message(const std::string& message, const int& payload_type, int sock_fd){
    char header[14];
    strcpy(&header[0], "i3-ipc");
    uint32_t message_length = message.length();
    memcpy(&header[6], &message_length, 4);
    memcpy(&header[10], (uint32_t*)&payload_type, 4);

    if (write(sock_fd, &header[0], 14) == -1) {
        qDebug() << "Could not send header";
    }

    if (write(sock_fd, message.c_str(), message_length) == -1) {
        qDebug() << "Could not send message";
    }
}


int init_sway_socket(const int& timeout_sec){
    char* socket_path = getenv("SWAYSOCK");
    if (socket_path == NULL) {
        exit(1);
    }

    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    sockaddr_un address;

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, socket_path);

    if (connect(sock_fd, (sockaddr*)&address, sizeof(address)) == -1) {
        exit(1);
    }

    struct timeval timeout;
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;

    if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        qDebug() << "Could not set timeout on socket: " << strerror(errno);
    }

    return sock_fd;
}

void subscribe_sway_message(std::string message, std::function<void(std::string)> callback){
    int sock_fd = init_sway_socket(0);
    send_ipc_message(message, message_type::SUBSCRIBE, sock_fd);
    bool is_subscription = true;
    get_sub_response(sock_fd, callback);
}

std::string send_sway_message(std::string message, message_type payload_type){
    int sock_fd = init_sway_socket(3);
    send_ipc_message(message, payload_type, sock_fd);
    bool is_subscription = false;
    return get_ipc_response(sock_fd);
}
