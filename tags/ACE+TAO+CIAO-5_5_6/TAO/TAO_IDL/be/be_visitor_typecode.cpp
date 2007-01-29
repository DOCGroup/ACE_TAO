//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_typecode.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for TypeCodes for various types
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_array.h"
#include "be_enum.h"
#include "be_enum_val.h"
#include "be_exception.h"
#include "be_field.h"
#include "be_interface.h"
#include "be_component.h"
#include "be_home.h"
#include "be_module.h"
#include "be_predefined_type.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_type.h"
#include "be_typedef.h"
#include "be_union.h"
#include "be_union_branch.h"
#include "be_valuebox.h"
#include "be_valuetype.h"
#include "be_eventtype.h"
#include "be_helper.h"
#include "be_extern.h"
#include "utl_identifier.h"
#include "ast_union_label.h"

#include "be_visitor_typecode.h"
#include "be_visitor_context.h"

#include "be_visitor_typecode/typecode_decl.cpp"
#include "be_visitor_typecode/typecode_defn.cpp"

#include "be_visitor_typecode/alias_typecode.cpp"
#include "be_visitor_typecode/enum_typecode.cpp"
#include "be_visitor_typecode/objref_typecode.cpp"
#include "be_visitor_typecode/struct_typecode.cpp"
#include "be_visitor_typecode/union_typecode.cpp"
#include "be_visitor_typecode/value_typecode.cpp"


ACE_RCSID (be,
           be_visitor_typecode,
           "$Id$")
