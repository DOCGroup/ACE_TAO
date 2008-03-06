#ifndef CIAO_LOGGER_SERVICE_H_
#define CIAO_LOGGER_SERVICE_H_

#include "CIAO_Logger_Export.h"
#include "CIAOLoggerFactory.h"
#include "File_Logger_Backend.h"

namespace CIAO
  {

  class CIAO_Logger_Export Logger_Service : public CIAOLoggerFactory
    {
    public:
      virtual int init (int argc, ACE_TCHAR * argv[]);
      virtual ACE_Log_Msg_Backend * get_logger_backend (CORBA::ORB_ptr orb);
    private:
      ACE_CString filename_;
    };

} // DAnCE

ACE_FACTORY_DECLARE (CIAO_Logger, Logger_Service);

#endif /*LOGGER_SERVICE_H_*/
