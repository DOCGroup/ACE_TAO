// $Id$

#include "orbsvcs/Trader/Interpreter.h"
#include "orbsvcs/Trader/Constraint_Visitors.h"
#include "ace/OS_NS_string.h"

template<class T> class Literal_Interpreter;

template<class T>
class Literal_Visitor: public TAO_Constraint_Visitor
{
public:
  Literal_Visitor (Literal_Interpreter<T>* interp);
  virtual ~Literal_Visitor (void);

  virtual int visit_constraint (TAO_Unary_Constraint*);

  virtual int visit_with (TAO_Unary_Constraint*) { return 1; }
  virtual int visit_min (TAO_Unary_Constraint*) { return 1; }
  virtual int visit_max (TAO_Unary_Constraint*) { return 1; }
  virtual int visit_first (TAO_Noop_Constraint*) { return 1; }
  virtual int visit_random (TAO_Noop_Constraint*) { return 1; }

  virtual int visit_and (TAO_Binary_Constraint*) { return 1; }
  virtual int visit_or (TAO_Binary_Constraint*) { return 1; }
  virtual int visit_not (TAO_Unary_Constraint*) { return 1; }

  virtual int visit_exist (TAO_Unary_Constraint*) { return 1; }
  virtual int visit_unary_minus (TAO_Unary_Constraint*) { return 1; }

  virtual int visit_add (TAO_Binary_Constraint*) { return 1; }
  virtual int visit_sub (TAO_Binary_Constraint*) { return 1; }
  virtual int visit_mult (TAO_Binary_Constraint*) { return 1; }
  virtual int visit_div (TAO_Binary_Constraint*) { return 1; }

  virtual int visit_twiddle (TAO_Binary_Constraint*) { return 1; }
  virtual int visit_in (TAO_Binary_Constraint*) { return 1; }

  virtual int visit_less_than (TAO_Binary_Constraint*) { return 1; }
  virtual int visit_less_than_equal (TAO_Binary_Constraint*) { return 1; }
  virtual int visit_greater_than (TAO_Binary_Constraint*) { return 1; }
  virtual int visit_greater_than_equal (TAO_Binary_Constraint*) { return 1; }
  virtual int visit_equal (TAO_Binary_Constraint*) { return 1; }
  virtual int visit_not_equal (TAO_Binary_Constraint*) { return 1; }

  virtual int visit_literal (TAO_Literal_Constraint*);
  virtual int visit_property (TAO_Property_Constraint*) { return 1; }

private:
  Literal_Interpreter<T>* interp_;
};

template<class T>
Literal_Visitor<T>::Literal_Visitor (Literal_Interpreter<T>* interp)
 : interp_ (interp)
{
}

template<class T>
Literal_Visitor<T>::~Literal_Visitor (void)
{
}

template<class T> int
Literal_Visitor<T>::visit_constraint (TAO_Unary_Constraint* c)
{
  return c->operand ()->accept (this);
}

template<class T> int
Literal_Visitor<T>::visit_literal (TAO_Literal_Constraint* literal)
{
  this->interp_->constraint(*literal);
  return 1;
}

template<class T>
class Literal_Interpreter: public TAO_Interpreter
{
public:
  Literal_Interpreter (void);
  virtual ~Literal_Interpreter (void);

  bool test (const char* str,
             T expected);
  void constraint (TAO_Literal_Constraint& constraint);
  const TAO_Literal_Constraint& constraint (void) const;

private:
  bool compare (T expected) const;

  TAO_Literal_Constraint constraint_;
};

template<class T>
Literal_Interpreter<T>::Literal_Interpreter (void)
{
}

template<class T>
Literal_Interpreter<T>::~Literal_Interpreter (void)
{
}

template<class T> bool
Literal_Interpreter<T>::test (const char* str,
                              T expected)
{
  this->build_tree (str);
  if (this->root_ == 0)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Invalid interpreter string.\n"));
      return false;
    }

  Literal_Visitor<T> visitor (this);
  this->root_->accept (&visitor);

  return this->compare(expected);
}

template<> bool
Literal_Interpreter<const char*>::compare (const char* expected) const
{
  return (ACE_OS::strcmp(static_cast<const char*> (this->constraint_),
                         expected) == 0);
}

template<class T> bool
Literal_Interpreter<T>::compare (T expected) const
{
  return (static_cast<T> (this->constraint_) == expected);
}

template<class T> void
Literal_Interpreter<T>::constraint (TAO_Literal_Constraint& constraint)
{
  this->constraint_ = constraint;
}

template<class T> const TAO_Literal_Constraint&
Literal_Interpreter<T>::constraint (void) const
{
  return this->constraint_;
}

int
ACE_TMAIN (int, ACE_TCHAR **)
{
  int status = 0;
  {
    Literal_Interpreter<CORBA::ULongLong> u_interp;
    if (!u_interp.test ("993834343433882",
                        ACE_UINT64_LITERAL (993834343433882)))
      {
        status++;
        ACE_ERROR ((LM_ERROR, "ERROR: CORBA::ULongLong test failed.\n"));
      }

    Literal_Interpreter<CORBA::LongLong> interp;
    if (!interp.test ("-1879048193", -1879048193))
      {
        status++;
        ACE_ERROR ((LM_ERROR, "ERROR: CORBA::LongLong test failed.\n"));
      }

    // Since we're comparing signed and unsigned, the signed should get
    // promoted to unsigned.  However, it gets logically promoted, not
    // binarily.  So, a negative value is converted into zero as an
    // unsigned value.
    if (interp.constraint () > u_interp.constraint ())
      {
        status++;
        ACE_ERROR ((LM_ERROR, "ERROR: Mixed greater than test 1 failed.\n"));
      }
  }
  {
    Literal_Interpreter<CORBA::Double> u_interp;
    if (!u_interp.test ("993834343433882.88837719", 993834343433882.88837719))
      {
        status++;
        ACE_ERROR ((LM_ERROR, "ERROR: CORBA::Double test 1 failed.\n"));
      }

    Literal_Interpreter<CORBA::Double> interp;
    if (!interp.test ("-993834343433882.88837719",
                       -993834343433882.88837719))
      {
        status++;
        ACE_ERROR ((LM_ERROR, "ERROR: CORBA::Double test 2 failed.\n"));
      }

    if (interp.constraint () > u_interp.constraint ())
      {
        status++;
        ACE_ERROR ((LM_ERROR, "ERROR: Mixed greater than test 2 failed.\n"));
      }
  }
  {
    Literal_Interpreter<CORBA::Boolean> interp;
    if (!interp.test ("TRUE", true))
      {
        status++;
        ACE_ERROR ((LM_ERROR, "ERROR: CORBA::Boolean test 1 failed.\n"));
      }
  }
  {
    Literal_Interpreter<CORBA::Boolean> interp;
    if (!interp.test ("FALSE", false))
      {
        status++;
        ACE_ERROR ((LM_ERROR, "ERROR: CORBA::Boolean test 2 failed.\n"));
      }
  }
  {
    Literal_Interpreter<const char*> interp;
    if (!interp.test ("'This is a test'", "This is a test"))
      {
        status++;
        ACE_ERROR ((LM_ERROR, "ERROR: const char* test failed.\n"));
      }
  }
  return status;
}
