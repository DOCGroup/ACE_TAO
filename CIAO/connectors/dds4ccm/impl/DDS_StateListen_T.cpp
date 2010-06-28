// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/DataReaderStateListener_T.h"
#include "dds4ccm/impl/DataWriterListener_T.h"
#include "dds4ccm/impl/StateListenerControl_T.h"
#include "dds4ccm/impl/PortStatusListener_T.h"

#include "dds4ccm/impl/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
DDS_StateListen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::DDS_StateListen_T (void) :
  data_control_ (new CCM_DDS_StateListenerControl_T
    < ::CCM_DDS::CCM_StateListenerControl> ())
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
DDS_StateListen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::~DDS_StateListen_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_StateListen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::activate (
  typename CCM_TYPE::statelistener_type::_ptr_type listener,
  ::CCM_DDS::PortStatusListener_ptr status,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_StateListen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::activate");

  try
    {
      if (::CORBA::is_nil (this->listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->listener_,
                            DataReaderStateListener (
                              listener,
                              status,
                              this->data_control_.in (),
                              reactor),
                            CORBA::NO_MEMORY ());
        }
      this->data_reader_.set_listener (
        this->listener_.in (),
        DataReaderStateListener::get_mask (listener));
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_StateListen_T::activate: Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::CCM_StateListenerControl_ptr
DDS_StateListen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_data_control (void)
{
  DDS4CCM_TRACE ("DDS_StateListen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_data_control");

  return ::CCM_DDS::CCM_StateListenerControl::_duplicate (
    this->data_control_.in ());
}


