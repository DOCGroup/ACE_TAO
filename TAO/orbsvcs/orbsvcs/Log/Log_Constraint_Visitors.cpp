// $Id$

#include "Log_Constraint_Visitors.h"


TAO_Log_Constraint_Evaluator::
TAO_Log_Constraint_Evaluator (DsLogAdmin::LogRecord &rec)
  :rec_ (rec)
{
  // why do this?
  this->props_.close ();
  this->props_.open ();

  TAO_String_Hash_Key name = (const char*) "id";
  this->props_.bind (name, 0);

  name = (const char*) "time";
  this->props_.bind (name, 1);

}

int
TAO_Log_Constraint_Evaluator::
visit_property (TAO_Property_Constraint* literal)
{
  int return_value = -1, prop_index = 0;
  // Handle case where property is not, in fact, mapped to a value
  TAO_String_Hash_Key prop_name ((const char*) literal->name ());

  if (this->props_.find (prop_name, prop_index) == 0)
    {
      TAO_ENV_DECLARE_NEW_ENV;

      CORBA::Any *value = 0;
      // Retrieve the value of the property from the Property_Evaluator
      ACE_TRY
        {
          // value = this->prop_eval_.property_value (prop_index TAO_ENV_ARG_PARAMETER);
          ACE_NEW_THROW_EX (value,
                            CORBA::Any,
                            CORBA::NO_MEMORY ());
          ACE_TRY_CHECK;

          #if defined (ACE_LACKS_LONGLONG_T)
             if (prop_index == 0)
               *value <<= ACE_U64_TO_U32  (this->rec_.id);
             else
               *value <<= ACE_U64_TO_U32  (this->rec_.time);
          #else

             if (prop_index == 0)
               *value <<= ACE_static_cast (ACE_UINT32, (this->rec_.id));
             else
               *value <<= ACE_static_cast (ACE_UINT32, (this->rec_.time));
          #endif
        }
      ACE_CATCHANY
        {
          return -1;
        }
      ACE_ENDTRY;
      //      ACE_CHECK_RETURN (-1);

      if (value != 0)
        {
          this->queue_.enqueue_head (TAO_Literal_Constraint (value));
          return_value = 0;
        }
    }

  return return_value;
}
