/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = FILENAME
//   Identity_i.h
//
// = DESCRIPTION
//    Implements the interface in Identity.idl.
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    with modifications by Bala Natarajan <bala@cs.wustl.edu>
// ============================================================================
#ifndef IDENTITY_I_H_
#define IDENTITY_I_H_

#include "IdentityS.h"

// This is to remove "inherits via dominance" warnings from MSVC.
#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

class Identity_i :
  public virtual POA_Identity,
  public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //    This class implements Identity.idl interface.
public:

  Identity_i (const char *name,
              PortableServer::POA_ptr poa);
  // Constructor - initializes the name of this object.

  ~Identity_i (void);
  // Destructor.

  virtual void get_name (CORBA::String_out name
                         TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Returns the name of this object.

  PortableServer::POA_ptr _default_POA (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Method for the POA that will return the persistent POA_ptr stored
  // in here..

private:

  CORBA::String_var name_;
  // Stores the name of this object.

  PortableServer::POA_var poa_;
};

#endif /* IDENTITY_I_H_ */
