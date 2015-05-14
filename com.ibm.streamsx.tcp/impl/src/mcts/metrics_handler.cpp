/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/
#include "mcts/metrics_handler.h"

namespace mcts 
{
	MetricsHandler::MetricsHandler(Handler handler)
        : handler_(handler)
    {
    }

	void MetricsHandler::handleMetrics(int64_t nConn)
    {
            handler_( nConn);
    }

}
