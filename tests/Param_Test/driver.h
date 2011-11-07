
//=============================================================================
/**
 *  @file    driver.h
 *
 *  $Id$
 *
 *  Header file for the driver program. The driver is a singleton.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef DRIVER_H
#define DRIVER_H

#include "options.h"
#include "param_testC.h"

/**
 * @class Driver
 *
 * @brief Driver
 *
 * Driver program for the client
 */
class Driver
{
public:
  // = Constructor and destructor.
  Driver (void);
  ~Driver (void);

  /// Initialize the driver object
  int init (int argc, ACE_TCHAR **argv);

  /// Execute client example code.
  int run (void);

private:
  /// underlying ORB (we do not own it)
  CORBA::ORB_var orb_;

  /// object reference (we do not own it)
  Param_Test_var objref_;
};

#endif /* !defined */
