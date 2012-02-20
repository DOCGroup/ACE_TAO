
//=============================================================================
/**
 *  @file    be_visitor_typecode.cpp
 *
 *  $Id$
 *
 *  Visitors for generation of code for TypeCodes for various types
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "be_visitor_typecode.h"
#include "be_visitor_context.h"

#include "be_array.h"
#include "be_enum.h"
#include "be_enum_val.h"
#include "be_exception.h"
#include "be_field.h"
#include "be_interface.h"
#include "be_connector.h"
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
#include "be_util.h"

#include "utl_identifier.h"

#include "ast_union_label.h"

