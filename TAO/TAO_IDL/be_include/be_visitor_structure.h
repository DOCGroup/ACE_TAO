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
//    be_visitor_structure.h
//
// = DESCRIPTION
//    Concrete visitor for the Structure class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_BE_VISITOR_STRUCTURE_H
#define TAO_BE_VISITOR_STRUCTURE_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

// generic visitor
#include "be_visitor_structure/structure.h"
// concrete visitors
#include "be_visitor_structure/structure_ch.h"
#include "be_visitor_structure/structure_ci.h"
#include "be_visitor_structure/structure_cs.h"
#include "be_visitor_structure/any_op_ch.h"
#include "be_visitor_structure/any_op_cs.h"
#include "be_visitor_structure/cdr_op_ch.h"
#include "be_visitor_structure/cdr_op_cs.h"

#endif // TAO_BE_VISITOR_STRUCTURE_H
