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

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "tao/Versioned_Namespace.h"

#include "tao/ORB.h"
#include "tao/Object.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class T>
typename T::_ptr_type
resolve_init (CORBA::ORB_ptr orb,
              const char *id)
{
    typename T::_var_type ref;
    CORBA::Object_var     obj;
    try
    {
      CORBA::Object_var obj;
      obj = orb->resolve_initial_references(id);

      ref = T::_narrow(obj.in());
    }

    catch (const CORBA::Exception&)
    {
      ORBSVCS_ERROR((
                  LM_ERROR,
                  " (%P|%t) Unable to initialize %s.\n",
              id
            ));
      if (!CORBA::is_nil(obj.in()))
        ORBSVCS_ERROR((
                    LM_ERROR,
                    "    due to narrowing problem\n"
              ));
      throw;
    }

    return ref._retn();
}

#ifdef TAO_Naming_Export
template<class T>
typename T::_ptr_type
resolve (CosNaming::NamingContext_ptr context,
         const CosNaming::Name &id)
{
    CORBA::Object_var obj;
    obj = context->resolve(id);

    ACE_ASSERT(!CORBA::is_nil(obj.in()));

    typename T::_var_type ref = T::_narrow(obj.in());
    return ref._retn();
}
#endif

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
