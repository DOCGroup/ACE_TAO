// $Id$

// ============================================================================
//
// = FILENAME
//    Criteria_Evaluaotr.h
//
// = DESCRIPTION
//    An Evaluator for the CosLifeCycle Criteria
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "orbsvcs/CosLifeCycleC.h"

#if !defined (CRITERIA_EVALUATOR_H_H)
#define CRITERIA_EVALUATOR_H_H

class Criteria_Evaluator
{
  // = TILE
  //   A CosLifeCycle conforming Generic Factory for the Quoter
  //   example. It uses the Naming Service to find a fitting factory.

public:
  Criteria_Evaluator (CosLifeCycle::Criteria &criteria);
  ~Criteria_Evaluator ();

  class NotAvailable : public CORBA::UserException
  {
  public:
    NotAvailable (void); // default ctor

    NotAvailable (const char *reason);    // ctor

    NotAvailable(const NotAvailable &); // copy ctor

    ~NotAvailable (void); // dtor

    NotAvailable &operator= (const NotAvailable &); // copy operator

    static NotAvailable *_narrow (CORBA::Exception *);

    CORBA::String_var reason_;
  };

  // get the initialization
  TAO_Unbounded_Object_Sequence<CosLifeCycle::NVP> *getInitialization (CORBA::Environment &env);
  
  // get a filter string  
  CORBA::String getFilter (CORBA::Environment &env);

  // get the logical location, not implemented
  TAO_Unbounded_Object_Sequence<CosLifeCycle::NVP> *getLogicalLocation (CORBA::Environment &env) 
  {
    return 0;
  }

  // get the preferences, not implemented
  CORBA::String getPreferences (CORBA::Environment &env) 
  {
    return 0;
  }

private:
  CORBA::Any *getCriteriaMember (const CORBA::String member_name);
    
  CosLifeCycle::Criteria &criteria_;
};

#endif /* CRITERIA_EVALUATOR_H */