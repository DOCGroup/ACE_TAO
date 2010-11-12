// $Id$

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "CCM_DataReader.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename BASE_TYPE, typename WRITER_TYPE, typename VALUE_TYPE>
    InstanceHandleManager_T<BASE_TYPE, WRITER_TYPE, VALUE_TYPE>::InstanceHandleManager_T (void)
    {
    }

    template <typename BASE_TYPE, typename WRITER_TYPE, typename VALUE_TYPE>
    InstanceHandleManager_T<BASE_TYPE, WRITER_TYPE, VALUE_TYPE>::~InstanceHandleManager_T (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::~InstanceHandleManager_T");
    }

    template <typename BASE_TYPE, typename WRITER_TYPE, typename VALUE_TYPE>
    typename WRITER_TYPE::_ptr_type
    InstanceHandleManager_T<BASE_TYPE, WRITER_TYPE, VALUE_TYPE>::dds_writer (void)
    {
      if (!::CORBA::is_nil (this->dds_writer_.in ()))
        {
          return this->dds_writer_.in ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "CIAO::NDDS::DataWriter_T <DDS_TYPE>::dds_writer - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
    }

    template <typename BASE_TYPE, typename WRITER_TYPE, typename VALUE_TYPE>
    ::DDS::InstanceHandle_t
    InstanceHandleManager_T<BASE_TYPE, WRITER_TYPE, VALUE_TYPE>::register_instance (const VALUE_TYPE& datum)
    {
      return this->dds_writer ()->register_instance (datum);
    }

    template <typename BASE_TYPE, typename WRITER_TYPE, typename VALUE_TYPE>
    void
    InstanceHandleManager_T<BASE_TYPE, WRITER_TYPE, VALUE_TYPE>::unregister_instance (
      const VALUE_TYPE& datum,
      const ::DDS::InstanceHandle_t & instance_handle)
    {
      this->dds_writer ()->unregister_instance (datum, instance_handle);
    }

    template <typename BASE_TYPE, typename WRITER_TYPE, typename VALUE_TYPE>
    void
    InstanceHandleManager_T<BASE_TYPE, WRITER_TYPE, VALUE_TYPE>::set_dds_writer (
      ::DDS::DataWriter_ptr dds_writer)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::InstanceHandleManager_T::set_dds_writer");

      this->dds_writer_ = WRITER_TYPE::_narrow (dds_writer);
    }
  }
}
