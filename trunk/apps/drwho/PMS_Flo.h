/* -*- C++ -*- */
// $Id$

/* Provides the server's lookup table abstraction for `flo' users... */

#ifndef _FMS_FLO_H
#define _FMS_FLO_H

#include "PM_Server.h"

class PMS_Flo : public PM_Server
{
protected:
  virtual int encode (char *packet, int &total_bytes);
  virtual int decode (char *packet, int &total_bytes);

public:
	      PMS_Flo (void);
};

#ifdef __OPTIMIZE__
inline 
PMS_Flo::PMS_Flo (void)
{}
#endif /* __OPTIMIZE__ */
#endif
