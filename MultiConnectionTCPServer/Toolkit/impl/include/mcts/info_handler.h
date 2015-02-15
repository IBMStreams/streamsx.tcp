/*
# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2012 
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.
*/

#ifndef MULTI_CONNECTION_TCP_SERVER_INFO_HANDLER
#define MULTI_CONNECTION_TCP_SERVER_INFO_HANDLER

#include <streams_boost/function.hpp>
#include <string>

namespace mcts 
{
    ///

	class InfoHandler
    {
    public:
        typedef streams_boost::function<void (std::string const & status, std::string const & ip, uint32_t port)> Handler;
        InfoHandler(Handler handler);
        void handleInfo(std::string const & status, std::string const & ip, uint32_t port);
    private:
        Handler handler_;
    };
}

#endif /* MULTI_CONNECTION_TCP_SERVER_CONN_INFO_HANDLER */

