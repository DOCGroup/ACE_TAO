/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataWriterListener_T :
      public ::DDS::DataWriterListener,
      private ACE_Copy_Disabled
    {
    public:
      /// Constructor
      DataWriterListener_T (
        typename CCM_TYPE::context_type::_ptr_type context,
        CCM_DDS::ConnectorStatusListener_ptr error_listener);

      /// Destructor
      virtual ~DataWriterListener_T (void);

      static ::DDS::StatusMask get_mask (void);

    private:
      typename CCM_TYPE::context_type::_var_type context_;
      CCM_DDS::ConnectorStatusListener_var  error_listener_;
    };
  }
}

#include "dds4ccm/impl/ndds/DataWriterListener_T.cpp"
