//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_args.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Argument
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

// include all the individual files
#include "be_visitor_argument/arglist.cpp"
#include "be_visitor_argument/argument.cpp"
#include "be_visitor_argument/docall_cs.cpp"
#include "be_visitor_argument/marshal_ss.cpp"
#include "be_visitor_argument/post_docall_cs.cpp"
#include "be_visitor_argument/post_docall_compiled_cs.cpp"
#include "be_visitor_argument/post_marshal_ss.cpp"
#include "be_visitor_argument/post_upcall_ss.cpp"
#include "be_visitor_argument/pre_docall_cs.cpp"
#include "be_visitor_argument/pre_upcall_ss.cpp"
#include "be_visitor_argument/upcall_ss.cpp"
#include "be_visitor_argument/vardecl_ss.cpp"
#include "be_visitor_argument/compiled_marshal_cs.cpp"
#include "be_visitor_argument/compiled_marshal_ss.cpp"
#include "be_visitor_argument/pre_invoke_cs.cpp"

ACE_RCSID(be, be_visitor_argument, "$Id$")

