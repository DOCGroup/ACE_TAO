// $Id$

#ifndef DDS4CCM_LOGGER_SERVICE_H_
#define DDS4CCM_LOGGER_SERVICE_H_

#include "DDS4CCM_Logger_Export.h"
#include "ace/Service_Object.h"

namespace CIAO
{
  namespace DDS4CCM
    {
      class DDS4CCM_Logger_Export Logger_Service : public ACE_Service_Object
        {
        public:
          Logger_Service (void);
          virtual int init (int ARGC, ACE_TCHAR * ARGV[]);

          void init ();

          static int Initializer (void);
        private:
          void parse_args (int argc, ACE_TCHAR **argv);

          ACE_TString filename_;
          bool trace_;
        };
    }
}

static int TAO_Requires_DDS4CCM_Logger_Service_Initializer =
  ::CIAO::DDS4CCM::Logger_Service::Initializer ();

typedef ::CIAO::DDS4CCM::Logger_Service DDS4CCM_LOGGER_SERVICE;

ACE_STATIC_SVC_DECLARE (DDS4CCM_LOGGER_SERVICE)
ACE_FACTORY_DECLARE (DDS4CCM_Logger, DDS4CCM_LOGGER_SERVICE)

#endif /*LOGGER_SERVICE_H_*/
