// $Id$

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

  /// Constructor
  ORT_test_i ();

  CORBA::Boolean request_server (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#endif /* ORT_TEST_I_H */
