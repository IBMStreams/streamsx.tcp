/*
# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2012 
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.
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
