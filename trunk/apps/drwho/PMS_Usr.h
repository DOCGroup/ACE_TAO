/* -*- C++ -*- */
// $Id$

/* Provides the client's lookup table abstraction for `Usr' users... */

#ifndef _FMS_USR_H
#define _FMS_USR_H

#include "PM_Server.h"

class PMS_Usr : public PM_Server
{
protected:
  virtual int		encode (char *packet, int &total_bytes);
  virtual int		decode (char *packet, int &total_bytes);

public:
	                PMS_Usr (void);
};

#ifdef __OPTIMIZE__
inline 
PMS_Usr::PMS_Usr (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _FMS_USR_H */
