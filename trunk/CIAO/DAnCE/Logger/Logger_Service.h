// $Id$
#ifndef LOGGER_SERVICE_H_
#define LOGGER_SERVICE_H_

#include "DAnCE/Logger/DAnCE_Logger_Export.h"
#include "ace/Service_Object.h"

namespace DAnCE
{
  class DAnCE_Logger_Export Logger_Service : public ACE_Service_Object
  {
    public:
      Logger_Service (void);
      virtual int init (int argc, ACE_TCHAR * argv[]);

      static int Initializer (void);
    private:
      void parse_args (int argc, ACE_TCHAR **argv);

      ACE_TString filename_;
      bool trace_;
  };
}

static int TAO_Requires_DAnCE_Logger_Service_Initializer =
  DAnCE::Logger_Service::Initializer ();

ACE_STATIC_SVC_DECLARE (Logger_Service)
ACE_FACTORY_DECLARE (DAnCE_Logger, Logger_Service)

#endif /*LOGGER_SERVICE_H_*/
