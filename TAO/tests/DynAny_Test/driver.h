
//=============================================================================
/**
 *  @file    driver.h
 *
 *  Header file for the driver program.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (DRIVER_H)
#define DRIVER_H

#include "tao/ORB.h"

class Driver
{
public:
  // = Constructor and destructor.
  Driver () = default;
  ~Driver ();
  Driver (Driver const&) = delete;
  Driver (Driver&&) = delete;
  Driver& operator= (Driver const&) = delete;
  Driver& operator= (Driver&&) = delete;

  enum class Test_Type
  {
    none,
    dynany,
    dynarray,
    dynenum,
    dynsequence,
    dynstruct,
    dynunion
  };

  /// Initialize the driver object.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Parse command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Execute test code.
  /**
   * @return The number of errors detected
   */
  int run ();

private:
  /// underlying ORB (we do not own it)
  CORBA::ORB_var orb_;

  Test_Type test_type_ {Test_Type::none};

  bool debug_ {};
};

#endif /* DRIVER_H */
