
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
#include "be_visitor_interface/thru_poa_collocated_sh.h"
#include "be_visitor_interface/thru_poa_collocated_ss.h"
#include "be_visitor_interface/direct_collocated_sh.h"
#include "be_visitor_interface/direct_collocated_ss.h"
#include "be_visitor_interface/any_op_ch.h"
#include "be_visitor_interface/any_op_cs.h"
#include "be_visitor_interface/cdr_op_ch.h"
#include "be_visitor_interface/cdr_op_ci.h"
#include "be_visitor_interface/cdr_op_cs.h"
#include "be_visitor_interface/tie_sh.h"
#include "be_visitor_interface/tie_si.h"
#include "be_visitor_interface/ami_interface_ch.h"
#include "be_visitor_interface/smart_proxy_ch.h"
#include "be_visitor_interface/smart_proxy_cs.h"
#include "be_visitor_interface/interceptors_ch.h"
#include "be_visitor_interface/interceptors_cs.h"
#include "be_visitor_interface/interceptors_sh.h"
#include "be_visitor_interface/interceptors_ss.h"

#endif /* _BE_VISITOR_INTERFACE_H */
