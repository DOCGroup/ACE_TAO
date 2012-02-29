// $Id$

#include "Analyzer.h"

//============= Test main() ============
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      DynAnyAnalyzer analyzer (argc, argv);

      ////////////////////////////////////////////
      // DynValue_Test::BoxedLong
      ////////////////////////////////////////////

      const CORBA::Long l1= -157;

      ACE_DEBUG ((LM_DEBUG,
                  "%N:%l Creating DynValue_Test::BoxedLong\n"));
      DynValue_Test::BoxedLong *p_myBoxedLong =0;
      ACE_NEW_THROW_EX (
        p_myBoxedLong,
        DynValue_Test::BoxedLong (l1),
        CORBA::NO_MEMORY ());
      DynValue_Test::BoxedLong_var myBoxedLong (p_myBoxedLong);
      ACE_DEBUG ((LM_DEBUG,
                  "..%N:%l checking value, long is %d\n",
                  myBoxedLong->_value ()           ));
      if (l1 != myBoxedLong->_value ())
        {
          ACE_DEBUG ((LM_DEBUG,
                      "..%N:%l FAILED should have been %d\n", l1));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Inserting into an any\n"));
      CORBA::Any myAny;
      myAny <<= myBoxedLong.in ();

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from an any\n"));
      DynValue_Test::BoxedLong
        *myBoxedLongExtracted =
          reinterpret_cast<DynValue_Test::BoxedLong *> (9);
      if (!(myAny >>= myBoxedLongExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (!myBoxedLongExtracted)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "..%N:%l FAILED Null pointer extracted\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::BoxedLong *> (9)
          == myBoxedLongExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG,
                  "..%N:%l checking value, long is %d\n",
                  myBoxedLongExtracted->_value () ));
      if (l1 != myBoxedLongExtracted->_value ())
        {
          ACE_DEBUG ((LM_DEBUG,
                      "..%N:%l FAILED should have been %d\n", l1));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Creating dynamic any from any\n"));
      DynamicAny::DynAny_var dany_boxed (analyzer.DynAny (myAny));
      analyzer.analyze (dany_boxed.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing dynamic any for non-null\n"));
      DynamicAny::DynValueCommon *dvc =
        dynamic_cast<DynamicAny::DynValueCommon *> (dany_boxed.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon *\n"));
          return 1;
        }
      if (dvc->is_null ())
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED this dynamic any is NULL!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Creating dynamic any from IDL generated typecode\n"));
      DynamicAny::DynAny_var
        dany (analyzer.DynAny (DynValue_Test::_tc_BoxedLong));
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing typecode generated dynamic any for null\n"));
      dvc= dynamic_cast<DynamicAny::DynValueCommon *> (dany.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon\n"));
          return 1;
        }
      if (!(dvc->is_null ()))
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED this dynamic any has a value!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing for inequality with original dyanamic any\n"));
      if (dany->equal (dany_boxed.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED they test equal before setting shorts\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Setting to value and test for inequality with original dyanamic any\n"));
      dvc->set_to_value ();
      analyzer.analyze (dany.in ());
      if (dany->equal (dany_boxed.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED they test equal before setting long\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Setting the long of the typecode generated dynamic any\n"));
      dany->insert_long (l1);
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing for equality with original dyanamic any\n"));
      if (!(dany->equal (dany_boxed.in ())))
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED they test unequal after setting long\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Setting typecode dynamic any back to NULL\n"));
      dvc->set_to_null ();
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Setting the original dynamic any to NULL object\n"));
      dvc= dynamic_cast<DynamicAny::DynValueCommon *> (dany_boxed.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon *\n"));
          return 1;
        }
      dvc->set_to_null ();
      analyzer.analyze (dany_boxed.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing for equality with typecode dyanamic any\n"));
      if (!(dany->equal (dany_boxed.in ())))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test unequal!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Converting from NULL dynamic any back to any\n"));
      CORBA::Any_var myAny_var (dany_boxed->to_any ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from this any\n"));
      myBoxedLongExtracted=
        reinterpret_cast<DynValue_Test::BoxedLong *> (9);
      if (!(myAny_var.in () >>= myBoxedLongExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::BoxedLong *> (9)
          == myBoxedLongExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }
      if (myBoxedLongExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED non-null pointer extracted\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Setting the dynamic any to VALUE object\n"));
      dvc->set_to_value ();
      analyzer.analyze (dany_boxed.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Resetting Typecode dynamic any from original any\n"));
      dany->from_any (myAny);
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing for equality with original dyanamic any\n"));
      if (!(dany->equal (dany_boxed.in ())))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test unequal!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Converting from dynamic any back to any\n"));
      myAny_var = dany_boxed->to_any ();

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from this any\n"));
      myBoxedLongExtracted=
        reinterpret_cast<DynValue_Test::BoxedLong *> (9);
      if (!(myAny_var.in () >>= myBoxedLongExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (!myBoxedLongExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED null pointer extracted\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::BoxedLong *> (9)
          == myBoxedLongExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG,
                  "..%N:%l checking value, long is %d\n",
                  myBoxedLongExtracted->_value () ));
      if (l1 != myBoxedLongExtracted->_value ())
        {
          ACE_DEBUG ((LM_DEBUG,
                      "..%N:%l FAILED should have been %d\n", l1));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Attempting to get_long from dynamic any\n"));
      CORBA::Long myLongExtracted = dany_boxed->get_long ();
      ACE_DEBUG ((LM_DEBUG,
                  "..%N:%l checking value, long is %d\n",
                  myLongExtracted ));
      if (l1 != myLongExtracted)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "..%N:%l FAILED should have been %d\n", l1));
          return 1;
        }

      ////////////////////////////////////////////
      // DynValue_Test::NestedValue
      ////////////////////////////////////////////

      const CORBA::Short s1= -17;
      const CORBA::Short s2= -23;

      ACE_DEBUG ((LM_DEBUG,
                  "\n%N:%l Creating DynValue_Test::NestedValue\n"));
      DynValue_Test::NestedValue *p_myNestedValue =0;
      ACE_NEW_THROW_EX (
        p_myNestedValue,
        NestedValue_impl(s1, s2),
        CORBA::NO_MEMORY ());
      DynValue_Test::NestedValue_var myNestedValue (p_myNestedValue);
      ACE_DEBUG ((LM_DEBUG,
                  "..%N:%l checking value, shorts are %hd, %hd\n",
                  myNestedValue->Nested_s1 (),
                  myNestedValue->Nested_s2 ()              ));
      if (s1 != myNestedValue->Nested_s1 () ||
          s2 != myNestedValue->Nested_s2 ())
        {
          ACE_DEBUG ((LM_DEBUG,
                      "..%N:%l FAILED should have been %hd, %hd\n",
                      s1, s2));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Inserting into an any\n"));
      myAny <<= myNestedValue.in ();

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from an any\n"));
      DynValue_Test::NestedValue::_ptr_type
        myNestedValueExtracted =
          reinterpret_cast<DynValue_Test::NestedValue::_ptr_type> (9);
      if (!(myAny >>= myNestedValueExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (!myNestedValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "..%N:%l FAILED Null pointer extracted\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::NestedValue::_ptr_type> (9)
          == myNestedValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "..%N:%l checking value, shorts are %hd, %hd\n",
                  myNestedValueExtracted->Nested_s1 (),
                  myNestedValueExtracted->Nested_s2 ()));
      if (s1 != myNestedValueExtracted->Nested_s1 () ||
          s2 != myNestedValueExtracted->Nested_s2 ()   )
        {
          ACE_DEBUG ((LM_DEBUG,
                      "..%N:%l FAILED should have been %hd, %hd\n", s1, s2));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Creating dynamic any from any\n"));
      DynamicAny::DynAny_var dany_nested = analyzer.DynAny (myAny);
      analyzer.analyze (dany_nested.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing dynamic any for non-null\n"));
      dvc = dynamic_cast<DynamicAny::DynValueCommon *>
              (dany_nested.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon *\n"));
          return 1;
        }
      if (dvc->is_null ())
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED this dynamic any is NULL!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Creating dynamic any from IDL generated typecode\n"));
      dany = analyzer.DynAny (DynValue_Test::_tc_NestedValue);
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing typecode generated dynamic any for null\n"));
      dvc= dynamic_cast<DynamicAny::DynValueCommon *> (dany.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon\n"));
          return 1;
        }
      if (!(dvc->is_null ()))
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED this dynamic any has a value!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing for inequality with original dyanamic any\n"));
      if (dany->equal (dany_nested.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED they test equal before setting shorts\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Setting to value and test for inequality with original dyanamic any\n"));
      dvc->set_to_value ();
      analyzer.analyze (dany.in ());
      if (dany->equal (dany_nested.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED they test equal before setting shorts\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Setting the shorts of the typecode generated dynamic any\n"));
      dany->rewind ();
      dany->insert_short (s1);
      if (!(dany->next ()))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED to move on to second short\n"));
          return 1;
        }
      dany->insert_short (s2);
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing for equality with original dyanamic any\n"));
      if (!(dany->equal (dany_nested.in ())))
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED they test unequal after setting shorts\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Setting typecode dynamic any back to NULL\n"));
      dvc->set_to_null ();
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Setting the original dynamic any to NULL object\n"));
      dvc= dynamic_cast<DynamicAny::DynValueCommon *> (dany_nested.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon *\n"));
          return 1;
        }
      dvc->set_to_null ();
      analyzer.analyze (dany_nested.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing for equality with typecode dyanamic any\n"));
      if (!(dany->equal (dany_nested.in ())))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test unequal!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Converting from NULL dynamic any back to any\n"));
      myAny_var = dany_nested->to_any ();

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from this any\n"));
      myNestedValueExtracted=
        reinterpret_cast<DynValue_Test::NestedValue::_ptr_type> (9);
      if (!(myAny_var.in () >>= myNestedValueExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::NestedValue::_ptr_type> (9)
          == myNestedValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }
      if (myNestedValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED non-null pointer extracted\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Setting the dynamic any to VALUE object\n"));
      dvc->set_to_value ();
      analyzer.analyze (dany_nested.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Resetting Typecode dynamic any from original any\n"));
      dany->from_any (myAny);
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing for equality with original dyanamic any\n"));
      if (!(dany->equal (dany_nested.in ())))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test unequal!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Converting from dynamic any back to any\n"));
      myAny_var = dany_nested->to_any ();

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Attempting to extract from this any (without factory)\n"));
      myNestedValueExtracted=
        reinterpret_cast<DynValue_Test::NestedValue::_ptr_type> (9);
      if (myAny_var.in () >>= myNestedValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED the extraction occured without factory\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Attempting to get_val from dynamic any (without factory)\n"));
      try
        {
          myNestedValueExtracted=
            dynamic_cast<DynValue_Test::NestedValue*> (
              dany_nested->get_val ());
          if (myNestedValueExtracted)
            {
              ACE_DEBUG ((LM_DEBUG,
                "..%N:%l FAILED get_val successful without factory\n"));
              return 1;
            }
        }
      catch (const CORBA::MARSHAL &)
        {
          // Expected with no factory registered
        }

      CORBA::ValueFactoryBase *vf= 0;
      ACE_NEW_THROW_EX (vf, NestedValue_Factory, CORBA::NO_MEMORY ());
      analyzer.register_factory (vf);

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Extracting from this any (now with factory)\n"));
      if (!(myAny_var.in () >>= myNestedValueExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED the extraction\n"));
          return 1;
        }
      if (!myNestedValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED Null pointer extracted\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::NestedValue::_ptr_type> (9)
          == myNestedValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "..%N:%l checking value, shorts are %hd, %hd\n",
                  myNestedValueExtracted->Nested_s1 (),
                  myNestedValueExtracted->Nested_s2 ()));
      if (s1 != myNestedValueExtracted->Nested_s1 () ||
          s2 != myNestedValueExtracted->Nested_s2 ()   )
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED should have been %hd, %hd\n", s1, s2));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Attempting to get_val from dynamic any (now with factory)\n"));
      myNestedValueExtracted=
        dynamic_cast<DynValue_Test::NestedValue*> (
          dany_nested->get_val ());
      if (!myNestedValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED get_val unsuccessful even with factory\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "..%N:%l checking value, shorts are %hd, %hd\n",
                  myNestedValueExtracted->Nested_s1 (),
                  myNestedValueExtracted->Nested_s2 ()));
      if (s1 != myNestedValueExtracted->Nested_s1 () ||
          s2 != myNestedValueExtracted->Nested_s2 ()   )
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED should have been %hd, %hd\n", s1, s2));
          return 1;
        }
      myNestedValueExtracted->_remove_ref ();

      ////////////////////////////////////////////
      // DynValue_Test::NullValue
      ////////////////////////////////////////////

      ACE_DEBUG ((LM_DEBUG, "\n%N:%l Creating DynValue_Test::NullValue\n"));
      DynValue_Test::NullValue *p_myNullValue =0;
      ACE_NEW_THROW_EX (p_myNullValue, NullValue_impl, CORBA::NO_MEMORY ());
      DynValue_Test::NullValue_var myNullValue (p_myNullValue);

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Inserting into an any\n"));
      myAny <<= myNullValue.in ();

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from an any\n"));
      DynValue_Test::NullValue::_ptr_type myNullValueExtracted=
        reinterpret_cast<DynValue_Test::NullValue::_ptr_type> (9);
      if (!(myAny >>= myNullValueExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (!myNullValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED Null pointer extracted\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::NullValue::_ptr_type> (9)
          == myNullValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Creating dynamic any from any\n"));
      DynamicAny::DynAny_var dany_null (analyzer.DynAny (myAny));
      analyzer.analyze (dany_null.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing dynamic any for non-null\n"));
      dvc= dynamic_cast<DynamicAny::DynValueCommon *> (dany_null.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon *\n"));
          return 1;
        }
      if (dvc->is_null ())
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED this dynamic any is NULL!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Creating dynamic any from IDL generated typecode\n"));
      dany = analyzer.DynAny (DynValue_Test::_tc_NullValue);
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing typecode generated dynamic any for null\n"));
      dvc= dynamic_cast<DynamicAny::DynValueCommon *> (dany.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon *\n"));
          return 1;
        }
      if (!(dvc->is_null ()))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED this dynamic any has a value!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Testing for inequality with original dyanamic any\n"));
      if (dany->equal (dany_null.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILED they test equal before setting to a value\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG,
        "..%N:%l Setting to value and test for equality with original dyanamic any\n"));
      dvc->set_to_value ();
      analyzer.analyze (dany.in ());
      if (!(dany->equal (dany_null.in ())))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test unequal\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Setting typecode dynamic any back to NULL\n"));
      dvc->set_to_null ();
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Setting the original dynamic any to NULL object\n"));
      dvc= dynamic_cast<DynamicAny::DynValueCommon *> (dany_null.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG,
            "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon\n"));
          return 1;
        }
      dvc->set_to_null ();
      analyzer.analyze (dany_null.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing for equality with typecode dyanamic any\n"));
      if (!(dany->equal (dany_null.in ())))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test unequal!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Converting from NULL dynamic any back to any\n"));
      myAny_var= dany_null->to_any ();

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from this any\n"));
      myNullValueExtracted= reinterpret_cast<DynValue_Test::NullValue::_ptr_type> (9);
      if (!(myAny_var.in () >>= myNullValueExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::NullValue::_ptr_type> (9)
          == myNullValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }
      if (myNullValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED non-null pointer extracted\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Setting the original dynamic any to VALUE object\n"));
      dvc->set_to_value ();
      analyzer.analyze (dany_null.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Resetting Typecode dynamic any from original any\n"));
      dany->from_any (myAny);
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing for equality with original dyanamic any\n"));
      if (!(dany->equal (dany_null.in ())))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test unequal!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Converting from dynamic any back to any\n"));
      myAny_var = dany_null->to_any ();

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Attempting to extract from this any (without factory)\n"));
      myNullValueExtracted= reinterpret_cast<DynValue_Test::NullValue::_ptr_type> (9);
      if (myAny_var.in () >>= myNullValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED the extraction occured without factory\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Attempting to get_val from dynamic any (without factory)\n"));
      try
        {
          myNullValueExtracted= dynamic_cast<DynValue_Test::NullValue*> (dany_null->get_val ());
          if (myNullValueExtracted)
            {
              ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED get_val successful without factory\n"));
              return 1;
            }
        }
      catch (const CORBA::MARSHAL &)
        {
          // Expected with no factory registered
        }

      ACE_NEW_THROW_EX (vf, NullValue_Factory, CORBA::NO_MEMORY ());
      analyzer.register_factory (vf);

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from this any (now with factory)\n"));
      if (!(myAny_var.in () >>= myNullValueExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED the extraction\n"));
          return 1;
        }
      if (!myNullValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED Null pointer extracted\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::NullValue::_ptr_type> (9) == myNullValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Attempting to get_val from dynamic any (now with factory)\n"));
      myNullValueExtracted= dynamic_cast<DynValue_Test::NullValue*> (dany_null->get_val ());
      if (!myNullValueExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED get_val unsuccessful even with factory\n"));
          return 1;
        }
      myNullValueExtracted->_remove_ref ();

      ////////////////////////////////////////////
      // DynValue_Test::Trunc
      ////////////////////////////////////////////

      const CORBA::Short us1= 12345;
      const CORBA::Short us2= 321;

      DynValue_Test::NullValue *p_myNullValue2 =0;
      ACE_NEW_THROW_EX (p_myNullValue2, NullValue_impl, CORBA::NO_MEMORY ());
      DynValue_Test::NullValue_var myNullValue2 (p_myNullValue2);

      ACE_DEBUG ((LM_DEBUG, "\n%N:%l Creating DynValue_Test::Trunc\n"));
      DynValue_Test::Trunc *p_myTrunc =0;
      ACE_NEW_THROW_EX (
        p_myTrunc,
        Trunc_impl (
          us1,
          us2,
          myBoxedLong.in (),
          dynamic_cast<NestedValue_impl *> (myNestedValue.in ()),
          dynamic_cast<NullValue_impl *> (myNullValue.in ()),
          dynamic_cast<NullValue_impl *> (myNullValue.in ()),
          dynamic_cast<NullValue_impl *> (myNullValue2.in ())     ),
        CORBA::NO_MEMORY ());
      DynValue_Test::Trunc_var myTrunc (p_myTrunc);

      ACE_DEBUG ((LM_DEBUG, "..%N:%l checking embedded values, "));
      myTrunc->print ();
      if (us1 != myTrunc.in ()->Base_us1 ()                 ||
          us2 != myTrunc.in ()->Base_us2 ()                 ||
          l1 != myTrunc.in ()->Trunc_bl()->_value ()        ||
          s1 != myTrunc.in ()->Trunc_nested()->Nested_s1 () ||
          s2 != myTrunc.in ()->Trunc_nested()->Nested_s2 ()   )
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED should have been %u, %u, %d and %d, %d\n", us1, us2, l1, s1, s2));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Inserting into an any\n"));
      myAny <<= myTrunc.in ();

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from an any\n"));
      DynValue_Test::Trunc::_ptr_type myTruncExtracted=
        reinterpret_cast<DynValue_Test::Trunc::_ptr_type> (9);
      if (!(myAny >>= myTruncExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (!myTruncExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED Null pointer extracted\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "..%N:%l checking embedded values, "));
      myTruncExtracted->print ();
      if (us1 != myTruncExtracted->Base_us1 ()                 ||
          us2 != myTruncExtracted->Base_us2 ()                 ||
          l1 != myTruncExtracted->Trunc_bl()->_value ()        ||
          s1 != myTruncExtracted->Trunc_nested()->Nested_s1 () ||
          s2 != myTruncExtracted->Trunc_nested()->Nested_s2 ()   )
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED should have been %u, %u, %d and %d, %d\n", us1, us2, l1, s1, s2));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Creating dynamic any from any\n"));
      DynamicAny::DynAny_var dany_trunc (analyzer.DynAny (myAny));
      analyzer.analyze (dany_trunc.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing dynamic any for non-null\n"));
      dvc= dynamic_cast<DynamicAny::DynValueCommon *> (dany_trunc.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon *\n"));
          return 1;
        }
      if (dvc->is_null ())
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED this dynamic any is NULL!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Creating dynamic any from IDL generated typecode DynValue_Test::_tc_Trunc\n"));
      dany = analyzer.DynAny (DynValue_Test::_tc_Trunc);
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing typecode generated dynamic any for null\n"));
      dvc= dynamic_cast<DynamicAny::DynValueCommon *> (dany.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon *\n"));
          return 1;
        }
      if (!(dvc->is_null ()))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED this dynamic any has a value!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing for inequality with original dyanamic any\n"));
      if (dany->equal (dany_trunc.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test equal before setting shorts\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Setting to value and testing for inequality with original dyanamic any\n"));
      dvc->set_to_value ();
      analyzer.analyze (dany.in ());
      if (dany->equal (dany_trunc.in ()))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test equal before setting members\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Setting the members of the typecode generated dynamic any\n"));
      dany->rewind ();
      dany->insert_ushort (us1);
      if (!(dany->next ()))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED to move on to 2nd member\n"));
          return 1;
        }
      dany->insert_ushort (us2);
      if (!(dany->next ()))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED to move on to 3rd member\n"));
          return 1;
        }
      DynamicAny::DynValueCommon_var
        dmem (dynamic_cast<DynamicAny::DynValueCommon *> (dany->current_component ()));
      if (!dmem.in())
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILURE can not obtain 3rd current_component ()\n"));
          return 1;
        }
      dmem->set_to_value ();
      dmem->insert_long (l1);
      if (!(dany->next ()))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED to move on to 4th member\n"));
          return 1;
        }
      dmem= dynamic_cast<DynamicAny::DynValueCommon *> (dany->current_component ());
      if (!dmem.in())
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILURE can not obtain 4th current_component ()\n"));
          return 1;
        }
      dmem->set_to_value ();
      dmem->insert_val (myNestedValue.in ());
      if (!(dany->next ()))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED to move on to 5th member\n"));
          return 1;
        }
      dmem= dynamic_cast<DynamicAny::DynValueCommon *> (dany->current_component ());
      if (!dmem.in())
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILURE can not obtain 5th current_component ()\n"));
          return 1;
        }
      dmem->set_to_value ();
      dmem->insert_val (myNullValue.in ());
      if (!(dany->next ()))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED to move on to 6th member\n"));
          return 1;
        }
      dmem= dynamic_cast<DynamicAny::DynValueCommon *> (dany->current_component ());
      if (!dmem.in())
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILURE can not obtain 6th current_component ()\n"));
          return 1;
        }
      dmem->set_to_value ();
      dmem->insert_val (myNullValue.in ());
      if (!(dany->next ()))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED to move on to 7th member\n"));
          return 1;
        }
      dmem= dynamic_cast<DynamicAny::DynValueCommon *> (dany->current_component ());
      if (!dmem.in())
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILURE can not obtain 7th current_component ()\n"));
          return 1;
        }
      dmem->set_to_value ();
      dmem->insert_val (myNullValue2.in ());
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing for equality with original dyanamic any\n"));
      if (!(dany->equal (dany_trunc.in ())))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test unequal after setting members\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Setting typecode dynamic any back to NULL\n"));
      dvc->set_to_null ();
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Setting the original dynamic any to NULL object\n"));
      dvc = dynamic_cast<DynamicAny::DynValueCommon *> (dany_trunc.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon *\n"));
          return 1;
        }
      dvc->set_to_null ();
      analyzer.analyze (dany_trunc.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing for equality with typecode dyanamic any\n"));
      if (!(dany->equal (dany_trunc.in ())))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test unequal!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Converting from NULL dynamic any back to any\n"));
      myAny_var= dany_trunc->to_any ();

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from this any\n"));
      myTruncExtracted= reinterpret_cast<DynValue_Test::Trunc::_ptr_type> (9);
      if (!(myAny_var.in () >>= myTruncExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::Trunc::_ptr_type> (9) == myTruncExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }
      if (myTruncExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED non-null pointer extracted\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Setting the dynamic any to VALUE object\n"));
      dvc->set_to_value ();
      analyzer.analyze (dany_trunc.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Resetting Typecode dynamic any from original any\n"));
      dany->from_any (myAny);
      analyzer.analyze (dany.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing for equality with original dyanamic any\n"));
      if (!(dany->equal (dany_trunc.in ())))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED they test unequal!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Converting from dynamic any back to any\n"));
      myAny_var = dany_trunc->to_any ();

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Attempting to extract from this any (without factory)\n"));
      myTruncExtracted= reinterpret_cast<DynValue_Test::Trunc::_ptr_type> (9);
      if (myAny_var.in () >>= myTruncExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED the extraction occured without factory\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Attempting to get_val from dynamic any (without factory)\n"));
      try
        {
          myTruncExtracted= dynamic_cast<DynValue_Test::Trunc *> (dany_trunc->get_val ());
          if (myTruncExtracted)
            {
              ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED get_val successful without factory\n"));
              return 1;
            }
        }
      catch (const CORBA::MARSHAL &)
        {
          // Expected with no factory registered
        }

      ACE_NEW_THROW_EX (vf, Trunc_Factory, CORBA::NO_MEMORY ());
      analyzer.register_factory (vf);

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from this any (now with factory)\n"));
      if (!(myAny_var.in () >>= myTruncExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (!myTruncExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED Null pointer extracted\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::Trunc::_ptr_type> (9) == myTruncExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "..%N:%l checking embedded values, "));
      myTruncExtracted->print ();
      if (us1 != myTruncExtracted->Base_us1 ()                 ||
          us2 != myTruncExtracted->Base_us2 ()                 ||
          l1 != myTruncExtracted->Trunc_bl()->_value ()        ||
          s1 != myTruncExtracted->Trunc_nested()->Nested_s1 () ||
          s2 != myTruncExtracted->Trunc_nested()->Nested_s2 ()   )
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED should have been %u, %u, %d and %d, %d\n", us1, us2, l1, s1, s2));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Attempting to get_val from dynamic any (now with factory)\n"));
      myTruncExtracted= dynamic_cast<DynValue_Test::Trunc*> (dany_trunc->get_val ());
      if (!myTruncExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED get_val unsuccessful even with factory\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "..%N:%l checking embedded values, "));
      myTruncExtracted->print ();
      if (us1 != myTruncExtracted->Base_us1 ()                 ||
          us2 != myTruncExtracted->Base_us2 ()                 ||
          l1 != myTruncExtracted->Trunc_bl()->_value ()        ||
          s1 != myTruncExtracted->Trunc_nested()->Nested_s1 () ||
          s2 != myTruncExtracted->Trunc_nested()->Nested_s2 ()   )
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED should have been %u, %u, %d and %d, %d\n", us1, us2, l1, s1, s2));
          return 1;
        }
      myTruncExtracted->_remove_ref ();

      ///////////////////////////////////////////////////
      // DynValue_Test::Trunc -> DynValue_Test::BaseValue
      ///////////////////////////////////////////////////

      ACE_DEBUG ((LM_DEBUG, "\n%N:%l Placing DynValue_Test::Trunc into DynValue_Test::BaseValue\n"));
      DynValue_Test::BaseValue::_ptr_type p_myBaseValue = myTrunc.in ();

      ACE_DEBUG ((LM_DEBUG, "..%N:%l checking values, "));
      p_myBaseValue->print ();
      if (us1 != p_myBaseValue->Base_us1 () ||
          us2 != p_myBaseValue->Base_us2 ()    )
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED should have been %u and %u\n", us1, us2));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Inserting into an any\n"));
      CORBA::Any myAny_trunc;
      myAny_trunc <<= p_myBaseValue;
      ACE_DEBUG ((LM_DEBUG, "..%N:%l any reports type %C\n", myAny_trunc._tao_get_typecode ()->id ()));
      myAny_trunc.type (DynValue_Test::_tc_BaseValue);
      ACE_DEBUG ((LM_DEBUG, "..%N:%l any NOW reports type %C\n", myAny_trunc._tao_get_typecode ()->id ()));

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting BaseValue from an any\n"));
      DynValue_Test::BaseValue::_ptr_type myBaseExtracted=
        reinterpret_cast<DynValue_Test::BaseValue::_ptr_type> (9);
      if (!(myAny_trunc >>= myBaseExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (!myBaseExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED Null pointer extracted\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "..%N:%l checking embedded values, "));
      myBaseExtracted->print ();
      if (us1 != myBaseExtracted->Base_us1 () ||
          us2 != myBaseExtracted->Base_us2 ()    )
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED should have been %u and %u\n", us1, us2));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Creating dynamic any from any\n"));
      DynamicAny::DynAny_var dany_base (analyzer.DynAny (myAny_trunc));
      analyzer.analyze (dany_base.in ());

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Testing dynamic any for non-null\n"));
      dvc= dynamic_cast<DynamicAny::DynValueCommon *> (dany_base.in ());
      if (!dvc)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILURE can not obtain DynamicAny::DynValueCommon *\n"));
          return 1;
        }
      if (dvc->is_null ())
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED this dynamic any is NULL!\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Converting from dynamic any back to any\n"));
      myAny_var = dany_base->to_any ();


      ACE_DEBUG ((LM_DEBUG, "..%N:%l Attempting to extract from this any (without factory)\n"));
      myBaseExtracted= reinterpret_cast<DynValue_Test::BaseValue::_ptr_type> (9);
      if (myAny_var.in () >>= myBaseExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED the extraction occured without factory\n"));
          return 1;
        }

      ACE_NEW_THROW_EX (vf, BaseValue_Factory, CORBA::NO_MEMORY ());
      analyzer.register_factory (vf);

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Extracting from this any (now with factory)\n"));
      if (!(myAny_var.in () >>= myBaseExtracted))
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED extraction\n"));
          return 1;
        }
      if (!myBaseExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED Null pointer extracted\n"));
          return 1;
        }
      if (reinterpret_cast<DynValue_Test::BaseValue::_ptr_type> (9) == myBaseExtracted)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED pointer NOT extracted\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG, "..%N:%l checking embedded values, "));
      myBaseExtracted->print ();
      if (us1 != myBaseExtracted->Base_us1 () ||
          us2 != myBaseExtracted->Base_us2 ()   )
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED should have been %u, and %u\n", us1, us2));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "..%N:%l Creating dynamic any from truncted any (no truncation allowed)\n"));
      try
        {
          DynamicAny::DynAny_var dany_base (analyzer.DynAny_NoTrunc (myAny_trunc));
          ACE_DEBUG ((LM_DEBUG, "..%N:%l FAILED, obtained Dynamic any\n"));
          analyzer.analyze (dany_base.in ());
          return 1;
        }
      catch (const DynamicAny::MustTruncate &)
        {
          ACE_DEBUG ((LM_DEBUG, "..%N:%l obtained MustTruncate as expected\n"));
        }

      ACE_DEBUG ((LM_DEBUG, "\nAll tests passed\n"));
    }
  catch (const CORBA::TRANSIENT &)
    {
      ACE_DEBUG ((LM_INFO, "Transient\n"));
    }
  catch (const CORBA::COMM_FAILURE &)
    {
      ACE_DEBUG ((LM_INFO, "COMM_FAILURE\n"));
    }
  catch (const CORBA::Exception & ex)
    {
      ex._tao_print_exception ("Tests failed with CORBA exception:");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Test failed with NON-CORBA exception\n"), 1);
    }

  return 0;
}
