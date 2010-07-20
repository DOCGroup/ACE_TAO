// $Id$
/**
 * @file SHS_CORBA_Transport.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 */

#ifndef SHS_CORBA_TRANSPORT_H
#define SHS_CORBA_TRANSPORT_H

#include "System_Health/Interceptors/SHS_Transport.h"
#include "System_Health/Interceptors/DAnCE_SHS_CORBA_Transport_export.h"
#include "System_Health/Daemon/SHS_DaemonC.h"

namespace DAnCE
{
  namespace SHS
  {
    class DAnCE_SHS_CORBA_Transport_Export SHS_CORBA_Transport : 
      public virtual SHS_Transport
    {
    public:
      SHS_CORBA_Transport (void);
      
      ~SHS_CORBA_Transport (void);
      
      virtual void configure (const Deployment::Properties &);
      
      virtual void push_event (const Status_Update &update);
    private:
      DAnCE::SHS::SHS_Daemon_var shs_daemon_;
    };
  }
}
#endif
