// -*- C++ -*-

//=============================================================================
/**
 * @file      DB_Connection.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_DB_CONNECTION_H_
#define _CUTS_DB_CONNECTION_H_

#include "DB_Exception.h"

// Forward decl.
class CUTS_DB_Query;

//=============================================================================
/**
 * @class CUTS_DB_Connection
 *
 * Base class for all database connection implemenations.
 */
//=============================================================================

class CUTS_DB_UTILS_Export CUTS_DB_Connection
{
public:
  /// Constructor.
  CUTS_DB_Connection (void);

  /// Destructor.
  virtual ~CUTS_DB_Connection (void);

  /**
   * Create a connection.
   *
   * @param[in]     username        Name of the user.
   * @param[in]     password        Password of the user.
   * @param[in]     server          Location of the database.
   * @param[in]     port            Port number for connection.
   */
  virtual void connect (const char * username,
                        const char * password,
                        const char * server,
                        int port)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /// Close the current connection.
  virtual void disconnect (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;


  /**
   * Create a database database query. The query created will be
   * dependent on the implemenation.
   *
   * @return     Pointer to a database query.
   */
  virtual CUTS_DB_Query * create_query (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Determine if there is a valid connection.
   *
   * @retval      true        There is a connection.
   * @retval      false       There is not a connection.
   */
  bool is_connected (void) const;

protected:
  /// Holds the current connection state.
  bool connected_;

private:
  // Prevent the following operations.
  CUTS_DB_Connection (const CUTS_DB_Connection &);
  const CUTS_DB_Connection & operator = (const CUTS_DB_Connection &);
};

#if defined (__CUTS_INLINE__)
#include "DB_Connection.inl"
#endif

#endif  // !defined _CUTS_DB_CONNECTION_H_
