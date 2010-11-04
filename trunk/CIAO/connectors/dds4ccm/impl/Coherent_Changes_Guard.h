/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef _CIAO_COHERENT_CHANGES_GUARD
#define _CIAO_COHERENT_CHANGES_GUARD

#include "ace/Copy_Disabled.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_NDDS_Export Coherent_Changes_Guard :
      private ACE_Copy_Disabled
    {
      public:
        Coherent_Changes_Guard (::DDS::Publisher_ptr p, bool coherent_write);
        ~Coherent_Changes_Guard ();
      private:
        ::DDS::Publisher_var p_;
        bool const coherent_write_;
    };
  }
}

#endif /* _CIAO_COHERENT_CHANGES_GUARD */

