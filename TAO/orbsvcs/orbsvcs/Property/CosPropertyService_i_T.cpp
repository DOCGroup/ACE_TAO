#ifndef TAO_COSPROPERTYSERVICE_I_T_C
#define TAO_COSPROPERTYSERVICE_I_T_C

// $Id$
#include "CosPropertyService_i.h"

// Makes default sized hash_table_. All the sequences are set the max
// length as 0.

template <class IF>
TAO_PropertySet<IF>::TAO_PropertySet (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "TAO_PropertySet Constructor-Hash_Table created, current_size %d, total_size %d \n",
              this->hash_table_.current_size (),
              this->hash_table_.total_size ()));
}

// Init values that the PropertySetFactory will want to specify. Write
// the allowed properties in the Hash Table with *fixed-normal* mode.
// @@ Need to check for duplicate properties and raise exceptions.

template <class IF>
TAO_PropertySet<IF>::TAO_PropertySet (const CosPropertyService::PropertyTypes allowed_property_types,
                                  const CosPropertyService::Properties allowed_properties,
                                  CORBA::Environment &TAO_IN_ENV)
  : allowed_property_types_ (allowed_property_types),
    allowed_property_names_ (allowed_properties.length ())
{
  ACE_DEBUG ((LM_DEBUG,
              "TAO_PropertySet Constructor-Hash_Table:-current_size %d, total_size %d \n",
              this->hash_table_.current_size (),
              this->hash_table_.total_size ()));

  // Set the length for the sequence.
  this->allowed_property_names_.length (allowed_properties.length ());

  ACE_DEBUG ((LM_DEBUG,
              "Allowed property types size %d\nAllowed properties size%d\n",
              this->allowed_property_types_.length (),
              allowed_properties.length ()));

  // Keep the allowed property names in the sequence..
  for (size_t ni = 0;
       ni < allowed_properties.length ();
       ni++)
    this->allowed_property_names_[ni] =
      allowed_properties[ni].property_name;

  // Define the allowed properties in the hash table.
  TAO_TRY
    {
      this->define_properties (allowed_properties,
                               TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_PropertySet-Constructor");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

#if 0
  // Keep all the allowed properties in the Hash_Table and keep their
  // names in the allowed_property_names_.
  for (size_t pi = 0; pi < allowed_properties.length (); pi++)
    {
      // Put it in the hash table.
      CosProperty_Hash_Key hash_key (allowed_properties[pi].property_name);
      CosProperty_Hash_Value hash_value (allowed_properties[pi].property_value,
                                         CosPropertyService::fixed_normal);
      CosProperty_Hash_Entry_ptr entry_ptr;
      CosProperty_Hash_Key old_key;
      CosProperty_Hash_Value old_value;

      int ret = this->hash_table_.bind (hash_key,
                                        hash_value,
                                        entry_ptr);

      // @@ Check the return value.

      // Store the property name in the sequence.
      this->allowed_property_names_[pi] = allowed_properties[pi].property_name;
    }
#endif /* 0 */

// TAO_PropertySetDef's constructor needs this, for initializing the
// allowed properties' sequence withe just the maximum length.
template <class IF>
TAO_PropertySet<IF>::TAO_PropertySet (const CosPropertyService::PropertyTypes allowed_property_types,
                                  const CORBA::ULong number_of_allowed_properties,
                                  CORBA::Environment &TAO_IN_ENV)
  : allowed_property_types_ (allowed_property_types),
    allowed_property_names_ (number_of_allowed_properties)
{
  ACE_UNUSED_ARG (TAO_IN_ENV);
}
// PropertySetFactory needs this constructor. Store all the initial
// properies with *normal* modes.

template <class IF>
TAO_PropertySet<IF>::TAO_PropertySet (const CosPropertyService::Properties initial_properties,
                                  CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "TAO_PropertySet Hash_Table:-current_size %d\ntotal_size %d\nInitialProps %d\n",
              this->hash_table_.current_size (),
              this->hash_table_.total_size (),
              initial_properties.length ()));

  // Define all the initial properties in the Property Set. All take
  // *normal* modes.
  TAO_TRY
    {
      this->define_properties (initial_properties,
                               TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_PropertySet-Constructor");
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}
#if 0
  // Store all the initial properties in the Hash Table with *normal* mode.
  for (pi = 0; pi < initial_properties.length (); pi++)
    {
      // Put it in the hash table.
      CosProperty_Hash_Key hash_key (initial_properties[pi].property_name);
      CosProperty_Hash_Value hash_value (initial_properties[pi].property_value,
                                         CosPropertyService::normal);
      CosProperty_Hash_Entry_ptr entry_ptr;
      CosProperty_Hash_Key old_key;
      CosProperty_Hash_Value old_value;

      int ret = this->hash_table_.bind (hash_key,
                                        hash_value,
                                        entry_ptr);
      // @@ Check the return value.
    }
#endif /* 0 */

// Destructor. All sequences will be deleted.
template <class IF>
TAO_PropertySet<IF>::~TAO_PropertySet (void)
{
}

// Function to modify or add a property to the PropertySet alex: Just
// adds the name value to the set... Doesn't check for Typecode
// overwriting, duplicate names, void names etc, yet.  @@ Using Normal
// mode as the default mode of properties, I need to change this based
// on the Initial set of allowed modes, if there is anything like that
// set by the client.

template <class IF>
void
TAO_PropertySet<IF>::define_property (const char *property_name,
                                  const CORBA::Any &property_value,
                                  CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "define_property : property_name %s \n",
              property_name));

  // Check the name's validity.
  if (property_name == 0)
    TAO_THROW (CosPropertyService::InvalidPropertyName());

  // Is this type allowed?
  if (is_type_allowed (property_value.type ()) != 1)
    TAO_THROW (CosPropertyService::UnsupportedTypeCode());

  // Is this property allowed?
  if (is_property_allowed (property_name) != 1)
    TAO_THROW (CosPropertyService::UnsupportedProperty());

  // Try to bind the property. (*normal* mode is used).
  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value (property_value,
                                     CosPropertyService::normal);
  CosProperty_Hash_Entry_ptr entry_ptr;
  CosProperty_Hash_Key old_key;
  CosProperty_Hash_Value old_value;

  int ret = this->hash_table_.bind (hash_key,
                                    hash_value,
                                    entry_ptr);
  switch (ret)
    {
    case 0:
      // Bind succesful.
      ACE_DEBUG ((LM_DEBUG,
                  "define_property: Successful\n"));
      break;
    case 1:
      // Property already exists.

      // Is the pointer valid?
      if (entry_ptr == 0)
        TAO_THROW (CORBA::UNKNOWN ());

      // If type is not the same, raise exception.
      if (entry_ptr->int_id_.pvalue_->type () != property_value.type ())
        TAO_THROW (CosPropertyService::ConflictingProperty());

      // If mode is read only, raise exception.
      if ((entry_ptr->int_id_.pmode_ == CosPropertyService::read_only) ||
          (entry_ptr->int_id_.pmode_ == CosPropertyService::fixed_readonly))
        TAO_THROW (CosPropertyService::ReadOnlyProperty());

      // Use the mode that is already there.
      hash_value.pmode_ = entry_ptr->int_id_.pmode_;

      // Everything is fine. Overwrite the value.
      if (this->hash_table_.rebind (hash_key,
                                    hash_value,
                                    old_key,
                                    old_value) > 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Property Defined\n"));
          break;
        }
    default:
      // Error. ret is -1 or rebid returned other than 1.
      TAO_THROW (CORBA::UNKNOWN ());
    }

  return;
}

// Tells whether this type is allowed in this property set or no.
template <class IF>
CORBA::Boolean
TAO_PropertySet<IF>::is_type_allowed (CORBA::TypeCode_ptr type)
{
  ACE_TRACE ("is_type_allowed\n");

  // If the sequence is empty, no constraints.
  if (this->allowed_property_types_.length () == 0)
    return 1;

  // Check in the allowed_property_types sequence.
  CORBA::Boolean ret_val = 0;
  for (size_t ti = 0;
       ti < this->allowed_property_types_.length ();
       ti++)
    {
      TAO_TRY
        {
          ret_val = ((const CORBA::TypeCode *)this->allowed_property_types_[ti])->equal (type,
                                                                                         TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (ret_val == 1)
            return 1;
        }
      TAO_CATCHANY
        {
          TAO_TRY_ENV.print_exception ("TAO_PropertySet::is_type_allowed");
          return ret_val;
        }
      TAO_ENDTRY;
    }

  // Type not found.
  return ret_val;
}

// Tells whether this property is allowed in this property or no.
template <class IF>
CORBA::Boolean
TAO_PropertySet<IF>::is_property_allowed (const char* property_name)
{
  ACE_TRACE ("is_property_allowed\n");

  // If  the sequence is empty, no constraints.
  if (this->allowed_property_names_.length() == 0)
    return 1;

  // Check in the allowed_property_names.
  for (size_t ni = 0;
       ni < this->allowed_property_names_.length ();
       ni++)
    if ((const char *) this->allowed_property_names_[ni] == property_name)
      return 1;

  // Name not found.
  return 0;
}

// Defining a sequence of properties
//
// Check for overwriting, duplicate names, void names etc and raise
// appropriate exceptions.

template <class IF>
void
TAO_PropertySet<IF>::define_properties (const CosPropertyService::Properties &nproperties,
                                    CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "define_properties\n"));

  // Get the length.
  size_t sequence_length = nproperties.length ();

  // Define multiple exceptions object.
  CosPropertyService::MultipleExceptions multi_ex;

  for (size_t pi = 0; pi < sequence_length; pi++)
    {
      TAO_TRY
        {
          // Define this property.
          this->define_property (nproperties [pi].property_name.in (),
                                 nproperties [pi].property_value,
                                 TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
      TAO_CATCH (CosPropertyService::InvalidPropertyName, ex)
        {
          TAO_TRY_ENV.print_exception ("InvalidPropertyName");
          size_t len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::invalid_property_name;
          multi_ex.exceptions[len].failing_property_name =
            nproperties[pi].property_name;
        }
      TAO_CATCH (CosPropertyService::ConflictingProperty, ex)
        {
          TAO_TRY_ENV.print_exception ("ConflictingProperty");
          size_t len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::conflicting_property;
          multi_ex.exceptions[len].failing_property_name =
            nproperties[pi].property_name;
        }
      TAO_CATCH (CosPropertyService::ReadOnlyProperty, ex)
        {
          TAO_TRY_ENV.print_exception ("ReadOnlyProperty");
          size_t len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::read_only_property;
          multi_ex.exceptions[len].failing_property_name =
            nproperties[pi].property_name;
        }
      TAO_CATCH (CosPropertyService::UnsupportedTypeCode, ex)
        {
          TAO_TRY_ENV.print_exception ("UnsupportedTypeCode");
          size_t len = multi_ex.exceptions.length ();
          multi_ex.exceptions.length (len + 1);
          multi_ex.exceptions[len].reason =
            CosPropertyService::unsupported_type_code;
          multi_ex.exceptions[len].failing_property_name =
            nproperties[pi].property_name;
        }
       TAO_CATCH (CosPropertyService::UnsupportedProperty, ex)
         {
           TAO_TRY_ENV.print_exception ("UnsupportedProperty");
           size_t len = multi_ex.exceptions.length ();
           multi_ex.exceptions.length (len + 1);
           multi_ex.exceptions[len].reason =
             CosPropertyService::unsupported_property;
           multi_ex.exceptions[len].failing_property_name =
             nproperties[pi].property_name;
         }
       TAO_CATCH (CORBA::SystemException, sysex)
         {
           // Print it and throw it again.
           TAO_TRY_ENV.print_exception ("Unknown Exception");
           TAO_RETHROW;
         }
       TAO_ENDTRY;
    }

  // Raise the multi exception if needed.
  if (multi_ex.exceptions.length () > 0)
    TAO_THROW (CosPropertyService::MultipleExceptions (multi_ex));
}

// Returns the current number of properties associated with this
// PropertySet.

template <class IF>
CORBA::ULong
TAO_PropertySet<IF>::get_number_of_properties (CORBA::Environment &TAO_IN_ENV)
{
  ACE_UNUSED_ARG (TAO_IN_ENV);

  return this->hash_table_.current_size ();
}

//  Returns all of the property names currently defined in the
//  PropertySet. If the PropertySet contains more than how_many
//  property names, then the remaining property names are put into the
//  PropertyNamesIterator.

template <class IF>
void
TAO_PropertySet<IF>::get_all_property_names (CORBA::ULong how_many,
                                             CosPropertyService::PropertyNames_out property_names,
                                             CosPropertyService::PropertyNamesIterator_out rest,
                                             CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "get_all_property_names\n"));

  // Allocating storage is a must.
  ACE_NEW (property_names,
           CosPropertyService::PropertyNames);

  size_t num_of_properties =
    this->get_number_of_properties (TAO_IN_ENV);

  if (num_of_properties == 0)
    // Nothing to do.
    return;

  // Set the length of the property_names appropriately.
  CORBA::ULong sequence_length = 0;

  if (how_many > 0)
    {
      if (how_many >= num_of_properties)
        sequence_length = num_of_properties;
      else
        sequence_length = how_many;
      property_names->length (sequence_length);
    }

  // Iterate thru names and put them in the property_names.

  CosProperty_Hash_Entry_ptr entry_ptr;
  CosProperty_Hash_Iterator iterator (this->hash_table_);

  for (size_t ni = 0;
       ni < sequence_length;
       ni++, iterator.advance ())
    if (iterator.next (entry_ptr) != 0)
      property_names [ni] =
        CORBA::string_dup (entry_ptr->ext_id_.pname_.in ());

  // If there are some more properties, put them in the
  // iterator. How?? Make a new PropertySet and use that to create
  // propertyNames Iterator.

  if (num_of_properties > how_many)
    {
      TAO_PropertySet<POA_CosPropertyService::PropertySet> *property_set;

      ACE_NEW (property_set, TAO_PropertySet<POA_CosPropertyService::PropertySet>);

      for (size_t i = how_many;
           i < num_of_properties;
           i++, iterator.advance ())
        if (iterator.next (entry_ptr) != 0)
          if (property_set->hash_table_.bind (entry_ptr->ext_id_,
                                              entry_ptr->int_id_) < 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Err: get_all_property_names\n"));

      // Make the NamesIterator out of this TAO_PropertySet.
      TAO_PropertyNamesIterator *names_iterator;
      ACE_NEW (names_iterator, TAO_PropertyNamesIterator (*property_set));

      rest = CosPropertyService::PropertyNamesIterator::_duplicate (names_iterator->_this (TAO_IN_ENV));
    }
}

// Returns the value of a property in the PropertySet.

template <class IF>
CORBA::Any *
TAO_PropertySet<IF>::get_property_value (const char *property_name,
                                     CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "get_property_value\n"));

  // Check the name's validity.
  if (property_name == 0)
    TAO_THROW_RETURN (CosPropertyService::InvalidPropertyName(),
                      0);

  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Value hash_value;

  if (this->hash_table_.find (hash_key, hash_value) != 0)
    TAO_THROW_RETURN (CosPropertyService::PropertyNotFound(),
                      0);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Find succeeded \n"));

  // Dump the value got.
  CORBA::Any::dump (hash_value.pvalue_.in ());

  // Return the pointer.
  return hash_value.pvalue_._retn ();
}

