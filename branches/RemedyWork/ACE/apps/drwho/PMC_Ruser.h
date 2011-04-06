/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PMC_Ruser.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _PMC_RUSER_H
#define _PMC_RUSER_H

#include "PM_Client.h"

/**
 * @class PMC_Ruser
 *
 * @brief Provides the client's lookup table abstraction for `ruser' users...
 */
class PMC_Ruser : public PM_Client
{

public:
  PMC_Ruser (void);
  virtual void process (void);

protected:
  char *handle_protocol_entries (const char *cp,
                                 const char *host_name,
                                 const char * = 0);
  Protocol_Record *insert_protocol_info (Protocol_Record &protocol_record);
  virtual int encode (char *packet, int &total_bytes);
  virtual int decode (char *packet, int &total_bytes);
};

#endif /* _PMC_RUSER_H */
