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

#define CASEE(type,CT,str) case CORBA::tk_##type: {\
  CORBA::CT b = da->get_##type() ; \
  tab(level_);\
  if (debug_) \
    ACE_DEBUG ((LM_DEBUG, str , b)); \
  } break;

DynAnyAnalyzer::DynAnyAnalyzer(CORBA::ORB_ptr orb, DynamicAny::DynAnyFactory_ptr dynany_factory, int debug)
  : orb_ (CORBA::ORB::_duplicate(orb)),
    dynany_factory_ (DynamicAny::DynAnyFactory::_duplicate(dynany_factory)),
    level_ (0),
    debug_ (debug)
{
}

void DynAnyAnalyzer::tab(int t)
{
  if (debug_)
    {
      for( int i = 0 ; i < t ; i++ )
          ACE_DEBUG ((LM_DEBUG,
                      "\t"));
    }
}

DynAnyAnalyzer::~DynAnyAnalyzer()
{
}

void DynAnyAnalyzer::resetTab()
{
  level_ = 0;
}

void DynAnyAnalyzer::analyze (DynamicAny::DynAny_ptr da ACE_ENV_ARG_DECL)
{
  CORBA::TypeCode_var tc = da->type();

  // strip aliases
  while( tc->kind() == CORBA::tk_alias )
    {
      tc = tc->content_type();
    }

  switch( tc->kind() )
   {
     case CORBA::tk_struct :
      {
        DynamicAny::DynStruct_var ds
          = DynamicAny::DynStruct::_narrow(da ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        tab(level_);

        if (debug_)
          ACE_DEBUG ((LM_DEBUG,
                     "STRUCT\n"));

        if( da->seek(0) )
          {
            level_++;
            do
              {
                DynamicAny::DynAny_var cc    = ds->current_component(ACE_ENV_SINGLE_ARG_PARAMETER);
                ACE_CHECK;

                DynamicAny::FieldName_var fn = ds->current_member_name(ACE_ENV_SINGLE_ARG_PARAMETER);
                ACE_CHECK;

                tab(level_);

                if (debug_)
                  ACE_DEBUG ((LM_DEBUG,
                             "Member = %s\n", fn.in()));

                if (!CORBA::is_nil (cc.in ()))
                  {
                    this->analyze (cc.in() ACE_ENV_ARG_PARAMETER);
                    ACE_CHECK;
                  }

              } while( da->next() );
            level_--;
          }
      }
      break; // end tk_struct

     case CORBA::tk_sequence:
       {
         DynamicAny::DynSequence_var ds
           = DynamicAny::DynSequence::_narrow(da ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        int i = 0;

        tab(level_);

        if (debug_)
          ACE_DEBUG ((LM_DEBUG,
                      "SEQUENCE\n"));

        if( ds->seek(0) )
          {
            level_++;
            do
              {
                tab(level_);

                if (debug_)
                  ACE_DEBUG ((LM_DEBUG,
                              "[%d]\n", i));

                DynamicAny::DynAny_var cc( ds->current_component() );
                ACE_CHECK;

                if (!CORBA::is_nil (cc.in ()))
                  {
                    analyze(cc.in() ACE_ENV_ARG_PARAMETER);
                    ACE_CHECK;
                  }

                i++;
              } while( da->next() );
            level_--;
         }
       }
       break; // end tk_sequence

     case CORBA::tk_array:
       {
          tab(level_);

          if (debug_)
            ACE_DEBUG ((LM_DEBUG,
                       "ARRAY\n"));

          level_++;
          for( unsigned int i = 0 ; i < tc->length() ; i++ )
            {
              tab(level_);

              if (debug_)
                ACE_DEBUG ((LM_DEBUG,
                            "[%d]\n", i));

              DynamicAny::DynAny_var cc = da->current_component();

              if (!CORBA::is_nil (cc.in ()))
                {
                  analyze(cc.in() ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK;
                }

              da->next();
            }
          level_--;
       }
       break;

     case CORBA::tk_union:
       {
         DynamicAny::DynUnion_var value
           = DynamicAny::DynUnion::_narrow(da ACE_ENV_ARG_PARAMETER);
         ACE_CHECK;

         if( !value->has_no_active_member() )
           {
             DynamicAny::DynAny_var disc = value->member();

              if (!CORBA::is_nil (disc.in ()))
                {
                  this->analyze(disc.in() ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK;
                }
           }
       }
       break;

     case CORBA::tk_any:
       {
         DynamicAny::DynAny_var dynany;
         CORBA::Any_var any = da->get_any();

         dynany = dynany_factory_->create_dyn_any(any.in() ACE_ENV_ARG_PARAMETER);
         ACE_CHECK

         if (!CORBA::is_nil (dynany.in ()))
           {
             this->analyze(dynany.in() ACE_ENV_ARG_PARAMETER);
             ACE_CHECK;
           }

         dynany->destroy();
       }
       break;

     case CORBA::tk_enum:
       {
         DynamicAny::DynEnum_var value
           = DynamicAny::DynEnum::_narrow(da ACE_ENV_ARG_PARAMETER);
         ACE_CHECK;

          CORBA::String_var s = value->get_as_string();
          tab(level_);

          if (debug_)
            ACE_DEBUG ((LM_DEBUG,
                       "  Value (enum) = %s\n", s.in()));
       }
       break;

     CASEE(boolean,Boolean,"  Value (bool) = %d\n");
     CASEE(short,Short,"  Value (short) = %d\n");
     CASEE(ushort,UShort,"  Value (ushort) = %u\n");
     CASEE(long,Long,"  Value (long) = %d\n");
     CASEE(ulong,ULong,"  Value (ulong) = %u\n");
     CASEE(longlong,LongLong,"  Value (longlong) %Ld\n");
     CASEE(ulonglong,ULongLong,"  Value (ulonglong) %Lu\n");
     CASEE(char,Char,"  Value (char) = %c\n");
     CASEE(float,Float,"  Value (float) = %f\n");
     CASEE(double,Double,"  Value (double) = %f\n");
     CASEE(octet,Octet,"  Value (octet) = %c\n");

     case CORBA::tk_string:
       {
         CORBA::String_var b( da->get_string() );

         tab(level_);
         if (debug_)
           ACE_DEBUG ((LM_DEBUG,
                      "  Value (string) = %s\n", b.in()));
       }
       break;

     case CORBA::tk_TypeCode:
       {
         tab(level_);
         if (debug_) \
           ACE_DEBUG ((LM_DEBUG,
                       "  Value (TypeCode) = %d\n" , da->get_typecode()->kind()));
       }
       break;

     default:
       {
         tab(level_);
         if (debug_)
           ACE_DEBUG ((LM_DEBUG,
                      "  unhandled typecode = %d\n", (int) tc->kind()));
       }
       break;
   }

   if (debug_)
     ACE_DEBUG ((LM_DEBUG,
                 "\n"));
}