// Returns the values of the properties listed in property_names. When
// the boolean flag is true, the Properties parameter contains valid
// values for all requested property names. If false, then all
// properties with a value of type tk_void may have failed due to
// PropertyNotFound or InvalidPropertyName.

template <class IF>
CORBA::Boolean
TAO_PropertySet<IF>::get_properties (const CosPropertyService::PropertyNames &property_names,
                                 CosPropertyService::Properties_out nproperties,
                                 CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "get_properties\n"));

  // Allocate memory for the out parameter.
  ACE_NEW_RETURN (nproperties,
                  CosPropertyService::Properties,
                  0);

  // Validate the length.
  size_t n = property_names.length ();
  if (n == 0)
    return 0;

  // Set the length for the out parameter.
  nproperties->length (n);

  CORBA::Any_ptr any_ptr = 0;
  CORBA::Boolean ret_val = 1;

  for (size_t i = 0; i < n; i++)
    {
      TAO_IN_ENV.clear ();
      any_ptr = get_property_value (property_names [i],
                                    TAO_IN_ENV);

      if (any_ptr != 0)
        {
          // Property is found.
          nproperties [i].property_name = property_names [i];
          nproperties [i].property_value = *any_ptr;
          CORBA::Any::dump (nproperties [i].property_value);
        }
      else
        {
          // Invalid name. Ret value is False.
          ret_val = 0;

          // Assign void type to this name in the out parameter.
          nproperties [i].property_name =
            property_names [i];

          // @@ Make an any value with tk_void type. Using replace
          // method, <<= operator doesnot exist yet for this.
          nproperties [i].property_value.replace (CORBA::_tc_void,
                                                  0,
                                                  0,
                                                  TAO_IN_ENV);
          TAO_CHECK_ENV_RETURN (TAO_IN_ENV, 0);
        }
    }

  return ret_val;
}

