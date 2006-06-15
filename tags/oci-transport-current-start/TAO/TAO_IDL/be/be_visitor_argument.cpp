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

#include "be_visitor_argument.h"
#include "be_visitor_context.h"
#include "be_argument.h"
#include "be_type.h"
#include "be_array.h"
#include "be_enum.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_component.h"
#include "be_component_fwd.h"
#include "be_native.h"
#include "be_predefined_type.h"
#include "be_string.h"
#include "be_typedef.h"
#include "be_structure.h"
#include "be_union.h"
#include "be_sequence.h"
#include "be_valuebox.h"
#include "be_valuetype.h"
#include "be_valuetype_fwd.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_home.h"
#include "be_operation.h"
#include "be_helper.h"
#include "utl_identifier.h"
#include "nr_extern.h"
#include "ace/Log_Msg.h"

#include "be_visitor_argument/arglist.cpp"
#include "be_visitor_argument/argument.cpp"
#include "be_visitor_argument/marshal_ss.cpp"
#include "be_visitor_argument/post_upcall_ss.cpp"
#include "be_visitor_argument/upcall_ss.cpp"
#include "be_visitor_argument/vardecl_ss.cpp"
#include "be_visitor_argument/invoke_cs.cpp"
#include "be_visitor_argument/paramlist.cpp"

ACE_RCSID (be,
           be_visitor_argument,
           "$Id$")
