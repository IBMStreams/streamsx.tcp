
package TCPServer_cpp;
use strict; use Cwd 'realpath';  use File::Basename;  use lib dirname(__FILE__);  use SPL::Operator::Instance::OperatorInstance; use SPL::Operator::Instance::Context; use SPL::Operator::Instance::Expression; use SPL::Operator::Instance::ExpressionTree; use SPL::Operator::Instance::ExpressionTreeVisitor; use SPL::Operator::Instance::ExpressionTreeCppGenVisitor; use SPL::Operator::Instance::InputAttribute; use SPL::Operator::Instance::InputPort; use SPL::Operator::Instance::OutputAttribute; use SPL::Operator::Instance::OutputPort; use SPL::Operator::Instance::Parameter; use SPL::Operator::Instance::StateVariable; use SPL::Operator::Instance::Window; 
sub main::generate($$) {
   my ($xml, $signature) = @_;  
   print "// $$signature\n";
   my $model = SPL::Operator::Instance::OperatorInstance->new($$xml);
   unshift @INC, dirname ($model->getContext()->getOperatorDirectory()) . "/../impl/nl/include";
   $SPL::CodeGenHelper::verboseMode = $model->getContext()->isVerboseModeOn();
   # Licensed Materials - Property of IBM
   # Copyright IBM Corp. 2012 
   # US Government Users Restricted Rights - Use, duplication or
   # disclosure restricted by GSA ADP Schedule Contract with
   # IBM Corp.
   print "\n";
   print "\n";
   print '/* Additional includes go here */', "\n";
   print '#include <streams_boost/bind.hpp>', "\n";
   print '#include "mcts/data_item.h"', "\n";
   print "\n";
     use TCPServerCommon;
     TCPServerCommon::verify($model);
   print "\n";
   print "\n";
   SPL::CodeGen::implementationPrologue($model);
   print "\n";
   print "\n";
     my $port = $model->getParameterByName("port")->getValueAt(0)->getCppExpression();
     my $threadPoolSize = $model->getParameterByName("threadPoolSize")->getValueAt(0)->getCppExpression();
     my $connectionCap = $model->getParameterByName("connectionCap");
     if (not defined($connectionCap)) {
         $connectionCap = "std::numeric_limits<std::size_t>::max()";
     } else {
         $connectionCap = $connectionCap->getValueAt(0)->getCppExpression();
     }
     my $blockSize = $model->getParameterByName("blockSize");
     if (not defined($blockSize)) {
         $blockSize = 0;
     } else {
         $blockSize = $blockSize->getValueAt(0)->getCppExpression();
     }
   
     my $keepAlive = $model->getParameterByName("keepAlive");
     # Apply default value for keepAlive.
     $keepAlive = $keepAlive ? $keepAlive->getValueAt(0)->getCppExpression() : "";
   
   	my $oport0 = $model->getOutputPortAt(0);
     	my $oport1 = $model->getOutputPortAt(1);
   
   print "\n";
   print "\n";
   print '	';
   if ($oport0->getAttributeByName("line")){
   print "\n";
   print '		mcts::outFormat_t MY_OPERATOR_SCOPE::MY_OPERATOR::outFormat_ = mcts::line;', "\n";
   print '	';
   }else{
   print "\n";
   print '		mcts::outFormat_t MY_OPERATOR_SCOPE::MY_OPERATOR::outFormat_ = mcts::block;', "\n";
   print '	';
   }
   print "\n";
   print "\n";
   print '__thread MY_OPERATOR_SCOPE::MY_OPERATOR::OPort0Type * MY_OPERATOR_SCOPE::MY_OPERATOR::otuple0_ = NULL;', "\n";
   print "\n";
   if ($oport1) {
   print "\n";
   print '__thread MY_OPERATOR_SCOPE::MY_OPERATOR::OPort1Type * MY_OPERATOR_SCOPE::MY_OPERATOR::otuple1_ = NULL;', "\n";
   }
   print "\n";
   print "\n";
   print '// Constructor', "\n";
   print 'MY_OPERATOR_SCOPE::MY_OPERATOR::MY_OPERATOR()', "\n";
   print '    : server_(SPL::Functions::Utility::getHostName().string(), ';
   print $port;
   print ', ';
   print $threadPoolSize;
   print ', ';
   print $connectionCap;
   print ', ';
   print $blockSize;
   print ',  MY_OPERATOR_SCOPE::MY_OPERATOR::outFormat_,', "\n";
   print '              streams_boost::bind(&MY_OPERATOR_SCOPE::MY_OPERATOR::handleData, this, _1, _2, _3) ,', "\n";
   print '              streams_boost::bind(&MY_OPERATOR_SCOPE::MY_OPERATOR::handleInfo, this, _1, _2, _3)', "\n";
   print '              )', "\n";
   print '{', "\n";
   print '    // Initialization code goes here', "\n";
   print '    // If the user provided a KeepAlive parameter, we can set that now.', "\n";
   print '    ';
   if ($keepAlive ne "") {
   print "\n";
   print '       server_.setKeepAlive(';
   print $keepAlive;
   print '.get_time(), ';
   print $keepAlive;
   print '.get_probes(), ';
   print $keepAlive;
   print '.get_interval());', "\n";
   print '    ';
   }
   print "\n";
   print "\n";
   print '}', "\n";
   print "\n";
   print '// Destructor', "\n";
   print 'MY_OPERATOR_SCOPE::MY_OPERATOR::~MY_OPERATOR() ', "\n";
   print '{', "\n";
   print '    // Finalization code goes here', "\n";
   print '    SPL::AutoMutex am(mutex_);', "\n";
   print '    for (size_t i=0, iu=otuples0_.size(); i<iu; ++i)', "\n";
   print '        delete otuples0_[i];', "\n";
   if ($oport1) {
   print "\n";
   print '    for (size_t i=0, iu=otuples1_.size(); i<iu; ++i)', "\n";
   print '        delete otuples1_[i];', "\n";
   }
   print "\n";
   print '}', "\n";
   print "\n";
   print '// Notify port readiness', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::allPortsReady() ', "\n";
   print '{', "\n";
   print '    createThreads(1); // Create source thread', "\n";
   print '}', "\n";
   print ' ', "\n";
   print '// Processing for source and threaded operators   ', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::process(uint32_t idx)', "\n";
   print '{', "\n";
   print '    if(!getPE().getShutdownRequested()) ', "\n";
   print '        server_.run();', "\n";
   print '}', "\n";
   print "\n";
   print '// Notify pending shutdown', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::prepareToShutdown() ', "\n";
   print '{', "\n";
   print '    server_.stop();', "\n";
   print '}', "\n";
   print "\n";
   print '// For handling data from the server', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::handleData(std::string & line, std::string const & ipAddress, uint32_t port)', "\n";
   print '{', "\n";
   print '    if(otuple0_==NULL) {', "\n";
   print '        otuple0_ = new OPort0Type();', "\n";
   print '        SPL::AutoMutex am(mutex_);', "\n";
   print '        otuples0_.push_back(otuple0_);', "\n";
   print '    }', "\n";
   print "\n";
   print '	//splBlob_ = SPL::Functions::String::convertToBlob(line);', "\n";
   print '    ', "\n";
   print '    ';
   if ($oport0->getAttributeByName("line")) {
   print '	', "\n";
   print '	    otuple0_->get_line() = line;', "\n";
   print '    ';
   }
   print "\n";
   print "\n";
   print '    ';
   if ($oport0->getAttributeByName("block")) {
   print '	', "\n";
   print '	    otuple0_->get_block() = SPL::Functions::String::convertToBlob(line);', "\n";
   print '    ';
   }
   print "\n";
   print "\n";
   print "\n";
   print '    ';
   if ($oport0->getAttributeByName("srcIP")) {
   print "\n";
   print '        otuple0_->get_srcIP() = ipAddress;', "\n";
   print '    ';
   }
   print "\n";
   print '    ';
   if ($oport0->getAttributeByName("srcPort")) {
   print "\n";
   print '        otuple0_->get_srcPort() = port;', "\n";
   print '    ';
   }
   print "\n";
   print '    submit(*otuple0_, 0); ', "\n";
   print '}', "\n";
   print "\n";
   print '// For handling connection status info from the server', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::handleInfo(std::string const & status, std::string const & ipAddress, uint32_t port)', "\n";
   print '{', "\n";
   print "\n";
   print '	//SPLLOG(L_ERROR,"TCPServer, connection status: " << status << ":" << ipAddress << ":" << port << "!", "handleInfo");', "\n";
   print "\n";
   print '	';
   if ($oport1) {
   print "\n";
   print '    if(otuple1_==NULL) {', "\n";
   print '        otuple1_ = new OPort1Type();', "\n";
   print '        SPL::AutoMutex am(mutex_);', "\n";
   print '        otuples1_.push_back(otuple1_);', "\n";
   print '    }', "\n";
   print '    otuple1_->get_status() = status;', "\n";
   print '    otuple1_->get_srcIP() = ipAddress;', "\n";
   print '    otuple1_->get_srcPort() = port;', "\n";
   print '    submit(*otuple1_, 1); ', "\n";
   print '    ';
   }
   print "\n";
   print '}', "\n";
   print "\n";
   SPL::CodeGen::implementationEpilogue($model);
   print "\n";
   print "\n";
   CORE::exit $SPL::CodeGen::USER_ERROR if ($SPL::CodeGen::sawError);
}
1;
