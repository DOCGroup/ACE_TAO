// $Id$
#ifndef DAnCE_LOGGERFACTORY_BASE_H_
#define DAnCE_LOGGERFACTORY_BASE_H_

#include "ace/Service_Object.h"
#include "tao/ORB.h"
#include "ace/Log_Msg_Backend.h"

namespace DAnCE
  {

  class DAnCELoggerFactory : public ACE_Service_Object
    {
    public:
      virtual ACE_Log_Msg_Backend * get_logger_backend (CORBA::ORB_ptr orb) = 0;
    };
} // DAnCE

#endif /*LOGGERFACTORY_H_*/
