/*
# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2012 
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.
*/

#ifndef MULTI_CONNECTION_TCP_SERVER_IO_SERVICE_POOL
#define MULTI_CONNECTION_TCP_SERVER_IO_SERVICE_POOL

#include <streams_boost/asio.hpp>
#include <vector>
#include <streams_boost/shared_ptr.hpp>

namespace mcts 
{
    /// Represents an io service pool
    class io_service_pool
    {
    public:
        /// Construct the io_service pool.
        io_service_pool(std::size_t poolSize);
        
        /// Run all io_service objects in the pool.
        void run();
        
        /// Stop all io_service objects in the pool
        void stop();
        
        /// Get an io_service to use
        streams_boost::asio::io_service & get_io_service();
        
    private:
        typedef streams_boost::shared_ptr<streams_boost::asio::io_service> io_service_ptr;
        typedef streams_boost::shared_ptr<streams_boost::asio::io_service::work> work_ptr;
        
        /// The pool of io_services
        std::vector<io_service_ptr> ioServices_;
        
        /// The work that keeps the io_services running
        std::vector<work_ptr> work_;
        
        /// The next io_service to use for a connection
        std::size_t nextIOService_;
    };

} 

#endif /* MULTI_CONNECTION_TCP_SERVER_IO_SERVICE_POOL */
