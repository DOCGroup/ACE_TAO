/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DATAWRITER_H
#define DDS4CCM_DATAWRITER_H

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
    class DDS4CCM_DDS_NDDS_Export CCM_DDS_DataWriter_Base
      : public virtual ::DDS::DataWriter,
        public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      explicit CCM_DDS_DataWriter_Base (DDSDataWriter * dw);

      DDSDataWriter * get_impl (void);

      void set_impl (DDSDataWriter * dw);

    protected:
      DDSDataWriter * impl (void);

    private:
      DDSDataWriter * impl_;
    };
  }
}

#endif /* DDS4CCM_DATAWRITER_H */