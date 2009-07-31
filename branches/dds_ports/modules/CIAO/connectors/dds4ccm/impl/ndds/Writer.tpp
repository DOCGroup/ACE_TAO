// $Id$
#include "dds4ccm/impl/ndds/DataWriter.h"
#include "dds4ccm/impl/ndds/Utils.h"

#include "ciao/Logger/Log_Macros.h"

// Implementation skeleton constructor
template <typename NDDS_TYPE, typename BASE >
CIAO::DDS4CCM::RTI::Writer_T<NDDS_TYPE, BASE>::Writer_T (::DDS::DataWriter_ptr writer)
  : impl_ (0)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Writer_T::Writer_T");

  RTI_DataWriter_i *rdw = dynamic_cast <RTI_DataWriter_i *> (writer);

  if (rdw == 0)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::Writer_T - "
                   "Unable to cast provided DataWriter to servant\n"));
      throw CORBA::INTERNAL ();
    }

  impl_ =  NDDS_TYPE::data_writer::narrow (rdw->get_datawriter ());

  if (!impl_)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::Writer_T - "
                   "Unable to narrow the provided writer entity to the specific "
                   "type necessary to publish messages\n"));
      throw CORBA::INTERNAL ();
    }
}

// Implementation skeleton destructor
template <typename NDDS_TYPE, typename BASE >
CIAO::DDS4CCM::RTI::Writer_T<NDDS_TYPE, BASE>::~Writer_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Writer_T::~Writer_T");
}

template <typename NDDS_TYPE, typename BASE >
void
CIAO::DDS4CCM::RTI::Writer_T<NDDS_TYPE, BASE>::write (const typename NDDS_TYPE::value_type an_instance)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Writer_T::write");

  if (an_instance == 0)
    {
      CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::write - "
                   "Write was provided a null instance to write\n"));
      return;
    }

  CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::write - "
               "Preparing to write to DDS\n"));
  DDS_ReturnCode_t retval = this->impl_->write (an_instance,
                                 DDS_HANDLE_NIL);

  if (retval != DDS_RETCODE_OK)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::write - "
                   "Write unsuccessful, received error code %C\n",
                   translate_retcode (retval)));
      throw CCM_DDS::InternalError (retval, 0);
    }

  CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::write - "
               "Write successful\n"));
}
