// $Id$

#include "DomainParticipantListener.h"
#include "DomainParticipant.h"
#include "SampleLostStatus.h"
#include "SubscriptionMatchedStatus.h"
#include "RequestedDeadlineMissedStatus.h"
#include "SampleRejectedStatus.h"
#include "LivelinessChangedStatus.h"
#include "RequestedIncompatibleQosStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_DomainParticipantListener_i::RTI_DomainParticipantListener_i (
        ::DDS::DomainParticipantListener_ptr p)
          : impl_ (::DDS::DomainParticipantListener::_duplicate (p))
      {
      }

      RTI_DomainParticipantListener_i::~RTI_DomainParticipantListener_i (void)
      {
      }

      ::DDS::DomainParticipantListener_ptr
      RTI_DomainParticipantListener_i::get_domainparticipantlistener (void)
      {
        return ::DDS::DomainParticipantListener::_duplicate (this->impl_.in ());
      }
    }
  }
}

