// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_root.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Root
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_root.h"

// include all the individual files
#include "be_visitor_root/root.cpp"
#include "be_visitor_root/root_ch.cpp"
#include "be_visitor_root/root_ci.cpp"
#include "be_visitor_root/root_cs.cpp"
#include "be_visitor_root/root_sh.cpp"
#include "be_visitor_root/root_si.cpp"
#include "be_visitor_root/root_ss.cpp"
#include "be_visitor_root/root_is.cpp"
#include "be_visitor_root/root_ih.cpp"
#include "be_visitor_root/any_op.cpp"
#include "be_visitor_root/cdr_op.cpp"

ACE_RCSID(be, be_visitor_root, "$Id$")

