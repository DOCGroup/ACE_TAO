// $Id$

#include "ODBC_Connection.h"
#include "ODBC_Query.h"

#if !defined (__CUTS_INLINE__)
#include "ODBC_Connection.inl"
#endif

#include "ace/Log_Msg.h"
#include <sstream>

//
// ODBC_Connection
//
ODBC_Connection::ODBC_Connection (void)
: env_ (SQL_NULL_HANDLE),
  conn_ (SQL_NULL_HANDLE)
{
  // Allocate environment handle. Eventually we would like to be
  // able to attach to user-defined environments.
  SQL_VERIFY (::SQLAllocHandle (SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->env_),
              ODBC_Env_Exception (this->env_));

  // Set the version of ODBC. Right now we only support ODBC 3.x
  SQL_VERIFY (::SQLSetEnvAttr (this->env_,
                               SQL_ATTR_ODBC_VERSION,
                               (void *)SQL_OV_ODBC3,
                               0),
              ODBC_Env_Exception (this->env_));

  // Allocate the connection handle.
  SQL_VERIFY (::SQLAllocHandle (SQL_HANDLE_DBC, this->env_, &this->conn_),
              ODBC_Env_Exception (this->env_));
}

//
// ~ODBC_Connection
//
ODBC_Connection::~ODBC_Connection (void)
{
  // release the resource for the connection
  if (this->conn_ != SQL_NULL_HANDLE)
    ::SQLFreeHandle (SQL_HANDLE_DBC, this->conn_);

  // free the environment variable
  if (this->env_ != SQL_NULL_HANDLE)
    ::SQLFreeHandle (SQL_HANDLE_ENV, this->env_);
}

//
// connect
//
void ODBC_Connection::connect (const char * username,
                               const char * password,
                               const char * server,
                               int port)
                               ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  // Genereate the connection string for MySQL.
  std::ostringstream connstr;

#if defined (_WIN32) || defined (WIN32)
  connstr << "Driver={MySQL ODBC 3.51 Driver};"
          << "Uid=" << username << ";"
          << "Pwd=" << password << ";"
#else
  connstr << "Dsn=myodbc3;"
          << "User=" << username << ";"
          << "Password=" << password << ";"
#endif  // defined _WIN32
          << "Database=cuts;"
          << "Server=" << server << ";"
          << "Port=" << port << ";"
          << "Option=3;" << std::ends;

  short result = 0;
  SQLCHAR tempstr[1024];

  SQL_VERIFY (
    ::SQLDriverConnect (this->conn_,
                        0,
                        (SQLCHAR *)connstr.str ().c_str (),
                        SQL_NTS,
                        tempstr,
                        sizeof (tempstr),
                        &result,
                        SQL_DRIVER_NOPROMPT) ,
    ODBC_Conn_Exception (this->conn_));

  // Pass control to the base class.
  CUTS_DB_Connection::connect (username, password, server, port);
}

//
// disconnect
//
void ODBC_Connection::disconnect (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  if (this->conn_ != SQL_NULL_HDBC)
  {
    SQL_VERIFY (::SQLDisconnect (this->conn_),
                ODBC_Conn_Exception (this->conn_));
  }

  // Pass control to the base class.
  CUTS_DB_Connection::disconnect ();
}

//
// create_query
//
CUTS_DB_Query * ODBC_Connection::create_query (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  return new ODBC_Query (this->conn_);
}

//
// create_query
//
ODBC_Query * ODBC_Connection::create_odbc_query (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  return new ODBC_Query (this->conn_);
}
