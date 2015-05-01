/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#ifndef MULTI_CONNECTION_TCP_SERVER_REQUEST_HANDLER
#define MULTI_CONNECTION_TCP_SERVER_REQUEST_HANDLER

#include <streams_boost/function.hpp>
#include <string>

namespace mcts 
{
    class DataItem;

    /// Represents the handler that will consume the data
    class DataHandler 
    {
    public:
        typedef streams_boost::function<void (std::string & line, std::string const & ip, uint32_t port)> Handler;
        DataHandler(Handler handler);
        void handleData(DataItem & dataItem, std::string const & ip, uint32_t port);
    private:
        Handler handler_;
    };
}

#endif /* MULTI_CONNECTION_TCP_SERVER_REQUEST_HANDLER */
