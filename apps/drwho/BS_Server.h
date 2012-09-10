/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    BS_Server.h
 *
 *  $Id$
 *
 *  Provides the server's binary search lookup table abstraction.
 *
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _BS_SERVER_H
#define _BS_SERVER_H

#include "Binary_Search.h"

/**
 * @class BS_Server
 *
 * @brief Provides the server's binary search lookup table abstraction.
 */
class BS_Server : public Binary_Search
{
public:
  // = Initialization.
  BS_Server (const char *packet);

  /**
   * This function is used to merge the <key_name> from server
   * <host_name> into the sorted list of userids kept on the client's
   * side.
   */
  virtual Protocol_Record *insert (const char *key_name,
                                   int max_len = MAXUSERIDNAMELEN);

  /**
   * An iterator, similar to Binary_Search::get_next_friend, though in
   * this case the friend records are returned in the order they
   * appeared in the friend file, rather than in sorted order.  Also,
   * we skip over entries that don't have any hosts associated with
   * them.
   */
  virtual Protocol_Record *get_next_entry (void);
};

#endif /* _BS_SERVER_H */
