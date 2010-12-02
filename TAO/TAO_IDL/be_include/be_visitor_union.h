/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_visitor_union.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Union class
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef TAO_BE_VISITOR_UNION_H
#define TAO_BE_VISITOR_UNION_H

#include "be_enum.h"
#include "be_structure.h"

#include "be_visitor_scope.h"
#include "be_visitor_enum.h"
#include "be_visitor_structure.h"

#include "be_visitor_union/union.h"
#include "be_visitor_union/union_ch.h"
#include "be_visitor_union/union_ci.h"
#include "be_visitor_union/union_cs.h"
#include "be_visitor_union/discriminant_ch.h"
#include "be_visitor_union/discriminant_ci.h"
#include "be_visitor_union/discriminant_cs.h"
#include "be_visitor_union/any_op_ch.h"
#include "be_visitor_union/any_op_cs.h"
#include "be_visitor_union/cdr_op_ch.h"
#include "be_visitor_union/cdr_op_cs.h"

#endif // TAO_BE_VISITOR_UNION_H
