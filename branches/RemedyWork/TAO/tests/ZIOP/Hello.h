//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb);

  virtual Test::Octet_Seq *get_big_reply ();
  virtual void big_request (const ::Test::Octet_Seq & octet_in);

  // = The skeleton methods
  virtual char * get_string (const char * mystring);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ULong length_;

  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
