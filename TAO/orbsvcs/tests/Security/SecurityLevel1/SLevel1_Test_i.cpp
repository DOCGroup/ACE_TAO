#include "SLevel1_Test_i.h"

ACE_RCSID (SecurityLevel1,
           SLevel1_Test_i,
           "$Id$")

SLevel1_Server_i::SLevel1_Server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    ss_current_ ()
{
}

void
SLevel1_Server_i::authorize_level1 (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  /* All the code concerned with the SecurityCurrent should be either
     here or could be in an interceptor. To make life easy, I did it
     here. It should be in one of these places since the Current
     object is thread specific */

  // Get a reference to the SecurityCurrent object.
  if (CORBA::is_nil (this->ss_current_.in ()))
    {
      CORBA::Object_var obj =
        this->orb_->resolve_initial_references ("SecurityCurrent",
                                                ACE_TRY_ENV);
      ACE_CHECK;

      this->ss_current_ =
        SecurityLevel1::Current::_narrow (obj.in (), ACE_TRY_ENV);
      ACE_CHECK;

      // Check for nil reference.
      if (CORBA::is_nil (current.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      " (%P|%t) Unable to initialize the "
                      "SecurityCurrent object\n"));

          return;
        }
    }

  Security::AttributeType desired_attribute;

  /// @@ Need to check more about this variable
  desired_attribute.attribute_family.family_definer = 0;

  /// Implies Privilege Attributes
  desired_attribute.attribute_family.family = 1;

  ///  AccessId: the identity of the principal used for access
  ///  control
  desired_attribute.attibute_type = 2;

  /// Define the AttributeTypeList
  Security::AttributeTypeList attribute_type_list;
  attribute_type_list.length (1);
  attribute_type_list[0] = &desired_attribute;

  /// Get the desired security attributes
  Security::AttributeList_var attribute_list =
    this->ss_current_->get_attributes (attribute_type_list,
                                       ACE_TRY_ENV);
  ACE_CHECK;

  /* @@ What did we do till now ??
   *    We set attribute_type_list so that we get the values of
   *    the variables we are interested in. Depending on how Ossama
   *    implements, the valid values of family_definer, family and the
   *    attribute type and its values all change .. but the process is
   *    similar.
   *    The type which is returned from the get_attribute method is a
   *    sequence of the AttributeList struct. This struct has three
   *    members: the attribute type, the defining_authority and the
   *    value of the attribute type.
   *
   * @@ What should we do now ??
   *
   *    Once we retrieve this sequence, we can invoke a method which
   *    has a generic list of attribute_types and corresponding values
   *    which are valid to authorize access to the requested
   *    invocation. Or complete this stuff with a single if condition
   *    make things easier.
   *    
   *    The condition could be as simple as below:
   */

  if (attribute_list [0].attribute_type == 2 && attribute_list
      [0].Value == "blah")
    {
      // Say this is the valid case in which the client is authorized
      // to access server object.
      attribute_1 = 0;
    }

  /// If the owner of this invocation is authorized to invoke this
  /// method, return 0 else return 1.
  if (authorize_1 == 0)
    return 0;
  else
    return 1;
}


CORBA::Boolean
SLevel1_Server_i::authorize_level2 (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  /// If the owner of this invocation is authorized to invoke this
  /// method, return 0 else return 1.
  if (authorize_2 == 0)
    return 0;
  else
    return 1;
}
