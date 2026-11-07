//=============================================================================
/**
 *  @file    Collocation_Tester.h
 *
 *   This file contains the class that tests the TAO's collocation
 *   mechanism.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


// -- Custom App. Include --
#include "MultipleC.h"

#include "ace/Log_Msg.h"

struct Quote
{
  inline static constexpr char top[] = "Ciao";
  inline static constexpr char left[] = "Hola";
  inline static constexpr char right[] = "Hello";
  inline static constexpr char bottom[] = "Previet";
};


class Collocation_Tester
{
public:
  // -- Constructor/Destructors --
  explicit Collocation_Tester (CORBA::Object_ptr const object);
  Collocation_Tester (Collocation_Tester const &) = delete;
  Collocation_Tester (Collocation_Tester &&) = delete;
  Collocation_Tester &operator= (Collocation_Tester const &) = delete;
  Collocation_Tester &operator= (Collocation_Tester &&) = delete;

  // -- Command --
  /// Runs the test.
  void run () const;


private:
  // -- Helper Methods --

  /// Tests the method accessible thru the
  /// Top interface.
  int test_top () const;

  /// Tests the method accessible thru the
  /// Right interface.
  int test_right () const;

  /// Tests the method accessible thru the
  /// Left interface.
  int test_left () const;

  /// Tests the method accessible thru the
  /// Bottom interface.
  int test_bottom () const;

  /**
   * This method tests wether the answer obtained
   * is the one expected. As strcmp, it returns zero
   * if a match occurs and a non-zero value if there
   * is no match (actually 1 is returned if there is
   * no match.
   */
  static int match_answer (char const *actual_answer,
                           char const *right_answer,
                           char const *method_name);

  void shutdown () const;
private:
  CORBA::Object_var const object_;
};
