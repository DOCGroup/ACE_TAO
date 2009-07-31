// $Id$

#include "DomainParticipantFactory.h"
#include "DomainParticipant.h"
#include "Utils.h"

#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/idl/dds4ccm_BaseC.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_DomainParticipantFactory_i::RTI_DomainParticipantFactory_i (void)
      {
        CIAO_TRACE ("RTI_DomainParticipantFactory_i::RTI_DomainParticipantFactory_i");
      }

      RTI_DomainParticipantFactory_i::~RTI_DomainParticipantFactory_i (void)
      {
        CIAO_TRACE ("RTI_DomainParticipantFactory_i::~RTI_DomainParticipantFactory_i");
      }

      ::DDS::DomainParticipant_ptr
      RTI_DomainParticipantFactory_i::create_participant (::DDS::DomainId_t domain_id,
                                                          const ::DDS::DomainParticipantQos & qos,
                                                          ::DDS::DomainParticipantListener_ptr a_listener,
                                                          ::DDS::StatusMask mask)
      {
        CIAO_TRACE ("RTI_DomainParticipantFactory_i::create_participant");

        CIAO_DEBUG ((LM_TRACE, CLINFO "RTI_DomainParticipantFactory_i::create_participant - "
                     "Creating domain participant\n"));
        DDSDomainParticipant *part = DDSDomainParticipantFactory::get_instance ()->
          create_participant (domain_id,
                              DDS_PARTICIPANT_QOS_DEFAULT,
                              0,
                              mask);

        if (!part)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_DomainParticipantFactory_i::create_participant - "
                         "Error: Unable to create DomainParticipant\n"));
            throw CCM_DDS::InternalError (1, 0);
          }

        ::DDS::DomainParticipant_var retval = new RTI_DomainParticipant_i (part);

        return retval._retn ();
      }


      ::DDS::ReturnCode_t
      RTI_DomainParticipantFactory_i::delete_participant (::DDS::DomainParticipant_ptr a_participant)
      {
        CIAO_TRACE ("RTI_DomainParticipantFactory_i::delete_participant");

        RTI_DomainParticipant_i *part = dynamic_cast< RTI_DomainParticipant_i * > (a_participant);

        if (!part)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_DomainParticipantFactory_i::delete_participant - "
                         "Unable to cast provided object reference to servant type, bailing.\n"));
            return DDS::RETCODE_ERROR;
          }
        CIAO_DEBUG ((LM_TRACE, CLINFO "RTI_DomainParticipantFactory_i::delete_participant - "
                     "Successfully casted provided object reference to servant type.\n"));

        DDS_ReturnCode_t retval = DDSDomainParticipantFactory::get_instance ()->
            delete_participant (part->get_participant ());

        if (retval != DDS_RETCODE_OK)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "RTI_DomainParticipantFactory_i::delete_participant - "
                         "RTI delete_participant returned non-ok error code %c\n",
                         translate_retcode (retval)));
          }
        else CIAO_DEBUG ((LM_INFO, CLINFO "RTI_DomainParticipantFactory_i::delete_participant - "
                          "Successfully deleted provided participant.\n"));

        return retval;
      }

      ::DDS::DomainParticipant_ptr
      RTI_DomainParticipantFactory_i::lookup_participant (::DDS::DomainId_t domain_id)
      {
        DDSDomainParticipant* dp = DDSDomainParticipantFactory::get_instance ()->lookup_participant (domain_id);
        ::DDS::DomainParticipant_var retval = new RTI_DomainParticipant_i (dp);
        return retval._retn ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipantFactory_i::set_default_participant_qos (const ::DDS::DomainParticipantQos & qos)
      {
        CIAO_TRACE ("RTI_DomainParticipantFactory_i::set_default_participant_qos");

        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipantFactory_i::get_default_participant_qos (::DDS::DomainParticipantQos & qos)
      {
        CIAO_TRACE ("RTI_DomainParticipantFactory_i::get_default_participant_qos");

        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipantFactory_i::set_qos (const ::DDS::DomainParticipantFactoryQos & qos)
      {
        CIAO_TRACE ("RTI_DomainParticipantFactory_i::set_qos");

        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_DomainParticipantFactory_i::get_qos (::DDS::DomainParticipantFactoryQos & qos)
      {
        CIAO_TRACE ("RTI_DomainParticipantFactory_i::get_qos");

        throw CORBA::NO_IMPLEMENT ();
      }

    }
  }
}
