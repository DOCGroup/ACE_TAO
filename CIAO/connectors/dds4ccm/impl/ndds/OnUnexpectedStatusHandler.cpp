// $Id$

#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/OnUnexpectedStatusHandler.h"
#include "ciao/Logger/Log_Macros.h"

CIAO::DDS4CCM::On_Unexpected_Status_Handler::On_Unexpected_Status_Handler (
  ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
  ::DDS::Entity_ptr entity,
  ::DDS::StatusKind status_kind) :
    error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener)),
    entity_ (::DDS::Entity::_duplicate (entity)),
    status_kind_ (status_kind)
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

CIAO::DDS4CCM::On_Unexpected_Status_Handler::~On_Unexpected_Status_Handler (void)
{
}

int
CIAO::DDS4CCM::On_Unexpected_Status_Handler::handle_exception (ACE_HANDLE)
{
  try
    {
      this->error_listener_->on_unexpected_status (this->entity_, this->status_kind_);
    }
  catch (...)
    {
    }
  return 0;
}
