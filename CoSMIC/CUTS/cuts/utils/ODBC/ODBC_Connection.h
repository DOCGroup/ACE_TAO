// -*- C++ -*-

//=============================================================================
/**
 * @file    ODBC_Connection.h
 *
 * $Id$
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _ODBC_CONNECTION_H_
#define _ODBC_CONNECTION_H_

#include "ODBC_Base.h"
#include "ODBC_Exception.h"
#include "cuts/utils/DB_Connection.h"

// Forward decl.
class ODBC_Query;

//=============================================================================
/**
 * @class ODBC_Connection
 *
 * @brief Wrapper class for handling ODBC connection objects.
 *
 * This class provides a minimal set of operations used in CUTS for
 * creating connection to a database using the ODBC protocol. Futhermore,
 * this class is the only way to create an ODBC_Query object for performing
 * SQL queries using the ODBC protocol.
 */
//=============================================================================

class CUTS_ODBC_Export ODBC_Connection :
  public ODBC_Base,
  public CUTS_DB_Connection
{
public:
  /// Constructor
  ODBC_Connection (void);

  /// Destructor
  virtual ~ODBC_Connection (void);

  /**
   * Establish an new ODBC connection. The information for
   * the establishing the connection is specified in the
   * parameter from the client.
   *
   * @param[in]     username      username for the connection
   * @param[in]     password      password for the @a username
   * @param[in]     host          location of the database
   * @param[in]     port          port of connection
   */
  virtual void connect (const char * username,
                        const char * password,
                        const char * host,
                        int port = 3306)
                        ACE_THROW_SPEC ((CUTS_DB_Exception));

  /// Close the existing connection
  virtual void disconnect (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /**
   * Create an ODBC query object.
   *
   * @return Pointer to the database query.
   */
  virtual CUTS_DB_Query * create_query (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /**
   * Create an ODBC query object.
   *
   * @return Pointer to the database query.
   */
  ODBC_Query * create_odbc_query (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

protected:
  /// Handle to the environment
  HENV env_;

  /// Handle to the database connection
  HDBC conn_;
};

#if defined (__CUTS_INLINE__)
#include "ODBC_Connection.inl"
#endif

#endif  // !defined _ODBC_CONNECTION_H_
