/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef WRITER_T_H_
#define WRITER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "InstanceHandleManager_T.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename WRITER_TYPE, typename TYPED_DDS_WRITER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    class Writer_T
      : public InstanceHandleManager_T <WRITER_TYPE, TYPED_DDS_WRITER, VALUE_TYPE>
    {
    public:
      /// Constructor
      Writer_T (void);

      /// Destructor
      virtual ~Writer_T (void);

      virtual void write_one (
        const VALUE_TYPE& an_instance,
        const ::DDS::InstanceHandle_t & instance_handle);

      virtual void write_many (const SEQ_VALUE_TYPE& instances);

      /**
       * @name is_coherent_write
       * Accessors for is_coherent_write
       */
      //@{
      virtual ::CORBA::Boolean is_coherent_write (void);

      virtual void is_coherent_write ( ::CORBA::Boolean value);
      //@}

    private:
      void write_i (const VALUE_TYPE& datum,
                     const ::DDS::InstanceHandle_t& instance_handle,
                     ::CCM_DDS::DataNumber_t index);

      bool is_coherent_write_;
    };
  }
}

#include "dds4ccm/impl/Writer_T.cpp"

#endif /* WRITER_T_H_ */
