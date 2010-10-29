/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DATAREADER_H
#define DATAREADER_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/impl/ndds/dds4ccm_dds_ndds_export.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#include "tao/LocalObject.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_NDDS_Export DDS_DataReader_Base
      : public virtual ::DDS::DataReader,
        public virtual ::CORBA::LocalObject
    {
    public:
      virtual ~DDS_DataReader_Base (void);

      virtual DDSDataReader * get_impl (void) = 0;
    };
  }
}

#endif
