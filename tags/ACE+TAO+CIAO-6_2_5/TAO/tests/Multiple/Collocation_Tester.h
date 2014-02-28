//=============================================================================
/**
 *  @file    Collocation_Tester.h
 *
 *  $Id$
 *
 *   This file contains the class that tests the TAO's collocation
 *   mechanism.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


// -- Custom App. Include --
#include "MultipleC.h"

#include "ace/Log_Msg.h"

struct Quote
{
  static const char *top;
  static const char *left;
  static const char *right;
  static const char *bottom;
};


class Collocation_Tester
{
public:

  // -- Constructor/Destructors --
  Collocation_Tester (CORBA::Object_ptr object);

  ~Collocation_Tester ();

  // -- Command --
  /// Runs the test.
  void run (void);


private:

  // -- Helper Methods --

  /// Tests the method accessible thru the
  /// Top interface.
  int  test_top (void);

  /// Tests the method accessible thru the
  /// Right interface.
  int test_right (void);

  /// Tests the method accessible thru the
  /// Left interface.
  int test_left (void);

  /// Tests the method accessible thru the
  /// Bottom interface.
  int test_bottom (void);

  /**
   * This method tests wether the answer obtained
   * is the one expected. As strcmp, it returns zero
   * if a match occurs and a non-zero value if there
   * is no match (actually 1 is returned if there is
   * no match.
   */
  int match_answer (const char *actual_answer,
                     const char *right_answer,
                     const char *method_name);

  void shutdown (void);
private:
  CORBA::Object_var object_;
};
