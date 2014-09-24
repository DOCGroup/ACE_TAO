// $Id$

#include "Analyzer.h"
#include "tao/DynamicAny/DynCommon.h"

DynAnyAnalyzer::DynAnyAnalyzer (int argc, ACE_TCHAR *argv[])
  : level_ (1u),
    orb_ (CORBA::ORB_init (argc, argv)),
    dany_fact_ (0)
{
  if (CORBA::is_nil (orb_.in ()))
    {
      ACE_DEBUG ((LM_ERROR, "Could not init orb\n"));
      throw 0;
    }

  CORBA::Object_var
    obj (orb_->resolve_initial_references ("DynAnyFactory"));
  dany_fact_ = DynamicAny::DynAnyFactory::_narrow(obj.in ());
  if (CORBA::is_nil (dany_fact_.in ()))
    {
      ACE_DEBUG ((LM_ERROR, "Nil dynamic any factory after narrow\n"));
      throw 0;
    }
}

DynamicAny::DynAny_ptr
DynAnyAnalyzer::DynAny_NoTrunc (const CORBA::Any &any)
{
  return this->dany_fact_
             ->create_dyn_any_without_truncation (any);
}

DynamicAny::DynAny_ptr
DynAnyAnalyzer::DynAny (const CORBA::Any &any)
{
  return this->dany_fact_
             ->create_dyn_any (any);
}

DynamicAny::DynAny_ptr
DynAnyAnalyzer::DynAny (const CORBA::TypeCode_ptr &tc)
{
  return this->dany_fact_
             ->create_dyn_any_from_type_code (tc);
}

void
DynAnyAnalyzer::register_factory (
  CORBA::ValueFactoryBase_var factory)
{
  factory_id * new_fact (
    dynamic_cast<factory_id *> (factory.in()) );
  if (new_fact)
    {
      tab("Registering factory for ValueType ");
      ACE_DEBUG ((LM_DEBUG, "%C\n", new_fact->id ()));
      CORBA::ValueFactoryBase_var old_fact (
        orb_->register_value_factory (
          new_fact->id (), factory.in () )  );
    }
}

void
DynAnyAnalyzer::get_base_types (
  CORBA::TypeCode_ptr tc,
  BaseTypesList_t &base_types,
  CORBA::ULong *total_member_count)
{
  // First initalize to the fully derived type we are
  // starting with.

  CORBA::ULong numberOfBases = 1u;
  base_types.size (numberOfBases);
  base_types[0] = TAO_DynAnyFactory::strip_alias (tc);
  if (total_member_count)
    {
      *total_member_count = base_types[0]->member_count ();
    }

  // Obtain each derived type's basetype and add this to the list

  CORBA::TypeCode_var
    base (base_types[0]->concrete_base_type());
  while (0 != base.in() &&
         CORBA::tk_value ==
         (base = TAO_DynAnyFactory::strip_alias (base.in()))
         ->kind ())
    {
      if (total_member_count)
        {
          *total_member_count += base->member_count ();
        }

      base_types.size (numberOfBases + 1);
      base_types[numberOfBases++] = CORBA::TypeCode::_duplicate (base.in ());
      base = base->concrete_base_type();
    }
}

CORBA::TypeCode_ptr
DynAnyAnalyzer::get_correct_base_type (
  const BaseTypesList_t &base_types,
  CORBA::ULong &index)
{
  // We work backwards through the list of derived types,
  // so index 0 is the first member of the extreme base type
  // (assuming it has any members) once we run out of that
  // base types members we move up the list to the next
  // derived type until that type's members are exhausted
  // and so on until we reach the member we have asked for.

  CORBA::ULong
    currentBase = base_types.size ();
  if (!currentBase)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO_DynValue_i::get_correct_base_type () ")
        ACE_TEXT ("BaseTypesList_t is not initialised\n")));
      return 0;
    }

  while (base_types[--currentBase]->member_count () <= index)
    {
      index -= base_types[currentBase]->member_count ();
      if (!currentBase)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO_DynValue_i::get_correct_base_type () ")
            ACE_TEXT ("BaseTypesList_t is not large enough\n")));
          return 0;
        }
    }

  // Note that the "index" has now been reduced to the range
  // of the returning base_type.
  return base_types[currentBase].in ();
}

