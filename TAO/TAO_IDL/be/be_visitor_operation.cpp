//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_operation.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Operation
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

// include all the individual files
#include "be_visitor_operation/operation.cpp"
#include "be_visitor_operation/arglist.cpp"
#include "be_visitor_operation/argument.cpp"
#include "be_visitor_operation/argument_invoke.cpp"
#include "be_visitor_operation/argument_marshal.cpp"
#include "be_visitor_operation/exceptlist_cs.cpp"
#include "be_visitor_operation/operation_ch.cpp"
#include "be_visitor_operation/operation_cs.cpp"
#include "be_visitor_operation/operation_sh.cpp"
#include "be_visitor_operation/operation_ih.cpp"
#include "be_visitor_operation/operation_ss.cpp"
#include "be_visitor_operation/operation_is.cpp"
#include "be_visitor_operation/rettype.cpp"
#include "be_visitor_operation/rettype_is.cpp"
#include "be_visitor_operation/rettype_assign_ss.cpp"
#include "be_visitor_operation/rettype_marshal_ss.cpp"
#include "be_visitor_operation/rettype_post_invoke_cs.cpp"
#include "be_visitor_operation/rettype_post_upcall_ss.cpp"
#include "be_visitor_operation/rettype_return_cs.cpp"
#include "be_visitor_operation/rettype_vardecl_cs.cpp"
#include "be_visitor_operation/rettype_vardecl_ss.cpp"
#include "be_visitor_operation/tie_sh.cpp"
#include "be_visitor_operation/tie_si.cpp"
#include "be_visitor_operation/rettype_pre_invoke_cs.cpp"

#include "be_visitor_operation/ami_ch.cpp"
#include "be_visitor_operation/ami_cs.cpp"
#include "be_visitor_operation/ami_handler_reply_stub_operation_ch.cpp"
#include "be_visitor_operation/ami_handler_reply_stub_operation_cs.cpp"
#include "be_visitor_operation/ami_exception_holder_operation_cs.cpp"
#include "be_visitor_operation/smart_proxy_ch.cpp"
#include "be_visitor_operation/smart_proxy_cs.cpp"
#include "be_visitor_operation/interceptors_ch.cpp"
#include "be_visitor_operation/interceptors_cs.cpp"
#include "be_visitor_operation/interceptors_arglist.cpp"
#include "be_visitor_operation/interceptors_sh.cpp"
#include "be_visitor_operation/interceptors_ss.cpp"
#include "be_visitor_operation/interceptors_exceptlist.cpp"
#include "be_visitor_operation/interceptors_info_rettype.cpp"
#include "be_visitor_operation/interceptors_result.cpp"
#include "be_visitor_operation/base_proxy_impl_ch.cpp"
#include "be_visitor_operation/proxy_impl_xh.cpp"
#include "be_visitor_operation/inv_arglist.cpp"
#include "be_visitor_operation/remote_proxy_impl_cs.cpp"
#include "be_visitor_operation/thru_poa_proxy_impl_ss.cpp"
#include "be_visitor_operation/direct_proxy_impl_ss.cpp"

// AMH
#include "be_visitor_operation/amh_ss.cpp"
#include "be_visitor_operation/amh_sh.cpp"
#include "be_visitor_operation/amh_rh_ss.cpp"
#include "be_visitor_operation/amh_rh_sh.cpp"

ACE_RCSID(be, be_visitor_operation, "$Id$")
