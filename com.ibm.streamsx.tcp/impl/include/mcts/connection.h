/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#ifndef MULTI_CONNECTION_TCP_SERVER_CONNECTION
#define MULTI_CONNECTION_TCP_SERVER_CONNECTION

#include "mcts/data_item.h"
#include "mcts/data_handler.h"
#include "mcts/info_handler.h"

#include <streams_boost/asio.hpp>
#include <streams_boost/array.hpp>
#include <streams_boost/shared_ptr.hpp>
#include <streams_boost/enable_shared_from_this.hpp>
#include <streams_boost/function.hpp>
#include <streams_boost/thread/mutex.hpp>

namespace mcts 
{

/// Represents a single connection from a client.
    class TCPConnection
        : public streams_boost::enable_shared_from_this<TCPConnection>
    {
    public:

    	/// Construct a connection with the given io_service.
        /// @param ioService the IO service
        /// @param handler the handler
        TCPConnection(streams_boost::asio::io_service& ioService, uint32_t blockSize, outFormat_t outFormat,
                      DataHandler & dHandler,  InfoHandler & iHandler);

        /// Destructor
        ~TCPConnection();
        
        static uint32_t getNumberOfConnections() 
        {
            return numConnections_;
        }

        /// Get the socket associated with the connection.
        streams_boost::asio::ip::tcp::socket & socket();

        /// Start the first asynchronous operation for the connection.
        void start();

    private:
        /// Handle completion of a read operation.
        void handleRead(const streams_boost::system::error_code& e,
                        std::size_t bytesTransferred);
        
        /// Socket for the connection.
        streams_boost::asio::ip::tcp::socket socket_;

        /// The handler used to process the incoming request.
        DataHandler & dataHandler_;

        /// The handler used to process the status messages (connect, disconnect, reject).
        InfoHandler & infoHandler_;

        /// Buffer for incoming data.
        streams_boost::array<char, 8192> buffer_;

        /// Buffer for outgoing data.
        streams_boost::array<char, streams_boost::asio::detail::default_max_transfer_size> bufferToSend_;

        /// The incoming data item
        DataItem dataItem_;

        /// IP address of the remote host
        std::string remoteIp_;

        /// Port of the remote host
        uint32_t remotePort_;

        // Blocksize
        uint32_t blockSize_;

        // output format
        outFormat_t outFormat_;

        static uint32_t numConnections_;

        streams_boost::mutex mutex_;


    };
    
    typedef streams_boost::shared_ptr<TCPConnection> TCPConnectionPtr;
    typedef streams_boost::weak_ptr<TCPConnection> TCPConnectionWeakPtr;
} 

#endif /* MULTI_CONNECTION_TCP_SERVER_CONNECTION */
