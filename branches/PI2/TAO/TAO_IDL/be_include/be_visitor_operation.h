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

#ifndef TAO_BE_VISITOR_OPERATION_H
#define TAO_BE_VISITOR_OPERATION_H

#include "be_visitor.h"
#include "be_visitor_scope.h"
#include "be_visitor_operation/operation.h"
#include "be_visitor_operation/operation_ch.h"
#include "be_visitor_operation/operation_cs.h"
#include "be_visitor_operation/operation_sh.h"
#include "be_visitor_operation/operation_ih.h"
#include "be_visitor_operation/operation_ss.h"
#include "be_visitor_operation/operation_is.h"
#include "be_visitor_operation/tie_sh.h"
#include "be_visitor_operation/tie_si.h"
#include "be_visitor_operation/thru_poa_collocated_sh.h"
#include "be_visitor_operation/thru_poa_collocated_ss.h"
#include "be_visitor_operation/direct_collocated_sh.h"
#include "be_visitor_operation/direct_collocated_ss.h"
#include "be_visitor_operation/arglist.h"
#include "be_visitor_operation/rettype.h"
#include "be_visitor_operation/rettype_is.h"
#include "be_visitor_operation/rettype_vardecl_cs.h"
#include "be_visitor_operation/rettype_post_invoke_cs.h"
#include "be_visitor_operation/rettype_return_cs.h"
#include "be_visitor_operation/exceptlist_cs.h"
#include "be_visitor_operation/argument.h"
#include "be_visitor_operation/argument_invoke.h"
#include "be_visitor_operation/argument_marshal.h"
#include "be_visitor_operation/rettype_vardecl_ss.h"
#include "be_visitor_operation/rettype_assign_ss.h"
#include "be_visitor_operation/rettype_post_upcall_ss.h"
#include "be_visitor_operation/rettype_marshal_ss.h"
#include "be_visitor_operation/rettype_pre_invoke_cs.h"
#include "be_visitor_operation/ami_ch.h"
#include "be_visitor_operation/ami_cs.h"
#include "be_visitor_operation/ami_handler_reply_stub_operation_ch.h"
#include "be_visitor_operation/ami_handler_reply_stub_operation_cs.h"
#include "be_visitor_operation/ami_exception_holder_operation_cs.h"
#include "be_visitor_operation/smart_proxy_ch.h"
#include "be_visitor_operation/smart_proxy_cs.h"
#include "be_visitor_operation/interceptors_ch.h"
#include "be_visitor_operation/interceptors_cs.h"
#include "be_visitor_operation/interceptors_arglist.h"
#include "be_visitor_operation/interceptors_sh.h"
#include "be_visitor_operation/interceptors_ss.h"
#include "be_visitor_operation/interceptors_exceptlist.h"


#endif // TAO_BE_VISITOR_OPERATION_H
