// -*- C++ -*-

//=============================================================================
/**
 * @file    DatabaseWorker.h
 *
 * $Id$
 *
 * This class contains the default database worker for CUTS
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_DATABASE_WORKER_H_
#define _CUTS_DATABASE_WORKER_H_

#include "cuts/Action.h"
#include "cuts/Action_T.h"
#include "cuts/Worker.h"
#include "cuts/Worker_T.h"
#include "cuts/WML_Macros.h"
#include "ace/Auto_Ptr.h"
#include <string>

// Forward decl.
class CUTS_DB_Connection;

// Forward decl.
class CUTS_DB_Query;

//=============================================================================
/**
 * @class CUTS_DatabaseWorker
 */
//=============================================================================

class CUTS_Export CUTS_Database_Worker :
  public CUTS_Worker_T <CUTS_Database_Worker>,
  public CUTS_Worker
{
public:
  //@@CUTS::WML
  //===========================================================================
  //@@CUTS::WML::ACTION[Create_Connection]
  CUTS_ACTION_DECLARE (CUTS_Database_Worker, Create_Connection)
  {
  public:
    CUTS_ACTION_DEFAULT_CONSTRUCTOR (CUTS_Database_Worker, Create_Connection)
    {

    }

    CUTS_ACTION_INIT_CONSTRUCTOR_BEGIN (CUTS_Database_Worker, Create_Connection)
      CUTS_ACTION_ARGUMENT (const char *, hostname)
      CUTS_ACTION_INIT_CONSTRUCTOR_END (CUTS_Database_Worker)
      CUTS_ACTION_PARAMETER_INIT (hostname_, hostname)
    {

    }

    CUTS_ACTION_DESTRUCTOR (Create_Connection)
    {

    }

    CUTS_ACTION_EXECUTE (create_connection,
                         (this->hostname_));

    CUTS_DECLARE_ACTION_FACTORY (CUTS_Database_Worker, Create_Connection);

    CUTS_ACTION_PARAMETER_LIST_BEGIN
    CUTS_ACTION_PARAMETER_DECLARE (std::string, hostname_);
    CUTS_ACTION_PARAMETER_LIST_END
  };

  //@@CUTS::WML::ACTION[Destroy_Connection]
  CUTS_ACTION_DECLARE (CUTS_Database_Worker, Destroy_Connection)
  {
  public:
    CUTS_ACTION_DEFAULT_CONSTRUCTOR (CUTS_Database_Worker, Destroy_Connection)
    {

    }

    CUTS_ACTION_DESTRUCTOR (Destroy_Connection)
    {

    }

    CUTS_ACTION_EXECUTE_NO_ARGS (destroy_connection);
    CUTS_DECLARE_ACTION_FACTORY (CUTS_Database_Worker, Destroy_Connection);
  };

  //@@CUTS::WML::ACTION[Process]
  CUTS_ACTION_DECLARE (CUTS_Database_Worker, Process)
  {
  public:
    CUTS_ACTION_DEFAULT_CONSTRUCTOR (CUTS_Database_Worker, Process)
      CUTS_ACTION_PARAMETER_INIT (count_, 0)
    {

    }

    CUTS_ACTION_INIT_CONSTRUCTOR_BEGIN (CUTS_Database_Worker, Process)
      CUTS_ACTION_ARGUMENT (size_t, count)
      CUTS_ACTION_INIT_CONSTRUCTOR_END (CUTS_Database_Worker)
      CUTS_ACTION_PARAMETER_INIT (count_, count)
    {

    }

    CUTS_ACTION_DESTRUCTOR (Process)
    {

    }

    CUTS_ACTION_EXECUTE (process,
                         (this->count_));

    CUTS_DECLARE_ACTION_FACTORY (CUTS_Database_Worker, Process);

    CUTS_ACTION_PARAMETER_LIST_BEGIN
    CUTS_ACTION_PARAMETER_DECLARE (size_t, count_);
    CUTS_ACTION_PARAMETER_LIST_END
  };
  //===========================================================================
  //@@CUTS::WML

  /// Constructor.
  CUTS_Database_Worker (void);

  /// Destructor.
  virtual ~CUTS_Database_Worker (void);

  /**
   * Excercise the database worker.
   *
   * @param[in]     reps       Number of repetitions.
   */
  virtual void process (size_t count);

  /**
   * Create a connection for the database worker to the
   * database on the specified host.
   *
   * @param[in]     hostname    Location of the database.
   */
  bool create_connection (const std::string & hostname);

  /// Destroy the database connection if it exists.
  void destroy_connection (void);

  /// Get the name of the host for the database.
  const char * hostname (void) const;

private:
  CUTS_ACTION_TABLE_DECLARE ();

  void fill_dataset (char * data, int size);

  /// Location of the database.
  std::string hostname_;

  /// Pointer to the database connection.
  ACE_Auto_Ptr <CUTS_DB_Connection> conn_;

  /// Pointer to the statement for the connection.
  ACE_Auto_Ptr <CUTS_DB_Query> stmt_;
};

//=============================================================================
/**
 * CUTS_Worker_Traits <CUTS_Database_Worker>
 */
//=============================================================================

template <>
class CUTS_Worker_Traits <CUTS_Database_Worker>
{
public:
  static const long worker_id_ = 3;
};

//=============================================================================
/**
 * CUTS_Action_Traits < >
 */
//=============================================================================

template <>
class CUTS_Action_Traits <CUTS_Database_Worker::Create_Connection>
{
public:
  static const long action_id_ = 1;
};

template <>
class CUTS_Action_Traits <CUTS_Database_Worker::Destroy_Connection>
{
public:
  static const long action_id_ = 2;
};

template <>
class CUTS_Action_Traits <CUTS_Database_Worker::Process>
{
public:
  static const long action_id_ = 3;
};

#if defined (__CUTS_INLINE__)
#include "cuts/DatabaseWorker.inl"
#endif  // defined __CUTS_INLINE__

#endif  // !defined _CUTS_DATABASE_WORKER_H_
