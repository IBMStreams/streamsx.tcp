#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################

SPL_PKGCFG=$(STREAMS_INSTALL)/bin/dst-pe-pkg-config.sh
SPL_PKG=dst-spl-pe-install
SPL_INCLUDE_OPTIONS = `$(SPL_PKGCFG) --cflags $(SPL_PKG)`
CXXFLAGS=-O3 -DNDEBUG -fPIC

.PHONY: clean distclean

all: lib/libstreams.spl.multiconnection.tcp.server.so

lib/server.o: src/mcts/server.cpp \
	      include/mcts/server.h \
	      include/mcts/connection.h \
	      include/mcts/data_handler.h \
	      include/mcts/error_handler.h \
	      include/mcts/info_handler.h 
	@echo Compiling 'src/mcts/server.cpp' ...
	mkdir   -p    ./lib
	@g++ -Wall $(CXXFLAGS) -I include $(SPL_INCLUDE_OPTIONS) -c src/mcts/server.cpp -o $@ 

lib/connection.o: src/mcts/connection.cpp \
	          include/mcts/connection.h \
                  include/mcts/data_item.h \
                  include/mcts/data_handler.h \
		  include/mcts/error_handler.h \
		  include/mcts/info_handler.h 
	@echo Compiling 'src/mcts/connection.cpp' ...
	@g++ -Wall $(CXXFLAGS) -I include $(SPL_INCLUDE_OPTIONS) -c src/mcts/connection.cpp -o $@ 

lib/data_handler.o: src/mcts/data_handler.cpp include/mcts/data_item.h 
	@echo Compiling 'src/mcts/data_handler.cpp' ...
	@g++ -Wall $(CXXFLAGS) -I include $(SPL_INCLUDE_OPTIONS) -c src/mcts/data_handler.cpp -o $@ 

lib/error_handler.o: src/mcts/error_handler.cpp include/mcts/data_item.h 
	@echo Compiling 'src/mcts/error_handler.cpp' ...
	@g++ -Wall $(CXXFLAGS) -I include $(SPL_INCLUDE_OPTIONS) -c src/mcts/error_handler.cpp -o $@ 

lib/info_handler.o: src/mcts/info_handler.cpp include/mcts/data_item.h 
	@echo Compiling 'src/mcts/info_handler.cpp' ...
	@g++ -Wall $(CXXFLAGS) -I include $(SPL_INCLUDE_OPTIONS) -c src/mcts/info_handler.cpp -o $@ 

lib/data_item.o: src/mcts/data_item.cpp 
	@echo Compiling 'src/mcts/data_item.cpp' ...
	@g++ -Wall $(CXXFLAGS) -I include $(SPL_INCLUDE_OPTIONS) -c src/mcts/data_item.cpp -o $@ 

lib/io_service_pool.o: src/mcts/io_service_pool.cpp 
	@echo Compiling 'src/mcts/io_service_pool.cpp' ...
	@g++ -Wall $(CXXFLAGS) -I include $(SPL_INCLUDE_OPTIONS) -c src/mcts/io_service_pool.cpp -o $@ 

lib/libstreams.spl.multiconnection.tcp.server.so: \
	lib/server.o \
	lib/connection.o \
	lib/data_item.o \
	lib/data_handler.o \
	lib/error_handler.o \
	lib/info_handler.o \
	lib/io_service_pool.o 
	@echo Building C++ shared library '$@'
	@g++ -shared -o $@ $^		

clean:
	rm -fr lib/*.o lib/*.so 

