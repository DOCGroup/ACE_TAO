//$Id$
// UUIDTest.cpp
// $Revision$ $Date$

#include <string>
//#include "JCAFCore/src/Common/Exception.h"
#include "ace/UUID.h"

class Tester
{
public:
  Tester();
  ~Tester();

  int init( int argc, char* argv[]);
  void fini();
  int test();

private:
  int test1( void);
  int test2( void);
  int test3( void);
  int test4( void);
  void test5( void*& d);
};

Tester::Tester()
{
}

Tester::~Tester()
{
}

int Tester::init( int, char*[])
{
  return 0;
}

void Tester::fini( void)
{
}

int Tester::test()
{
//    if (test1() == -1)
//      return -1;
//    if (test2() == -1)
//      return -1;
  ACE_UUID_GENERATOR::instance ()->init ();

  ACE_DEBUG ((LM_DEBUG,
	      "%s\n",
	      ACE_UUID_GENERATOR::instance ()->generateUUID ()->to_string ()->c_str ()));
  
  return 0;
}



// main ------------------------------------------------------------------------

int main(int argc, char* argv[])

{
  ACE::init();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Starting %s test at %D\n"), "UUIDTest"));

  int retValue = EXIT_SUCCESS;
  int testRetValue = 0;
  try  {

    Tester tester;

    if (tester.init( argc, argv) == -1) {

      ACE_DEBUG((
          LM_DEBUG,
          "UUIDTest: Tester::init failed\n"));
      return EXIT_FAILURE;

    }
    testRetValue = tester.test();
    tester.fini();

  } catch( std::exception& ex) {

    ACE_ERROR((
        LM_ERROR,
        ACE_TEXT( "(%P|%t)")
        ACE_TEXT( "UUIDTest - got exception %s\n"),
        ex.what()));
    testRetValue = -1;

  } catch( ...) {

    ACE_DEBUG((
        LM_DEBUG,
        "UUIDTest - unknown exception\n"));
    testRetValue = -1;

  }
  if (testRetValue == 0) {

    ACE_DEBUG((
        LM_DEBUG,
        "UUIDTest succeeded\n"));

  } else {

    ACE_DEBUG((
        LM_DEBUG,
        "UUIDTest failed\n"));
    retValue = EXIT_FAILURE;

  }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Ending %s test at %D\n"), "UUIDTest"));
  ACE::fini();
  return retValue;
}


