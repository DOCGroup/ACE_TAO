/* -*- C++ -*- */
// $Id$

/* Provides the server's lookup table abstraction for `all' users... */

#ifndef _FMS_ALL_H
#define _FMS_ALL_H

#include "PM_Server.h"

class PMS_All : public PM_Server
{
protected:
  virtual Protocol_Record 	*insert_protocol_info (Protocol_Record &protocol_record);
  virtual int		  	encode (char *packet, int &total_bytes);
  virtual int		  	decode (char *packet, int &total_bytes);

public:
				PMS_All (void);
};

#ifdef __OPTIMIZE__
inline
PMS_All::PMS_All (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _FMS_ALL_H */
