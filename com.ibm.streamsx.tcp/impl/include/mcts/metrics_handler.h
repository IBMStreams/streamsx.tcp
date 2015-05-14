/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#ifndef MULTI_CONNECTION_TCP_SERVER_METRICS_HANDLER
#define MULTI_CONNECTION_TCP_SERVER_METRICS_HANDLER

#include <streams_boost/function.hpp>
#include <string>

namespace mcts 
{
    ///

	class MetricsHandler
    {
    public:
        typedef streams_boost::function<void (int64_t nConn)> Handler;
        MetricsHandler(Handler handler);
        void handleMetrics(int64_t nConn);
    private:
        Handler handler_;
    };
}

#endif /* MULTI_CONNECTION_TCP_SERVER_CONN_METRICS_HANDLER */

