// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_interface.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Interface
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

#include "be_visitor_interface.h"

// include all the individual files
#include "be_visitor_interface/interface.cpp"
#include "be_visitor_interface/interface_ch.cpp"
#include "be_visitor_interface/interface_ci.cpp"
#include "be_visitor_interface/interface_cs.cpp"
#include "be_visitor_interface/interface_sh.cpp"
#include "be_visitor_interface/interface_ih.cpp"
#include "be_visitor_interface/interface_si.cpp"
#include "be_visitor_interface/interface_ss.cpp"
#include "be_visitor_interface/interface_is.cpp"
#include "be_visitor_interface/thru_poa_collocated_sh.cpp"
#include "be_visitor_interface/thru_poa_collocated_ss.cpp"
#include "be_visitor_interface/direct_collocated_sh.cpp"
#include "be_visitor_interface/direct_collocated_ss.cpp"
#include "be_visitor_interface/tie_sh.cpp"
#include "be_visitor_interface/tie_si.cpp"
#include "be_visitor_interface/any_op_ch.cpp"
#include "be_visitor_interface/any_op_cs.cpp"
#include "be_visitor_interface/cdr_op_ch.cpp"
#include "be_visitor_interface/cdr_op_ci.cpp"
#include "be_visitor_interface/cdr_op_cs.cpp"
#include "be_visitor_interface/ami_handler_fwd_ch.cpp"
#include "be_visitor_interface/ami_handler_fwd_ci.cpp"
#include "be_visitor_interface/ami_handler_stub_ch.cpp"
#include "be_visitor_interface/ami_handler_stub_cs.cpp"

#if 0 /* We are using normal servant code */
#include "be_visitor_interface/collocated_ami_handler_ch.cpp"
#include "be_visitor_interface/ami_handler_servant_ch.cpp"
#include "be_visitor_interface/ami_handler_servant_cs.cpp"
#endif /* 0 */

ACE_RCSID(be, be_visitor_interface, "$Id$")
