// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Quoter
//
// = FILENAME
//    Quoter_i.h
//
// = AUTHOR
//    Darrell Brunsch <brunsch@uci.edu>
//
// ============================================================================

#ifndef QUOTER_IMPL_H
#define QUOTER_IMPL_H

#include "QuoterS.h"

// Forward declaration.
class Quoter_i;

// Typedefs.
typedef Quoter_i *Quoter_i_ptr;
typedef Quoter_i_ptr Quoter_i_ref;

class Quoter_i: public POA_Stock::Quoter
{
  // = TITLE
  //   Quoter_i
  //
  // = DESCRIPTION
  //   Actual Quoter Implementation class.  Returns a quoter for a given stock
  //   and provides an example for the lifecycle functionality.
public:
  Quoter_i (const char *obj_name = "",
            const unsigned char use_LifeCycle_Service = 0,
            PortableServer::POA_ptr poa_ptr = 0);
  // Constructor (use_LifeCycle_Service is 1 if the LifeCycle_Service should be used
  // instead of the Quoter Generic_Factory

  ~Quoter_i (void);
  // Destructor

  virtual CORBA::Long get_quote (const char *stock_name
                                 ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Stock::Invalid_Stock,
                       Stock::Invalid_Quoter));
  // Returns the current quote for the stock <stock_name>

  // = Lifecycle methods

  virtual CosLifeCycle::LifeCycleObject_ptr copy (CosLifeCycle::FactoryFinder_ptr there,
                                                  const CosLifeCycle::Criteria &the_criteria
                                                  ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosLifeCycle::NoFactory,
                       CosLifeCycle::NotCopyable,
                       CosLifeCycle::InvalidCriteria,
                       CosLifeCycle::CannotMeetCriteria));
  // Make a copy of this object

  virtual void move (CosLifeCycle::FactoryFinder_ptr there,
                     const CosLifeCycle::Criteria &the_criteria
                     ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosLifeCycle::NoFactory,
                       CosLifeCycle::NotMovable,
                       CosLifeCycle::InvalidCriteria,
                       CosLifeCycle::CannotMeetCriteria));
  // Move this object using <there>

  virtual void remove (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosLifeCycle::NotRemovable));
  // Removes the object.

private:
  unsigned char use_LifeCycle_Service_;
  // This flag defines if a Generic Factory is used (0 by default) or
  // the more sophisticated LifeCycle Service (1)

  PortableServer::POA_var poa_var_;
  // Keep a reference to the POA for use by the move operation
};

// Forward declaration.
class Quoter_Factory_i;

typedef Quoter_Factory_i *Quoter_Factory_i_ptr;

class Quoter_Factory_i: public POA_Stock::Quoter_Factory
{
  // = TITLE
  //   Quoter_Factory_i
  //
  // = DESCRIPTION
  //   Factory object returning the quoter_impl objrefs.
public:
  Quoter_Factory_i (size_t num, PortableServer::POA_ptr poa_ptr);
  // Constructor that takes in the number of quoters in the pool.

  ~Quoter_Factory_i (void);
  // Destructor.

  int init (ACE_ENV_SINGLE_ARG_DECL);
  // Initialize everything in the factory

  virtual Stock::Quoter_ptr create_quoter (const char *name
                                           ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Stock::Invalid_Quoter));
  // Return the quoter by the id <name>.

private:
  PortableServer::POA_ptr poa_ptr_;
  // Pointer to the poa.

  Quoter_i **my_quoters_;
  // Array of quoters.

  size_t quoter_num_;
  // Number of quoters.

  size_t next_quoter_;
  // Which quoter to return next.
};

#endif /* QUOTER_IMPL_H */
