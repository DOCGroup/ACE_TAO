
#include <cstdio>
#include <iostream>
#include "HTTP_handler_bugs.h"

#define HTTP_SERVER_PORT  "12345"


using namespace std;


int ACE_TMAIN(int argc, ACE_TCHAR *argv[]) {
  int ret=0;
  try {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    CORBA::Object_var obj;
    obj=orb->string_to_object("corbaloc:iiop:127.0.0.1:" HTTP_SERVER_PORT "/GenericService");

    // First test is for bug where IOR string returned is malformed.
    // HTTP_handler returns IOR string in list of "ACE_Message_Block"s which are later concatenated as C strings.
    // Because "ACE_Message_Block"s are not zero terminated, a lot of garbage is picked up after end of each "ACE_Message_Block".
    // This is why IOR returned is malformed and fails processing.

    try {
      CORBA::Object_var tmp = orb->string_to_object("http://127.0.0.1:" HTTP_SERVER_PORT "/ior_with_http_response_headers");
    }
    catch (const CORBA::Exception& ex) {
      // Test failed
      ex._tao_print_exception ("Test 1 failed:");
      ret=1;
    }

    // Second test if for bug when there are no HTTP response headers returned from web server but just contents of IOR string
    // In order not to run into issue present in first bug, test IOR is returned zero terminated from web server
    // In reality IOR will not be zero terminated but here we want to avoid the first bug appearing

    try {
      CORBA::Object_var tmp = orb->string_to_object("http://127.0.0.1:" HTTP_SERVER_PORT "/ior_without_http_response_headers_zero_terminated");
      if (tmp==0) {
        ACE_OS::puts("Test 2 failed: Object is NULL.");
        ret=2;
      }
    }
    catch (const CORBA::Exception& ex) {
      // Test failed
      ex._tao_print_exception ("Test 2 failed:");
      ret=2;
    }

    orb->destroy ();
  }
  catch (const CORBA::Exception& ex) {
    ex._tao_print_exception ("Exception caught:");
    ret=3;
  }

  return ret;
}
