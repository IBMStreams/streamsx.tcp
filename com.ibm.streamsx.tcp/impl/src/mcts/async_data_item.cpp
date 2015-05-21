/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#include "mcts/async_data_item.h"

namespace mcts 
{

	AsyncDataItem::AsyncDataItem(Handler handler) : handler_(handler) {}

	bool AsyncDataItem::getValidConnection(TCPConnectionPtr & connPtr)
	{
		if(connPtr = connWeakPtr_.lock()) {
			return connPtr->socket().is_open();
		}

		return false;
	}

    void AsyncDataItem::setConnectionPtr(TCPConnectionWeakPtr const & connWeakPtr)
    {
    	connWeakPtr_ = connWeakPtr;
    }

	unsigned char const *  AsyncDataItem::getData()
	{
		return buffer_.getData();
	}

	uint64_t AsyncDataItem::getSize()
	{
		return buffer_.getSize();
	}

	void AsyncDataItem::setData(SPL::blob & raw)
	{
		uint64_t size = raw.getSize();
		if(raw.ownsData()) {
			buffer_.adoptData(raw.releaseData(size), size);
		}
		else {
			buffer_.adoptData(const_cast<unsigned char *>(raw.getData()), size);
		}

	}

	void AsyncDataItem::handleError(streams_boost::system::error_code const & e, std::string const & ip, uint32_t port)
    {
		handler_( e, ip,  port);
    }
}
