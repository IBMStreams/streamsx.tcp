#######################################################################
# Copyright (C)2012, International Business Machines Corporation and
# others. All Rights Reserved.
#######################################################################

all: mctsLib
	spl-make-toolkit -m -i .
	
clean: cleanspldoc
	cd impl; make clean
	spl-make-toolkit -c -i .
	
mctsLib:
	cd impl; make all
	
mctsLibClean:
	cd impl; make clean

spldoc:
	spl-make-doc -i .
	
cleanspldoc:
	rm -Rf doc		
	
		
	
	
	
	

