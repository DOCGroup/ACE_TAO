// -*- C++ -*-

//=============================================================================
/**
 *  @file    analyzer.cpp
 *
 *  $Id$
 *
 *  This file contains a DynAny analyzer that dumps the contents of a DynAny
 *  to the logging.
 *
 *  @author  Johnny Willemsen  (jwillemsen@remedy.nl)
 */
//=============================================================================

#include "analyzer.h"

#include "tao/DynamicAny/DynCommon.h"
#include "tao/DynamicAny/DynAnyFactory.h"

#define CASEE(type,CT,str) case CORBA::tk_##type: {\
  CORBA::CT b = da->get_##type() ; \
  tab (level_); \
  if (debug_) \
    ACE_DEBUG ((LM_DEBUG, str , b)); \
  } break;

#define CASEBS(type,CT,str) case CORBA::tk_##type: \
  {\
    CORBA::CT##Seq_var seq = da->get_##type##_seq (); \
    ++level_; \
    CORBA::ULong len = seq->length (); \
    tab (level_); \
    if (debug_) \
      { \
        ACE_DEBUG ((LM_DEBUG, "length = %u\n", len)); \
      } \
    for (CORBA::ULong i = 0; i < len; ++i) \
      { \
        CORBA::CT b = seq[i]; \
        tab (level_); \
        if (debug_) \
          { \
            ACE_DEBUG ((LM_DEBUG, "[%d]\n", i)); \
            ACE_DEBUG ((LM_DEBUG, str, b)); \
          } \
      } \
    --level_; \
  } \
  break;

DynAnyAnalyzer::DynAnyAnalyzer (CORBA::ORB_ptr orb,
                                DynamicAny::DynAnyFactory_ptr dynany_factory,
                                int debug)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    dynany_factory_ (DynamicAny::DynAnyFactory::_duplicate (dynany_factory)),
    level_ (0),
    debug_ (debug)
{
}

void DynAnyAnalyzer::tab (int t)
{
  if (debug_)
    {
      for (int i = 0 ; i < t ; ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "\t"));
        }
    }
}

DynAnyAnalyzer::~DynAnyAnalyzer (void)
{
}

void DynAnyAnalyzer::resetTab (void)
{
  level_ = 0;
}

