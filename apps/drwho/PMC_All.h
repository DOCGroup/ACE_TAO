/* -*- C++ -*- */
// $Id$

/* Provides the client's lookup table abstraction for `all' users... */

#ifndef _FMC_ALL_H
#define _FMC_ALL_H

#include "PM_Client.h"

class PMC_All : public PM_Client
{
protected:
  Protocol_Record 	*insert_protocol_info (Protocol_Record &protocol_record);
  virtual int		encode (char *packet, int &total_bytes);
  virtual int		decode (char *packet, int &total_bytes);

public:
			PMC_All (void);
  virtual void 		process (void);
};

#ifdef __OPTIMIZE__
inline
PMC_All::PMC_All (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _FMC_ALL_H */
