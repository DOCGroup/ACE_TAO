// $Id$

#include "DataWriter.h"

#include "dds4ccm/impl/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    DDS_DataWriter_Base::DDS_DataWriter_Base (DDSDataWriter * dw)
      : impl_ (dw)
    {
    }

    DDSDataWriter *
    DDS_DataWriter_Base::get_impl (void)
    {
      return this->impl_;
    }

    void
    DDS_DataWriter_Base::set_impl (DDSDataWriter * dw)
    {
      // Set a pointer to this class for the listener to use.
      this->impl_ = dw;
    }

    DDSDataWriter *
    DDS_DataWriter_Base::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}