void DynAnyAnalyzer::analyze (DynamicAny::DynAny_ptr da)
{
  CORBA::TypeCode_var tc = da->type ();

  CORBA::TCKind kind = tc->kind ();

  CORBA::TypeCode_var dup = CORBA::TypeCode::_duplicate (tc.in ());

  // strip aliases
  while (kind == CORBA::tk_alias)
    {
      dup = dup->content_type ();

      kind = dup->kind ();
    }

  switch (kind)
   {
     case CORBA::tk_struct:
      {
        DynamicAny::DynStruct_var ds
          = DynamicAny::DynStruct::_narrow (da);

        tab (level_);

        if (debug_)
          {
            ACE_DEBUG ((LM_DEBUG,
                      "STRUCT\n"));
          }

        if (da->seek (0) )
          {
            level_++;

            do
              {
                DynamicAny::DynAny_var cc =
                  ds->current_component ();

                DynamicAny::FieldName_var fn =
                  ds->current_member_name ();

                tab(level_);

                if (debug_)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                              "Member = %s\n", fn.in ()));
                  }

                if (!CORBA::is_nil (cc.in ()))
                  {
                    this->analyze (cc.in ());
                  }

              } while (da->next ());

            level_--;
          }
      }
      break; // end tk_struct

    case CORBA::tk_sequence:
      {
        if (TAO_DynCommon::is_basic_type_seq (tc.in ()))
          {
            this->analyze_basic_seq (dup.in (), da);
          }
        else
          {
            DynamicAny::DynSequence_var ds =
              DynamicAny::DynSequence::_narrow (da);

            int i = 0;
            tab(level_);

            if (debug_)
              {
                ACE_DEBUG ((LM_DEBUG,
                            "SEQUENCE\n"));
              }

            if (ds->seek (0UL))
              {
                level_++;

                do
                  {
                    tab(level_);

                    if (debug_)
                      ACE_DEBUG ((LM_DEBUG,
                                  "[%d]\n", i));

                    DynamicAny::DynAny_var cc (ds->current_component ());

                    if (!CORBA::is_nil (cc.in ()))
                      {
                        analyze (cc.in ());
                      }

                    i++;
                  } while (da->next ());

                level_--;
              }
          }
      }
      break; // end tk_sequence

    case CORBA::tk_array:
      {
        tab (level_);

        if (debug_)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "ARRAY\n"));
          }

        level_++;

        CORBA::ULong const len =
          dup->length ();

        for (CORBA::ULong i = 0 ; i < len; ++i)
          {
            tab (level_);

            if (debug_)
              {
                ACE_DEBUG ((LM_DEBUG, "[%d]\n", i));
              }

            DynamicAny::DynAny_var cc = da->current_component();

            if (!CORBA::is_nil (cc.in ()))
              {
                analyze (cc.in ());
              }

            da->next ();
          }
        level_--;
      }
      break;

    case CORBA::tk_union:
      {
        DynamicAny::DynUnion_var value =
          DynamicAny::DynUnion::_narrow (da);

        if (!value->has_no_active_member ())
          {
            DynamicAny::DynAny_var disc = value->member ();

            if (!CORBA::is_nil (disc.in ()))
              {
                this->analyze (disc.in ());
              }
          }
      }
      break;

    case CORBA::tk_any:
      {
        DynamicAny::DynAny_var dynany;
        CORBA::Any_var any = da->get_any ();

        dynany =
          dynany_factory_->create_dyn_any (any.in ());

        if (!CORBA::is_nil (dynany.in ()))
          {
            this->analyze (dynany.in ());
          }

        dynany->destroy ();
      }
      break;

    case CORBA::tk_enum:
      {
        DynamicAny::DynEnum_var value =
          DynamicAny::DynEnum::_narrow (da);

        CORBA::String_var s = value->get_as_string ();
        tab (level_);

        if (debug_)
          {
            ACE_DEBUG ((LM_DEBUG, "  Value (enum) = %s\n", s.in()));
          }
      }
      break;

    CASEE (boolean, Boolean, "  Value (bool) = %d\n");
    CASEE (short, Short, "  Value (short) = %d\n");
    CASEE (ushort, UShort, "  Value (ushort) = %u\n");
    CASEE (long, Long, "  Value (long) = %d\n");
    CASEE (ulong, ULong, "  Value (ulong) = %u\n");
    CASEE (longlong, LongLong, "  Value (longlong) %Ld\n");
    CASEE (ulonglong, ULongLong, "  Value (ulonglong) %Lu\n");
    CASEE (char, Char, "  Value (char) = %c\n");
    CASEE (float, Float, "  Value (float) = %f\n");
    CASEE (double, Double, "  Value (double) = %f\n");
    CASEE (octet, Octet, "  Value (octet) = %c\n");

    case CORBA::tk_string:
      {
        CORBA::String_var b (da->get_string ());
        tab (level_);

        if (debug_)
          {
            ACE_DEBUG ((LM_DEBUG, "  Value (string) = %s\n", b.in ()));
          }
      }
      break;

    case CORBA::tk_TypeCode:
      {
        tab (level_);

        if (debug_)
          {
            CORBA::TCKind const kind =
              da->get_typecode ()->kind ();

            ACE_DEBUG ((LM_DEBUG,
                        "  Value (TypeCode) = %d\n",
                        static_cast<int> (kind)));
          }
      }
      break;

    default:
      {
        tab (level_);

        if (debug_)
          {
            CORBA::TCKind const kind =
              tc->kind ();

            ACE_DEBUG ((LM_DEBUG,
                        "  unhandled typecode = %d\n",
                        static_cast<int> (kind)));
          }
      }
      break;
  }

  if (debug_)
    {
      ACE_DEBUG ((LM_DEBUG, "\n"));
    }
}

void
DynAnyAnalyzer::analyze_basic_seq (CORBA::TypeCode_ptr tc,
                                   DynamicAny::DynAny_ptr da)
{
  CORBA::TypeCode_var ct = tc->content_type ();
  CORBA::TCKind tk = ct->kind ();

  tab (level_);

  if (debug_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "BASIC TYPE SEQUENCE\n"));
    }

  switch (tk)
    {
      CASEBS (boolean, Boolean, "  Value (bool) = %d\n");
      CASEBS (octet, Octet, "  Value (octet) = %c\n");
      CASEBS (char, Char, "  Value (char) = %c\n");
      CASEBS (wchar, WChar, "  Value (wchar) = %u\n");
      CASEBS (short, Short, "  Value (short) = %d\n");
      CASEBS (ushort, UShort, "  Value (ushort) = %u\n");
      CASEBS (long, Long, "  Value (long) = %d\n");
      CASEBS (ulong, ULong, "  Value (ulong) = %u\n");
      CASEBS (longlong, LongLong, "  Value (longlong) = %Ld\n");
      CASEBS (ulonglong, ULongLong, "  Value (ulonglong) = %Lu\n");
      CASEBS (float, Float, "  Value (float) = %f\n");
      CASEBS (double, Double, "  Value (double) = %f\n");
      case CORBA::tk_longdouble:
      default:
        tab (level_);

        if (debug_)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "  unhandled typecode = %d\n",
                        static_cast<int> (tk)));
          }

        break;
    }
}
