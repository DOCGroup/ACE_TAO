/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef _CIAO_COHERENT_CHANGES_GUARD
#define _CIAO_COHERENT_CHANGES_GUARD

#include "ace/Copy_Disabled.h"
#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    /**
     * Guard class which calls begin_coherent_changes() on
     * construction and end_coherent_changes() at destruction
     */
    class DDS4CCM_DDS_IMPL_Export Coherent_Changes_Guard :
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

