/* -*- C++ -*- */
// $Id$

/* Provides the client's lookup table abstraction for `Usr' users... */

#ifndef _FMC_USR_H
#define _FMC_USR_H

#include "PM_Client.h"

class PMC_Usr : public PM_Client
{
private:
  char	                *usr_name;

protected:
  virtual int		encode (char *packet, int &total_bytes);
  virtual int		decode (char *packet, int &total_bytes);

public:
	                PMC_Usr (char *usr_name);
  virtual void          process (void);
};

#ifdef __OPTIMIZE__
inline 
PMC_Usr::PMC_Usr (char *u_name): usr_name (u_name)
{}
#endif /* __OPTIMIZE__ */
#endif /* _FMC_USR_H */
