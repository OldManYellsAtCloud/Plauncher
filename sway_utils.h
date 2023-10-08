#ifndef SWAY_UTILS_H
#define SWAY_UTILS_H

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

std::string send_sway_message(const std::string& message, const message_type& payload_type, const int& timeout_sec = 3, void(*callback)(std::string) = nullptr);

#endif // SWAY_UTILS_H
