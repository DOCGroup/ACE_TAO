//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_module.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Module
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_module.h"

// include all the individual files
#include "be_visitor_module/module.cpp"
#include "be_visitor_module/module_ch.cpp"
#include "be_visitor_module/module_sh.cpp"
#include "be_visitor_module/module_ih.cpp"
#include "be_visitor_module/any_op.cpp"
#include "be_visitor_module/cdr_op.cpp"

ACE_RCSID(be, be_visitor_module, "$Id$")

