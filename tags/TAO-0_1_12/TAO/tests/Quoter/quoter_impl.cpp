// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Quoter
//
// = FILENAME
//    quoter_impl.cpp
//
// = AUTHOR
//    Darrell Brunsch
//
// ============================================================================

#include "tao/corba.h"
//#include "ace/Auto_Ptr.h"
#include "quoter_impl.h"

// Constructor

Quoter_Factory_Impl::Quoter_Factory_Impl (void)
{
}

// Destructor

Quoter_Factory_Impl::~Quoter_Factory_Impl (void)
{
}

Stock::Quoter_ptr  
Quoter_Factory_Impl::create_quoter (const char *name,  
                                    CORBA::Environment &env)
{
  ACE_UNUSED_ARG (name);
  return my_quoter_._this (env);
}

// Constructor

Quoter_Impl::Quoter_Impl (const char *)
{
}

// Destructor

Quoter_Impl::~Quoter_Impl (void)
{
}

Quoter_Impl::get_quote (char const *stock_name, class CORBA_Environment &env)
{
  ACE_UNUSED_ARG (stock_name);
  ACE_UNUSED_ARG (env);
 
  return 42;
}

// Shutdown.

void Quoter_Impl::destroy (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);

  ACE_DEBUG ((LM_DEBUG, 
              "%s",
	      "I have been asked to shut down "));
	      
  TAO_ORB_Core_instance ()->orb ()->shutdown ();
}




