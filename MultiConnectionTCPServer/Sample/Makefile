# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2012 
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.

.PHONY: all standalone distributed clean 

SPLC_FLAGS1 ?= -a -t ../Toolkit --output-directory ./output/sender
SPLC_FLAGS2 ?= -a -t ../Toolkit --output-directory ./output/receiver
SPLC = $(STREAMS_INSTALL)/bin/sc
SPL_CMD_ARGS ?= 
SPL_MAIN_COMPOSITE1 = TCPSender
SPL_MAIN_COMPOSITE2 = TCPReceiver

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
