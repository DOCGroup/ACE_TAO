//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_operation.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Operation
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

// include all the individual files
#include "be_visitor_operation/operation.cpp"
#include "be_visitor_operation/arglist.cpp"
#include "be_visitor_operation/argument.cpp"
#include "be_visitor_operation/argument_invoke.cpp"
#include "be_visitor_operation/argument_marshal.cpp"
#include "be_visitor_operation/collocated_sh.cpp"
#include "be_visitor_operation/collocated_ss.cpp"
#include "be_visitor_operation/exceptlist_cs.cpp"
#include "be_visitor_operation/operation_ch.cpp"
#include "be_visitor_operation/operation_cs.cpp"
#include "be_visitor_operation/operation_sh.cpp"
#include "be_visitor_operation/operation_ih.cpp"
#include "be_visitor_operation/operation_ss.cpp"
#include "be_visitor_operation/operation_is.cpp"
#include "be_visitor_operation/rettype.cpp"
#include "be_visitor_operation/rettype_is.cpp"
#include "be_visitor_operation/rettype_assign_ss.cpp"
#include "be_visitor_operation/rettype_docall_cs.cpp"
#include "be_visitor_operation/rettype_marshal_ss.cpp"
#include "be_visitor_operation/rettype_post_docall_cs.cpp"
#include "be_visitor_operation/rettype_post_upcall_ss.cpp"
#include "be_visitor_operation/rettype_pre_docall_cs.cpp"
#include "be_visitor_operation/rettype_return_cs.cpp"
#include "be_visitor_operation/rettype_vardecl_cs.cpp"
#include "be_visitor_operation/rettype_vardecl_ss.cpp"
#include "be_visitor_operation/tie_sh.cpp"
#include "be_visitor_operation/tie_si.cpp"
#include "be_visitor_operation/compiled_marshal.cpp"
#include "be_visitor_operation/rettype_pre_invoke_cs.cpp"

ACE_RCSID(be, be_visitor_operation, "$Id$")

