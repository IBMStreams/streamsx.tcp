#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################


.PHONY: all standalone distributed clean 

SPLC_FLAGS1 ?= -a -t ../../com.ibm.streamsx.tcp --output-directory ./output/sender  --data-directory ./data
SPLC_FLAGS2 ?= -a -t ../../com.ibm.streamsx.tcp --output-directory ./output/receiver --data-directory ./data
SPLC = $(STREAMS_INSTALL)/bin/sc
SPL_CMD_ARGS ?= 
SPL_MAIN_COMPOSITE1 = sample::TcpServerDuplex
SPL_MAIN_COMPOSITE2 = sample::TcpServerDuplexBlob

all: standalone

standalone:
	$(SPLC) $(SPLC_FLAGS1) -T -M $(SPL_MAIN_COMPOSITE1) $(SPL_CMD_ARGS)
	$(SPLC) $(SPLC_FLAGS2) -T -M $(SPL_MAIN_COMPOSITE2) $(SPL_CMD_ARGS)

distributed:
	$(SPLC) $(SPLC_FLAGS1) -M $(SPL_MAIN_COMPOSITE1) $(SPL_CMD_ARGS)
	$(SPLC) $(SPLC_FLAGS2) -M $(SPL_MAIN_COMPOSITE2) $(SPL_CMD_ARGS)

clean: 
	$(SPLC) $(SPLC_FLAGS1) -C -M $(SPL_MAIN_COMPOSITE1)
	$(SPLC) $(SPLC_FLAGS2) -C -M $(SPL_MAIN_COMPOSITE2)
