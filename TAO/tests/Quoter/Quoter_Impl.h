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

#include "quoterS.h"

// Forward declarations.
class Quoter_Impl;

// Typedefs.
typedef Quoter_Impl *Quoter_Impl_ptr;
typedef Quoter_Impl_ptr Quoter_Impl_ref;

class Quoter_Impl: public POA_Stock::Quoter
{
  // = TITLE
  //   @@ Write
  //
  // = DESCRIPTION
  //   @@ Write
public:
  Quoter_Impl (const char *obj_name = 0);
  // Constructor

  ~Quoter_Impl (void);
  // Destructor

  virtual CORBA::Long get_quote (const char *stock_name,
                                 CORBA::Environment &env);
  
  virtual void destroy (CORBA_Environment &env);

  virtual CosLifeCycle::LifeCycleObject_ptr copy (CosLifeCycle::FactoryFinder_ptr there,
                                                  const CosLifeCycle::Criteria &the_criteria,
                                                  CORBA::Environment &_tao_environment);

  virtual void move (CosLifeCycle::FactoryFinder_ptr there,
                     const CosLifeCycle::Criteria &the_criteria,
                     CORBA::Environment &_tao_environment);

  virtual void remove (CORBA::Environment &_tao_environment);
};

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
  Quoter_Factory_Impl (void);
  // Constructor.

  ~Quoter_Factory_Impl (void);
  // Destructor.

  virtual Stock::Quoter_ptr  create_quoter (const char *name,  
                                            CORBA::Environment &env);
  // Return the quoter by the id <name>.

private:
  Quoter_Impl my_quoter_;
  // @@ Do we just want one of these?!
};

#endif /* QUOTER_IMPL_H */
