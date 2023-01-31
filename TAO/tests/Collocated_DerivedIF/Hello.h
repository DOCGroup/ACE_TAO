
#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

  /// Implement the Test::HelloAgain interface
  class HelloAgain
    : //public virtual Test::Hello,
      public virtual POA_Test::HelloAgain
  {
  public:
    /// Constructor
    HelloAgain(CORBA::ORB_ptr orb,
           ACE_thread_t thr_id);

    // = The skeleton methods
    virtual char * get_string_again ();
    virtual char * get_string ();
    virtual void shutdown ();


  private:
    /// Use an ORB reference to convert strings to objects and shutdown
    /// the application.
    CORBA::ORB_var orb_;
    ACE_thread_t thr_id_;
  };


#include /**/ "ace/post.h"
#endif /* HELLO_H */
