/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#include "mcts/data_item.h"
#include "mcts/server.h"

#include <iostream>



namespace mcts 
{

	bool DataItem::hasCompleteItems() const
    {
        return !completeItems_.empty();
    }

    size_t DataItem::getNumberOfCompleteItems() const
    {
        return completeItems_.size();
    }

    std::string & DataItem::getCompleteItemAt(size_t i)
    {
        return completeItems_[i];
    }

    void DataItem::removeCompleteItems()
    {
        completeItems_.clear();
    }


    // DataItem::addData
    // behavior dependent on 'blockSize' and 'outFormat'
    //
    // outFormat block
    // collect data until buffer_.size() >= blockSize
    //
    // outFormat line
    // collect data until "\n" (CR) received
    // OR
    // buffer_.size() >= blockSize (in case of blockSize > 0)


    void DataItem::addData(char * start, char * end, uint32_t blockSize, outFormat_t outFormat)
    {
        char * current = start;
        while (current!=end) {
        	if (outFormat == line){
                if (*current!='\n') {
                    buffer_.push_back(*current);
                	size_t size = buffer_.size();
                    if ((blockSize != 0) && (size >= blockSize)){
                        completeItems_.push_back(std::string(&buffer_[0], &buffer_[0] + size));
                        buffer_.clear();
                    }
                } else {
                	size_t size = buffer_.size();
                    if (buffer_[size-1]=='\r')
                        size--;
                    completeItems_.push_back(std::string(&buffer_[0], &buffer_[0] + size));
                    buffer_.clear();
                }
                current++;
        	}else{
        		buffer_.push_back(*current);
        		size_t size = buffer_.size();
        		if (size >=blockSize){
        			completeItems_.push_back(std::string(&buffer_[0],  &buffer_[0] + size));
        			buffer_.clear();
        		}
        		current++;
        	}
        }
    }

    void DataItem::flushData(outFormat_t outFormat)
    {
	size_t size = buffer_.size();
	// remove \r in case of line format
	if ((outFormat == line) && (buffer_[size]=='\r'))
			size--;
	completeItems_.push_back(std::string(&buffer_[0], &buffer_[0] +  size));
	buffer_.clear();
    }

}