template <class IF>
void
TAO_PropertySet<IF>::get_all_properties (CORBA::ULong how_many,
                                     CosPropertyService::Properties_out nproperties,
                                     CosPropertyService::PropertiesIterator_out rest,
                                     CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "get_all_properties\n"));

  // Allocate memory for the out parameter.
  ACE_NEW (nproperties,
           CosPropertyService::Properties);

  // Validate the length.
  size_t num_of_properties =
    hash_table_.current_size ();

  if (num_of_properties == 0)
    return;

  // Set the length for the nproperties if how_many > 0.
  CORBA::ULong sequence_length = 0;

  if (how_many > 0)
    {
      if (how_many >= num_of_properties)
        sequence_length = num_of_properties;
      else
        sequence_length = how_many;
      nproperties->length (sequence_length);
    }

  ACE_DEBUG ((LM_DEBUG,
              "PropertySet::get_all_properties -seq-length :%d\n",
              sequence_length));

  // Prepare an iterator and iterate thru the PropertySet. Retrive the
  // values.
  CosProperty_Hash_Iterator iterator (this->hash_table_);
  CosProperty_Hash_Entry_ptr entry_ptr = 0;

  for (CORBA::ULong i = 0;
       i < sequence_length;
       i++, iterator.advance ())
    if (iterator.next (entry_ptr) != 0)
      {
        nproperties[i].property_name =
          CORBA::string_dup (entry_ptr->ext_id_.pname_.in ());
        nproperties[i].property_value =
          entry_ptr->int_id_.pvalue_.in ();
      }

  // If there are more properties, put them in the PropertiesIterator.
  // Make a new TAO_PropertySet and use that to create an Properties
  // iterator.  put that in a iterator and assign that to the out
  // paramerter.

  if (num_of_properties > how_many)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "DB:TAO_PropertySet-Creating PropertySet iterator\n"));
      TAO_PropertySet<POA_CosPropertyService::PropertySet> *prop_set;

      ACE_NEW (prop_set, TAO_PropertySet<POA_CosPropertyService::PropertySet>);

      for (size_t i = sequence_length;
           i < num_of_properties;
           i++, iterator.advance ())
        if (iterator.next (entry_ptr) != 0
            && prop_set->hash_table_.bind (entry_ptr->ext_id_,
                                           entry_ptr->int_id_) < 0)
          ACE_DEBUG ((LM_DEBUG, "Err: get_all_properties\n"));

      ACE_DEBUG ((LM_DEBUG,
                  "DB:New PropertySet created--size %d\n",
                  prop_set->get_number_of_properties (TAO_IN_ENV)));

      // Make the iterator out of the new TAO_Propset.
      TAO_PropertiesIterator *iterator = 0;
      ACE_NEW (iterator,
               TAO_PropertiesIterator (*prop_set));

      rest = CosPropertyService::PropertiesIterator::_duplicate (iterator->_this (TAO_IN_ENV));
      TAO_CHECK_ENV_PRINT_RETURN_VOID (TAO_IN_ENV,
                                       "CosPropertyService::PropertiesIterator::_duplicate");

      ACE_DEBUG ((LM_DEBUG, "DB:TAO_PropSet::get_all_properties-Done\n"));
    }
}

