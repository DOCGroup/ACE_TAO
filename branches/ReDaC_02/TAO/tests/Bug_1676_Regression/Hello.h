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

  // = The skeleton methods
	virtual ::Test::StringList * get_stringList (
		ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
		)
		ACE_THROW_SPEC ((
		::CORBA::SystemException
		));

    virtual void get_stringList2 (
        ::CORBA::Boolean initialize,
        ::Test::StringList_out osl
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        ::CORBA::SystemException
      ));
    
    virtual void mod_stringList (
        ::Test::StringList & iosl
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        ::CORBA::SystemException
      ));
    
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
