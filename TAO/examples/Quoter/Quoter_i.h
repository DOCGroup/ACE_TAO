
//=============================================================================
/**
 *  @file    Quoter_i.h
 *
 *  @author Darrell Brunsch <brunsch@uci.edu>
 */
//=============================================================================


#ifndef QUOTER_IMPL_H
#define QUOTER_IMPL_H

#include "QuoterS.h"

// Forward declaration.
class Quoter_i;

// Typedefs.
using Quoter_i_ptr = Quoter_i *;
using Quoter_i_ref = Quoter_i_ptr;

/**
 * @class Quoter_i:
 *
 * @brief Quoter_i
 *
 * Actual Quoter Implementation class.  Returns a quoter for a given stock
 * and provides an example for the lifecycle functionality.
 */
class Quoter_i: public POA_Stock::Quoter
{
public:
  /// Constructor (use_LifeCycle_Service is true if the LifeCycle_Service should be used
  /// instead of the Quoter Generic_Factory
  explicit Quoter_i (char const *obj_name = "",
                     bool use_LifeCycle_Service = false,
                     PortableServer::POA_ptr poa_ptr = nullptr);

  /// Destructor
  ~Quoter_i () override = default;

  /// Returns the current quote for the stock <stock_name>
  CORBA::Long get_quote (char const *stock_name) override;

  // = Lifecycle methods

  /// Make a copy of this object
  CosLifeCycle::LifeCycleObject_ptr copy (CosLifeCycle::FactoryFinder_ptr there,
                                          CosLifeCycle::Criteria const &the_criteria) override;

  /// Move this object using <there>
  void move (CosLifeCycle::FactoryFinder_ptr there,
             CosLifeCycle::Criteria const &the_criteria) override;

  /// Removes the object.
  void remove () override;

private:
  /// This flag defines if a Generic Factory is used (false by default) or
  /// the more sophisticated LifeCycle Service (true)
  bool use_LifeCycle_Service_ {};

  /// Keep a reference to the POA for use by the move operation
  PortableServer::POA_var poa_var_ {};
};

// Forward declaration.
class Quoter_Factory_i;

using Quoter_Factory_i_ptr = Quoter_Factory_i *;

/**
 * @class Quoter_Factory_i:
 *
 * @brief Quoter_Factory_i
 *
 * Factory object returning the quoter_impl objrefs.
 */
class Quoter_Factory_i: public POA_Stock::Quoter_Factory
{
public:
  /// Constructor that takes in the number of quoters in the pool.
  Quoter_Factory_i (size_t num, PortableServer::POA_ptr poa_ptr);

  /// Destructor.
  ~Quoter_Factory_i () override;

  /// Initialize everything in the factory
  int init ();

  /// Return the quoter by the id <name>.
  Stock::Quoter_ptr create_quoter (char const *name) override;

private:
  /// Pointer to the poa.
  PortableServer::POA_ptr poa_ptr_ {};

  /// Array of quoters.
  Quoter_i **my_quoters_ {};

  /// Number of quoters.
  size_t quoter_num_ {};

  /// Which quoter to return next.
  size_t next_quoter_ {};
};

#endif /* QUOTER_IMPL_H */
