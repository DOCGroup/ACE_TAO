
//=============================================================================
/**
 *  @file    driver.h
 *
 *  $Id$
 *
 *  Header file for the driver program.
 *
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
  Driver (void);
  ~Driver (void);

enum TEST_TYPE
  {
    NO_TEST,
    TEST_DYNANY,
    TEST_DYNARRAY,
    TEST_DYNENUM,
    TEST_DYNSEQUENCE,
    TEST_DYNSTRUCT,
    TEST_DYNUNION
  };

  /// Initialize the driver object.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Parse command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Execute test code.
  /**
   * @return The number of errors detected
   */
  int run (void);

private:
  /// underlying ORB (we do not own it)
  CORBA::ORB_var orb_;

  TEST_TYPE test_type_;

  int debug_;
};

#endif /* DRIVER_H */
