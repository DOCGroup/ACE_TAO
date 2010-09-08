// $Id$
#ifndef LOGGER_SERVICE_H_
#define LOGGER_SERVICE_H_

#include "Logger/DAnCE_Logger_Export.h"
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

typedef DAnCE::Logger_Service DANCE_LOGGER_SERVICE;

ACE_STATIC_SVC_DECLARE (DANCE_LOGGER_SERVICE)
ACE_FACTORY_DECLARE (DAnCE_Logger, DANCE_LOGGER_SERVICE)

#endif /*LOGGER_SERVICE_H_*/
