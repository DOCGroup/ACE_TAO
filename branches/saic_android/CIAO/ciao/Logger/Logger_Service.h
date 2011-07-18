// $Id$
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
      Logger_Service (void);

      virtual int init (int argc, ACE_TCHAR * argv[]);
      virtual ACE_Log_Msg_Backend * get_logger_backend (CORBA::ORB_ptr orb);

    private:

      void parse_args (int argc, ACE_TCHAR **argv);
      void set_levels (void);

      ACE_TString filename_;
      bool trace_;
      int log_level_;
    };

} // DAnCE

ACE_FACTORY_DECLARE (CIAO_Logger, Logger_Service)

#endif /*LOGGER_SERVICE_H_*/
