/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_DATAWRITERLISTENER_T
#define DDS_DATAWRITERLISTENER_T

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
      DataWriterListener_T (void);

      /// Destructor
      virtual ~DataWriterListener_T (void);

      static ::DDS::StatusMask get_mask (void);
    };
  }
}

#include "dds4ccm/impl/ndds/DataWriterListener_T.cpp"

#endif /* DDS_DATAWRITERLISTENER_T */
