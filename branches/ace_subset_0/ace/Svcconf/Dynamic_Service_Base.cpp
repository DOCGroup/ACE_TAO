// $Id$

#include "ace/Svcconf/Dynamic_Service_Base.h"
#include "ace/Svcconf/Service_Config.h"
#include "ace/Svcconf/Service_Repository.h"

#ifdef ACE_SUBSET_0
#include "ace/Logging/Log_Msg.h"
#endif

ACE_RCSID(ace, Dynamic_Service_Base, "$Id$")

#ifdef ACE_SUBSET_0
void
ACE_Dynamic_Service_Base::dump (void) const
{
  ACE_TRACE ("ACE_Dynamic_Service_Base::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
#endif

// Get the instance using <name>.

void *
ACE_Dynamic_Service_Base::instance (const ACE_TCHAR *name)
{
  ACE_TRACE ("ACE_Dynamic_Service_Base::instance");
  const ACE_Service_Type *svc_rec;

  if (ACE_Service_Repository::instance ()->find (name,
                                                 &svc_rec) == -1)
    return 0;

  const ACE_Service_Type_Impl *type = svc_rec->type ();

  if (type == 0)
    return 0;

  void *obj = type->object ();
  return obj;
}
