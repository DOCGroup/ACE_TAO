// Dynamic_Service.cpp
// $Id$

#if !defined (ACE_DYNAMIC_SERVICE_C)
#define ACE_DYNAMIC_SERVICE_C

#define ACE_BUILD_DLL
#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Dynamic_Service.h"

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
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("find")),
                      0);

  const ACE_Service_Type_Impl *type = svc_rec->type ();
  
  if (type == 0)
    ACE_ERROR_RETURN ((LM_ERROR,  
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("type")),
                      0);
  else
    {
      const void *obj = type->object ();
      // This should be an RTTI typesafe downcast...
      SERVICE *n = (SERVICE *) obj;
      return n;
    }
}

#endif /* ACE_DYNAMIC_SERVICE_C */
