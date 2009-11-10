// $Id$

#include "Utils.h"
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      const char * translate_retcode (DDS_ReturnCode_t ret)
      {
#define RTI_DDS_RETCODE(X) case X: return #X
        switch (ret)
          {
            RTI_DDS_RETCODE (DDS_RETCODE_OK);
            RTI_DDS_RETCODE (DDS_RETCODE_ERROR);
            RTI_DDS_RETCODE (DDS_RETCODE_UNSUPPORTED);
            RTI_DDS_RETCODE (DDS_RETCODE_BAD_PARAMETER);
            RTI_DDS_RETCODE (DDS_RETCODE_PRECONDITION_NOT_MET);
            RTI_DDS_RETCODE (DDS_RETCODE_OUT_OF_RESOURCES);
            RTI_DDS_RETCODE (DDS_RETCODE_NOT_ENABLED);
            RTI_DDS_RETCODE (DDS_RETCODE_IMMUTABLE_POLICY);
            RTI_DDS_RETCODE (DDS_RETCODE_INCONSISTENT_POLICY);
            RTI_DDS_RETCODE (DDS_RETCODE_ALREADY_DELETED);
            RTI_DDS_RETCODE (DDS_RETCODE_TIMEOUT);
            RTI_DDS_RETCODE (DDS_RETCODE_NO_DATA);
            RTI_DDS_RETCODE (DDS_RETCODE_ILLEGAL_OPERATION);
          }
        return "***Unknown enum value, update RTI::translate_retcode()";
#undef RTI_DDS_RETCODE
      }
    }
  }
}
