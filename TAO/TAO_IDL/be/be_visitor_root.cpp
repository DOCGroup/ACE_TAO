// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_root.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Root
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_constant.h"
#include "be_enum.h"
#include "be_exception.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_module.h"
#include "be_root.h"
#include "be_structure.h"
#include "be_structure_fwd.h"
#include "be_typedef.h"
#include "be_union.h"
#include "be_union_fwd.h"
#include "be_valuetype.h"
#include "be_valuetype_fwd.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_component.h"
#include "be_component_fwd.h"
#include "be_home.h"
#include "be_helper.h"
#include "be_extern.h"

#include "be_visitor_root.h"
#include "be_visitor_constant.h"
#include "be_visitor_enum.h"
#include "be_visitor_exception.h"
#include "be_visitor_interface.h"
#include "be_visitor_interface_fwd.h"
#include "be_visitor_module.h"
#include "be_visitor_structure.h"
#include "be_visitor_structure_fwd.h"
#include "be_visitor_typedef.h"
#include "be_visitor_union.h"
#include "be_visitor_union_fwd.h"
#include "be_visitor_valuetype.h"
#include "be_visitor_valuetype_fwd.h"
#include "be_visitor_eventtype.h"
#include "be_visitor_eventtype_fwd.h"
#include "be_visitor_component.h"
#include "be_visitor_component_fwd.h"
#include "be_visitor_home.h"
#include "be_visitor_context.h"

#include "be_visitor_root/root.cpp"
#include "be_visitor_root/root_ch.cpp"
#include "be_visitor_root/root_ci.cpp"
#include "be_visitor_root/root_cs.cpp"
#include "be_visitor_root/root_sh.cpp"
#include "be_visitor_root/root_si.cpp"
#include "be_visitor_root/root_ss.cpp"
#include "be_visitor_root/root_sth.cpp"
#include "be_visitor_root/root_is.cpp"
#include "be_visitor_root/root_ih.cpp"
#include "be_visitor_root/any_op.cpp"
#include "be_visitor_root/cdr_op.cpp"

ACE_RCSID (be, 
           be_visitor_root, 
           "$Id$")
