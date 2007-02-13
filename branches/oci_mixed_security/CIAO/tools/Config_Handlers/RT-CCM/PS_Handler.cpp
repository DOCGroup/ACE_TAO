// $Id$

#include "PS_Handler.h"
#include "PM_Handler.h"
#include "CIAOServerResources.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    bool
    PS_Handler::policy_set (const PolicySet &src,
                            ::CIAO::DAnCE::PolicySet &dest)
    {
      if (src.id_p ())
        dest.Id = CORBA::string_dup (src.id ().c_str ());

      // Make room for all of the policies
      CORBA::ULong len (dest.policies.length ());
      dest.policies.length (len +
                            src.count_priorityModel () +
                            src.count_threadpool () +
                            src.count_priorityBandedConnection ());

      for (PolicySet::priorityModel_const_iterator i = src.begin_priorityModel ();
           i != src.end_priorityModel ();
           ++i)
        {
          ::CIAO::DAnCE::PriorityModelPolicyDef pmd;

          PM_Handler::priority_model_pd (*i, pmd);

          dest.policies[len++].PriorityModelDef (pmd);
        }

      for (PolicySet::threadpool_const_iterator i = src.begin_threadpool ();
           i != src.end_threadpool ();
           ++i)
        {
          ::CIAO::DAnCE::ThreadpoolPolicyDef tpd;

          tpd.Id = CORBA::string_dup (i->id ().c_str ());

          dest.policies[len++].ThreadpoolDef (tpd);
        }


      for (PolicySet::priorityBandedConnection_const_iterator i = src.begin_priorityBandedConnection ();
           i != src.end_priorityBandedConnection ();
           ++i)
        {
          ::CIAO::DAnCE::PriorityBandedConnectionPolicyDef pbc;

          pbc.Id = CORBA::string_dup (i->id ().c_str ());

          dest.policies[len++].PriorityBandedConnectionDef (pbc);
        }



      return true;
    }


    PolicySet
    PS_Handler::policy_set (const ::CIAO::DAnCE::PolicySet &src)
    {
      PolicySet ps;

      if (src.Id.in ())
        ps.id (src.Id.in ());

      for (CORBA::ULong i = 0;
           i < src.policies.length ();
           ++i)
        {
          ACE_ERROR ((LM_ERROR,
                      "Attempting switch for i = %d\n",
                      i));

          switch (src.policies[i]._d ())
            {
            case ::CIAO::DAnCE::PRIORITY_MODEL_POLICY_TYPE:
              ps.add_priorityModel (
                PM_Handler::priority_model_pd (src.policies[i].PriorityModelDef ()));
              break;

            case ::CIAO::DAnCE::THREADPOOL_POLICY_TYPE:
              ps.add_threadpool  (src.policies[i].ThreadpoolDef ().Id.in ());
              break;

            case ::CIAO::DAnCE::PRIORITY_BANDED_CONNECTION_POLICY_TYPE:
              ps.add_priorityBandedConnection (src.policies[i].PriorityBandedConnectionDef ().Id.in ());
              break;

            case 0:
              ACE_ERROR ((LM_ERROR,
                          "Skipping invalid policy.\n"));
              break;

            default:
              ACE_ERROR ((LM_ERROR,
                          "Bad policy stored in policy_set: %i\n",
                          src.policies[i]._d ()));
              throw 1;
            }
        }

      return ps;
    }

  }

}
