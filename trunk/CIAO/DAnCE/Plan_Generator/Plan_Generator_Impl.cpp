/**
 * $Id$
 * @file Plan_Generator_Impl.cpp
 * @author Shanshan Jiang <shanshan.jiang@vanderbilt.edu>
 */

#include "Plan_Generator_Impl.h"
#include "orbsvcs/CosNamingC.h"
#include "PCVisitor.h"
#include "PCVisitorBase.h"

namespace CIAO
{
  namespace Plan_Generator
  {
    static CORBA::Object_ptr
    fetch_reference_naming (CORBA::ORB_ptr orb,
                            const char *repoman_name = 0
                            ACE_ENV_ARG_DECL)
    {
      CORBA::Object_var tmp =
        orb->resolve_initial_references ("NameService"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::NamingContext_var pns =
        CosNaming::NamingContext::_narrow (tmp.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name name (1);
      name.length (1);
      
      name[0].id = CORBA::string_dup (repoman_name);

      return pns->resolve (name
                           ACE_ENV_ARG_PARAMETER);
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
          obj = fetch_reference_naming (orb, rm_name ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
      else
        {
          obj = orb->string_to_object (rm_name ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      this->rm_ = Deployment::RepositoryManager::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (CORBA::is_nil (this->rm_.in ()))
        {
          ACE_ERROR ((LM_ERROR,
            "(%P|%t) CIAO_PlanGenerator: nil Repository"
            " Manager reference, narrow failed\n"));
          return false;
        }

      return true;
    }

    bool
    Plan_Generator_i::generate_plan (Deployment::DeploymentPlan &plan,
                                     const char *package_uri,
                                     bool use_package_name)
    {

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

              for (size_t i = 0; i < seq->length (); ++i)
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
      ::Deployment::PackageConfiguration_var pc;
      
      CORBA::StringSeq_var seq = rm_->findNamesByType (specific_type);
      for (size_t i = 0; i < seq->length (); ++i)
        {
          pc = rm_->findPackageByName (seq[i]);

          PCVisitor pcv (plan, *pc, false);
          index = pcv.Visit ();
        }

      return true;
    }

  }
}
