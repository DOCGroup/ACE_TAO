// -*- C++ -*-
//=============================================================================
/**
 *  @file   resolve_init.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef RESOVLE_INIT_H
#define RESOVLE_INIT_H

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class T>
typename T::_ptr_type
resolve_init (CORBA::ORB_ptr orb,
              const char *id ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
    typename T::_var_type ref;
    CORBA::Object_var     obj;
    ACE_TRY
    {
      CORBA::Object_var obj;
      obj = orb->resolve_initial_references(id ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ref = T::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
    }

    ACE_CATCHANY
    {
      ACE_ERROR((
                  LM_ERROR,
                  " (%P|%t) Unable to initialize %s.\n",
              id
            ));
      if (!CORBA::is_nil(obj.in()))
        ACE_ERROR((
                    LM_ERROR,
                    "    due to narrowing problem\n"
              ));
      ACE_RE_THROW;
    }

    ACE_ENDTRY;
    return ref._retn();
}

#ifdef TAO_Naming_Export
template<class T>
typename T::_ptr_type
resolve (CosNaming::NamingContext_ptr context,
         const CosNaming::Name &id
           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
    CORBA::Object_var obj;
    obj = context->resolve(id ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(T::_nil());

    ACE_ASSERT(!CORBA::is_nil(obj.in()));

    typename T::_var_type ref = T::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
    return ref._retn();
}
#endif

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
