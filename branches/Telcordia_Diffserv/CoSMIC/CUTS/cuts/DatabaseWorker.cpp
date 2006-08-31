// $Id$

#include "cuts/DatabaseWorker.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/DatabaseWorker.inl"
#endif

#include "cuts/utils/ODBC/ODBC_Connection.h"
#include "cuts/utils/DB_Query.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Basic_Types.h"
#include "ace/Log_Msg.h"
#include "ace/OS_Memory.h"
#include <sstream>

#define MAX_DATA_SIZE 255

//@@ CUTS [WORKER_TABLE]
CUTS_ACTION_TABLE_BEGIN (CUTS_Database_Worker, CUTS_Worker)
CUTS_ACTION_TABLE_ENTRY ("Create_Connection",
                         CUTS_Database_Worker::Create_Connection);
CUTS_ACTION_TABLE_ENTRY ("Destroy_Connection",
                         CUTS_Database_Worker::Destroy_Connection);
CUTS_ACTION_TABLE_ENTRY ("Process",
                         CUTS_Database_Worker::Process);
CUTS_ACTION_TABLE_END (CUTS_Database_Worker)
//@@ CUTS [WORKER_TABLE]

//
// CUTS_DatabaseWorker
//
CUTS_Database_Worker::CUTS_Database_Worker (void)
: hostname_ (CUTS_DEFAULT_HOSTNAME)
{
  // Allocate a new <MyODBC_Connection> object.
  ODBC_Connection * conn = 0;
  ACE_NEW (conn, ODBC_Connection ());

  this->conn_.reset (conn);
}

//
// ~CUTS_DatabaseWorker
//
CUTS_Database_Worker::~CUTS_Database_Worker (void)
{
  this->destroy_connection ();
}

//
// process
//
void CUTS_Database_Worker::process (size_t count)
{
  if (this->stmt_.get ())
  {
    std::ostringstream sqlstr;
    char dataset[MAX_DATA_SIZE];

    long worktag = ACE_OS::rand () % ACE_INT32_MAX;

    for (size_t i = 0; i < count; i ++)
    {
      int size = ACE_OS::rand () % MAX_DATA_SIZE;
      fill_dataset (dataset, size);

      // create a SQL statement for the testing record
      sqlstr
        << "insert into scratchpad (component_id, worktag, dataset) values ("
        << this->parent_ << "," << worktag << ",'" << dataset << "');"
        << std::ends;
    }

    try
    {
      this->stmt_->execute_no_record (sqlstr.str ().c_str ());

      sqlstr.seekp(0);

      // Delete the insert SQL statement.
      sqlstr
        << "DELETE FROM scratchpad WHERE component_id = "
        << this->parent_ << " AND worktag = " << worktag << ";" << std::ends;

      this->stmt_->execute_no_record (sqlstr.str ().c_str ());
    }
    catch (CUTS_DB_Exception & ex)
    {
      ex.print ();
    }
    catch (...)
    {
      ACE_ERROR ((LM_ERROR,
                  "[%M] -%T - unknown exception in "
                  "CUTS_Database_Worker::process\n"));
    }
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                "[%M] -%T - No ODBC statement allocated. Please verify "
                "you are connected to a database\n"));
  }
}

//
// create_connection
//
bool CUTS_Database_Worker::create_connection (const std::string & hostname)
{
  // We need to kill any existing connections before we
  // continue. This also involves killing the associated
  // <stmt_> for the connection.
  this->destroy_connection ();

  try
  {
    // We are now ready to establish a new connection to the specified
    // hostname.
    this->conn_->connect (CUTS_USERNAME,
                          CUTS_PASSWORD,
                          hostname.c_str (),
                          CUTS_DEFAULT_PORT);

    // We need to allocate an <ODBC_Stmt> for the connection.
    // This will prevent us from having to allocate one each
    // time.
    this->stmt_.reset (this->conn_->create_query ());
    this->hostname_ = hostname;

    return true;
  }
  catch (CUTS_DB_Exception & ex)
  {
    ex.print ();
  }
  catch (...)
  {
    ACE_ERROR ((LM_ERROR,
                "[%M] -%T - unknown exception in "
                "CUTS_Database_Worker::create_connection\n"));
  }

  return false;
}

//
// fill_dataset
//
void CUTS_Database_Worker::fill_dataset (char * dataset, int size)
{
  // determine the fill character
  static char fill_value =
    static_cast <int> (fill_value) >= static_cast <int> ('Z')
    ? 'A' : fill_value + 1;

  // fill the dataset with the next value
  char * endpoint = dataset + size;
  std::fill (dataset, endpoint, fill_value);
  dataset[size] = '\0';
}

//
// destroy_connection
//
void CUTS_Database_Worker::destroy_connection (void)
{
  if (this->conn_->is_connected ())
  {
    this->stmt_.reset ();
    this->conn_->disconnect ();
  }
}
