
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

#include "be_visitor_valuetype.h"

#include "be_visitor_valuetype/valuetype.cpp"
#include "be_visitor_valuetype/valuetype_ch.cpp"
#include "be_visitor_valuetype/valuetype_obv_ch.cpp"
#include "be_visitor_valuetype/valuetype_obv_ci.cpp"
#include "be_visitor_valuetype/valuetype_obv_cs.cpp"
#include "be_visitor_valuetype/valuetype_ci.cpp"
#include "be_visitor_valuetype/valuetype_cs.cpp"
#include "be_visitor_valuetype/any_op_ch.cpp"
#include "be_visitor_valuetype/any_op_cs.cpp"
#include "be_visitor_valuetype/cdr_op_ch.cpp"
#include "be_visitor_valuetype/cdr_op_ci.cpp"
#include "be_visitor_valuetype/cdr_op_cs.cpp"
#include "be_visitor_valuetype/marshal_ch.cpp"
#include "be_visitor_valuetype/marshal_cs.cpp"
#include "be_visitor_valuetype/arglist.cpp"
#include "be_visitor_valuetype/field_ch.cpp"
#include "be_visitor_valuetype/field_ci.cpp"
#include "be_visitor_valuetype/field_cs.cpp"
#include "be_visitor_valuetype/field_cdr_ch.cpp"
#include "be_visitor_valuetype/field_cdr_ci.cpp"
#include "be_visitor_valuetype/obv_module.cpp"
#include "be_visitor_valuetype/ami_exception_holder_ch.cpp"
#include "be_visitor_valuetype/ami_exception_holder_cs.cpp"
#include "be_visitor_valuetype/valuetype_init.cpp"
#include "be_visitor_valuetype/valuetype_init_ch.cpp"
#include "be_visitor_valuetype/valuetype_init_ci.cpp"
#include "be_visitor_valuetype/valuetype_init_cs.cpp"
#include "be_visitor_valuetype/valuetype_init_arglist_ch.cpp"


