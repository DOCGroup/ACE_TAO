
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_interface.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface class
 *
 *
 *  @author Carlos O'Ryan and Aniruddha Gokhale
 */
//=============================================================================

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
#include "be_visitor_interface/interface_ss.h"
#include "be_visitor_interface/interface_is.h"
#include "be_visitor_interface/interface_ex_idl.h"
#include "be_visitor_interface/interface_svth.h"
#include "be_visitor_interface/interface_svts.h"
#include "be_visitor_interface/any_op_ch.h"
#include "be_visitor_interface/any_op_cs.h"
#include "be_visitor_interface/cdr_op_ch.h"
#include "be_visitor_interface/cdr_op_cs.h"
#include "be_visitor_interface/tie_sh.h"
#include "be_visitor_interface/tie_ss.h"
#include "be_visitor_interface/smart_proxy_ch.h"
#include "be_visitor_interface/smart_proxy_cs.h"

// Facets
#include "be_visitor_interface/facet_svth.h"
#include "be_visitor_interface/facet_svts.h"

// Proxy Impls
#include "be_visitor_interface/direct_proxy_impl_sh.h"
#include "be_visitor_interface/direct_proxy_impl_ss.h"

// AMI4CCM
#include "be_visitor_interface/ami4ccm_rh_ex_idl.h"
#include "be_visitor_interface/ami4ccm_sendc_ex_idl.h"
#include "be_visitor_interface/ami4ccm_conn_ex_idl.h"

// AMH
#include "be_visitor_interface/amh_ch.h"
#include "be_visitor_interface/amh_sh.h"
#include "be_visitor_interface/amh_ss.h"
#include "be_visitor_interface/amh_rh_sh.h"
#include "be_visitor_interface/amh_rh_ss.h"

#endif /* _BE_VISITOR_INTERFACE_H */
