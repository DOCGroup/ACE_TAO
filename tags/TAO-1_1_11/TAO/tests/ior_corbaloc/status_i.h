//
// $Id$
//

#ifndef STATUS_I_H
#define STATUS_I_H

#include "corbalocS.h"
#include <string>

class corbaloc_Status_i : public POA_corbaloc::Status {
public:
  corbaloc_Status_i (CORBA::Environment & =
                     TAO_default_environment());

  CORBA::Boolean print_status (CORBA::Environment & = TAO_default_environment()) 
    throw (CORBA::SystemException);
  
};

#endif /* STATUS_I_H */
