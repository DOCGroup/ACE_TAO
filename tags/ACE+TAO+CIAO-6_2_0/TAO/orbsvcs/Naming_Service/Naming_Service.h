// -*- C++ -*-

//==========================================================================
/**
 *  @file    Naming_Service.h
 *
 *  $Id$
 *
 *    This class implements the functionality of a Naming_Service in
 *    a stand-alone process.
 *
 *  @author Nagarajan Surendran (naga@cs.wustl.edu) Marina Spivak <marina@cs.wustl.edu>
 */
//==========================================================================


#ifndef TAO_NAMING_SERVICE_H
#define TAO_NAMING_SERVICE_H

#include /**/ "ace/pre.h"
#include "tao/ORB.h"

#include "orbsvcs/Naming/Naming_Server.h"

/**
 * @class TAO_Naming_Service
 *
 * @brief Defines a class that encapsulates the implementation of the
 * COS Naming Service.
 *
 * This class makes use of the <TAO_Naming_Server>
 * to implement the COS Naming Service.
 */
class TAO_Naming_Service
{
public:
  /// Default Constructor.
  TAO_Naming_Service (void);

  /// Constructor taking the command-line arguments.
  TAO_Naming_Service (int argc, ACE_TCHAR* argv[]);

  /// Initialize the Naming Service with the arguments.
  virtual int init (int argc, ACE_TCHAR* argv[]);

  /// The opposite of init().
  virtual int fini (void);

  /// Run the TAO_Naming_Service.
  virtual int run (void);

  /// Shut down the TAO_Naming_Service; you must still call fini().
  virtual void shutdown (void);

  /// Destructor.
  virtual ~TAO_Naming_Service (void);

protected:

  /// Parse the command line arguments to find
  /// the timeout period.
  int parse_args (int &argc, ACE_TCHAR* argv[]);

  /// The ORB.
  CORBA::ORB_var orb_;

  /// Naming Server instance.
  TAO_Naming_Server my_naming_server_;

  /// After how long the server should stop listening to requests (in
  /// seconds).
  long time_;

  /// Number of threads for running the ORB. Default is 1
  int num_threads_;
};

#include /**/ "ace/post.h"

#endif /* TAO_NAMING_SERVICE_H */
