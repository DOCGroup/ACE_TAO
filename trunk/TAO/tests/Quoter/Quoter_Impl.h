// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Quoter
// 
// = FILENAME
//    Quoter_Impl.h
//
// = AUTHOR
//    Darrell Brunsch 
// 
// ============================================================================

#if !defined (QUOTER_IMPL_H)
#define	QUOTER_IMPL_H

#include "QuoterS.h"

// Forward declaration.
class Quoter_Impl;

// Typedefs.
typedef Quoter_Impl *Quoter_Impl_ptr;
typedef Quoter_Impl_ptr Quoter_Impl_ref;

class Quoter_Impl: public POA_Stock::Quoter
{
  // = TITLE
  //   Quoter_Impl
  //
  // = DESCRIPTION
  //   Actual Quoter Implementation class.  Returns a quoter for a given stock
  //   and provides an example for the lifecycle functionality.
public:
  Quoter_Impl (const char *obj_name = "",  const unsigned char use_LifeCycle_Service = 0);
  // Constructor (use_LifeCycle_Service is 1 if the LifeCycle_Service should be used 
  // instead of the Quoter Generic_Factory

  ~Quoter_Impl (void);
  // Destructor

  virtual CORBA::Long get_quote (const char *stock_name,
                                 CORBA::Environment &env);
  // Returns the current quote for the stock <stock_name>
  
  // = Lifecycle methods

  virtual CosLifeCycle::LifeCycleObject_ptr copy (CosLifeCycle::FactoryFinder_ptr there,
                                                  const CosLifeCycle::Criteria &the_criteria,
                                                  CORBA::Environment &_tao_environment);
  // Make a copy of this object

  virtual void move (CosLifeCycle::FactoryFinder_ptr there,
                     const CosLifeCycle::Criteria &the_criteria,
                     CORBA::Environment &_tao_environment);
  // Move this object using <there>

  virtual void remove (CORBA::Environment &_tao_environment);
  // Removes the object.

private:  
  unsigned char use_LifeCycle_Service_;
  // This flag defines if a Generic Factory is used (0 by default) or 
  // the more sophisticated LifeCycle Service
};

// Forward declaration.
class Quoter_Factory_Impl;

typedef Quoter_Factory_Impl *Quoter_Factory_Impl_ptr;

class Quoter_Factory_Impl: public POA_Stock::Quoter_Factory
{
  // = TITLE
  //   Quoter_Factory_Impl
  //
  // = DESCRIPTION
  //   Factory object returning the quoter_impl objrefs.
public:
  Quoter_Factory_Impl (size_t num);
  // Constructor that takes in the number of quoters in the pool.

  ~Quoter_Factory_Impl (void);
  // Destructor.

  virtual Stock::Quoter_ptr create_quoter (const char *name,  
                                           CORBA::Environment &env);
  // Return the quoter by the id <name>.

private:
  Quoter_Impl **my_quoters_;
  // Array of quoters.

  size_t quoter_num_;
  // Number of quoters.

  size_t next_quoter_;
  // Which quoter to return next.
};

#endif /* QUOTER_IMPL_H */
