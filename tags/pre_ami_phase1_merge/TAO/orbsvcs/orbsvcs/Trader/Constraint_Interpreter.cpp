// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Constraint_Interpreter.cpp
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#include "ace/OS.h"
#include "Constraint_Interpreter.h"

ACE_RCSID(Trader, Constraint_Interpreter, "$Id$")

  // *************************************************************
  // TAO_Interpreter
  // *************************************************************

TAO_Interpreter::~TAO_Interpreter (void)
{
  delete root_;
}

int
TAO_Interpreter::build_tree(const char* constraints)
{
  TAO_Lex_String_Input::reset((char*)constraints);
  int return_value = 0;

  yyval.constraint_ = 0;
  return_value = ::yyparse();

  if (return_value == 0 && yyval.constraint_ != 0)
    this->root_ = yyval.constraint_;
  else
    {
      while (yylex() > 0)
        ;
      this->root_ = 0;
    }

  return return_value;
}


int
TAO_Interpreter::is_empty_string(const char* str)
{
  int return_value = 0;

  if (str != 0)
    {
      int i = 0;
      while (str[i] != '\0')
        {
          if (str[i] != ' ')
            break;

          i++;
        }

      if (str[i] == '\0')
        return_value = 1;
    }

  return return_value;
}

  // *************************************************************
  // TAO_Constraint_Interpreter
  // *************************************************************

TAO_Constraint_Interpreter::
TAO_Constraint_Interpreter (const CosTradingRepos::ServiceTypeRepository::TypeStruct& ts,
                            const char* constraints,
                            CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CosTrading::IllegalConstraint))
  : TAO_Interpreter ()
{
  TAO_Constraint_Validator type_checker (ts);

  if (TAO_Interpreter::is_empty_string (constraints))
    this->root_ = new TAO_Literal_Constraint ((CORBA::Boolean) 1);
  else
    {
      if (this->build_tree (constraints) != 0)
        TAO_THROW (CosTrading::IllegalConstraint (constraints));

      if (type_checker.validate (this->root_) == -1)
        TAO_THROW (CosTrading::IllegalConstraint (constraints));
    }
}

TAO_Constraint_Interpreter::
TAO_Constraint_Interpreter(TAO_Constraint_Validator& validator,
                           const char* constraints,
                           CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CosTrading::IllegalConstraint))
{
  if (TAO_Interpreter::is_empty_string (constraints))
    this->root_ = new TAO_Literal_Constraint ((CORBA::Boolean) 1);
  else
    {
      if (this->build_tree (constraints) != 0)
        TAO_THROW (CosTrading::IllegalConstraint (constraints));

      if (validator.validate (this->root_) == -1)
        TAO_THROW (CosTrading::IllegalConstraint (constraints));
    }
}

TAO_Constraint_Interpreter::~TAO_Constraint_Interpreter (void)
{
}

CORBA::Boolean
TAO_Constraint_Interpreter::evaluate (CosTrading::Offer* offer)
{
  TAO_Constraint_Evaluator evaluator (offer);
  return evaluator.evaluate_constraint (this->root_);
}

CORBA::Boolean
TAO_Constraint_Interpreter::evaluate(TAO_Constraint_Evaluator& evaluator)
{
  return evaluator.evaluate_constraint (this->root_);
}

  // *************************************************************
  // TAO_Preference_Interpreter
  // *************************************************************

TAO_Preference_Interpreter::
TAO_Preference_Interpreter(const CosTradingRepos::ServiceTypeRepository::TypeStruct& ts,
                           const char* preference,
                           CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CosTrading::Lookup::IllegalPreference))
  : TAO_Interpreter ()
{
  TAO_Constraint_Validator type_checker (ts);

  if (TAO_Interpreter::is_empty_string (preference))
    this->root_ = new TAO_Noop_Constraint (TAO_FIRST);
  else
    {
      if (this->build_tree (preference) != 0)
        TAO_THROW (CosTrading::Lookup::IllegalPreference (preference));

      if (type_checker.validate (this->root_) == -1)
        TAO_THROW (CosTrading::Lookup::IllegalPreference (preference));
    }
}

TAO_Preference_Interpreter::
TAO_Preference_Interpreter(TAO_Constraint_Validator& validator,
                           const char* preference,
                           CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CosTrading::Lookup::IllegalPreference))
    : TAO_Interpreter ()
{
  if (TAO_Interpreter::is_empty_string (preference))
    this->root_ = new TAO_Noop_Constraint (TAO_FIRST);
  else
    {
      if (this->build_tree (preference) != 0)
        TAO_THROW (CosTrading::Lookup::IllegalPreference (preference));

      if (validator.validate (this->root_) == -1)
        TAO_THROW (CosTrading::Lookup::IllegalPreference (preference));
    }
}

