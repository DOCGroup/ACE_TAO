
//=============================================================================
/**
 *  @file    Quoter_i.h
 *
 *  $Id$
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
typedef Quoter_i *Quoter_i_ptr;
typedef Quoter_i_ptr Quoter_i_ref;

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
  /// Constructor (use_LifeCycle_Service is 1 if the LifeCycle_Service should be used
  /// instead of the Quoter Generic_Factory
  Quoter_i (const char *obj_name = "",
            const unsigned char use_LifeCycle_Service = 0,
            PortableServer::POA_ptr poa_ptr = 0);

  /// Destructor
  ~Quoter_i (void);

  /// Returns the current quote for the stock <stock_name>
  virtual CORBA::Long get_quote (const char *stock_name);

  // = Lifecycle methods

  /// Make a copy of this object
  virtual CosLifeCycle::LifeCycleObject_ptr copy (CosLifeCycle::FactoryFinder_ptr there,
                                                  const CosLifeCycle::Criteria &the_criteria);

  /// Move this object using <there>
  virtual void move (CosLifeCycle::FactoryFinder_ptr there,
                     const CosLifeCycle::Criteria &the_criteria);

  /// Removes the object.
  virtual void remove (void);

private:
  /// This flag defines if a Generic Factory is used (0 by default) or
  /// the more sophisticated LifeCycle Service (1)
  unsigned char use_LifeCycle_Service_;

  /// Keep a reference to the POA for use by the move operation
  PortableServer::POA_var poa_var_;
};

// Forward declaration.
class Quoter_Factory_i;

typedef Quoter_Factory_i *Quoter_Factory_i_ptr;

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
  ~Quoter_Factory_i (void);

  /// Initialize everything in the factory
  int init (void);

  /// Return the quoter by the id <name>.
  virtual Stock::Quoter_ptr create_quoter (const char *name);

private:
  /// Pointer to the poa.
  PortableServer::POA_ptr poa_ptr_;

  /// Array of quoters.
  Quoter_i **my_quoters_;

  /// Number of quoters.
  size_t quoter_num_;

  /// Which quoter to return next.
  size_t next_quoter_;
};

#endif /* QUOTER_IMPL_H */
