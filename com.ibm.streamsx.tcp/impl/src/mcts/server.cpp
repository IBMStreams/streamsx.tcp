/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#include "mcts/server.h"

#include <streams_boost/thread.hpp>
#include <streams_boost/bind.hpp>
#include <streams_boost/make_shared.hpp>
#include <streams_boost/shared_ptr.hpp>
#include <streams_boost/weak_ptr.hpp>
#include <streams_boost/lexical_cast.hpp>
#include <vector>
#include <iostream>

#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/select.h>
#include <netinet/tcp.h>

namespace mcts 
{
    TCPServer::TCPServer(std::string const & address, uint32_t port, 
                         std::size_t threadPoolSize,
                         std::size_t maxConnections,
                         uint32_t blockSize,
                         outFormat_t outFormat,
                         DataHandler::Handler dHandler,
                         AsyncDataItem::Handler eHandler,
                         InfoHandler::Handler iHandler,
                         MetricsHandler::Handler mHandler)
        : threadPoolSize_(threadPoolSize),
          maxConnections_(maxConnections),
          blockSize_(blockSize),
          keepAliveIdleTime_(0),
          keepAliveMaxProbesCnt_(0),
          keepAliveProbeInterval_(0),
          ioServicePool_(threadPoolSize),
          acceptor_(ioServicePool_.get_io_service()),
          infoHandler_(iHandler),
          dataHandler_(dHandler),
          errorHandler_(eHandler),
          metricsHandler_(mHandler),
          outFormat_(outFormat),
    	nextConnection_(new TCPConnection(ioServicePool_.get_io_service(), blockSize_, outFormat_, dataHandler_, infoHandler_))
    {
    	streams_boost::asio::ip::tcp::resolver resolver(acceptor_.get_io_service());
        streams_boost::asio::ip::tcp::resolver::query query(address, streams_boost::lexical_cast<std::string>(port));
        streams_boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(streams_boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
        acceptor_.async_accept(nextConnection_->socket(),
                               streams_boost::bind(&TCPServer::handleAccept, this,
                                                   streams_boost::asio::placeholders::error));
    }
    
    void TCPServer::setKeepAlive(int32_t time, int32_t probes, int32_t interval)
    {
       // Let us set the Keep Alive socket options into our member variables.
       if (time > 0)
       {
          keepAliveIdleTime_ = time;
       }

       if (probes > 0)
       {
          keepAliveMaxProbesCnt_ = probes;
       }
      
       if (interval > 0)
       {
          keepAliveProbeInterval_ = interval;
       }

       /*
       std::cout << "keepAliveIdleTime = " << keepAliveIdleTime_ << ", keepAliveMaxProbesCnt = " <<
          keepAliveMaxProbesCnt_ << ", keepAliveProbeInterval = " << keepAliveProbeInterval_ << std::endl;
       */
    }

    void TCPServer::run()
    {
        ioServicePool_.run();
    }
    
    void TCPServer::stop()
    {
        ioServicePool_.stop();
    }

    void TCPServer::handleAccept(streams_boost::system::error_code const & e)
    {
        if (!e) {
        	if (TCPConnection::getNumberOfConnections()<=maxConnections_) {
        		nextConnection_->start();

        		// Add a new connection to the response connection map
        		mapConnection(nextConnection_);
        		// Update number of open connections metric
        		metricsHandler_.handleMetrics((int64_t)mcts::TCPConnection::getNumberOfConnections());


        		// Set the KeepAlive values as given by the user.
        		if (keepAliveIdleTime_ || keepAliveMaxProbesCnt_ || keepAliveProbeInterval_) {
        			int32_t _fd = static_cast<int32_t> (nextConnection_->socket().native());
        			// std::cout << "Boost socket to native descriptor _fd = " << _fd << std::endl;

        			int32_t valopt = 1;
        			socklen_t len = sizeof(valopt);
        			if (setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, &valopt, len) < 0) {
        				// std::cout << "TCPServer: SO_KEEPALIVE failed: " << std::endl;
        			}

        			if (keepAliveIdleTime_) {
        				valopt = keepAliveIdleTime_;
        				if (setsockopt(_fd, IPPROTO_TCP, TCP_KEEPIDLE, &valopt, len) < 0) {
        					// std::cout << "TCPServer: TCP_KEEPIDLE failed: " << std::endl;
        				}
        			}

        			if (keepAliveMaxProbesCnt_) {
        				valopt = keepAliveMaxProbesCnt_;
        				if (setsockopt(_fd, IPPROTO_TCP, TCP_KEEPCNT, &valopt, len) < 0) {
        					// std::cout << "TCPServer: TCP_KEEPCNT failed: " << std::endl;
        				}
        			}

        			if (keepAliveProbeInterval_) {
        				valopt = keepAliveProbeInterval_;
        				if (setsockopt(_fd, IPPROTO_TCP, TCP_KEEPINTVL, &valopt, len) < 0) {
        					// std::cout << "TCPServer: TCP_KEEPINTVL failed: " << std::endl;
        				}
        			}

        			// std::cout << "All KeepAlive values were set correctly." << std::endl;
        		}
        	}


        	nextConnection_.reset(new TCPConnection(ioServicePool_.get_io_service(), blockSize_, outFormat_, dataHandler_, infoHandler_));
            acceptor_.async_accept(nextConnection_->socket(),
                                   streams_boost::bind(&TCPServer::handleAccept, this,
                                                       streams_boost::asio::placeholders::error));
        }
    }

