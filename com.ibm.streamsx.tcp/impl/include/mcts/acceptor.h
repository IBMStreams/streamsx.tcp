/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#ifndef MULTI_CONNECTION_TCP_SERVER_ACCEPTOR
#define MULTI_CONNECTION_TCP_SERVER_ACCEPTOR

#include "mcts/connection.h"

#include <streams_boost/asio.hpp>
#include <streams_boost/shared_ptr.hpp>
#include <streams_boost/enable_shared_from_this.hpp>

namespace mcts 
{

/// Represents a single listener acceptor.
    class TCPAcceptor
        : public streams_boost::enable_shared_from_this<TCPAcceptor>
    {
    public:
    	/// Construct an acceptor with the given io_service.
        /// @param ioService the IO service
        TCPAcceptor(streams_boost::asio::io_service& ioService, std::string const & address, uint32_t port);

        /// Get the acceptor.
        streams_boost::asio::ip::tcp::acceptor & getAcceptor();

        /// Get the next connection.
        TCPConnectionPtr & nextConnection();

    private:
        /// The next connection to be accepted.
        TCPConnectionPtr nextConnection_;

        /// Acceptor used to listen for incoming connections.
        streams_boost::asio::ip::tcp::acceptor acceptor_;
    };
    
    typedef streams_boost::shared_ptr<TCPAcceptor> TCPAcceptorPtr;
} 

#endif /* MULTI_CONNECTION_TCP_SERVER_ACCEPTOR */
