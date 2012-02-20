
//=============================================================================
/**
 *  @file    attribute.h
 *
 *  $Id$
 *
 *  Visitors for generation of code for Attribute
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "be_array.h"
#include "be_component.h"
#include "be_enum.h"
#include "be_eventtype.h"
#include "be_home.h"
#include "be_predefined_type.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_typedef.h"
#include "be_union.h"
#include "be_valuebox.h"
#include "be_valuetype.h"
#include "be_attribute.h"
#include "be_operation.h"
#include "be_predefined_type.h"
#include "be_argument.h"

#include "be_helper.h"

#include "be_visitor_attribute.h"
#include "be_visitor_context.h"
#include "be_visitor_operation.h"
#include "be_visitor_any_extracted_type_decl.h"

#include "ast_generator.h"
#include "utl_identifier.h"
#include "utl_exceptlist.h"
#include "global_extern.h"
#include "ace/Log_Msg.h"

