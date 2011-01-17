// -*- C++ -*-

#include "ace/Auto_Functor.h"
#include "ace/streams.h"
#include "adbc/SQLite/Connection.h"
#include "adbc/SQLite/Exception.h"

static const char * __DROP_STMT__ =
"DROP TABLE IF EXISTS signup_sheet";

static const char * __CREATE_STMT__ =
"CREATE TABLE IF NOT EXISTS signup_sheet("
"uid INTEGER PRIMARY KEY AUTOINCREMENT,"
"timeofday DATETIME,"
"firstname TEXT,"
"middlename TEXT,"
"surname TEXT"
")";

static const char * __INSERT_STMT__ =
"INSERT INTO signup_sheet (timeofday, firstname, middlename, surname) "
"VALUES (DATETIME('NOW'), 'James', 'H.', 'Hill')";

static const char * __SELECT_STMT__ =
"SELECT * FROM signup_sheet";

//
// main
//
int ACE_TMAIN (int argc, char * argv [])
{
  try
  {
    // Open a connection to the database, which is a SQLite3 file.
    ::ADBC::SQLite::Connection conn;
    conn.connect ("simple.db");

    // Allocate a new query. Future versions will allow you to
    // declare a query on the stack.
    ::ADBC::SQLite::Query query (conn);

    // Execute queries that have no record.
    query.execute_no_record (__DROP_STMT__);
    query.execute_no_record (__CREATE_STMT__);
    query.execute_no_record (__INSERT_STMT__);

    // Execute a query that has a record.
    ::ADBC::SQLite::Record & record = query.execute (__SELECT_STMT__);

    // View the results of the query.
    ACE_CString timeofday, firstname, middlename, surname;

    for ( ; !record.done (); record.advance ())
    {
      record.get_data (1, timeofday);
      record.get_data (2, firstname);
      record.get_data (3, middlename);
      record.get_data (4, surname);

      std::cout << timeofday << " - "
                << firstname << " " << middlename << " " << surname
                << std::endl;
    }

    return 0;
  }
  catch (const ::ADBC::SQLite::Exception & ex)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%T (%t) - %M - %s\n"),
                ex.message ().c_str ()));
  }
  catch (const ::ADBC::Exception & ex)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%T (%t) - %M - %s\n"),
                ex.message ().c_str ()));
  }
  catch (...)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%T (%t) - %M - caught unknown exception\n")));
  }

  return 1;
}
