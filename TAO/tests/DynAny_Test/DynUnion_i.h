// $Id$

// ========================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    DynUnion_i.h
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ========================================================================

#if !defined DYNUNION_I_H
#define DYNUNION_I_H

class DynUnion_i : public POA_DynUnion
{
public:
  DynUnion_i (const CORBA_Any& any);
  DynUnion_i (CORBA_TypeCode_ptr tc);
  ~DynUnion_i (void);

  CORBA::Boolean set_as_default (CORBA::Environment& env);
  DynAny_ptr discriminator (CORBA::Environment& env);
  TCKind discriminator_kind (CORBA::Environment& env);
  DynAny_ptr member (CORBA::Environment& env);
  char* member_name (CORBA::Environment& env);
  void member_name (const char* member_name, CORBA::Environment& env);
  TCKind member_kind (CORBA::Environment& env);

private:
  CORBA::Long default_index_;
  DynAny_var discriminator_;
  CORBA::TCKind discriminator_kind_;
  DynAny_var member_;
};

#endif /* DYNUNION_I_H */
