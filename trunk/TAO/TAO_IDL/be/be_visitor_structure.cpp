//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_structure.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Structure
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_field.h"
#include "be_structure.h"
#include "be_helper.h"
#include "be_extern.h"

#include "be_visitor_structure.h"
#include "be_visitor_field.h"
#include "be_visitor_typecode.h"
#include "be_visitor_context.h"

#include "be_visitor_structure/structure.cpp"
#include "be_visitor_structure/structure_ch.cpp"
#include "be_visitor_structure/structure_ci.cpp"
#include "be_visitor_structure/structure_cs.cpp"
#include "be_visitor_structure/any_op_ch.cpp"
#include "be_visitor_structure/any_op_cs.cpp"
#include "be_visitor_structure/cdr_op_ch.cpp"
#include "be_visitor_structure/cdr_op_ci.cpp"
#include "be_visitor_structure/cdr_op_cs.cpp"

ACE_RCSID (be, 
           be_visitor_structure, 
           "$Id$")
