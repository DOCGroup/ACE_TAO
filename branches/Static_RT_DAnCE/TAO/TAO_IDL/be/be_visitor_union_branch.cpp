//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_union_branch.cpp
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Union_Branch" node
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_array.h"
#include "be_enum.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_predefined_type.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_typedef.h"
#include "be_union.h"
#include "be_union_branch.h"
#include "be_valuebox.h"
#include "be_valuetype.h"
#include "be_valuetype_fwd.h"
#include "be_helper.h"
#include "ast_union_label.h"
#include "utl_identifier.h"
#include "nr_extern.h"

#include "be_visitor_union_branch.h"
#include "be_visitor_array.h"
#include "be_visitor_enum.h"
#include "be_visitor_sequence.h"
#include "be_visitor_structure.h"
#include "be_visitor_union.h"
#include "be_visitor_context.h"
#include "ace/Log_Msg.h"

#include "be_visitor_union_branch/private_ch.cpp"
#include "be_visitor_union_branch/public_assign_cs.cpp"
#include "be_visitor_union_branch/public_constructor_cs.cpp"
#include "be_visitor_union_branch/public_reset_cs.cpp"
#include "be_visitor_union_branch/public_ch.cpp"
#include "be_visitor_union_branch/public_ci.cpp"
#include "be_visitor_union_branch/public_cs.cpp"
#include "be_visitor_union_branch/cdr_op_ch.cpp"
#include "be_visitor_union_branch/cdr_op_cs.cpp"
#include "be_visitor_union_branch/serializer_op_ch.cpp"
#include "be_visitor_union_branch/serializer_op_cs.cpp"

ACE_RCSID (be,
           be_visitor_union_branch,
           "$Id$")
