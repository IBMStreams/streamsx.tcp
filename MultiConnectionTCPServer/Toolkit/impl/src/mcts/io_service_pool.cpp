/*
# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2012 
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.
*/

#include "mcts/io_service_pool.h"

#include <stdexcept>
#include <streams_boost/thread.hpp>
#include <streams_boost/bind.hpp>
#include <streams_boost/shared_ptr.hpp>

namespace mcts 
{

    io_service_pool::io_service_pool(std::size_t poolSize)
        : nextIOService_(0)
    {
        if (poolSize == 0)
            throw std::runtime_error("io_service_pool size is 0");

        // Give all the io_services work to do so that their run() functions will not
        // exit until they are explicitly stopped.
        for (std::size_t i = 0; i < poolSize; ++i) {
            io_service_ptr ioService(new streams_boost::asio::io_service);
            work_ptr work(new streams_boost::asio::io_service::work(*ioService));
            ioServices_.push_back(ioService);
            work_.push_back(work);
        }
    }

    void io_service_pool::run()
    {
        // Create a pool of threads to run all of the io_services.
        std::vector<streams_boost::shared_ptr<streams_boost::thread> > threads;
        for (std::size_t i = 0; i < ioServices_.size(); ++i) {
            streams_boost::shared_ptr<streams_boost::thread> thread(new streams_boost::thread(
                streams_boost::bind(&streams_boost::asio::io_service::run, ioServices_[i])));
            threads.push_back(thread);
        }

        // Wait for all threads in the pool to exit.
        for (std::size_t i = 0; i < threads.size(); ++i)
            threads[i]->join();
    }

    void io_service_pool::stop()
    {
        // Explicitly stop all io_services.
        for (std::size_t i = 0; i < ioServices_.size(); ++i)
            ioServices_[i]->stop();
    }
    
    streams_boost::asio::io_service& io_service_pool::get_io_service()
    {
        // Use a round-robin scheme to choose the next io_service to use.
        streams_boost::asio::io_service & ioService = *ioServices_[nextIOService_];
        ++nextIOService_;
        if (nextIOService_ == ioServices_.size())
            nextIOService_ = 0;
        return ioService;
    }
}
