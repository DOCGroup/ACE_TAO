// $Id$
#include "dds4ccm/impl/ndds/DataWriter.h"
#include "dds4ccm/impl/ndds/Utils.h"

#include "ciao/Logger/Log_Macros.h"

CIAO::DDS4CCM::RTI::Coherent_Write_Guard::Coherent_Write_Guard (DDSPublisher* p, bool coherent_write) : p_ (p), coherent_write_ (coherent_write)
{
  if (this->coherent_write_)
    {
      p_->begin_coherent_changes ();
    }
}

CIAO::DDS4CCM::RTI::Coherent_Write_Guard::~Coherent_Write_Guard ()
{
  if (this->coherent_write_)
    {
      this->p_->end_coherent_changes ();
    }
}

// Implementation skeleton constructor
template <typename NDDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Writer_T<NDDS_TYPE, CCM_TYPE>::Writer_T (::DDS::DataWriter_ptr writer)
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
template <typename NDDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Writer_T<NDDS_TYPE, CCM_TYPE>::~Writer_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Writer_T::~Writer_T");
}

template <typename NDDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Writer_T<NDDS_TYPE, CCM_TYPE>::write (const typename NDDS_TYPE::value_type & an_instance)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Writer_T::write");

  CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::write - "
               "Preparing to write to DDS\n"));
  DDS_ReturnCode_t const retval = this->impl_->write (an_instance, DDS_HANDLE_NIL);

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

template <typename NDDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Writer_T<NDDS_TYPE, CCM_TYPE>::write (const typename NDDS_TYPE::seq_type& instances, bool coherent_write)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Writer_T::write");

  Coherent_Write_Guard guard (this->impl_->get_publisher(), coherent_write);

  CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::write - "
               "Preparing to write to DDS\n"));
  for (::CORBA::ULong index = 0; index < instances.length(); index++)
    {
      DDS_ReturnCode_t const retval = this->impl_->write (instances[index],
                                     DDS_HANDLE_NIL);

      if (retval != DDS_RETCODE_OK)
        {
         CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::write - "
                       "Write unsuccessful, received error code %C\n",
                       translate_retcode (retval)));
          throw CCM_DDS::InternalError (retval, index);
        }
    }

  CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::write - "
               "Write successful\n"));
}