TAO_Preference_Interpreter::~TAO_Preference_Interpreter()
{
}

void
TAO_Preference_Interpreter::
order_offer (CosTrading::Offer* offer,
             CosTrading::OfferId offer_id)
{
  TAO_Constraint_Evaluator evaluator (offer);
  this->order_offer (evaluator, offer, offer_id);
}

void
TAO_Preference_Interpreter::
order_offer (TAO_Constraint_Evaluator& evaluator,
             CosTrading::Offer* offer,
             CosTrading::OfferId offer_id)
{
  if (this->root_ != 0)
    {
      Preference_Info pref_info;

      pref_info.offer_ = offer;
      pref_info.offer_id_ = offer_id;
      pref_info.evaluated_ = 1;

      if (evaluator.evaluate_preference (this->root_, pref_info.value_) == 0)
        {
          // If the evaluation succeeds, insert the node into the
          // correct place in the queue.
          TAO_Expression_Type expr_type = this->root_->expr_type ();

          if (expr_type == TAO_FIRST ||
              (expr_type == TAO_WITH &&
               ! ACE_static_cast (CORBA::Boolean, pref_info.value_)))
            this->offers_.enqueue_tail (pref_info);
          else
            this->offers_.enqueue_head (pref_info);

          if (expr_type == TAO_MIN || expr_type == TAO_MAX)
            {
              Ordered_Offers::ITERATOR offer_iter (this->offers_);

              // Push the new item down the list until the min/max
              // criterion is satisfied. Observe the evaluation
              // failed / evaluation suceeded partion in the list.
              offer_iter.advance ();
              for (int i = 1;
                   ! offer_iter.done ();
                   offer_iter.advance (), i++)
                {
                  Preference_Info* current_offer;
                  offer_iter.next (current_offer);

                  // Maintain the sorted order in the first partition.
                  if (current_offer->evaluated_ == 1 &&
                      ((expr_type == TAO_MIN &&
                       pref_info.value_ > current_offer->value_) ||
                      (expr_type == TAO_MAX &&
                       pref_info.value_ < current_offer->value_)))
                    {
                      // Swap the out of order pair
                      this->offers_.set (*current_offer, i - 1);
                      this->offers_.set (pref_info, i);
                    }
                  else
                    break;
                }
            }
        }
      else
        {
          // If the evaluation fails, just tack the sucker onto the
          // end of the queue.
          pref_info.evaluated_ = 0;
          this->offers_.enqueue_tail (pref_info);
        }
    }
}

int
TAO_Preference_Interpreter::
remove_offer (CosTrading::Offer*& offer,
              CosTrading::OfferId& offer_id)
{
  int return_value = -1;
  Preference_Info pref_info;

  return_value = this->offers_.dequeue_head (pref_info);

  if (return_value == 0)
    {
      offer = pref_info.offer_;
      offer_id = pref_info.offer_id_;
    }

  return return_value;
}

int
TAO_Preference_Interpreter::
remove_offer (CosTrading::Offer*& offer)
{
  CosTrading::OfferId offer_id = 0;
  return this->remove_offer (offer, offer_id);
}


int
TAO_Preference_Interpreter::num_offers (void)
{
  return this->offers_.size();
}

  // *************************************************************
  // Ugly Lex/Yacc Stuff
  // *************************************************************

char* TAO_Lex_String_Input::string_ = 0;
char* TAO_Lex_String_Input::current_ = 0;
char* TAO_Lex_String_Input::end_ = 0;

// Routine to have Lex read its input from the constraint string.

int
TAO_Lex_String_Input::copy_into(char* buf, int max_size)
{
  int chars_left =  TAO_Lex_String_Input::end_ - TAO_Lex_String_Input::current_;
  int n = (max_size > chars_left) ? chars_left : max_size;

  if (n > 0)
    {
      memcpy(buf, TAO_Lex_String_Input::current_, n);
      TAO_Lex_String_Input::current_ += n;
    }

  return n;
}

void
TAO_Lex_String_Input::reset(char* input_string)
{
  TAO_Lex_String_Input::string_ = input_string;
  TAO_Lex_String_Input::current_ = input_string;
  TAO_Lex_String_Input::end_ = input_string +
    ACE_OS::strlen(TAO_Lex_String_Input::string_);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<TAO_Preference_Interpreter::Preference_Info>;
template class ACE_Unbounded_Queue<TAO_Preference_Interpreter::Preference_Info>;
template class ACE_Unbounded_Queue_Iterator<TAO_Preference_Interpreter::Preference_Info>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<TAO_Preference_Interpreter::Preference_Info>
#pragma instantiate ACE_Unbounded_Queue<TAO_Preference_Interpreter::Preference_Info>
#pragma instantiate ACE_Unbounded_Queue_Iterator<TAO_Preference_Interpreter::Preference_Info>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
