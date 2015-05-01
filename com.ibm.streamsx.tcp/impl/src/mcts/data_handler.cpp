/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
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
