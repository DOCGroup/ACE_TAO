#ifndef LOGGER_SERVICE_H_
#define LOGGER_SERVICE_H_

#include "DAnCE_Logger_Export.h"
#include "Starter/DAnCELoggerFactory.h"
#include "File_Logger_Backend.h"

namespace DAnCE
  {

  class DAnCE_Logger_Export Logger_Service : public DAnCELoggerFactory
    {
    public:
      virtual int init (int argc, ACE_TCHAR * argv[]);
      virtual ACE_Log_Msg_Backend * get_logger_backend (CORBA::ORB_ptr orb);
    private:
      ACE_CString filename_;
    };

} // DAnCE

ACE_FACTORY_DECLARE (DAnCE_Logger, Logger_Service);

#endif /*LOGGER_SERVICE_H_*/
