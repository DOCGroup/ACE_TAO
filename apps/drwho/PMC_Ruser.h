/* -*- C++ -*- */
// $Id$

/* Provides the client's lookup table abstraction for `ruser' users... */

#ifndef _FMC_RUSER_H
#define _FMC_RUSER_H

#include "PM_Client.h"

class PMC_Ruser : public PM_Client
{
protected:
  char          	*handle_protocol_entries (char *cp, char *host_name, char * = 0);
  Protocol_Record 	*insert_protocol_info (Protocol_Record &protocol_record);
  virtual int		encode (char *packet, int &total_bytes);
  virtual int		decode (char *packet, int &total_bytes);

public:
			PMC_Ruser (void);
  virtual void 		process (void);
};

#ifdef __OPTIMIZE__
inline
PMC_Ruser::PMC_Ruser (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _FMC_RUSER_H */