#define CASEE(type,CT,str) case CORBA::tk_##type: {\
  CORBA::CT b = da->get_##type();\
  if (!newline) tab(""); ACE_DEBUG ((LM_DEBUG, str , b));\
} break;

void
DynAnyAnalyzer::analyze (
  DynamicAny::DynAny_ptr da,
  CORBA::Boolean newline)
{
  CORBA::TypeCode_var tc (da->type ());
  CORBA::TCKind kind (tc->kind ());
  CORBA::TypeCode_var
    dup (CORBA::TypeCode::_duplicate (tc.in ()));

  // strip aliases
  while (CORBA::tk_alias == kind)
    {
      dup = dup->content_type ();
      kind = dup->kind ();
    }

  switch (kind)
    {
    case CORBA::tk_value_box:
      {
        DynamicAny::DynValueBox_var
          box (DynamicAny::DynValueBox::_narrow (da));

        if (!newline)
          {
            tab ("");
          }
        if (box->is_null ())
          {
            ACE_DEBUG ((LM_DEBUG, "{NULL} "));
          }
        ACE_DEBUG ((LM_DEBUG, "ValueBox Type: %C \"%C\": ",
                    tc->name (), tc->id ())             );
        if (box->is_null ())
          {
            ACE_DEBUG ((LM_DEBUG, "\n"));
          }
        else
          {
            DynamicAny::DynAny_var
              cc (box->current_component ());
            this->analyze (cc.in (), true);
          }
      }
      break; // end tk_valuebox

    case CORBA::tk_value:
      {
        DynamicAny::DynValue_var
          dvt (DynamicAny::DynValue::_narrow (da));

        if (!newline)
          {
            tab ("");
          }
        if (dvt->is_null ())
          {
            ACE_DEBUG ((LM_DEBUG, "{NULL} "));
          }

        BaseTypesList_t base_types;
        get_base_types (tc.in (), base_types);

        for (CORBA::ULong i= 0u; i < base_types.size(); ++i)
          {
            if (i)
              {
                tab (": ");
              }
            else
              {
                ACE_DEBUG ((LM_DEBUG, "ValueType: "));
              }

            switch (base_types[i]->type_modifier ())
              {
              case CORBA::VM_CUSTOM:
                ACE_DEBUG ((LM_DEBUG, "Custom "));
                break;

              case CORBA::VM_ABSTRACT:
                ACE_DEBUG ((LM_DEBUG, "Abstract "));
                break;

              case CORBA::VM_TRUNCATABLE:
                ACE_DEBUG ((LM_DEBUG, "Truncatable "));
                break;

              default:
                break;
              }

            ACE_DEBUG ((LM_DEBUG, "%C \"%C\"\n",
                        base_types[i]->name (),
                        base_types[i]->id ()  ));
            ++level_;
          }
         level_ -= base_types.size();

        if (!(dvt->is_null ()))
          {
            CORBA::ULong member_number = 0u;
            ++level_;
            if (da->seek (0)) do
              {
                DynamicAny::DynAny_var
                  cc (dvt->current_component ());
                DynamicAny::FieldName_var
                  fn (dvt->current_member_name ());
                CORBA::ULong sub_member_number = member_number;
                const CORBA::TypeCode_ptr
                  base = get_correct_base_type (
                           base_types,
                           sub_member_number);
                const char *const visability =
                  ((CORBA::PRIVATE_MEMBER ==
                    base->member_visibility (sub_member_number)) ?
                    "Private" : "Public ");
                tab ("[");
                ACE_DEBUG ((LM_DEBUG, "%03u] %C \"%C\": ",
                  ++member_number, visability, fn.in ()   ));
                if (CORBA::is_nil (cc.in ()))
                  {
                    ACE_DEBUG ((LM_DEBUG, " {Null}\n"));
                  }
                else
                  {
                    this->analyze (cc.in (), true);
                  }
              } while (da->next ());
            else
              {
                tab("{Has No Members}\n");
              }
            --level_;
          }
      }
      break; // end tk_value

    case CORBA::tk_struct:
      {
        DynamicAny::DynStruct_var
          ds (DynamicAny::DynStruct::_narrow (da));

        if (newline)
          {
            ACE_DEBUG ((LM_DEBUG, "\n"));
          }
        tab ("Struct: ");
        ACE_DEBUG ((LM_DEBUG, "%C \"%C\"",
                    tc->name (), tc->id () ));
        if (da->seek (0))
          {
            ACE_DEBUG ((LM_DEBUG, "\n"));
            ++level_;
            do
              {
                DynamicAny::DynAny_var
                  cc (ds->current_component ());
                DynamicAny::FieldName_var
                  fn (ds->current_member_name ());

                tab ("Member = ");
                ACE_DEBUG ((LM_DEBUG, "%C\n", fn.in ()));
                if (!CORBA::is_nil (cc.in ()))
                  {
                    this->analyze (cc.in ());
                  }
              } while (da->next ());
            --level_;
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, " Null\n"));
          }
      }
      break; // end tk_struct

    case CORBA::tk_union:
      {
        if (newline)
          {
            ACE_DEBUG ((LM_DEBUG, "\n"));
          }
        tab ("Union: ");
        ACE_DEBUG ((LM_DEBUG, "%C \"%C\"",
                    tc->name (), tc->id () ));
        DynamicAny::DynUnion_var
          value (DynamicAny::DynUnion::_narrow (da));
        if (value->has_no_active_member ())
          {
            ACE_DEBUG ((LM_DEBUG, " No Active member\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "\n"));
            DynamicAny::DynAny_var disc (value->member ());
            if (!CORBA::is_nil (disc.in ()))
              {
                ++level_;
                this->analyze (disc.in ());
                --level_;
              }
          }
      }
      break; // end tk_union

    case CORBA::tk_enum:
      {
        DynamicAny::DynEnum_var
          value (DynamicAny::DynEnum::_narrow (da));

        if (newline)
          {
            ACE_DEBUG ((LM_DEBUG, "\n"));
          }
        tab ("Enum: ");
        ACE_DEBUG ((LM_DEBUG, "%C \"%C\"",
                    tc->name (), tc->id () ));
        CORBA::String_var s (value->get_as_string ());
        ACE_DEBUG ((LM_DEBUG, " value %C\n", s.in()));
      }
      break; // end tk_enum

    case CORBA::tk_sequence:
      {
        if (TAO_DynCommon::is_basic_type_seq (tc.in ()))
          {
            if (newline) ACE_DEBUG ((LM_DEBUG, "\n"));
            this->analyze_basic_seq (dup.in (), da);
          }
        else
          {
            DynamicAny::DynSequence_var
              ds (DynamicAny::DynSequence::_narrow (da));

            int i = 0;
            if (newline)
              {
                ACE_DEBUG ((LM_DEBUG, "\n"));
              }
            tab ("Sequence:\n");
            if (ds->seek (0))
              {
                ++level_;
                do
                  {
                    tab ("");
                    ACE_DEBUG ((LM_DEBUG, "[%03d]\n", i));
                    DynamicAny::DynAny_var
                      cc (ds->current_component ());
                    if (!CORBA::is_nil (cc.in ()))
                      {
                        analyze (cc.in ());
                      }
                    ++i;
                  } while (da->next ());
                --level_;
              }
          }
      }
      break; // end tk_sequence

    case CORBA::tk_array:
      {
        if (newline)
          {
            ACE_DEBUG ((LM_DEBUG, "\n"));
          }
        tab ("Array\n");
        --level_;

        CORBA::ULong const len = dup->length ();
        for (CORBA::ULong i = 0u; i < len; ++i)
          {
            tab ("");
            ACE_DEBUG ((LM_DEBUG, "[%03d]\n", i));
            DynamicAny::DynAny_var
              cc (da->current_component());
            if (!CORBA::is_nil (cc.in ()))
              {
                analyze (cc.in ());
              }
            da->next ();
          }
        --level_;
      }
      break; // end tk_array

    case CORBA::tk_any:
      {
        tab ("Any\n");
        CORBA::Any_var any (da->get_any ());
        DynamicAny::DynAny_var
          dynany (DynAny (any.in ()));
        if (!CORBA::is_nil (dynany.in ()))
          {
            ++level_;
            this->analyze (dynany.in ());
            --level_;
          }
      }
      break; // end tk_any

    CASEE (boolean, Boolean, "(bool) = %d\n");
    CASEE (short, Short, "(short) = %hd\n");
    CASEE (ushort, UShort, "(unsigned short) = %hu\n");
    CASEE (long, Long, "(long) = %d\n");
    CASEE (ulong, ULong, "(unsigned long) = %u\n");
    CASEE (longlong, LongLong, "(longlong) %Ld\n");
    CASEE (ulonglong, ULongLong, "(unsigned longlong) %Lu\n");
    CASEE (char, Char, "(char) = %c\n");
    CASEE (float, Float, "(float) = %f\n");
    CASEE (double, Double, "(double) = %f\n");
    CASEE (octet, Octet, "(octet) = %c\n");

    case CORBA::tk_string:
      {
        CORBA::String_var b (da->get_string ());
        if (!newline)
          {
            tab("");
          }
        ACE_DEBUG ((LM_DEBUG, "(string) = \"%C\"\n",
                    b.in ()                         ));
      }
      break; // end tk_string

    case CORBA::tk_TypeCode:
      {
        const CORBA::TCKind
          kind =da->get_typecode ()->kind ();
        if (!newline)
          {
            tab("");
          }
        ACE_DEBUG ((LM_DEBUG, "(TypeCode) = %d\n",
                    static_cast<int> (kind)       ));
      }
      break; // end tk_TypeCode

    default:
      {
        const CORBA::TCKind
          kind = tc->kind ();
        if (newline)
          {
            ACE_DEBUG ((LM_DEBUG, "\n"));
          }
        tab ("***Unhandled*** TypeCode = ");
        ACE_DEBUG ((LM_DEBUG, "%d\n",
                    static_cast<int> (kind) ));
      }
      break;
    }
}

