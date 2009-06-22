// $Id$
#ifndef LOGGER_SERVICE_H_
#define LOGGER_SERVICE_H_

#include "DAnCE/Logger/DAnCE_Logger_Export.h"
#include "DAnCE/Logger/DAnCELoggerFactory.h"
#include "DAnCE/Logger/File_Logger_Backend.h"

namespace DAnCE
  {

    class DAnCE_Logger_Export Logger_Service : public ::DAnCE::DAnCELoggerFactory
    {
    public:
      Logger_Service (void);
      virtual int init (int argc, ACE_TCHAR * argv[]);
      virtual ACE_Log_Msg_Backend * get_logger_backend (CORBA::ORB_ptr orb);
    private:
      void parse_args (int argc, ACE_TCHAR **argv);
      void set_levels (void);

      ACE_CString filename_;
      bool trace_;
      int log_level_;
    };

} // DAnCE

ACE_FACTORY_DECLARE (DAnCE_Logger, Logger_Service)

#endif /*LOGGER_SERVICE_H_*/
