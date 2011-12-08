
//=============================================================================
/**
 *  @file    be_visitor_structure.cpp
 *
 *  $Id$
 *
 *  Visitors for generation of code for Structure
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "ace/Log_Msg.h"

#include "be_field.h"
#include "be_structure.h"
#include "be_helper.h"
#include "be_extern.h"
#include "be_module.h"
#include "be_interface.h"
#include "be_util.h"

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
#include "be_visitor_structure/cdr_op_cs.cpp"
