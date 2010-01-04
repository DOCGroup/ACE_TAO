// -*- C++ -*-

//=============================================================================
/**
 * @file      Connection.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ADBC_CONNECTION_H_
#define _ADBC_CONNECTION_H_

#include "Exception.h"

namespace ADBC
{
// Forward decl.
class Query;

/**
 * @class Connection
 *
 * Base class for all database connection implemenations.
 */
class ADBC_Export Connection
{
public:
  /// Constructor.
  Connection (void);

  /// Destructor.
  virtual ~Connection (void);

  /**
   * Create a connection.
   *
   * @param[in]     username        Name of the user.
   * @param[in]     password        Password of the user.
   * @param[in]     server          Location of the database.
   * @param[in]     port            Port number for connection.
   */
  virtual void connect (const ACE_CString & connstr) = 0;

  /// Close the current connection.
  virtual void disconnect (void) = 0;

  /**
   * Create a database database query. The query created will be
   * dependent on the implemenation.
   *
   * @return     Pointer to a database query.
   */
  virtual Query * create_query (void) = 0;

  /**
   * Determine if there is a valid connection.
   *
   * @retval      true        There is a connection.
   * @retval      false       There is not a connection.
   */
  virtual bool is_connected (void) const;

protected:
  /// Holds the current connection state.
  bool connected_;

  /// The connction string for the connection.
  ACE_CString connstr_;

private:
  // Prevent the following operations.
  Connection (const Connection &);
  const Connection & operator = (const Connection &);
};
}

#if defined (__ADBC_INLINE__)
#include "Connection.inl"
#endif

#endif  // !defined _ADBC_DB_CONNECTION_H_
