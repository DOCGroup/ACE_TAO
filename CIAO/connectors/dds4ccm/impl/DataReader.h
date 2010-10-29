/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DATAREADER_H
#define DDS4CCM_DATAREADER_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#include "tao/LocalObject.h"

#if (CIAO_DDS4CCM_NDDS==1)
#include "ndds/ndds_cpp.h"
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export CCM_DDS_DataReader_Base
      : public virtual ::DDS::CCM_DataReader,
        public virtual ::CORBA::LocalObject
    {
    public:
      virtual ~CCM_DDS_DataReader_Base (void);

      virtual DDSDataReader * get_impl (void) = 0;
    };
  }
}

#endif
