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
        switch (ret)
          {
          case DDS_RETCODE_OK:
            return "DDS_RETCODE_OK";
            
          case DDS_RETCODE_ERROR:
            return "DDS_RETCODE_ERROR";
            
          case DDS_RETCODE_UNSUPPORTED:
            return "DDS_RETCODE_UNSUPPORTED";
            
          case DDS_RETCODE_BAD_PARAMETER:
            return "DDS_RETCODE_BAD_PARAMETER";
            
          case DDS_RETCODE_PRECONDITION_NOT_MET:
            return "DDS_RETCODE_PRECONDITION_NOT_MET";
            
          case DDS_RETCODE_OUT_OF_RESOURCES:
            return "DDS_RETCODE_OUT_OF_RESOURCES";
            
          case DDS_RETCODE_NOT_ENABLED:
            return "DDS_RETCODE_NOT_ENABLED";
            
          case DDS_RETCODE_IMMUTABLE_POLICY:
            return "DDS_RETCODE_IMMUTABLE_POLICY";
            
          case DDS_RETCODE_INCONSISTENT_POLICY:
            return "DDS_RETCODE_INCONSISTENT_POLICY";
            
          case DDS_RETCODE_ALREADY_DELETED:
            return "DDS_RETCODE_ALREADY_DELETED";
            
          case DDS_RETCODE_TIMEOUT:
            return "DDS_RETCODE_TIMEOUT";
            
          case DDS_RETCODE_NO_DATA:
            return "DDS_RETCODE_NO_DATA";
            
          case DDS_RETCODE_ILLEGAL_OPERATION:
            return "DDS_RETCODE_ILLEGAL_OPERATION";
          }
        
        return "UNKNOWN ERROR";
      }
    }
  }
}
