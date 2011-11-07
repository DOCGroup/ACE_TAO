// -*- C++ -*-

//=============================================================================
/**
 *  @file    Object_T.h
 *
 *  $Id$
 *
 *   Templatized utilities common to all IDL interfaces.
 *
 *  @author  Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_CORBA_OBJECT_T_H
#define TAO_CORBA_OBJECT_T_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"
#include "tao/Collocation_Strategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;
}

namespace TAO
{
  template<typename T>
  class Narrow_Utils
  {
  public:
    typedef T *T_ptr;

    static T_ptr narrow (CORBA::Object_ptr,
                         const char *repo_id);

    /// Version used the operators.
    static T_ptr unchecked_narrow (CORBA::Object_ptr);

  private:
    // Code for lazily evaluated IORs.
    static T_ptr lazy_evaluation (CORBA::Object_ptr);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Object_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Object_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_CORBA_OBJECT_T_H */
