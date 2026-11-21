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
  PM_Server () = default;
  ~PM_Server () override;

  int encode (char *packet, int &total_bytes) override = 0;
  int decode (char *packet, int &total_bytes) override = 0;
  virtual int process ();

protected:
  virtual char *handle_protocol_entries (char *bp,
                                         Drwho_Node *hp);
  Protocol_Record *insert_protocol_info (Protocol_Record &protocol_record) override;
};

#endif /* _PM_SERVER_H */