#define CASEBS(type,CT,str) case CORBA::tk_##type: {\
  CORBA::CT##Seq_var seq (da->get_##type##_seq ());\
  ++level_; CORBA::ULong len = seq->length ();\
  tab ("length = "); ACE_DEBUG ((LM_DEBUG, "%u\n", len));\
  for (CORBA::ULong i = 0; i < len; ++i) {\
    CORBA::CT b = seq[i];\
    tab (""); ACE_DEBUG ((LM_DEBUG, "[%3d] ", i));\
    ACE_DEBUG ((LM_DEBUG, str, b)); }\
  --level_;\
} break;

void
DynAnyAnalyzer::analyze_basic_seq (
  CORBA::TypeCode_ptr tc,
  DynamicAny::DynAny_ptr da)
{
  CORBA::TypeCode_var ct (tc->content_type ());
  CORBA::TCKind tk = ct->kind ();

  tab ("BASIC Type Sequence - ");
  switch (tk)
    {
      CASEBS (boolean, Boolean, "Value (bool) = %d\n");
      CASEBS (octet, Octet, "Value (octet) = %c\n");
      CASEBS (char, Char, "Value (char) = %c\n");
      CASEBS (wchar, WChar, "Value (wchar) = %u\n");
      CASEBS (short, Short, "Value (short) = %d\n");
      CASEBS (ushort, UShort, "Value (ushort) = %u\n");
      CASEBS (long, Long, "Value (long) = %d\n");
      CASEBS (ulong, ULong, "Value (ulong) = %u\n");
      CASEBS (longlong, LongLong, "Value (longlong) = %Ld\n");
      CASEBS (ulonglong, ULongLong, "Value (ulonglong) = %Lu\n");
      CASEBS (float, Float, "Value (float) = %f\n");
      CASEBS (double, Double, "Value (double) = %f\n");

      case CORBA::tk_longdouble:
      default:
        ACE_DEBUG ((LM_DEBUG,
                    "***Unhandled*** TypeCode = %d\n",
                    static_cast<int> (tk)             ));
        break;
    }
}
