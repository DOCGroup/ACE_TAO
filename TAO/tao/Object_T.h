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

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;
}

namespace TAO
{
  class Collocation_Proxy_Broker;

  typedef 
    Collocation_Proxy_Broker * (* Proxy_Broker_Factory)(CORBA::Object_ptr);

  template<typename T>
  class Narrow_Utils
  {
  public:
    typedef T *T_ptr;

    static T_ptr narrow (CORBA::Object_ptr,
                         const char *repo_id,
                         Proxy_Broker_Factory
                         ACE_ENV_ARG_DECL);

    static T_ptr narrow (CORBA::AbstractBase_ptr,
                         const char *repo_id
                         ACE_ENV_ARG_DECL);

    static T_ptr unchecked_narrow (CORBA::Object_ptr,
                                   Proxy_Broker_Factory);

    static T_ptr unchecked_narrow (CORBA::AbstractBase_ptr);
  private:
    // Code for lazily evaluated IORs.
    static T_ptr lazy_evaluation (CORBA::Object_ptr);
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Object_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Object_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_CORBA_OBJECT_T_H */
