// $Id$

#ifndef CIAO_LOGGER_SERVICE_H_
#define CIAO_LOGGER_SERVICE_H_

#include "CIAO_Logger_Export.h"
#include "ace/Service_Object.h"

namespace CIAO
  {
    class CIAO_Logger_Export Logger_Service : public ACE_Service_Object
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

static int TAO_Requires_CIAO_Logger_Service_Initializer =
  CIAO::Logger_Service::Initializer ();

ACE_STATIC_SVC_DECLARE (Logger_Service)
ACE_FACTORY_DECLARE (CIAO_Logger, Logger_Service)

#endif /*LOGGER_SERVICE_H_*/
