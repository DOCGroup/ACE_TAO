// -*- C++ -*-
//
// $Id$

#ifndef SIMPLE_H
#define SIMPLE_H

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
#endif /* _MSC_VER */

/// Implement the Test::Simple interface
class Simple
  : public virtual POA_Test::Simple
  , public virtual PortableServer::RefCountServantBase
{
public:

  Simple (void);
  /// Constructor
  
  virtual char * get_string (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* SIMPLE_H */
