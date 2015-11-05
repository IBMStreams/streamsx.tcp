/*
#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################
*/

#ifndef MULTI_CONNECTION_TCP_SERVER_ASYNC_DATA_ITEM
#define MULTI_CONNECTION_TCP_SERVER_ASYNC_DATA_ITEM

#include <streams_boost/asio.hpp>
#include <streams_boost/array.hpp>
#include <streams_boost/function.hpp>
#include <streams_boost/system/error_code.hpp>

#include "mcts/connection.h"

#include <SPL/Runtime/Serialization/NetworkByteBuffer.h>
#include <SPL/Runtime/Type/Blob.h>

namespace mcts 
{

	class AsyncDataItem
    {

    public:
        typedef streams_boost::function<void (streams_boost::system::error_code const & e, std::string const & ip, uint32_t port)> Handler;

		/// Construct new async data item for use with async_write with given error function.
		AsyncDataItem(Handler handler);

        /// Return valid connection.
//        bool getValidConnection(TCPConnectionPtr & connPtr);

        /// Return valid connection.
//        void setConnectionPtr(TCPConnectionWeakPtr const & connWeakPtr);

        /// Get a buffer to string.
        streams_boost::asio::const_buffers_1 getBuffer();

        /// Get an array of buffers to blob size and blob data.
        streams_boost::array<streams_boost::asio::const_buffer, 2> getBuffers();

        /// Adopt blob data to the buffer (no copy).
        template<outFormat_t Format>
        void setData(SPL::blob & raw);

        void handleError(streams_boost::system::error_code const & e, std::string const & ip, uint32_t port);
        void handleError(streams_boost::system::error_code const & e, std::string const & ip, uint32_t port, TCPConnectionWeakPtr connWeakPtr);

    private:
        /// Reference to bounded connection.
//        TCPConnectionWeakPtr connWeakPtr_;

        /// The unique buffer size to prepend to the data.
        SPL::NetworkByteBuffer bufferSize_;

        /// The unique buffer to send the data.
        SPL::blob buffer_;

        /// The handler used to process error messages.
        Handler handler_;
    };

    typedef streams_boost::shared_ptr<AsyncDataItem> AsyncDataItemPtr;

}

#endif /* MULTI_CONNECTION_TCP_SERVER_ASYNC_DATA_ITEM */
