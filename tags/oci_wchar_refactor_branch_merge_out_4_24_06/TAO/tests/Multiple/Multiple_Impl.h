//$Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO/tests/Multiple
//
// = FILENAME
//    Multiple_Impl.h
//
// = DESCRIPTION
//     This file contains the servant implementation used to test the
//     new collocation collocation scheme.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================


#ifndef TAO_MULTIPLE_IMPL_H_
#define TAO_MULTIPLE_IMPL_H_

// -- App. Specific Include --
#include "MultipleS.h"

class Bottom_Impl : public virtual POA_Multiple::Bottom
{
// = TITLE
//    Bottom_Impl
//
// = DESCRIPTION
//    This class implements the servant for the Bottom interface
//    defined in the IDL file.

public:

  // Ctor-Dtor
  Bottom_Impl (CORBA::ORB_ptr orb);
  virtual ~Bottom_Impl (void);

  // IDL Interface Methods
  virtual char * top_quote (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual char * left_quote (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual char * right_quote (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual char * bottom_quote (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
private:
  CORBA::ORB_var orb_;

};



class Delegated_Bottom_Impl : public virtual POA_Multiple::Bottom
{
// = TITLE
//    Delegated_Bottom_Impl
//
// = DESCRIPTION
//    This class implements the servant for the Bottom interface
//    which delegates all the request to another Bottom corba object.
//    This call forwarding is made using different strategy depending
//    on the option set for the ORB.

public:

  // Ctor-Dtor
  Delegated_Bottom_Impl (Multiple::Bottom_ptr delegate, CORBA::ORB_ptr orb);
  virtual ~Delegated_Bottom_Impl (void);

  // IDL Interface Methods.
  virtual char * top_quote (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
  virtual char * left_quote (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
  virtual char * right_quote (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));
  virtual char * bottom_quote (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     ));

private:
  Multiple::Bottom_var delegate_;

  CORBA::ORB_var orb_;

};
#endif /* TAO_MULTIPLE_IMPL_H_ */
