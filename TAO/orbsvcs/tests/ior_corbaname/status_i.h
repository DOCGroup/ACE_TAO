//
// $Id$
//

#ifndef STATUS_I_H
#define STATUS_I_H

#include "corbanameS.h"

class corbaname_Status_i : public POA_corbaname::Status {
public:
  corbaname_Status_i (void);

  CORBA::Boolean print_status (void);

};

#endif /* STATUS_I_H */
