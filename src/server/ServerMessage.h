#ifndef SERVER_MESSAGE_H
#define SERVER_MESSAGE_H

#include <vector>

namespace DataExporter
{

    struct ServerMessage
    {
        ServerMessage() = default;
        ServerMessage(const std::vector<uint8_t> &data)
        {
            size = data.size() * sizeof(uint8_t);
            body = data;
        }

        uint64_t size = 0;
        std::vector<uint8_t> body;
    };

}

#endif