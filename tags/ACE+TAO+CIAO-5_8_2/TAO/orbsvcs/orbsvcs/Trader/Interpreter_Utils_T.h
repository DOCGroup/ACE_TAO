// -*- C++ -*-

// $Id$

#ifndef TAO_INTERPRETER_UTILS_T_H
#define TAO_INTERPRETER_UTILS_T_H
#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class OPERAND_TYPE>
CORBA::Boolean TAO_find (const CORBA::Any& sequence, const OPERAND_TYPE& operand);

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Trader/Interpreter_Utils_T.cpp"
#endif  /* ACE_TEMPLATES_REQUIRE_SOURCE */

#include /**/ "ace/post.h"
#endif /* TAO_INTERPRETER_UTILS_T_H */
