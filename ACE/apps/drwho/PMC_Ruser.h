/**
 *  @file    PMC_Ruser.h
 *
 *  @author Douglas C. Schmidt
 */

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
  PMC_Ruser ();
  void process () override;

protected:
  char *handle_protocol_entries (char const *cp,
                                 char const *host_name,
                                 char const * = nullptr) override;
  Protocol_Record *insert_protocol_info (Protocol_Record &protocol_record) override;
  int encode (char *packet, int &total_bytes) override;
  int decode (char *packet, int &total_bytes) override;
};

#endif /* _PMC_RUSER_H */
