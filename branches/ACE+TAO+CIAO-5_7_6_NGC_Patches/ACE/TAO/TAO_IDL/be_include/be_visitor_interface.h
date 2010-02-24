
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_interface.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface class
//
// = AUTHOR
//    Carlos O'Ryan and Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_BE_VISITOR_INTERFACE_H
#define TAO_BE_VISITOR_INTERFACE_H

#include "idl_defines.h"
#include "ace/SString.h"

#include "be_visitor_scope.h"
#include "be_visitor_interface/interface.h"
#include "be_visitor_interface/interface_ch.h"
#include "be_visitor_interface/interface_ci.h"
#include "be_visitor_interface/interface_cs.h"
#include "be_visitor_interface/interface_sh.h"
#include "be_visitor_interface/interface_ih.h"
#include "be_visitor_interface/interface_si.h"
#include "be_visitor_interface/interface_ss.h"
#include "be_visitor_interface/interface_is.h"
#include "be_visitor_interface/interface_ex_idl.h"
#include "be_visitor_interface/interface_svh.h"
#include "be_visitor_interface/interface_svs.h"
#include "be_visitor_interface/any_op_ch.h"
#include "be_visitor_interface/any_op_cs.h"
#include "be_visitor_interface/cdr_op_ch.h"
#include "be_visitor_interface/cdr_op_cs.h"
#include "be_visitor_interface/tie_sh.h"
#include "be_visitor_interface/tie_ss.h"
#include "be_visitor_interface/ami_interface_ch.h"
#include "be_visitor_interface/smart_proxy_ch.h"
#include "be_visitor_interface/smart_proxy_cs.h"

// Proxy Brokers
#include "be_visitor_interface/strategized_proxy_broker_sh.h"
#include "be_visitor_interface/strategized_proxy_broker_ss.h"

// Proxy Impls
#include "be_visitor_interface/direct_proxy_impl_sh.h"
#include "be_visitor_interface/direct_proxy_impl_ss.h"

// AMH
#include "be_visitor_interface/amh_ch.h"
#include "be_visitor_interface/amh_sh.h"
#include "be_visitor_interface/amh_si.h"
#include "be_visitor_interface/amh_ss.h"
#include "be_visitor_interface/amh_rh_sh.h"
#include "be_visitor_interface/amh_rh_ss.h"

#endif /* _BE_VISITOR_INTERFACE_H */
