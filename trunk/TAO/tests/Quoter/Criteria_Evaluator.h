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

#include "QuoterC.h"
#include "orbsvcs/CosLifeCycleC.h"

#if !defined (CRITERIA_EVALUATOR_H_H)
#define CRITERIA_EVALUATOR_H_H

class Quoter_Criteria_Evaluator : public Stock::Criteria_Evaluator
{
  // = TILE
  //  

public:
  Quoter_Criteria_Evaluator (const CosLifeCycle::Criteria criteria);
  ~Quoter_Criteria_Evaluator ();
  
  Stock::Criteria_Evaluator::SeqNamedValuePair * getInitialization (CORBA::Environment &_tao_environment);

  char * getFilter (CORBA::Environment &_tao_environment);

  Stock::Criteria_Evaluator::SeqNamedValuePair * getLogicalLocation (CORBA::Environment &_tao_environment) 
    {
      return 0;
    }

  char * getPreferences (CORBA::Environment &_tao_environment) 
    {
      return 0;
    }

  void setCriteria (const CosLifeCycle::Criteria & criteria,
		      CORBA::Environment &_tao_environment);

private:
  CORBA::Any *getCriteriaMember (const CORBA::String member_name);
    
  CosLifeCycle::Criteria criteria_;
};

#endif /* CRITERIA_EVALUATOR_H */
