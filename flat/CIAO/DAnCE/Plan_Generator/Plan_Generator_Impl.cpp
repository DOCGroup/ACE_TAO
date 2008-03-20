/**
 * $Id$
 * @file Plan_Generator_Impl.cpp
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 */

#include "Plan_Generator_Impl.h"
#include "orbsvcs/CosNamingC.h"
#include "PCVisitor.h"
#include "PCVisitorBase.h"
#include "ace/OS.h"

namespace DAnCE
{
  namespace Plan_Generator
  {
    // @todo make this a private method
    static CORBA::Object_ptr
    fetch_reference_naming (CORBA::ORB_ptr orb,
                            const char *repoman_name = 0)
    {
      CORBA::Object_var tmp = orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var pns =
        CosNaming::NamingContext::_narrow (tmp.in ());

      if (CORBA::is_nil (pns.in ()))
        {
          return CORBA::Object::_nil ();
        }

      CosNaming::Name name (1);
      name.length (1);

      name[0].id = CORBA::string_dup (repoman_name);

      return pns->resolve (name);
    }

    Plan_Generator_i::Plan_Generator_i ()
      : rm_ ()
    {
    }

    bool
    Plan_Generator_i::init (CORBA::ORB_ptr orb,
                            bool rm_use_naming,
                            const char *rm_name)
    {
      CORBA::Object_var obj;

      if (rm_use_naming)
        {
          obj = fetch_reference_naming (orb, rm_name);
        }
      else
        {
          obj = orb->string_to_object (rm_name);
        }

      this->rm_ = Deployment::RepositoryManager::_narrow (obj.in ());

      if (CORBA::is_nil (this->rm_.in ()))
        {
          DANCE_ERROR((LM_ERROR, "[%M] (%P|%t) CIAO_PlanGenerator: nil Repository "
            "Manager reference, narrow failed\n"));
          return false;
        }

      return true;
    }

    bool
    Plan_Generator_i::generate_plan (Deployment::DeploymentPlan &plan,
                                     const char *package_uri,
                                     bool use_package_name)
    {
      if (CORBA::is_nil (rm_.in ()))
        {
          DANCE_ERROR((LM_ERROR, "[%M] (%P|%t) CIAO_Repoman: nil Execution\n"));
          return false;
        }

      ::Deployment::PackageConfiguration_var pc;

      ACE_TCHAR package[PACKAGE_NAME_LEN];

      size_t length = ACE_OS::strlen (package_uri);
      size_t pos1 = 0;
      size_t pos2 = ACE_OS::strcspn (package_uri + pos1, "+");

      while (pos1 < length)
        {
          ACE_OS::strsncpy (package, package_uri + pos1, pos2 + 1);

          if (use_package_name)
            {
              pc = this->rm_->findPackageByName (package);

              PCVisitor pcv (plan, *pc, true);
              pcv.Visit ();
            }
          else
            {
              CORBA::StringSeq_var seq = this->rm_->findNamesByType (package);

              for (CORBA::ULong i = 0; i < seq->length (); ++i)
                {
                  pc = this->rm_->findPackageByName (seq[i]);

                  PCVisitor pcv (plan, *pc, true);
                  pcv.Visit ();
                }
            }

          pos1 += pos2 + 1;
          pos2 = ACE_OS::strcspn (package_uri + pos1, "+");
        }

        return true;
    }

    bool
    Plan_Generator_i::generate_plan (Deployment::DeploymentPlan &plan,
                                     const char *specific_type,
                                     int &index)
    {
      if (CORBA::is_nil (rm_.in ()))
        {
          DANCE_ERROR((LM_ERROR, "[%M] (%P|%t) CIAO_Repoman: nil Execution\n"));
          return false;
        }

      ::Deployment::PackageConfiguration_var pc;

      CORBA::StringSeq_var seq = rm_->findNamesByType (specific_type);
      for (CORBA::ULong i = 0; i < seq->length (); ++i)
        {
          pc = rm_->findPackageByName (seq[i]);

          PCVisitor pcv (plan, *pc, false);
          index = pcv.Visit ();
        }

      return true;
    }
  }
}
