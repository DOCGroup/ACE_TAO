//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_tmplinst.cpp
//
// = DESCRIPTION
//    Visitors for generation of explicit template instantiations.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_root.h"
#include "be_module.h"
#include "be_interface.h"
#include "be_valuetype.h"
#include "be_array.h"
#include "be_enum.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_union.h"
#include "be_operation.h"
#include "be_argument.h"
#include "be_interface_fwd.h"
#include "be_valuetype_fwd.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_structure_fwd.h"
#include "be_field.h"
#include "be_union_fwd.h"
#include "be_union_branch.h"
#include "be_typedef.h"
#include "be_helper.h"
#include "be_extern.h"
#include "utl_identifier.h"

#include "be_visitor_tmplinst.h"
#include "be_visitor_context.h"

#include "be_visitor_tmplinst/tmplinst.cpp"
#include "be_visitor_tmplinst/arg_tmplinst.cpp"
#include "be_visitor_tmplinst/tmplinst_cs.cpp"
#include "be_visitor_tmplinst/tmplinst_ss.cpp"

ACE_RCSID (be, 
           be_visitor_tmplinst, 
           "$Id$")

