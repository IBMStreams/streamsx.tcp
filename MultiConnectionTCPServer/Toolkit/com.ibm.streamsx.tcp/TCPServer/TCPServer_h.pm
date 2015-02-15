
package TCPServer_h;
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
   print '#include "mcts/server.h"', "\n";
   print '#include "mcts/data_item.h"', "\n";
   print "\n";
   #  use TCPServerCommon;
   #  TCPServerCommon::verify($model);
   print "\n";
   print "\n";
   SPL::CodeGen::headerPrologue($model);
   print "\n";
   print "\n";
   print 'class MY_OPERATOR : public MY_BASE_OPERATOR ', "\n";
   print '{', "\n";
   print "\n";
   print "\n";
   print '//typedef enum {line, block} outFormat_t;', "\n";
   print "\n";
   print 'public:', "\n";
   print '    // Constructor', "\n";
   print '    MY_OPERATOR();', "\n";
   print "\n";
   print '    // Destructor', "\n";
   print '    virtual ~MY_OPERATOR(); ', "\n";
   print "\n";
   print '    // Notify port readiness', "\n";
   print '    void allPortsReady(); ', "\n";
   print "\n";
   print '    // Notify pending shutdown', "\n";
   print '    void prepareToShutdown(); ', "\n";
   print "\n";
   print '    // Processing for source and threaded operators   ', "\n";
   print '    void process(uint32_t idx);', "\n";
   print "\n";
   print '    // For handling data from the server', "\n";
   print '    void handleData(std::string & line, std::string const & ipAddress, uint32_t port);', "\n";
   print "\n";
   print '	// For handling connection status info from the server', "\n";
   print '    void handleInfo(std::string const & status, std::string const & ipAddress, uint32_t port);', "\n";
   print "\n";
   print 'private:', "\n";
   print '    static __thread OPort0Type * otuple0_;', "\n";
   print '    std::vector<OPort0Type *> otuples0_;', "\n";
   print '    ', "\n";
   print '	static mcts::outFormat_t outFormat_;', "\n";
   print "\n";
   print '	';
   my $oport1 = $model->getOutputPortAt(1);
   print "\n";
   print '	';
   if ($oport1) {
   print "\n";
   print '    static __thread OPort1Type * otuple1_;', "\n";
   print '    std::vector<OPort1Type *> otuples1_;', "\n";
   print '    ';
   }
   print "\n";
   print '    ', "\n";
   print '    SPL::Mutex mutex_;', "\n";
   print '    mcts::TCPServer server_;', "\n";
   print '}; ', "\n";
   print "\n";
   SPL::CodeGen::headerEpilogue($model);
   print "\n";
   print "\n";
   CORE::exit $SPL::CodeGen::USER_ERROR if ($SPL::CodeGen::sawError);
}
1;
