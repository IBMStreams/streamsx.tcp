/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/
#include "mcts/info_handler.h"

namespace mcts 
{
	InfoHandler::InfoHandler(Handler handler)
        : handler_(handler)
    {
    }

	void InfoHandler::handleInfo(std::string const & status, std::string const & ip, uint32_t port)
    {
            handler_( status, ip,  port);
    }

}
