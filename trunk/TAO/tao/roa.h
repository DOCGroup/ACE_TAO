/* This may look like C, but it's really -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    roa.h
//
// = DESCRIPTION
//     
// = AUTHOR
//     Chris Cleeland & David Brownell
// 
// ============================================================================

#if !defined (ACE_ROA_H)
#  define ACE_ROA_H

#if 0
#  include "ace/INET_Addr.h"

#  include "tao/boa.h"
#  include "tao/params.h"
#  include "tao/connect.h"
#endif

#include "tao/corba.h"

class ACE_Svc_Export ROA;
typedef ROA* ROA_ptr;

class ACE_Svc_Export ROA : public CORBA_BOA
// = TITLE
//     Realtime Object Adapter class.
{
public:

private:
  // = Copy and assignment:  just say no
};

#endif /* TAO_ROA_H */
