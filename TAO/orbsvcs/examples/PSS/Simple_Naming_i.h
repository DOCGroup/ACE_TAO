/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/examples/PSS
//
// = DESCRIPTION
//      This class implements the bind and find methods in
//      simple_naming.idl which in turn use the PSDL implementation.
//
// = AUTHORS
//     Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef SIMPLE_NAMING_I_H
#define SIMPLE_NAMING_I_H
#include /**/ "ace/pre.h"

#include "Simple_NamingS.h"
#include "Simple_NamingC.h"

class TAO_PSDL_Code_Gen;

class Naming_Context_i : public virtual POA_Simple_Naming::Naming_Context
{
 public:

  Naming_Context_i (CORBA::ORB_ptr orb);

  ~Naming_Context_i (void);

  virtual int bind (const char *n,
                    const char *obj);

  virtual char *find (const char *n);

private:

  TAO_PSDL_Code_Gen *code_gen_;

  CORBA::ORB_var orb_;
};


#include /**/ "ace/post.h"
#endif /* SIMPLE_NAMING_I_H */