// Deletes the specified property if it exists from a PropertySet.

template <class IF>
void
TAO_PropertySet<IF>::delete_property (const char *property_name,
                                  CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "delete_property\n"));

  // Check the name's validity.
  if (property_name == 0)
    TAO_THROW (CosPropertyService::InvalidPropertyName());

  CosProperty_Hash_Key hash_key (property_name);
  CosProperty_Hash_Entry_ptr entry_ptr = 0;

  // Get the entry_ptr and delete it.
  if (this->hash_table_.find (hash_key,
                              entry_ptr) < 0)
    TAO_THROW (CosPropertyService::PropertyNotFound());

  ACE_DEBUG ((LM_DEBUG, "Property found\n"));

  // If property is fixed, then raise exception.
  if ((entry_ptr->int_id_.pmode_ == CosPropertyService::fixed_normal)
      || (entry_ptr->int_id_.pmode_ == CosPropertyService::fixed_readonly))
    TAO_THROW (CosPropertyService::FixedProperty());

  // Unbind this property.
  if (this->hash_table_.unbind (entry_ptr) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Unbind failed\n"));
      TAO_THROW (CORBA::UNKNOWN ());
    }

  // @@ Purify this and check. Deallocate the memory.
  // delete entry_ptr;

  ACE_DEBUG ((LM_DEBUG, "delete_property done\n"));

  return;
}

