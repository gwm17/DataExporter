#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "ThreadSafeQueue.h"
#include "ServerMessage.h"

#include <memory>
#include <thread>

#include <boost/asio.hpp>

namespace DataExporter
{

    // Abstraction representing a connection to our server.
    // Allows the server to worry only about handling opening/closing connections, rather
    // than actually transmitting data
    class TCPServerConnection
    {
    public:
        using Ref = std::shared_ptr<TCPServerConnection>;

        TCPServerConnection(asio::io_context &context, asio::ip::tcp::socket socket);
        ~TCPServerConnection();

        bool IsConnected() { return m_socket.is_open(); }
        void Send(const ServerMessage &message);

        void Disconnect();

    private:
        void WriteHeader();
        void WriteBody();

        boost::asio::ip::tcp::socket m_socket;
        boost::asio::io_context &m_ioContextHandle;

        ThreadSafeQueue<ServerMessage> m_queue;
    };

    // Our actual server. Server basically does two things: listen for connections, and post data to active connections (clients)
    // We leverage the async powers of asio. This way our program doesn't block and wait when posting data to the clients.
    class TCPServer
    {
    public:
        TCPServer(uint16_t serverPort);
        ~TCPServer();

        void StartServer();
        void ShutdownServer();

        void MessageClients(const ServerMessage &message);

        bool IsActive() const { return m_acceptor.is_open(); }

    private:
        void WaitForClient();

        uint16_t m_acceptorPort;

        boost::asio::io_context m_context;
        boost::asio::ip::tcp::acceptor m_acceptor;

        std::vector<TCPServerConnection::Ref> m_clients;

        std::thread m_ioThread;
    };
}

#endif