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

#include "idl_defines.h"

#include "be_null_return_emitter.h"

#include "be_visitor_scope.h"
#include "be_visitor_operation/operation.h"
#include "be_visitor_operation/operation_ch.h"
#include "be_visitor_operation/operation_cs.h"
#include "be_visitor_operation/operation_sh.h"
#include "be_visitor_operation/operation_ih.h"
#include "be_visitor_operation/operation_ss.h"
#include "be_visitor_operation/operation_is.h"
#include "be_visitor_operation/operation_svs.h"
#include "be_visitor_operation/operation_exs.h"
#include "be_visitor_operation/tie_sh.h"
#include "be_visitor_operation/tie_ss.h"
#include "be_visitor_operation/arglist.h"
#include "be_visitor_operation/rettype.h"
#include "be_visitor_operation/rettype_return_cs.h"
#include "be_visitor_operation/exceptlist_cs.h"
#include "be_visitor_operation/exceptlist_ss.h"
#include "be_visitor_operation/argument.h"
#include "be_visitor_operation/argument_invoke.h"
#include "be_visitor_operation/argument_marshal.h"
#include "be_visitor_operation/ami_ch.h"
#include "be_visitor_operation/ami_cs.h"
#include "be_visitor_operation/ami_handler_reply_stub_operation_ch.h"
#include "be_visitor_operation/ami_handler_reply_stub_operation_cs.h"
#include "be_visitor_operation/ami_exception_holder_operation_cs.h"
#include "be_visitor_operation/smart_proxy_ch.h"
#include "be_visitor_operation/smart_proxy_cs.h"
#include "be_visitor_operation/upcall_command_ss.h"
#include "be_visitor_operation/proxy_impl_xh.h"
#include "be_visitor_operation/direct_proxy_impl_ss.h"

// AMH
#include "be_visitor_operation/amh_sh.h"
#include "be_visitor_operation/amh_ss.h"
#include "be_visitor_operation/amh_rh_sh.h"
#include "be_visitor_operation/amh_rh_ss.h"

#endif // TAO_BE_VISITOR_OPERATION_H
