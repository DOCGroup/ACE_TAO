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

#include "be_array.h"
#include "be_enum.h"
#include "be_exception.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_component.h"
#include "be_component_fwd.h"
#include "be_home.h"
#include "be_predefined_type.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_structure_fwd.h"
#include "be_union.h"
#include "be_union_fwd.h"
#include "be_type.h"
#include "be_typedef.h"
#include "be_valuebox.h"
#include "be_valuetype.h"
#include "be_valuetype_fwd.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_helper.h"
#include "be_extern.h"
#include "ast_root.h"
#include "utl_identifier.h"
#include "nr_extern.h"
#include "global_extern.h"

#include "be_visitor_sequence.h"
#include "be_visitor_context.h"
#include "ace/Log_Msg.h"

#include "be_visitor_sequence/any_op_ch.cpp"
#include "be_visitor_sequence/any_op_cs.cpp"
#include "be_visitor_sequence/cdr_op_ch.cpp"
#include "be_visitor_sequence/cdr_op_cs.cpp"
#include "be_visitor_sequence/serializer_op_ch.cpp"
#include "be_visitor_sequence/serializer_op_cs.cpp"
#include "be_visitor_sequence/buffer_type.cpp"
#include "be_visitor_sequence/sequence_base.cpp"
#include "be_visitor_sequence/sequence_ch.cpp"
#include "be_visitor_sequence/sequence_cs.cpp"

ACE_RCSID (be, 
           be_visitor_sequence, 
           "$Id$")
