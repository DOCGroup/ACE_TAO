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

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"

#include "tao/LocalObject.h"

class DDSDataWriter;

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_DataWriter_Base
      : public virtual ::DDS::DataWriter,
        public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      DDS_DataWriter_Base (void);

      virtual ::DDSDataWriter * get_rti_entity (void) = 0;

    private:
      virtual void set_rti_entity (::DDSDataWriter * dw) = 0;
    };
  }
}

#endif /* DDS4CCM_DATAWRITER_H */
