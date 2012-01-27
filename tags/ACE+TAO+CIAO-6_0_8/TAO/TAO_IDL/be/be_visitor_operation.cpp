
//=============================================================================
/**
 *  @file    be_visitor_operation.cpp
 *
 *  $Id$
 *
 *  Visitors for generation of code for Operation
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "be_argument.h"
#include "be_array.h"
#include "be_attribute.h"
#include "be_enum.h"
#include "be_exception.h"
#include "be_interface_fwd.h"
#include "be_native.h"
#include "be_operation.h"
#include "be_module.h"
#include "be_predefined_type.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_type.h"
#include "be_typedef.h"
#include "be_union.h"
#include "be_valuebox.h"
#include "be_valuetype.h"
#include "be_valuetype_fwd.h"
#include "be_component.h"
#include "be_component_fwd.h"
#include "be_connector.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_home.h"
#include "be_porttype.h"
#include "be_provides.h"
#include "be_factory.h"
#include "be_extern.h"
#include "be_helper.h"
#include "be_identifier_helper.h"
#include "be_util.h"

#include "utl_identifier.h"
#include "utl_exceptlist.h"
#include "nr_extern.h"

#include "be_visitor_operation.h"
#include "be_visitor_argument.h"
#include "be_visitor_context.h"
#include "ace/Log_Msg.h"
#include "ace/String_Base.h"

#include "be_visitor_operation/operation.cpp"
#include "be_visitor_operation/arglist.cpp"
#include "be_visitor_operation/argument.cpp"
#include "be_visitor_operation/argument_invoke.cpp"
#include "be_visitor_operation/argument_marshal.cpp"
#include "be_visitor_operation/exceptlist_cs.cpp"
#include "be_visitor_operation/exceptlist_ss.cpp"
#include "be_visitor_operation/operation_ch.cpp"
#include "be_visitor_operation/operation_cs.cpp"
#include "be_visitor_operation/operation_sh.cpp"
#include "be_visitor_operation/operation_ih.cpp"
#include "be_visitor_operation/operation_ss.cpp"
#include "be_visitor_operation/operation_is.cpp"
#include "be_visitor_operation/operation_svs.cpp"
#include "be_visitor_operation/operation_exs.cpp"
#include "be_visitor_operation/rettype.cpp"
#include "be_visitor_operation/rettype_return_cs.cpp"
#include "be_visitor_operation/tie_sh.cpp"
#include "be_visitor_operation/tie_ss.cpp"
#include "be_visitor_operation/upcall_command_ss.cpp"

// AMI
#include "be_visitor_operation/ami_cs.cpp"
#include "be_visitor_operation/ami_handler_reply_stub_operation_cs.cpp"

// Smart proxy
#include "be_visitor_operation/smart_proxy_ch.cpp"
#include "be_visitor_operation/smart_proxy_cs.cpp"

// Collocation
#include "be_visitor_operation/proxy_impl_xh.cpp"
#include "be_visitor_operation/direct_proxy_impl_ss.cpp"

// AMH
#include "be_visitor_operation/amh_ss.cpp"
#include "be_visitor_operation/amh_sh.cpp"
#include "be_visitor_operation/amh_rh_ss.cpp"
#include "be_visitor_operation/amh_rh_sh.cpp"

