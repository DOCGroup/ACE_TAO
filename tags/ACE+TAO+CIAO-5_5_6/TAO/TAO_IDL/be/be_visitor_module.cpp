//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_module.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Module
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_module.h"
#include "be_constant.h"
#include "be_enum.h"
#include "be_exception.h"
#include "be_native.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_structure.h"
#include "be_structure_fwd.h"
#include "be_typedef.h"
#include "be_union.h"
#include "be_union_fwd.h"
#include "be_valuebox.h"
#include "be_valuetype.h"
#include "be_valuetype_fwd.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_component.h"
#include "be_component_fwd.h"
#include "be_home.h"
#include "be_extern.h"
#include "be_helper.h"
#include "utl_err.h"

#include "be_visitor_module.h"
#include "be_visitor_constant.h"
#include "be_visitor_enum.h"
#include "be_visitor_native.h"
#include "be_visitor_exception.h"
#include "be_visitor_interface.h"
#include "be_visitor_interface_fwd.h"
#include "be_visitor_structure.h"
#include "be_visitor_structure_fwd.h"
#include "be_visitor_typedef.h"
#include "be_visitor_union.h"
#include "be_visitor_union_fwd.h"
#include "be_visitor_valuebox.h"
#include "be_visitor_valuetype.h"
#include "be_visitor_valuetype_fwd.h"
#include "be_visitor_component.h"
#include "be_visitor_component_fwd.h"
#include "be_visitor_home.h"
#include "be_visitor_context.h"

#include "be_visitor_module/module.cpp"
#include "be_visitor_module/module_ch.cpp"
#include "be_visitor_module/module_sh.cpp"
#include "be_visitor_module/module_ih.cpp"
#include "be_visitor_module/any_op.cpp"
#include "be_visitor_module/cdr_op.cpp"
#include "be_visitor_module/serializer_op.cpp"

ACE_RCSID (be,
           be_visitor_module,
           "$Id$")
