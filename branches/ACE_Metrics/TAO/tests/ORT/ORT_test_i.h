// -*- C++ -*-

//=============================================================================
/**
 * @file ORT_test_i.h
 *
 * $Id$
 *
 * Implementation header for the "server" IDL interface for the
 * ORT example.
 *
 * @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================


#ifndef ORT_TEST_I_H
#define ORT_TEST_I_H

#include "ORT_testS.h"


class ORT_test_i : public virtual POA_ObjectReferenceTemplate::ORT_test
{
public:

  ORT_test_i (CORBA::ORB_ptr orb);

  virtual CORBA::Boolean request_server (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  CORBA::ORB_var orb_;

};


#endif  /* ORT_TEST_I_H */
