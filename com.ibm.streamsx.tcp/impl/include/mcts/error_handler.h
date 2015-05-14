/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#ifndef MULTI_CONNECTION_TCP_SERVER_ERROR_HANDLER
#define MULTI_CONNECTION_TCP_SERVER_ERROR_HANDLER

#include <streams_boost/system/error_code.hpp>
#include <streams_boost/function.hpp>
#include <string>

namespace mcts 
{
    ///

	class ErrorHandler
    {
    public:
        typedef streams_boost::function<void (streams_boost::system::error_code const & e, std::string const & ip, uint32_t port)> Handler;
        ErrorHandler(Handler handler);
        void handleError(streams_boost::system::error_code const & e, std::string const & ip, uint32_t port);
    private:
        Handler handler_;
    };
}

#endif /* MULTI_CONNECTION_TCP_SERVER_CONN_ERROR_HANDLER */

