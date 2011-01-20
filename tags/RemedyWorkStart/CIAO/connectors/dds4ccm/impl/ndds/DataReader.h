/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DATAREADER_H
#define DATAREADER_H

#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "tao/LocalObject.h"

class DDSDataReader;

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_DataReader_Base
      : public virtual ::DDS::DataReader,
        public virtual ::CORBA::LocalObject
    {
    public:
      virtual ~DDS_DataReader_Base (void);

      virtual ::DDSDataReader * get_rti_entity (void) = 0;
    };
  }
}

#endif
