// $Id$

/**
 * @file
 *
 * This program checks if the compiler / platform supports exceptions,
 * in particular, if raising exceptions in constructors work.  The
 * motivation for this test was a discussion on the development
 * mailing list, and the documentation was captured in:
 *
 *   http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=3715
 *
 */

#include "test_config.h"

// The first part of the test is to compile this line.  If the program
// does not compile the platform is just too broken.
#include <memory>

// ... using exceptions with C++ also implies using the standard
// exceptions ...
#include <stdexcept>

// For extra challenge, we use the anonymous namespace
namespace
{
  int constructors = 0;
  int destructors = 0;
  int allocs = 0;
  int deallocs = 0;
  bool had_failure = false;

  void check_constructor_count(int expected,
                               char const * filename,
                               int lineno);
  void check_destructor_count(int expected,
                              char const * filename,
                              int lineno);
  void check_alloc_count(int expected,
                         char const * filename,
                         int lineno);
  void reset_counts();
  void never_reached(char const * filename,
                     int lineno);
  int status();

  /**
   * @class Base
   */
  class Base
  {
  public:
    Base()
    {
      ++constructors;
    }
    Base(Base const & )
    {
      ++constructors;
    }
    ~Base()
    {
      ++destructors;
    }

    void * operator new(size_t n)
    {
      ++allocs;
      return ::operator new(n);
    }
    void operator delete(void * x)
    {
      ++deallocs;
      return ::operator delete(x);
    }
  };

  /**
   * @class May_Pop
   *
   * Create a class that can raise exceptions in its constructor
   *
   */
  class May_Pop : public Base
  {
  public:
    explicit May_Pop(bool do_raise)
      // Even if an exception is raised, the base object is fully
      // constructed and must be fully destructed ...
      : Base()
    {
      if (do_raise)
        {
          throw std::runtime_error("requested exception");
        }
      // ... if an exception is raised this object is never
      // initialized and no constructor / destructor calls should take
      // place ...
      Base tmp;
    }
  };

  /**
   * @class Aggregate
   */
  class Aggregate : public Base
  {
  private:
    May_Pop m1;
    May_Pop m2;
    May_Pop m3;

  public:
    /**
     * Constructor.  Element m1 is fully initialized, its constructors
     * and destructors should run, m2 is partially initialized, its
     * destructor never runs, m3 is never initialized, neither its
     * constructor nor destructor runs.
     */
    Aggregate()
      : Base()
      , m1(false)
      , m2(true)
      , m3(false)
    {}
  };
}



int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_10_Test"));
  try
    {
      // ... start the test fresh ...
      reset_counts();
      // Let's try to build a simple object and destroy it, without any
      // exceptions raised ...
      May_Pop m1(false);
      // ... two instances of the base class should be created ...
      check_constructor_count(2, __FILE__, __LINE__);
      // ... but only one instance is destroyed at this point ...
      check_destructor_count(1, __FILE__, __LINE__);
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Unexpected exception caught\n")));
    }
  // ... now both instances are gone ...
  check_destructor_count(2, __FILE__, __LINE__);

  try
    {
      // ... start the test fresh ...
      reset_counts();
      // ... now raise an exception ...
      May_Pop m1(true);
      never_reached(__FILE__, __LINE__);
    }
  catch(...)
    {
      // ... only one instance gets created ...
      check_constructor_count(1, __FILE__, __LINE__);
      // ... and it is gone ...
      check_destructor_count(1, __FILE__, __LINE__);
    }

  try
    {
      // ... start the test fresh ...
      reset_counts();
      // ... now build a complex object with a failure in the middle ...
      Aggregate a;
      never_reached(__FILE__, __LINE__);
    }
  catch(...)
    {
      // ... check the expectations ...
      check_constructor_count(4, __FILE__, __LINE__);
      check_destructor_count(4, __FILE__, __LINE__);
    }

  try
    {
      // ... start the test fresh ...
      reset_counts();
      std::auto_ptr<Aggregate> b(new Aggregate);
      never_reached(__FILE__, __LINE__);
    }
  catch(...)
    {
      // ... check the expectations ...
      check_constructor_count(4, __FILE__, __LINE__);
      check_destructor_count(4, __FILE__, __LINE__);
      check_alloc_count(1, __FILE__, __LINE__);
    }

  ACE_END_TEST;
  return status();
}

namespace
{

void
check_constructor_count(int expected,
                        char const * filename,
                        int lineno)
{
  if (constructors == expected)
    {
      return;
    }
  had_failure = true;
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT("Expected %d constructor calls, had %d -- (%s:%d)\n"),
              expected, constructors, filename, lineno));
}

void
check_destructor_count(int expected,
                       char const * filename,
                       int lineno)
{
  if (destructors == expected)
    {
      return;
    }
  had_failure = true;
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT("Expected %d destructor calls, had %d -- (%s:%d)\n"),
              expected, destructors, filename, lineno));
}

void
check_alloc_count(int expected,
                  char const * filename,
                  int lineno)
{
  if (allocs == expected && deallocs == expected)
    {
      return;
    }
  had_failure = true;
  if (allocs != expected)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Expected %d alloc calls, had %d -- (%s:%d)\n"),
                  expected, allocs, filename, lineno));
    }
  if (deallocs != expected)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT("Expected %d dealloc calls, had %d -- (%s:%d)\n"),
                  expected, deallocs, filename, lineno));
    }
}

void
never_reached(char const * filename,
              int lineno)
{
  had_failure = true;
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT("Code should not have reached (%s:%d)\n"),
              filename, lineno));
}

void
reset_counts()
{
  constructors = 0;
  destructors = 0;
  allocs = 0;
  deallocs = 0;
}

int
status()
{
  if (had_failure)
    {
      return 1;
    }
  return 0;
}

}
