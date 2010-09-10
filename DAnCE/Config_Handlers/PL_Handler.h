// $Id$

/**
 * @file PL_Handler.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Handles PlanLocality elements
 *
 */

#ifndef CIAO_CONFIG_HANDLERS_PL_HANDLER_
#define CIAO_CONFIG_HANDLERS_PL_HANDLER_

#include "Config_Handlers_Export.h"
#include "ace/config-lite.h"

#include "Utils/Functors.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{// $Id$

  struct PlanLocality;
  class PlanLocalities;
}

namespace DAnCE
{
  namespace Config_Handlers
  {
    class PlanLocality;

    class Config_Handlers_Export PL_Handler
    {
    public:
      static void get_PlanLocality (const PlanLocality &desc,
                                    Deployment::PlanLocality &toconfig);
      
      static PlanLocality  plan_locality (const ::Deployment::PlanLocality &src);
    };
    
    typedef Sequence_Handler < PlanLocality,
                               ::Deployment::PlanLocalities,
                               ::Deployment::PlanLocality,
                               PL_Handler::get_PlanLocality >  PL_Functor;
    
  }
}

#endif
