/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#ifndef MULTI_CONNECTION_TCP_SERVER_SERVER
#define MULTI_CONNECTION_TCP_SERVER_SERVER

#include <string>
#include <vector>
#include <tr1/unordered_map>
#include <streams_boost/asio.hpp>
#include <streams_boost/shared_ptr.hpp>
#include <streams_boost/thread/mutex.hpp>

#include "mcts/async_data_item.h"
#include "mcts/data_item.h"
#include "mcts/connection.h"
#include "mcts/data_handler.h"
#include "mcts/info_handler.h"
#include "mcts/metrics_handler.h"
#include "mcts/io_service_pool.h"

namespace mcts 
{
    /// Class for multi-threaded TCP server
    class TCPServer
    {
    public:


        /// Construct the server to listen on the specified TCP address and port, and
        /// serve up files from the given directory
        /// @param address address to listen on
        /// @param port port to listen on
        /// @param threadPoolSize number of threads to use for the connections
        /// @param maxConnections maximum number of concurrent connections
        /// @param blockSize maximum number of bytes in sent tuples, dependent on outFormat
        /// @param outFormat : line (condition to send data is "\n") or block (condition to send data is blockSize)
        /// @param handler that will process the data 
        /// @param handler that will send connection status infos
        TCPServer(std::string const & address, uint32_t port, 
                  std::size_t threadPoolSize, std::size_t maxConnections, uint32_t blockSize, outFormat_t outFormat,
                  DataHandler::Handler dhandler, AsyncDataItem::Handler eHandler, InfoHandler::Handler iHandler, MetricsHandler::Handler mHandler);

        /// Set the keep alive socket options
        void setKeepAlive(int32_t time, int32_t probes, int32_t interval);


        /// Run the server's io_service loop
        void run();

        /// Stop the server
        void stop();
        
        /// Handle asynchronous write operation
        void handleWrite(SPL::blob & raw, bool delimited, std::string const & ipAddress, uint32_t port);

        void mapConnection(TCPConnectionWeakPtr conn);

    private:
        /// Handle completion of an asynchronous accept operation
        /// @param e the error code of the operation
        void handleAccept(streams_boost::system::error_code const & e);
        
        /// The number of threads that will call io_service::run()
        std::size_t threadPoolSize_;

        /// The number of concurrent connections allowed
        std::size_t maxConnections_;

        /// The maximum size of receive buffer, flush buffer in case of overflow
        uint32_t blockSize_;

        /// The number of seconds that the connection sits idle before a keepalive probe is sent
        std::size_t keepAliveIdleTime_;

        /// The maximum number of probes to send to establish the state of the connection
        std::size_t keepAliveMaxProbesCnt_;

        /// The time interval in seconds between each probe
        std::size_t keepAliveProbeInterval_;

        /// Service pool used for async operations
        io_service_pool ioServicePool_;
        
        /// Acceptor used to listen for incoming connections.
        streams_boost::asio::ip::tcp::acceptor acceptor_;
        
        /// The handler used to process the status messages (connect, disconnect, reject).
        InfoHandler infoHandler_;

        /// The handler for all incoming requests.
        DataHandler dataHandler_;

        /// The handler used to process the error messages.
        AsyncDataItem errorHandler_;

        /// The handler used to process the metrics.
        MetricsHandler metricsHandler_;

        /// The format output data: line (as rstring) or block (as blob)
        outFormat_t outFormat_;

        /// The next connection to be accepted.
        TCPConnectionPtr nextConnection_;

        /// TCPConnection map to handle duplex communication
        std::tr1::unordered_map<std::string, TCPConnectionWeakPtr> connMap_;

        streams_boost::mutex mutex_;
    };  
} 

#endif /* MULTI_CONNECTION_TCP_SERVER_SERVER */
