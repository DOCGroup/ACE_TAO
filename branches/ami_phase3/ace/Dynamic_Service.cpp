// Dynamic_Service.cpp
// $Id$

#ifndef ACE_DYNAMIC_SERVICE_C
#define ACE_DYNAMIC_SERVICE_C

#define ACE_BUILD_DLL
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Repository.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(ace, Dynamic_Service, "$Id$")

template <class SERVICE> void
ACE_Dynamic_Service<SERVICE>::dump (void) const
{
  ACE_TRACE ("ACE_Dynamic_Service<SERVICE>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Get the instance using <name>.

template <class SERVICE> SERVICE *
ACE_Dynamic_Service<SERVICE>::instance (const char *name)
{
  ACE_TRACE ("ACE_Dynamic_Service::instance");
  const ACE_Service_Type *svc_rec;

  if (ACE_Service_Repository::instance ()->find (name,
                                                 &svc_rec) == -1)
    return 0;

  const ACE_Service_Type_Impl *type = svc_rec->type ();

  if (type == 0)
    return 0;
  else
    {
      void *obj = type->object ();
      return ACE_reinterpret_cast (SERVICE *, obj);
    }
}

#endif /* ACE_DYNAMIC_SERVICE_C */
