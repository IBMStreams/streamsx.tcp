/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#include "mcts/connection.h"

#include "mcts/data_handler.h"
#include "mcts/info_handler.h"

#include <vector>
#include <streams_boost/bind.hpp>


#include <iostream>

namespace mcts 
{
    uint32_t TCPConnection::numConnections_ = 0;

    TCPConnection::TCPConnection(streams_boost::asio::io_service & ioService, uint32_t blockSize, outFormat_t outFormat,
    						DataHandler & dHandler, InfoHandler & iHandler)
        : socket_(ioService),
          strand_(ioService),
          dataHandler_(dHandler),
          infoHandler_(iHandler),
          remoteIp_(""), // initialize with empty string
          blockSize_(blockSize),
          outFormat_(outFormat),
          numOutstandingWrites_(0),
          isShutdown_(false)
    {
        __sync_fetch_and_add(&numConnections_, 1);
    }

    TCPConnection::~TCPConnection()
    {
        __sync_fetch_and_sub(&numConnections_, 1);   
        if (remoteIp_.size() == 0) {
            infoHandler_.handleInfo("rejected",
            						socket_.remote_endpoint().address().to_string(),
            						socket_.remote_endpoint().port());
        }
        else {

            dataItem_.flushData(TCPConnection::outFormat_);
            if (dataItem_.hasCompleteItems()) {
                dataHandler_.handleData(dataItem_, remoteIp_, remotePort_);
                dataItem_.removeCompleteItems();
            } 
            infoHandler_.handleInfo("disconnected", remoteIp_, remotePort_);
        }
    }

    uint32_t * TCPConnection::getNumOutstandingWritesPtr()
    {
        return &numOutstandingWrites_;
    }

    std::string & TCPConnection::remoteIp()
    {
        return remoteIp_;
    }

    uint32_t TCPConnection::remotePort()
    {
        return remotePort_;
    }

    streams_boost::asio::ip::tcp::socket & TCPConnection::socket()
    {
        return socket_;
    }

    streams_boost::asio::io_service::strand & TCPConnection::strand()
    {
        return strand_;
    }

    void TCPConnection::start()
    {
        remoteIp_ = socket_.remote_endpoint().address().to_string();
        remotePort_ = socket_.remote_endpoint().port();
        infoHandler_.handleInfo("connected", remoteIp_, remotePort_);


        socket_.async_read_some(streams_boost::asio::buffer(buffer_),
                                streams_boost::bind(&TCPConnection::handleRead, shared_from_this(),
                                                    streams_boost::asio::placeholders::error,
                                                    streams_boost::asio::placeholders::bytes_transferred));
    }

    void TCPConnection::shutdown_conn(bool makeConnReadOnly, bool closeSocket)
    {
    	if(!isShutdown_) {
    		isShutdown_ = true;
    		streams_boost::system::error_code ec;
			if(makeConnReadOnly){
				socket_.shutdown(streams_boost::asio::ip::tcp::socket::shutdown_send, ec);
			}
			else {
				socket_.cancel(ec);
				socket_.shutdown(streams_boost::asio::ip::tcp::socket::shutdown_both, ec);
				if(closeSocket) socket_.close();
			}
    	}
    }

    void TCPConnection::handleRead(streams_boost::system::error_code const & e,
                                std::size_t bytesTransferred)
    {
        if (!e) {
            dataItem_.addData(buffer_.data(), buffer_.data() + bytesTransferred, TCPConnection::blockSize_, TCPConnection::outFormat_);
            if (dataItem_.hasCompleteItems()) {
                dataHandler_.handleData(dataItem_, remoteIp_, remotePort_);
                dataItem_.removeCompleteItems();
            } 
            socket_.async_read_some(streams_boost::asio::buffer(buffer_),
                                    streams_boost::bind(&TCPConnection::handleRead, shared_from_this(),
                                                        streams_boost::asio::placeholders::error,
                                                        streams_boost::asio::placeholders::bytes_transferred));
        }

        // If an error occurs then no new asynchronous operations are started. This
        // means that all shared_ptr references to the connection object will
        // disappear and the object will be destroyed automatically after this
        // handler returns. The connection class's destructor closes the socket.
    }
}
