/*
# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2012 
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.
*/

#ifndef MULTI_CONNECTION_TCP_SERVER_DATA_ITEM
#define MULTI_CONNECTION_TCP_SERVER_DATA_ITEM

#include <string>
#include <vector>
#include <stdint.h>


namespace mcts 
{

// output format
// line: rstring
// block: blob
typedef enum {line, block} outFormat_t;

 class DataItem
    {
    public:
        bool hasCompleteItems() const;
        size_t getNumberOfCompleteItems() const;
        std::string & getCompleteItemAt(size_t i);
        void removeCompleteItems();
        void addData(char * start, char * end, uint32_t blockSize, outFormat_t outFormat);
    private:
        std::vector<char> buffer_;
        std::vector<std::string> completeItems_;
    };
}

#endif /* MULTI_CONNECTION_TCP_SERVER_DATA_ITEM */
