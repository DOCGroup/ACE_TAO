// $Id$

#include "CCM_DataWriter.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

CIAO::DDS4CCM::CCM_DataWriter::CCM_DataWriter (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::CCM_DataWriter");
}

CIAO::DDS4CCM::CCM_DataWriter::~CCM_DataWriter (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::~CCM_DataWriter");
}

void
CIAO::DDS4CCM::CCM_DataWriter::set_impl (::DDS::DataWriter_ptr dr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::get_impl");
  this->impl_ = ::DDS::DataWriter::_duplicate (dr);
}

::DDS::DataWriter_ptr
CIAO::DDS4CCM::CCM_DataWriter::get_impl (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::get_impl");

  return ::DDS::DataWriter::_duplicate (this->impl ());
}

::DDS::DataWriter_ptr
CIAO::DDS4CCM::CCM_DataWriter::impl (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::impl");

  if (::CORBA::is_nil (this->impl_.in ()))
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
  else
    {
      return this->impl_.in ();
    }
}
