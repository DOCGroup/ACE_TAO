// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    DynEnum_i.h
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#if !defined DYNENUM_I_H
#define DYNENUM_I_H

#include "ace/Containers.h"

class DynEnum_i : public POA_DynEnum
{
public:
  // CORBA spec requires all numeric values to be non-negative,
  // (unsigned long) so we can't have a negative offset.
                                DynEnum_i           (const CORBA_Any& any);
                                DynEnum_i           (CORBA_TypeCode_ptr tc);
                                ~DynEnum_i          (void);

  // Functions specific to DynEnum
  char*                         value_as_string     (CORBA::Environment& env);
  void                          value_as_string     (const char* value_as_string,
                                                     CORBA::Environment& env);
  CORBA::ULong                  value_as_ulong      (CORBA::Environment&);
  void                          value_as_ulong      (CORBA::ULong value_as_ulong,
                                                     CORBA::Environment& env);

  // Common functions
  void                          assign              (DynAny_ptr dyn_any,
                                                     CORBA::Environment &env);
  DynAny_ptr                    copy                (CORBA::Environment &env);
  void                          destroy             (CORBA::Environment &env);
  void                          from_any            (const CORBA::Any& any,
                                                     CORBA::Environment &env);
  CORBA::Any_ptr                to_any              (CORBA::Environment &env);
  CORBA::TypeCode_ptr           type                (CORBA::Environment &);
  DynAny_ptr                    current_component   (CORBA::Environment &env);
  CORBA::Boolean                next                (CORBA::Environment &);
  CORBA::Boolean                seek                (CORBA::Long index,
                                                     CORBA::Environment &);
  void                          rewind              (CORBA::Environment &);

private:
  CORBA::TypeCode_var           type_;
  CORBA::ULong                  value_;
};

#endif /* DYNENUM_I_H */