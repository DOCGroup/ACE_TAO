/* -*- C++ -*- */
// $Id$

/* Provides the client's lookup table abstraction for `flo' users... */

#ifndef _FMC_FLO_H
#define _FMC_FLO_H

#include "PM_Client.h"

class PMC_Flo : public PM_Client
{
protected:
  virtual Protocol_Record 	*insert_protocol_info (Protocol_Record &protocol_record);
  virtual int			encode (char *packet, int &total_bytes);
  virtual int			decode (char *packet, int &total_bytes);

public:
				PMC_Flo (void);
  virtual void 			process (void);
};

#ifdef __OPTIMIZE__
inline 
PMC_Flo::PMC_Flo (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _FMC_FLO_H */
