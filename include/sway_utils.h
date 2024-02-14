#ifndef SWAY_UTILS_H
#define SWAY_UTILS_H

#include <functional>

enum message_type {
    RUN_COMMAND = 0,
    GET_WORKSPACES = 1,
    SUBSCRIBE = 2,
    GET_OUTPUTS = 3,
    GET_TREE = 4,
    GET_MARKS,
    GET_BAR_CONFIG,
    GET_VERSION,
    GET_BINDING_MODES,
    GET_CONFIG,
    SEND_TICK,
    SYNC,
    GET_BINDING_STATE = 12,
    GET_INPUTS = 100,
    GET_SEATS = 101
};

std::string send_sway_message(std::string message, message_type payload_type);
void subscribe_sway_message(std::string message, std::function<void(std::string)> callback);

#endif // SWAY_UTILS_H
