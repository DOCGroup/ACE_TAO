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
//    be_visitor_module.h
//
// = DESCRIPTION
//    Concrete visitor for the Module class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_BE_VISITOR_MODULE_H
#define TAO_BE_VISITOR_MODULE_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

#include "be_visitor_module/module.h"
// we need derived module visitors for the client and server header files. For
// the others, they use the default module visitor
#include "be_visitor_module/module_ch.h"
#include "be_visitor_module/module_sh.h"
#include "be_visitor_module/module_ih.h"
#include "be_visitor_module/any_op.h"
#include "be_visitor_module/cdr_op.h"

#endif // TAO_BE_VISITOR_MODULE_H
