// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_interface.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Interface
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

#include "be_visitor_interface.h"

// include all the individual files
#include "be_visitor_interface/interface.cpp"
#include "be_visitor_interface/interface_ch.cpp"
#include "be_visitor_interface/interface_ci.cpp"
#include "be_visitor_interface/interface_cs.cpp"
#include "be_visitor_interface/interface_sh.cpp"
#include "be_visitor_interface/interface_ih.cpp"
#include "be_visitor_interface/interface_si.cpp"
#include "be_visitor_interface/interface_ss.cpp"
#include "be_visitor_interface/interface_is.cpp"
#include "be_visitor_interface/thru_poa_collocated_sh.cpp"
#include "be_visitor_interface/thru_poa_collocated_ss.cpp"
#include "be_visitor_interface/direct_collocated_sh.cpp"
#include "be_visitor_interface/direct_collocated_ss.cpp"
#include "be_visitor_interface/tie_sh.cpp"
#include "be_visitor_interface/tie_si.cpp"
#include "be_visitor_interface/any_op_ch.cpp"
#include "be_visitor_interface/any_op_cs.cpp"
#include "be_visitor_interface/cdr_op_ch.cpp"
#include "be_visitor_interface/cdr_op_ci.cpp"
#include "be_visitor_interface/cdr_op_cs.cpp"
#include "be_visitor_interface/smart_proxy_ch.cpp"
#include "be_visitor_interface/smart_proxy_cs.cpp"
#include "be_visitor_interface/ami_interface_ch.cpp"
#include "be_visitor_interface/interceptors_ch.cpp"
#include "be_visitor_interface/interceptors_cs.cpp"
#include "be_visitor_interface/interceptors_sh.cpp"
#include "be_visitor_interface/interceptors_ss.cpp"

// Proxy Brokers
#include "be_visitor_interface/base_proxy_broker_ch.cpp"
#include "be_visitor_interface/proxy_brokers_ch.cpp"
#include "be_visitor_interface/remote_proxy_broker_ch.cpp"
#include "be_visitor_interface/remote_proxy_broker_cs.cpp"
#include "be_visitor_interface/strategized_proxy_broker_sh.cpp"
#include "be_visitor_interface/strategized_proxy_broker_ss.cpp"

// Proxy Impls
#include "be_visitor_interface/proxy_impls_ch.cpp"
#include "be_visitor_interface/base_proxy_impl_ch.cpp"
#include "be_visitor_interface/remote_proxy_impl_ch.cpp"
#include "be_visitor_interface/remote_proxy_impl_cs.cpp"
#include "be_visitor_interface/thru_poa_proxy_impl_sh.cpp"
#include "be_visitor_interface/thru_poa_proxy_impl_ss.cpp"
#include "be_visitor_interface/direct_proxy_impl_sh.cpp"
#include "be_visitor_interface/direct_proxy_impl_ss.cpp"

// AMH
#include "be_visitor_interface/amh_ch.cpp"
#include "be_visitor_interface/amh_ss.cpp"
#include "be_visitor_interface/amh_sh.cpp"
#include "be_visitor_interface/amh_rh_ss.cpp"
#include "be_visitor_interface/amh_rh_sh.cpp"

ACE_RCSID(be, be_visitor_interface, "$Id$")
