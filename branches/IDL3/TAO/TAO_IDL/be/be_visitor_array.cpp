//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_array.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Array
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_enum.h"
#include "be_type.h"
#include "be_typedef.h"
#include "be_predefined_type.h"
#include "be_array.h"
#include "be_string.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_structure.h"
#include "be_union.h"
#include "be_sequence.h"
#include "be_helper.h"
#include "be_extern.h"
#include "utl_identifier.h"

#include "be_visitor_array.h"
#include "be_visitor_context.h"

#include "be_visitor_array/array.cpp"
#include "be_visitor_array/array_ch.cpp"
#include "be_visitor_array/array_ci.cpp"
#include "be_visitor_array/array_cs.cpp"
#include "be_visitor_array/any_op_ch.cpp"
#include "be_visitor_array/any_op_cs.cpp"
#include "be_visitor_array/cdr_op_ch.cpp"
#include "be_visitor_array/cdr_op_ci.cpp"
#include "be_visitor_array/cdr_op_cs.cpp"

ACE_RCSID (be, 
           be_visitor_array, 
           "$Id$")
