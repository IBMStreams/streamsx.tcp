/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#include "mcts/async_data_item.h"

namespace mcts 
{

	AsyncDataItem::AsyncDataItem(Handler handler) : bufferSize_(sizeof(uint64_t)), handler_(handler) {}

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

    streams_boost::asio::const_buffers_1 AsyncDataItem::getBuffer()
    {
    	return streams_boost::asio::buffer(buffer_.getData(), buffer_.getSize());
    }

    streams_boost::array<streams_boost::asio::const_buffer, 2> AsyncDataItem::getBuffers()
    {
    	streams_boost::array<streams_boost::asio::const_buffer, 2> buffers =
    	{{
			streams_boost::asio::buffer(bufferSize_.getPtr(), sizeof(uint64_t)),
			streams_boost::asio::buffer(buffer_.getData(), buffer_.getSize())
    	}};

    	return buffers;
    }

//	unsigned char const *  AsyncDataItem::getData()
//	{
//		return buffer_.getData();
//	}
//
//	uint64_t AsyncDataItem::getSize()
//	{
//		return buffer_.getSize();
//	}

    template<outFormat_t Format>
	void AsyncDataItem::setData(SPL::blob & raw)
	{
		uint64_t size = raw.getSize();

		if(Format == mcts::block) {
			bufferSize_.setOCursor(0);
			bufferSize_.addUInt64(size);
		}

		if(raw.ownsData()) {
			buffer_.adoptData(raw.releaseData(size), size);
		}
		else {
			buffer_.adoptData(const_cast<unsigned char *>(raw.getData()), size);
		}

	}

    template void AsyncDataItem::setData<line>(SPL::blob & raw);
    template void AsyncDataItem::setData<block>(SPL::blob & raw);
    template void AsyncDataItem::setData<raw>(SPL::blob & raw);

	void AsyncDataItem::handleError(streams_boost::system::error_code const & e, std::string const & ip, uint32_t port)
    {
		if(TCPConnectionPtr connPtr = connWeakPtr_.lock()) {
			if (!e) __sync_fetch_and_sub(connPtr->getNumOutstandingWritesPtr(), 1);
		}

		handler_( e, ip,  port);
    }
}
