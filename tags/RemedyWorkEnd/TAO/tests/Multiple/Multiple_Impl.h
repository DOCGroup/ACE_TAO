//=============================================================================
/**
 *  @file    Multiple_Impl.h
 *
 *  $Id$
 *
 *   This file contains the servant implementation used to test the
 *   new collocation collocation scheme.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================



#ifndef TAO_MULTIPLE_IMPL_H_
#define TAO_MULTIPLE_IMPL_H_

// -- App. Specific Include --
#include "MultipleS.h"

/**
 * @class Bottom_Impl
 *
 * @brief Bottom_Impl
 *
 * This class implements the servant for the Bottom interface
 * defined in the IDL file.
 */
class Bottom_Impl : public virtual POA_Multiple::Bottom
{

public:

  // Ctor-Dtor
  Bottom_Impl (CORBA::ORB_ptr orb);
  virtual ~Bottom_Impl (void);

  // IDL Interface Methods
  virtual char * top_quote (void);

  virtual char * left_quote (void);

  virtual char * right_quote (void);

  virtual char * bottom_quote (void);

  virtual void shutdown (void);
private:
  CORBA::ORB_var orb_;

};



/**
 * @class Delegated_Bottom_Impl
 *
 * @brief Delegated_Bottom_Impl
 *
 * This class implements the servant for the Bottom interface
 * which delegates all the request to another Bottom corba object.
 * This call forwarding is made using different strategy depending
 * on the option set for the ORB.
 */
class Delegated_Bottom_Impl : public virtual POA_Multiple::Bottom
{

public:

  // Ctor-Dtor
  Delegated_Bottom_Impl (Multiple::Bottom_ptr delegate, CORBA::ORB_ptr orb);
  virtual ~Delegated_Bottom_Impl (void);

  // IDL Interface Methods.
  virtual char * top_quote (void);
  virtual char * left_quote (void);
  virtual char * right_quote (void);
  virtual char * bottom_quote (void);

  virtual void shutdown (void);

private:
  Multiple::Bottom_var delegate_;

  CORBA::ORB_var orb_;

};
#endif /* TAO_MULTIPLE_IMPL_H_ */
