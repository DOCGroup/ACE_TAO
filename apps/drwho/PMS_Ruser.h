/* -*- C++ -*- */
// $Id$

/* Provides the server's lookup table abstraction for `ruser' users... */

#ifndef _FMS_RUSER_H
#define _FMS_RUSER_H

#include "PM_Server.h"

class PMS_Ruser : public PM_Server
{
protected:
  virtual char			*handle_protocol_entries (char *bp, Drwho_Node *hp);
  virtual Protocol_Record 	*insert_protocol_info (Protocol_Record &protocol_record);
  virtual int			encode (char *packet, int &total_bytes);
  virtual int			decode (char *packet, int &total_bytes);

public:
  PMS_Ruser (void);
};

#ifdef __OPTIMIZE__
inline
PMS_Ruser::PMS_Ruser (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _FMS_RUSER_H */
