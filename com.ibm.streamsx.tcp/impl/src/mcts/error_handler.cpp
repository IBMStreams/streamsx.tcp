/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/
#include "mcts/error_handler.h"

namespace mcts 
{
	ErrorHandler::ErrorHandler(Handler handler)
        : handler_(handler)
    {
    }

	void ErrorHandler::handleError(streams_boost::system::error_code const & e, std::string const & ip, uint32_t port)
    {
            handler_( e, ip,  port);
    }

}
