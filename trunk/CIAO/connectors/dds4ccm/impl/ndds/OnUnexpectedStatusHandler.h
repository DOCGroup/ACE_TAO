// $Id$
/**
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Eventhandler on orb thread, handling the
 * received data from DDS.
 */

#ifndef CIAO_ON_UNEXPECTEDSTATUS_HANDLER_T
#define CIAO_ON_UNEXPECTEDSTATUS_HANDLER_T

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/idl/dds4ccm_ConnectorStatusListenerC.h"
#include "ace/Event_Handler.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class On_Unexpected_Status_Handler :
      public ACE_Event_Handler
    {
      public:
        On_Unexpected_Status_Handler (
            ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
            ::DDS::Entity_ptr entity,
            ::DDS::StatusKind status_kind);
        virtual ~On_Unexpected_Status_Handler ();

        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        ::CCM_DDS::ConnectorStatusListener_var error_listener_;
        ::DDS::Entity_var entity_;
        ::DDS::StatusKind status_kind_;
    };
  }
}

#endif
