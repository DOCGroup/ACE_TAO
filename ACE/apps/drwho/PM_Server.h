/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PM_Server.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _PM_SERVER_H
#define _PM_SERVER_H

#include "Protocol_Manager.h"

/**
 * @class PM_Server
 *
 * @brief Handle the server's lookup table abstraction.
 */
class PM_Server : public Protocol_Manager
{
public:
  PM_Server ();
  virtual ~PM_Server ();

  virtual int encode (char *packet, int &total_bytes) = 0;
  virtual int decode (char *packet, int &total_bytes) = 0;
  virtual int process ();

protected:
  virtual char *handle_protocol_entries (char *bp,
                                         Drwho_Node *hp);
  virtual Protocol_Record *insert_protocol_info (Protocol_Record &protocol_record);
};

#endif /* _PM_SERVER_H */
