#######################################################################
# Copyright (C)2012, 2015 International Business Machines Corporation and  
# others. All Rights Reserved.                                    
#######################################################################  
package TCPServerCommon;

use strict;
use warnings;
use Data::Dumper;

sub verify($)
{
   my ($model) = @_;
   verifyOutputPort($model);
   
	my $iport = $model->getInputPortAt(0);
	if ($iport)
	{
		verifyInputPort($model);
	}
	else {
		verifyResponseParams($model);
	}
}

sub verifyInputPort($) {

   my ($model) = @_;

   # check input port 0 (mandatory)
   my $iport = $model->getInputPortAt(0);
   my $type = $iport->getSPLTupleType();
   

    # attributes for port 0: name => type, [m|o] (mandatory "m" or optional "o")
	my %attributes = (	line => ["rstring","m"],
						block => ["blob","m"],
						raw => ["blob","m"],
						srcIP => ["rstring","o"],
						srcPort => ["uint32","o"]);

	my $noOfAttr = $iport->getNumberOfAttributes();
	my $noOfMandAttr = 0;
	my $a;
	my $t;
	
	for (my $i = 0; $i < $noOfAttr; ++$i) {
		$a = $iport->getAttributeAt($i)->getName();
		$t = $iport->getAttributeAt($i)->getSPLType();
		SPL::CodeGen::exitln("The '$a' attribute in the schema for input port number 0 is not allowed.", $iport->getSourceLocation()) if (not defined($attributes{$a}));
		SPL::CodeGen::exitln("The type of the '$a' attribute in the schema for port number 0 is '$t', but the '$attributes{$a}[0]' type is expected.", $iport->getSourceLocation()) if ($attributes{$a}[0] ne $t);
 
	}
	
	my $line = $iport->getAttributeByName("line");
	my $block = $iport->getAttributeByName("block");	
	
	#SPL::CodeGen::exitln("The schema for input port number 0 expects 'line' OR 'block' attribute.", $iport->getSourceLocation()) 
	#	if (defined $line && defined $block);
		
	# check that srcIP and #srcPort must exist		
	my $srcIP = $iport->getAttributeByName("srcIP");
	my $srcPort = $iport->getAttributeByName("srcPort");
	
	if (!(defined $srcIP))
	{
		SPL::CodeGen::exitln("The schema for input port number 0 expects 'rstring srcIP' as an attribute.", $iport->getSourceLocation())
	}
	
	if (!(defined $srcPort))
	{
		SPL::CodeGen::exitln("The schema for input port number 0 expects 'uint32 srcPort' as an attribute.", $iport->getSourceLocation())
	}
}


sub verifyOutputPort($){

my ($model) = @_;

# check output port 0 (mandatory)
   my $oport = $model->getOutputPortAt(0);
   my $type = $oport->getSPLTupleType();
   

# attributes for port 0: name => type, [m|o] (mandatory "m" or optional "o")
	my %attributes = (	line => ["rstring","m"],
						block => ["blob","m"],
						srcIP => ["rstring","o"],
						srcPort => ["uint32","o"]);

	my $noOfAttr = $oport->getNumberOfAttributes();
	my $noOfMandAttr = 0;
	my $a;
	my $t;
	
	for (my $i = 0; $i < $noOfAttr; ++$i) {
		$a = $oport->getAttributeAt($i)->getName();
		$t = $oport->getAttributeAt($i)->getSPLType();
		SPL::CodeGen::exitln("The '$a' attribute in the schema for output port number 0 is not allowed.", $oport->getSourceLocation()) if (not defined($attributes{$a}));
		SPL::CodeGen::exitln("The type of the '$a' attribute in the schema for port number 0 is '$t', but the '$attributes{$a}[0]' type is expected.", $oport->getSourceLocation()) if ($attributes{$a}[0] ne $t);
		$noOfMandAttr++ if ($attributes{$a}[1] eq "m"); 
	}
	SPL::CodeGen::exitln("The schema for output port number 0 expects 'line' OR 'block' attribute.", $oport->getSourceLocation()) 
		if ($noOfMandAttr > 1);
	
	
	my $bs_ = $model->getParameterByName("blockSize");
	my $block_ = $oport->getAttributeByName("block");
	my $line_ = $oport->getAttributeByName("line");

#	SPL::CodeGen::println(Dumper $bs_);

  	SPL::CodeGen::exitln("The parameter 'blockSize' (uint32) is mandatory in case of 'block' attribute on port number 0.", $oport->getSourceLocation()) 
  		if (!defined($bs_) && defined($block_) );

 	SPL::CodeGen::warnln("parameter 'blockSize': 0 is not recommended for output attribute block!", $oport->getSourceLocation()) 
  		if ( (defined $bs_) && ($bs_->getValueAt(0)->getSPLExpression() eq "0") && (defined $block_) );

 	SPL::CodeGen::exitln("parameter 'blockSize': 0 is not allowed for output attribute line!", $oport->getSourceLocation()) 
  		if ( (defined $bs_) && ($bs_->getValueAt(0)->getSPLExpression() eq "0") && (defined $line_) );


# check output for optional port 1
   $oport = $model->getOutputPortAt(1);
   if ($oport){
	   SPL::CodeGen::checkMinimalSchema ($oport, 
    	{ name => "status", type => "rstring"   },
       	{ name => "srcIP", type => "rstring"  },
       	{ name => "srcPort", type => "uint32" }
   	);
   }

}

sub verifyResponseParams($){

my ($model) = @_;
	if ($model->getParameterByName("broadcastResponse")) {
	 	SPL::CodeGen::warnln("parameter 'broadcastResponse' used without input port defined - will be ignored")
	}
	if ($model->getParameterByName("disableDuplexConnection")) {
	 	SPL::CodeGen::warnln("parameter 'disableDuplexConnection' used without input port defined - will be ignored")
	}
	if ($model->getParameterByName("maxUnreadResponseCount")) {
	 	SPL::CodeGen::warnln("parameter 'maxUnreadResponseCount' used without input port defined - will be ignored")
	}
}

1;

