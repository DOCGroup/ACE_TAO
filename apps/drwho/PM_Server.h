/* -*- C++ -*- */
// $Id$

/* Handle the server's lookup table abstraction. */

#ifndef _PM_SERVER_H
#define _PM_SERVER_H

#include "Protocol_Manager.h"

class PM_Server : public Protocol_Manager
{
protected:
  virtual char	          	*handle_protocol_entries (char *bp, Drwho_Node *hp);
  virtual Protocol_Record 	*insert_protocol_info (Protocol_Record &protocol_record);

public:
	                  	PM_Server (void);
  virtual		  	~PM_Server (void);

  virtual int		  	encode (char *packet, int &total_bytes) = 0;
  virtual int		  	decode (char *packet, int &total_bytes) = 0;
  virtual int		  	process (void);
};

#ifdef __OPTIMIZE__
inline
PM_Server::PM_Server (void)
{}

inline
PM_Server::~PM_Server (void)
{}
#endif /* __OPTIMIZE__ */
#endif
