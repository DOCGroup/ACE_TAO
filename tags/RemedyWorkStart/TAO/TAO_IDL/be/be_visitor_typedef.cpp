
//=============================================================================
/**
 *  @file    be_visitor_typedef.cpp
 *
 *  $Id$
 *
 *  Visitors for generation of code for Typedef
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#include "be_array.h"
#include "be_enum.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_module.h"
#include "be_predefined_type.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_type.h"
#include "be_typedef.h"
#include "be_union.h"
#include "be_valuebox.h"
#include "be_valuetype.h"
#include "be_extern.h"
#include "be_helper.h"

#include "be_visitor_typedef.h"
#include "be_visitor_array.h"
#include "be_visitor_enum.h"
#include "be_visitor_sequence.h"
#include "be_visitor_structure.h"
#include "be_visitor_typecode.h"
#include "be_visitor_union.h"
#include "be_visitor_context.h"
#include "utl_identifier.h"
#include "ace/Log_Msg.h"

#include "be_visitor_typedef/typedef.cpp"
#include "be_visitor_typedef/typedef_ch.cpp"
#include "be_visitor_typedef/typedef_ci.cpp"
#include "be_visitor_typedef/typedef_cs.cpp"
#include "be_visitor_typedef/any_op_ch.cpp"
#include "be_visitor_typedef/any_op_cs.cpp"
#include "be_visitor_typedef/cdr_op_ch.cpp"
#include "be_visitor_typedef/cdr_op_cs.cpp"
