/*
# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2012 
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.
*/

#include "mcts/data_handler.h"

#include "mcts/data_item.h"

namespace mcts 
{
    DataHandler::DataHandler(Handler handler)
        : handler_(handler)
    {
    }
    
    void DataHandler::handleData(DataItem & dataItem, std::string const & ip, uint32_t port)
    {
        for (size_t i=0, iu=dataItem.getNumberOfCompleteItems(); i<iu; ++i) {
            handler_(dataItem.getCompleteItemAt(i), ip, port);
        }
    }
}
