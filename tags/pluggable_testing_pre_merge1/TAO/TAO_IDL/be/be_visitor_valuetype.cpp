
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_valuetype.cpp
//
// = DESCRIPTION
//    Concrete visitor for the Valuetype class
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================


#include "idl.h"
#include "idl_extern.h"
#include "be.h"

#ifdef IDL_HAS_VALUETYPE

#include "be_visitor_valuetype.h"

#include "be_visitor_valuetype/valuetype.cpp"
#include "be_visitor_valuetype/valuetype_ch.cpp"
#include "be_visitor_valuetype/valuetype_obv_ch.cpp"
#include "be_visitor_valuetype/valuetype_obv_cs.cpp"
#include "be_visitor_valuetype/valuetype_ci.cpp"
#include "be_visitor_valuetype/valuetype_cs.cpp"
//#include "be_visitor_valuetype/valuetype_ih.cpp"
//#include "be_visitor_valuetype/valuetype_is.cpp"
//#include "be_visitor_valuetype/any_op_ch.cpp"
//#include "be_visitor_valuetype/any_op_cs.cpp"
#include "be_visitor_valuetype/cdr_op_ch.cpp"
#include "be_visitor_valuetype/cdr_op_cs.cpp"
#include "be_visitor_valuetype/marshal_ch.cpp"
#include "be_visitor_valuetype/marshal_cs.cpp"
#include "be_visitor_valuetype/arglist.cpp"
#include "be_visitor_valuetype/field_ch.cpp"
#include "be_visitor_valuetype/field_cs.cpp"
#include "be_visitor_valuetype/field_cdr_ci.cpp"
#include "be_visitor_valuetype/obv_module.cpp"

#endif /* IDL_HAS_VALUETYPE */

