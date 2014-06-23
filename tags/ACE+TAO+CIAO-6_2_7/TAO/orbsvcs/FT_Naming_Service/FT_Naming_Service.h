// -*- C++ -*-

//==========================================================================
/**
 *  @file    FT_Naming_Service.h
 *
 *  $Id$
 *
 *    This class provides an override for the Naming_Service class factory
 *    method: create_naming_server ()
 *
 *  @author Kevin Stanley <stanleyk@ociweb.com>
 */
//==========================================================================


#ifndef TAO_FT_NAMING_SERVICE_H
#define TAO_FT_NAMING_SERVICE_H

#include "tao/ORB.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Server.h"


/**
 * @class TAO_FT_Naming_Service
 *
 * @brief Defines a class that encapsulates the implementation of the
 * Fault Tolerant Naming Service.
 *
 */
class TAO_FT_Naming_Service
{
public:
  /// Default Constructor.
  TAO_FT_Naming_Service (void);

  /// Constructor taking the command-line arguments.
  TAO_FT_Naming_Service (int argc, ACE_TCHAR* argv[]);

  /// Initialize the Naming Service with the arguments.
  virtual int init (int argc, ACE_TCHAR* argv[]);

  /// The opposite of init().
  virtual int fini (void);

  /// Run the TAO_Naming_Service.
  virtual int run (void);

  /// Shut down the TAO_Naming_Service; you must still call fini().
  virtual void shutdown (void);

  /// Destructor.
  virtual ~TAO_FT_Naming_Service (void);

protected:

  /// Parse the command line arguments to find
  /// the timeout period.
  int parse_args (int &argc, ACE_TCHAR* argv[]);

  /// The ORB.
  CORBA::ORB_var orb_;

  /// Naming Server instance.
  TAO_FT_Naming_Server my_naming_server_;

  /// After how long the server should stop listening to requests (in
  /// seconds).
  long time_;

  /// Number of threads for running the ORB. Default is 1
  int num_threads_;
};

#endif /* TAO_FT_NAMING_SERVICE_H */
