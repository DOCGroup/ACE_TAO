/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_operation.h
//
// = DESCRIPTION
//    Concrete visitor for the Operation class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_OPERATION_H)
#define TAO_BE_VISITOR_OPERATION_H

#include "be_visitor.h"
#include "be_visitor_scope.h"
#include "be_visitor_operation/operation_ch.h"
#include "be_visitor_operation/operation_cs.h"
#include "be_visitor_operation/operation_sh.h"
#include "be_visitor_operation/operation_ss.h"
#include "be_visitor_operation/tie_sh.h"
#include "be_visitor_operation/tie_si.h"
#include "be_visitor_operation/collocated_sh.h"
#include "be_visitor_operation/collocated_ss.h"
#include "be_visitor_operation/arglist.h"
#include "be_visitor_operation/rettype.h"
#include "be_visitor_operation/rettype_vardecl_cs.h"
#include "be_visitor_operation/rettype_pre_docall_cs.h"
#include "be_visitor_operation/rettype_pre_docall_cs.h"
#include "be_visitor_operation/rettype_docall_cs.h"
#include "be_visitor_operation/rettype_post_docall_cs.h"
#include "be_visitor_operation/rettype_return_cs.h"
#include "be_visitor_operation/exceptlist_cs.h"
#include "be_visitor_operation/argument.h"
#include "be_visitor_operation/rettype_vardecl_ss.h"
#include "be_visitor_operation/rettype_assign_ss.h"
#include "be_visitor_operation/rettype_post_upcall_ss.h"
#include "be_visitor_operation/rettype_marshal_ss.h"

#endif // TAO_BE_VISITOR_OPERATION_H
