// $Id$
#include "dds4ccm/impl/ndds/DataWriter.h"
#include "dds4ccm/impl/ndds/Utils.h"

#include "ciao/Logger/Log_Macros.h"

// Implementation skeleton constructor
template <typename NDDS_TYPE, typename BASE >
CIAO::DDS4CCM::RTI::Updater_T<NDDS_TYPE, BASE>::Updater_T (::DDS::DataWriter_ptr dw)
  : impl_ (0)
{
  CIAO_TRACE ("############ CIAO::DDS4CCM::RTI::Updater_T::Updater_T");

  RTI_DataWriter_i *rdu = dynamic_cast <RTI_DataWriter_i *> (dw);

  if (rdu == 0)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "############ ############ CIAO::DDS4CCM::RTI::Updater_T::Updater - "
                   "Unable to cast provided DataUpdater to servant\n"));
      throw CORBA::INTERNAL ();
    }

  impl_ =  NDDS_TYPE::data_writer::narrow (rdu->get_datawriter ());

  if (!impl_)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "############ CIAO::DDS4CCM::RTI::Updater_T::Updater_T - "
                   "Unable to narrow the provided updater entity to the specific "
                   "type necessary to update messages\n"));
      throw CORBA::INTERNAL ();
    }
}

// Implementation skeleton destructor
template <typename NDDS_TYPE, typename BASE >
CIAO::DDS4CCM::RTI::Updater_T<NDDS_TYPE, BASE>::~Updater_T (void)
{
  CIAO_TRACE ("############ CIAO::DDS4CCM::RTI::Updater_T::~Updater_T");
}

template <typename NDDS_TYPE, typename BASE >
void
CIAO::DDS4CCM::RTI::Updater_T<NDDS_TYPE, BASE>::create (const typename NDDS_TYPE::value_type & an_instance)
{
  printf ("############ CIAO::DDS4CCM::RTI::Updater_T::create\n");
  ACE_UNUSED_ARG (an_instance);
}

template <typename NDDS_TYPE, typename BASE >
void
CIAO::DDS4CCM::RTI::Updater_T<NDDS_TYPE, BASE>::update (const typename NDDS_TYPE::value_type & an_instance)
{
  printf ("############ CIAO::DDS4CCM::RTI::Updater_T::update\n");
  ACE_UNUSED_ARG (an_instance);
}

template <typename NDDS_TYPE, typename BASE >
void
CIAO::DDS4CCM::RTI::Updater_T<NDDS_TYPE, BASE>::_cxx_delete (const typename NDDS_TYPE::value_type & an_instance)
{
  printf ("############ CIAO::DDS4CCM::RTI::Updater_T::delete\n");
  ACE_UNUSED_ARG (an_instance);
}
