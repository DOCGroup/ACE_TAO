// Dynamic_Service.cpp
// $Id$

#ifndef ACE_DYNAMIC_SERVICE_C
#define ACE_DYNAMIC_SERVICE_C

#include "ace/Dynamic_Service.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"

#if !defined (__ACE_INLINE__)
#include "ace/Dynamic_Service.i"
#endif /* __ACE_INLINE__ */

template <class TYPE> TYPE *
ACE_Dynamic_Service<TYPE>::instance (const ACE_TCHAR *name)
{
  ACE_Service_Object *svc_obj = 
    ACE_static_cast (ACE_Service_Object *, ACE_Dynamic_Service_Base::instance (name));
  return ACE_dynamic_cast (TYPE *, svc_obj);
}

ACE_RCSID(ace, Dynamic_Service, "$Id$")

#endif /* ACE_DYNAMIC_SERVICE_C */
