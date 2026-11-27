/**
 *  @file    SL_Server.h
 *
 *  @author Douglas C. Schmidt
 */

#ifndef _SL_SERVER_H
#define _SL_SERVER_H

#include "Single_Lookup.h"

/**
 * @class SL_Server
 *
 * @brief Provides the server's single user lookup table abstraction.
 */
class SL_Server : public Single_Lookup
{
public:
  explicit SL_Server (char const *packet);
  Protocol_Record *insert (char const *key_name,
                           int max_len = MAXUSERIDNAMELEN) override;
  Protocol_Record *get_each_entry () override;
};

#endif /* _SL_SERVER_H */
