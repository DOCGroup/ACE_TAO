/* -*- C++ -*- */
// $Id$

/* Provides the client side of the friend manager lookup table abstraction. */

#ifndef _PM_CLIENT_H
#define _PM_CLIENT_H

#include "Protocol_Manager.h"

class PM_Client : public Protocol_Manager
{
protected:
  int	                	max_key_length;

  virtual char			*handle_protocol_entries (char *cp, char *key_name1, char *key_name2 = 0);
  virtual Protocol_Record 	*insert_protocol_info (Protocol_Record &protocol_record);
public:
  				PM_Client (void);
  virtual			~PM_Client (void);

  virtual int			encode (char *packet, int &total_bytes) = 0;
  virtual int			decode (char *packet, int &total_bytes) = 0;
  virtual void			process (void);
};

#ifdef __OPTIMIZE__
inline
PM_Client::PM_Client (void): max_key_length (0)
{}

inline
PM_Client::~PM_Client (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _PM_CLIENT_H */

