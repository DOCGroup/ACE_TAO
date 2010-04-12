/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef WRITER_T_H
#define WRITER_T_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"
#include "InstanceHandleManager_T.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class Writer_T :
      public InstanceHandleManager_T <DDS_TYPE, CCM_TYPE, typename CCM_TYPE::writer_type>
    {
    public:
      /// Constructor
      Writer_T (void);

      /// Destructor
      virtual ~Writer_T (void);

      virtual void write_one (
        const typename DDS_TYPE::value_type& an_instance,
        const ::DDS::InstanceHandle_t & instance_handle);

      virtual void write_many (
        const typename CCM_TYPE::seq_type& instances);

      virtual ::CORBA::Boolean is_coherent_write (void);

      virtual void is_coherent_write ( ::CORBA::Boolean value);

    private:
      void write_i (const typename DDS_TYPE::value_type& datum,
                    const ::DDS::InstanceHandle_t& instance_handle,
                    ::CCM_DDS::DataNumber_t index);

      bool is_coherent_write_;
    };
  }
}

#include "dds4ccm/impl/dds/Writer_T.cpp"

#endif /* WRITER_T_H */