// Deletes the properties defined in the property_names
// parameter. This is a batch operation that returns the
// MultipleExceptions exception if any delete failed.

template <class IF>
void
TAO_PropertySet<IF>::delete_properties (const CosPropertyService::PropertyNames &property_names,
                                    CORBA::Environment &TAO_IN_ENV)
{
  ACE_DEBUG ((LM_DEBUG, "delete_properties\n"));

  // Get the length.
  size_t sequence_length = property_names.length ();

  // Declare multiple exceptions' objecct.
  CosPropertyService::MultipleExceptions *multi_ex = 0;
  ACE_NEW (multi_ex,
           CosPropertyService::MultipleExceptions);

  for (size_t pi = 0; pi < sequence_length; pi++)
    {
      TAO_TRY
        {
          // Delete this property.
          this->delete_property (property_names[pi],
                                 TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
      TAO_CATCH (CosPropertyService::InvalidPropertyName, ex)
        {
          TAO_TRY_ENV.print_exception ("InvalidPropertyName");

          // Put this exception in the multiple exception.
          size_t len = multi_ex->exceptions.length ();
          multi_ex->exceptions.length (len + 1);
          multi_ex->exceptions[len].reason =
            CosPropertyService::invalid_property_name;
          multi_ex->exceptions[len].failing_property_name =
            property_names[pi];
        }
      TAO_CATCH (CosPropertyService::PropertyNotFound, ex)
        {
          TAO_TRY_ENV.print_exception ("PropertyNotFound");

          // Put this exception in the multiple exception.
          size_t len = multi_ex->exceptions.length ();
          multi_ex->exceptions.length (len + 1);
          multi_ex->exceptions[len].reason =
            CosPropertyService::property_not_found;
          multi_ex->exceptions[len].failing_property_name =
            property_names[pi];
        }
      TAO_CATCH (CosPropertyService::FixedProperty, ex)
        {
          TAO_TRY_ENV.print_exception ("FixedProperty");

          // Put this exception in the multiple exception.
          size_t len = multi_ex->exceptions.length ();
          multi_ex->exceptions.length (len + 1);
          multi_ex->exceptions[len].reason =
            CosPropertyService::fixed_property;
          multi_ex->exceptions[len].failing_property_name =
            property_names[pi];
        }
      TAO_CATCH (CORBA::SystemException, sysex)
        {
          TAO_TRY_ENV.print_exception ("SystemException");

          // We cant afford to get this. Throw this.
          TAO_RETHROW;
        }
      TAO_ENDTRY;
    }

  // Raise the multiple exceptions if there are any.
  if (multi_ex->exceptions.length () > 0)
    TAO_THROW (CosPropertyService::MultipleExceptions (*multi_ex));

  ACE_DEBUG ((LM_DEBUG, "delete_properties done\n"));

  return;
}

// Delete all the properties in the current ProperySet : Delete the
// properties one by
// one.

template <class IF>
CORBA::Boolean
TAO_PropertySet<IF>::delete_all_properties (CORBA::Environment &TAO_IN_ENV)
{
  ACE_UNUSED_ARG (TAO_IN_ENV);

  TAO_TRY
    {
      // Get all the property names in a names' sequence.
      CosPropertyService::PropertyNames_ptr names_ptr = 0;
      CosPropertyService::PropertyNames_out names_out (names_ptr);
      CosPropertyService::PropertyNames_var names;

      CosPropertyService::PropertyNamesIterator_ptr iter_ptr = 0;
      CosPropertyService::PropertyNamesIterator_out iter_out (iter_ptr);
      CosPropertyService::PropertyNamesIterator_var iter;

      CORBA::ULong size = this->get_number_of_properties (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->get_all_property_names (size,
                                    names_out,
                                    iter_out,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Get the out values on to the var varibles.
      names = names_out.ptr ();
      iter = iter_out.ptr ();

      // Delete all these properties.
      this->delete_properties (names.in (),
                               TAO_TRY_ENV);

      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      // All properties didnt get deleted.
      TAO_TRY_ENV.print_exception ("delete_all_properties");

      return 0;
    }
  TAO_ENDTRY;

  // All properties deleted.
  return 1;
}

// Returns TRUE if the property is defined in the PropertySet.

template <class IF>
CORBA::Boolean
TAO_PropertySet<IF>::is_property_defined (const char *property_name,  CORBA::Environment &TAO_IN_ENV)
{
  ACE_UNUSED_ARG (TAO_IN_ENV);

  ACE_DEBUG ((LM_DEBUG, "is_property_defined\n"));

  CosProperty_Hash_Key hash_key (property_name);

  if (this->hash_table_.find (hash_key) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                "find successfull\n"));
      return 1;
    }
  else
    {
      ACE_DEBUG ((LM_ERROR,
                  "find failed\n"));
      return 0;
    }
}

template <class IF>
void
TAO_PropertySet<IF>::operator= (const TAO_PropertySet<IF> &)
{
  // Empty.
}
#endif /* TAO_COSPROPERTYSERVICE_I_T_C */
