#include "Hello.h"

CORBA::Boolean
Hello::ids_differ_ = true;

Hello::Hello (CORBA::ORB_ptr orb, Test::Hello_ptr server, CORBA::ULong server_id)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    server_ (Test::Hello::_duplicate (server)),
    server_id_ (server_id)
{
}

char *
Hello::get_string ()
{
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}

void
Hello::throw_exception ()
{
  throw Test::MyException ();
}

// Nested sequence step 1 - This op called by the client on the server
CORBA::Boolean
Hello::call_me_back (Test::Hello_ptr me)
{
  CORBA::Boolean result = me->call_back ();
  return result;
}

// Nested sequence step 2 - This op called by the server on the client
CORBA::Boolean
Hello::call_back ()
{
  CORBA::Boolean result = server_->check_request_id ();
  return result;
}

// Nested sequence step 3 - This op called by the client on the server
CORBA::Boolean
Hello::check_request_id ()
{
  // The server request interceptor will have set this public static member
  // false if the same retention id has been used twice
  return ids_differ_;
}

CORBA::ULong
Hello::drop_down_dead ()
{
  return server_id_;
}
