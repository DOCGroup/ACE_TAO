
//=============================================================================
/**
 *  @file    constant.h
 *
 *  Visitors for generation of code for Constant
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "be_constant.h"
#include "be_module.h"
#include "be_helper.h"
#include "be_extern.h"
#include "be_typedef.h"
#include "utl_identifier.h"
#include "ast_expression.h"

#include "be_visitor_constant.h"
#include "be_visitor_context.h"

const char *exprtype_to_cpp_corba_type (AST_Expression::ExprType et);
