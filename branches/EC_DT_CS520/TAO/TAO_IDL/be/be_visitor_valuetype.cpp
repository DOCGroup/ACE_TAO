
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

#include "be_argument.h"
#include "be_array.h"
#include "be_attribute.h"
#include "be_constant.h"
#include "be_enum.h"
#include "be_exception.h"
#include "be_eventtype.h"
#include "be_factory.h"
#include "be_field.h"
#include "be_interface_fwd.h"
#include "be_module.h"
#include "be_predefined_type.h"
#include "be_operation.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_structure_fwd.h"
#include "be_typedef.h"
#include "be_union.h"
#include "be_union_fwd.h"
#include "be_valuetype.h"
#include "be_valuetype_fwd.h"
#include "be_helper.h"
#include "be_extern.h"
#include "utl_identifier.h"
#include "utl_exceptlist.h"

#include "be_visitor_valuetype.h"
#include "be_visitor_argument.h"
#include "be_visitor_array.h"
#include "be_visitor_eventtype.h"
#include "be_visitor_field.h"
#include "be_visitor_constant.h"
#include "be_visitor_enum.h"
#include "be_visitor_exception.h"
#include "be_visitor_operation.h"
#include "be_visitor_sequence.h"
#include "be_visitor_structure.h"
#include "be_visitor_structure_fwd.h"
#include "be_visitor_typecode.h"
#include "be_visitor_typedef.h"
#include "be_visitor_union.h"
#include "be_visitor_union_fwd.h"
#include "be_visitor_context.h"

#include "be_visitor_valuetype/valuetype.cpp"
#include "be_visitor_valuetype/valuetype_ch.cpp"
#include "be_visitor_valuetype/valuetype_ci.cpp"
#include "be_visitor_valuetype/valuetype_cs.cpp"
#include "be_visitor_valuetype/valuetype_sh.cpp"
#include "be_visitor_valuetype/valuetype_si.cpp"
#include "be_visitor_valuetype/valuetype_ss.cpp"
#include "be_visitor_valuetype/valuetype_obv_ch.cpp"
#include "be_visitor_valuetype/valuetype_obv_ci.cpp"
#include "be_visitor_valuetype/valuetype_obv_cs.cpp"
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

ACE_RCSID (be,
           be_visitor_valuetype,
          "$Id$")
