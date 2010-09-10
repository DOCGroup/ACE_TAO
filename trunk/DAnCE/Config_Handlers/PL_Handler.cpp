// $Id$

#include "PL_Handler.h"
#include "IDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  namespace Config_Handlers
  {
    void
    PL_Handler::get_PlanLocality (const PlanLocality &desc,
                                  Deployment::PlanLocality &toconfig)
    {
      DANCE_TRACE ("PL_Handler::get_PlanLocality");

      switch (desc.constraint ().integral ())
        {
        case PlanLocalityKind::SameProcess_l:
          toconfig.constraint = Deployment::PlanSameProcess;
          break;

        case PlanLocalityKind::DifferentProcess_l:
          toconfig.constraint = Deployment::PlanDifferentProcess;
          break;

        case PlanLocalityKind::NoConstraint_l:
          toconfig.constraint = Deployment::PlanNoConstraint;
          break;
        }


      size_t pos = 0;
      toconfig.constrainedInstanceRef.length (desc.count_constrainedInstance ());

      for (PlanLocality::constrainedInstance_const_iterator i = desc.begin_constrainedInstance ();
           i != desc.end_constrainedInstance ();
           ++i)
        {
          CORBA::ULong tmp = 0;
          IDD_Handler::IDREF.find_ref ((*i)->idref ().id ().c_str (), tmp);

          toconfig.constrainedInstanceRef[pos] = tmp;
          ++pos;
        }
    }


  }
}
