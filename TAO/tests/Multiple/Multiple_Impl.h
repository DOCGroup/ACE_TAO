//=============================================================================
/**
 *  @file    Multiple_Impl.h
 *
 *   This file contains the servant implementation used to test the
 *   new collocation collocation scheme.
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
  explicit Bottom_Impl (CORBA::ORB_ptr const orb);
  Bottom_Impl (Bottom_Impl const &) = delete;
  Bottom_Impl (Bottom_Impl &&) = delete;
  Bottom_Impl &operator= (Bottom_Impl const &) = delete;
  Bottom_Impl &operator= (Bottom_Impl &&) = delete;

  // IDL Interface Methods
  char *top_quote () override;
  char *left_quote () override;
  char *right_quote () override;
  char *bottom_quote () override;
  void shutdown () override;
private:
  CORBA::ORB_var const orb_;
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
  explicit Delegated_Bottom_Impl (Multiple::Bottom_ptr const delegate);
  Delegated_Bottom_Impl (Delegated_Bottom_Impl const &) = delete;
  Delegated_Bottom_Impl (Delegated_Bottom_Impl &&) = delete;
  Delegated_Bottom_Impl &operator= (Delegated_Bottom_Impl const &) = delete;
  Delegated_Bottom_Impl &operator= (Delegated_Bottom_Impl &&) = delete;

  // IDL Interface Methods.
  char *top_quote () override;
  char *left_quote () override;
  char *right_quote () override;
  char *bottom_quote () override;
  void shutdown () override;

private:
  Multiple::Bottom_var const delegate_;
};
#endif /* TAO_MULTIPLE_IMPL_H_ */
