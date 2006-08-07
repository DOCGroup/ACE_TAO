// -*- C++ -*-

//=============================================================================
/**
 *  @file    Hello.h
 *
 *  $Id$
 *
 *  Header file for recursive type Any insertion and extraction test.
 *
 *  @author Ossama Othman
 */
//=============================================================================


#ifndef HELLO_H
#define HELLO_H

#include "TestS.h"

class Hello
  : public virtual POA_Test::Hello
{
public:

  /// Constructor
  Hello (CORBA::ORB_ptr orb);

  // Return the any provide as an "in" argument.
  virtual CORBA::Any * get_any (CORBA::Any const & the_any
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// The ORB which will be shutdown by Hello::shutdown().
  CORBA::ORB_var orb_;

};

#endif /* HELLO_H */
