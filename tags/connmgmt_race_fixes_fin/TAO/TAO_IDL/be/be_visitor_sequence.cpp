// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_sequence.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Sequence
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_sequence.h"

// include all the individual files
#include "be_visitor_sequence/any_op_ch.cpp"
#include "be_visitor_sequence/any_op_cs.cpp"
#include "be_visitor_sequence/cdr_op_ch.cpp"
#include "be_visitor_sequence/cdr_op_ci.cpp"
#include "be_visitor_sequence/cdr_op_cs.cpp"
#include "be_visitor_sequence/buffer_type.cpp"
#include "be_visitor_sequence/elemtype.cpp"
#include "be_visitor_sequence/sequence_base.cpp"
#include "be_visitor_sequence/sequence_ch.cpp"
#include "be_visitor_sequence/sequence_ci.cpp"
#include "be_visitor_sequence/sequence_cs.cpp"
#include "be_visitor_sequence/gen_unbounded_sequence_ch.cpp"
#include "be_visitor_sequence/gen_bounded_sequence_ch.cpp"
#include "be_visitor_sequence/gen_unbounded_obj_sequence_ch.cpp"
#include "be_visitor_sequence/gen_bounded_obj_sequence_ch.cpp"
#include "be_visitor_sequence/gen_bounded_str_sequence_ch.cpp"
#include "be_visitor_sequence/gen_bounded_wstr_sequence_ch.cpp"
#include "be_visitor_sequence/gen_unbounded_sequence_ci.cpp"
#include "be_visitor_sequence/gen_bounded_sequence_ci.cpp"
#include "be_visitor_sequence/gen_unbounded_obj_sequence_ci.cpp"
#include "be_visitor_sequence/gen_bounded_obj_sequence_ci.cpp"
#include "be_visitor_sequence/gen_bounded_str_sequence_ci.cpp"
#include "be_visitor_sequence/gen_bounded_wstr_sequence_ci.cpp"
#include "be_visitor_sequence/gen_unbounded_sequence_cs.cpp"
#include "be_visitor_sequence/gen_bounded_sequence_cs.cpp"
#include "be_visitor_sequence/gen_unbounded_obj_sequence_cs.cpp"
#include "be_visitor_sequence/gen_bounded_obj_sequence_cs.cpp"
#include "be_visitor_sequence/gen_bounded_str_sequence_cs.cpp"
#include "be_visitor_sequence/gen_bounded_wstr_sequence_cs.cpp"

ACE_RCSID(be, be_visitor_sequence, "$Id$")
