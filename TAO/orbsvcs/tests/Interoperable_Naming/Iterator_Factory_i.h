// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/Interoperable_Naming/
//
// = FILENAME
//     Iterator_Factory_i.h
//
// = DESCRIPTION
//     This class implements a simple CORBA object.
//
// = AUTHORS
//     Priyanka Gontla <pgontla@ece.uci.edu>
//
//
// ============================================================================

#ifndef ITERATOR_FACTORY_I_H
#define ITERATOR_FACTORY_I_H

#include "Web_ServerS.h"

class Web_Server_Iterator_Factory_i : virtual public POA_Web_Server::Iterator_Factory
{
 public:
  Web_Server_Iterator_Factory_i ();
};

#endif /* ITERATOR_FACTORY_I_H */
