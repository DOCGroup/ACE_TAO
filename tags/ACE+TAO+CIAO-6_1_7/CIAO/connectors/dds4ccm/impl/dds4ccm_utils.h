/**
 * @file dds4ccm_utils.h
 * @author Johnny Willemsen  (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Misc utility classes for DDS4CCM.
 */

#ifndef DDS4CCM_UTILS_UTILS_H
#define DDS4CCM_UTILS_UTILS_H

#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/idl/ccm_ddsC.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    inline const char * translate_ccm_dds_accessstatus (::CCM_DDS::AccessStatus ret)
    {
#define DDS4CCM_DDS_ACCESSSTATUS(X) case X: return #X
      switch (ret)
        {
          DDS4CCM_DDS_ACCESSSTATUS (CCM_DDS::FRESH_INFO);
          DDS4CCM_DDS_ACCESSSTATUS (CCM_DDS::ALREADY_SEEN);
        }
      return "***Unknown enum value, update CIAO::DDS4CCM::translate_ccm_dds_accessstatus()";
#undef DDS4CCM_DDS_ACCESSSTATUS
    }
  }
}

#endif
