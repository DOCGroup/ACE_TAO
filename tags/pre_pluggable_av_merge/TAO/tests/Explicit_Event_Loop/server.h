// $Id$

#ifndef server_HH_
#define server_HH_

#include "timeS.h"

class Time_impl : public virtual POA_Time {
public:
    virtual TimeOfDay get_gmt() throw(CORBA::SystemException);
};

#endif
