
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_valuebox.cpp
 *
 *  $Id$
 *
 *  Concrete visitor for the Valuebox class
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================

#include "ace/Log_Msg.h"

#include "utl_identifier.h"

#include "be_array.h"
#include "be_enum.h"
#include "be_extern.h"
#include "be_field.h"
#include "be_helper.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_module.h"
#include "be_predefined_type.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_typedef.h"
#include "be_union.h"
#include "be_union_branch.h"
#include "be_util.h"
#include "be_valuebox.h"

#include "be_visitor_context.h"
#include "be_visitor_sequence.h"
#include "be_visitor_typecode.h"
#include "be_visitor_union.h"
#include "be_visitor_valuebox.h"

#include "be_visitor_valuebox/valuebox.cpp"
#include "be_visitor_valuebox/valuebox_ch.cpp"
#include "be_visitor_valuebox/valuebox_ci.cpp"
#include "be_visitor_valuebox/valuebox_cs.cpp"
#include "be_visitor_valuebox/any_op_ch.cpp"
#include "be_visitor_valuebox/any_op_cs.cpp"
#include "be_visitor_valuebox/cdr_op_ch.cpp"
#include "be_visitor_valuebox/cdr_op_cs.cpp"
#include "be_visitor_valuebox/field_ch.cpp"
#include "be_visitor_valuebox/field_ci.cpp"
#include "be_visitor_valuebox/union_member_ci.cpp"
