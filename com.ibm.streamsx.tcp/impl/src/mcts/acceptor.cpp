/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#include "mcts/acceptor.h"

#include <streams_boost/bind.hpp>
#include <streams_boost/lexical_cast.hpp>

#include <iostream>

namespace mcts 
{
    TCPAcceptor::TCPAcceptor(streams_boost::asio::io_service & ioService, std::string const & address, uint32_t port) : acceptor_(ioService)
    {
		streams_boost::asio::ip::tcp::resolver resolver(ioService);
		streams_boost::asio::ip::tcp::resolver::query query(address, streams_boost::lexical_cast<std::string>(port));
		streams_boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
		acceptor_.open(endpoint.protocol());
		acceptor_.set_option(streams_boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor_.bind(endpoint);
		acceptor_.listen();
    }

    streams_boost::asio::ip::tcp::acceptor & TCPAcceptor::getAcceptor()
    {
    	return acceptor_;
    }

    TCPConnectionPtr & TCPAcceptor::nextConnection()
    {
    	return nextConnection_;
    }
}
