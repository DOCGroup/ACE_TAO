#include "ace/Dynamic_Service_Base.h"
#include "ace/ACE.h"
#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Types.h"
#include "ace/Log_Category.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

void
ACE_Dynamic_Service_Base::dump () const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Dynamic_Service_Base::dump");

  ACELIB_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACELIB_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
  ACELIB_DEBUG ((LM_DEBUG, ACE_END_DUMP));
#endif /* ACE_HAS_DUMP */
}

// Get the instance using <name> for the current global
// service configuration repository.

void *
ACE_Dynamic_Service_Base::instance (ACE_TCHAR const *name, bool no_global)
{
  ACE_TRACE ("ACE_Dynamic_Service_Base::instance");
  return instance (ACE_Service_Config::current (), name, no_global);
}

// Find a service registration

ACE_Service_Type const *
ACE_Dynamic_Service_Base::find_i (ACE_Service_Gestalt const* &repo,
                                  ACE_TCHAR const *name,
                                  bool no_global)
{
  ACE_TRACE ("ACE_Dynamic_Service_Base::find_i");
  ACE_Service_Type const *svc_rec {};

  ACE_Service_Gestalt* global = ACE_Service_Config::global ();

  for ( ; (repo->find (name, &svc_rec) == -1) && !no_global; repo = global)
    {
      // Check the static repo, too if different
      if (repo == global)
      {
        break;
      }
    }

  return svc_rec;
}

// Get the instance using <name> for specific configuration repository.
void *
ACE_Dynamic_Service_Base::instance (ACE_Service_Gestalt const* repo,
                                    ACE_TCHAR const *name,
                                    bool no_global)
{
  ACE_TRACE ("ACE_Dynamic_Service_Base::instance");

  void *obj {};
  ACE_Service_Type_Impl const *type {};

  ACE_Service_Gestalt const* repo_found = repo;
  ACE_Service_Type const *svc_rec = find_i (repo_found, name, no_global);
  if (svc_rec != nullptr)
    {
      type = svc_rec->type ();
      if (type != nullptr)
      {
        obj = type->object ();
      }
    }

  if (ACE::debug ())
    {
      ACE_GUARD_RETURN (ACE_Log_Msg, log_guard, *ACE_Log_Msg::instance (), nullptr);

      if (repo->repo_ != repo_found->repo_)
        {
          ACELIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("ACE (%P|%t) DSB::instance, repo=%@, name=%s")
                    ACE_TEXT (" type=%@ => %@")
                    ACE_TEXT (" [in repo=%@]\n"),
                    repo->repo_, name, type, obj,
                    repo_found->repo_));
        }
      else
        {
          ACELIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("ACE (%P|%t) DSB::instance, repo=%@, name=%s")
                    ACE_TEXT (" type=%@ => %@\n"),
                    repo->repo_, name, type, obj));
        }
    }

  return obj;
}

ACE_END_VERSIONED_NAMESPACE_DECL
