// $Id$
/**
 * @file SHS_Transport.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Abstraction for selecting the transport that SHS updates go over.
 */

#ifndef SHS_TRANSPORT_H
#define SHS_TRANSPORT_H

#include /**/ "ace/pre.h"

#include "Deployment/Deployment_DeploymentPlanC.h"
#include "System_Health/SHS_DataModelC.h"

namespace DAnCE
{
  namespace SHS
  {
    class SHS_Transport
    {
    public:
      virtual ~SHS_Transport (void) {}

      virtual void configure (const Deployment::Properties &) = 0;
      
      virtual void push_event (const Status_Update &update) = 0;
    };
  }
}

#include /**/ "ace/post.h"

#endif
