//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_union.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Union
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_enum.h"
#include "be_typedef.h"
#include "be_predefined_type.h"
#include "be_union.h"
#include "be_union_branch.h"
#include "be_helper.h"
#include "be_extern.h"
#include "ast_union_label.h"

#include "be_visitor_union.h"
#include "be_visitor_enum.h"
#include "be_visitor_context.h"
#include "be_visitor_union_branch.h"
#include "be_visitor_typecode.h"
#include "be_visitor_context.h"

#include "be_visitor_union/discriminant_ch.cpp"
#include "be_visitor_union/discriminant_ci.cpp"
#include "be_visitor_union/discriminant_cs.cpp"
#include "be_visitor_union/union.cpp"
#include "be_visitor_union/union_ch.cpp"
#include "be_visitor_union/union_ci.cpp"
#include "be_visitor_union/union_cs.cpp"
#include "be_visitor_union/any_op_ch.cpp"
#include "be_visitor_union/any_op_cs.cpp"
#include "be_visitor_union/cdr_op_ch.cpp"
#include "be_visitor_union/cdr_op_ci.cpp"
#include "be_visitor_union/cdr_op_cs.cpp"

ACE_RCSID (be, 
           be_visitor_union, 
           "$Id$")
