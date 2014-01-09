
//=============================================================================
/**
 *  @file    union.h
 *
 *  $Id$
 *
 *  Visitors for generation of code for Union
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "ace/Log_Msg.h"

#include "ast_union_label.h"

#include "be_enum.h"
#include "be_typedef.h"
#include "be_predefined_type.h"
#include "be_module.h"
#include "be_util.h"
#include "be_interface.h"
#include "be_union.h"
#include "be_union_branch.h"
#include "be_helper.h"
#include "be_extern.h"

#include "be_visitor_union.h"
#include "be_visitor_enum.h"
#include "be_visitor_context.h"
#include "be_visitor_union_branch.h"
#include "be_visitor_typecode.h"
#include "be_visitor_context.h"
