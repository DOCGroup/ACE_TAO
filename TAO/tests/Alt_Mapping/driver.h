
//=============================================================================
/**
 *  @file    driver.h
 *
 *  Header file for the driver program. The driver is a singleton.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef DRIVER_H
#define DRIVER_H

#include "options.h"
#include "alt_mappingC.h"

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
  Driver () = default;
  ~Driver () = default;
  Driver (Driver const &) = delete;
  Driver (Driver &&) = delete;
  Driver &operator= (Driver const &) = delete;
  Driver &operator= (Driver &&) = delete;

  /// Initialize the driver object
  int init (int argc, ACE_TCHAR **argv);

  /// Execute client example code.
  int run ();

private:
  /// underlying ORB (we do not own it)
  CORBA::ORB_var orb_;

  /// object reference (we do not own it)
  Alt_Mapping_var objref_;
};

#endif /* !defined */
