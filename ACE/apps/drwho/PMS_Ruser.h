/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PMS_Ruser.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _PMS_RUSER_H
#define _PMS_RUSER_H

#include "PM_Server.h"

/**
 * @class PMS_Ruser
 *
 * @brief Provides the server's lookup table abstraction for `ruser' users...
 */
class PMS_Ruser : public PM_Server
{
public:
  PMS_Ruser ();

protected:
  virtual char *handle_protocol_entries (char *bp, Drwho_Node *hp);
  virtual Protocol_Record *insert_protocol_info (Protocol_Record &protocol_record);
  virtual int encode (char *packet, int &total_bytes);
  virtual int decode (char *packet, int &total_bytes);
};

#endif /* _PMS_RUSER_H */
