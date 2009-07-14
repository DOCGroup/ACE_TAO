// $Id$
#ifndef LOGGERFACTORY_H_
#define LOGGERFACTORY_H_

#include "ace/Service_Object.h"
#include "tao/ORB.h"
#include "ace/Log_Msg_Backend.h"

namespace CIAO
  {

  class CIAOLoggerFactory : public ACE_Service_Object
    {
    public:
      virtual ACE_Log_Msg_Backend * get_logger_backend (CORBA::ORB_ptr orb) = 0;
    };
} // CIAO

#endif /*LOGGERFACTORY_H_*/
