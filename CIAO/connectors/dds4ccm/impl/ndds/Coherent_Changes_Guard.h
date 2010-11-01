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

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export Coherent_Changes_Guard :
      private ACE_Copy_Disabled
    {
      public:
        Coherent_Changes_Guard (::DDSPublisher* p, bool coherent_write);
        ~Coherent_Changes_Guard ();
      private:
        ::DDSPublisher* p_;
        bool const coherent_write_;
    };
  }
}

#endif /* _CIAO_COHERENT_CHANGES_GUARD */

