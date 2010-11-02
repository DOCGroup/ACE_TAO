// $Id$

#include "CCM_DataReader.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

CIAO::DDS4CCM::CCM_DataReader::CCM_DataReader (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::CCM_DataReader");
}

CIAO::DDS4CCM::CCM_DataReader::~CCM_DataReader (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::~CCM_DataReader");
}

void
CIAO::DDS4CCM::CCM_DataReader::set_impl (::DDS::DataReader_ptr dr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::get_impl");
  this->impl_ = ::DDS::DataReader::_duplicate (dr);
}

::DDS::DataReader_ptr
CIAO::DDS4CCM::CCM_DataReader::get_impl (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::get_impl");

  return ::DDS::DataReader::_duplicate (this->impl ());
}

::DDS::DataReader_ptr
CIAO::DDS4CCM::CCM_DataReader::impl (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::impl");

  if (::CORBA::is_nil (this->impl_.in ()))
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
  else
    {
      return this->impl_.in ();
    }
}
