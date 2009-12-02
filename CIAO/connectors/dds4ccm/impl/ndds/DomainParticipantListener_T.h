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
    class DomainParticipantListener_T :
      public ::DDS::DomainParticipantListener,
      private ACE_Copy_Disabled
    {
    public:
      /// Constructor
      DomainParticipantListener_T (
        ::CCM_DDS::ConnectorStatusListener_ptr error_listener);

      /// Destructor
      virtual ~DomainParticipantListener_T (void);

      static ::DDS::StatusMask get_mask (void);

    private:
      ::CCM_DDS::ConnectorStatusListener_var error_listener_;
    };
  }
}

#include "dds4ccm/impl/ndds/DomainParticipantListener_T.cpp"