    void TCPServer::handleWrite(SPL::blob & raw, bool delimited, std::string const & ipAddress, uint32_t port)
    {
    	std::stringstream connKey;
    	connKey << ipAddress << ":" << port;
		bool connExist = false;
		TCPConnectionWeakPtr connWeakPtr;

    	{
			#if (((STREAMS_BOOST_VERSION / 100) % 1000) < 53)
				streams_boost::mutex::scoped_lock scoped_lock(mutex_);
			#else
				streams_boost::unique_lock<streams_boost::mutex> scoped_lock(mutex_);
			#endif

			connExist = connMap_.count(connKey.str()) != 0;

			if (connExist) connWeakPtr = connMap_[connKey.str()];
    	}

    	if(connExist) {

    		AsyncDataItemPtr asyncDataItemPtr = streams_boost::make_shared<AsyncDataItem>(errorHandler_);
    		asyncDataItemPtr->setConnectionPtr(connWeakPtr);
    		TCPConnectionPtr connPtr;

    		/// Validate existing connection
			if (asyncDataItemPtr->getValidConnection(connPtr)) {

				asyncDataItemPtr->setData(raw);

				async_write(connPtr->socket(), streams_boost::asio::buffer(asyncDataItemPtr->getData(), asyncDataItemPtr->getSize()),
						connPtr->strand().wrap(
							streams_boost::bind(&AsyncDataItem::handleError, asyncDataItemPtr,
												streams_boost::asio::placeholders::error,
												ipAddress, port)
						)
				);
//				async_write(connPtr->socket(), streams_boost::asio::buffer(asyncDataItemPtr->getData(), asyncDataItemPtr->getSize()),
//						streams_boost::bind(&AsyncDataItem::handleError, asyncDataItemPtr,
//											streams_boost::asio::placeholders::error,
//											ipAddress, port));

				return;
			}

//			if(TCPConnectionPtr connPtr = connWeakPtr.lock()) {
//
//				if (connPtr->socket().is_open()) {
//
//					#if (((STREAMS_BOOST_VERSION / 100) % 1000) < 53)
//						streams_boost::mutex::scoped_lock data_scoped_lock(connPtr->mutex_);
//					#else
//						streams_boost::unique_lock<streams_boost::mutex> data_scoped_lock(connPtr->mutex_);
//					#endif
//
//					uint64_t size = raw.getSize();
//					if(raw.ownsData()) {
//						connPtr->bufferToSend_.adoptData(raw.releaseData(size), size);
//					}
//					else {
//						connPtr->bufferToSend_.adoptData(const_cast<unsigned char *>(raw.getData()), size);
//					}
//
//					async_write(connPtr->socket(), streams_boost::asio::buffer(connPtr->bufferToSend_.getData(), size),
//							streams_boost::bind(&ErrorHandler::handleError, errorHandler_,
//												streams_boost::asio::placeholders::error,
//												ipAddress, port));
//
//					return;
//				}
//			}

			#if (((STREAMS_BOOST_VERSION / 100) % 1000) < 53)
				streams_boost::mutex::scoped_lock scoped_lock(mutex_);
			#else
				streams_boost::unique_lock<streams_boost::mutex> scoped_lock(mutex_);
			#endif

			connMap_.erase(connKey.str());
    	}

		errorHandler_.handleError(streams_boost::system::error_code(streams_boost::asio::error::connection_aborted), ipAddress, port);
    }

    void TCPServer::mapConnection(TCPConnectionWeakPtr connPtr)
    {
    	std::stringstream connKey;
    	if(TCPConnectionPtr connTmpPtr = connPtr.lock()) {
			connKey << connTmpPtr->remoteIp() << ":" << connTmpPtr->remotePort();

			#if (((STREAMS_BOOST_VERSION / 100) % 1000) < 53)
				streams_boost::mutex::scoped_lock scoped_lock(mutex_);
			#else
				streams_boost::unique_lock<streams_boost::mutex> scoped_lock(mutex_);
			#endif

			connMap_[connKey.str()] = connPtr;
    	}
    }
}


