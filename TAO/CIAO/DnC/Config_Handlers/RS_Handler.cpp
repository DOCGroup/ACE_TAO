//$Id$

#ifndef RS_HANDLER_C
#define RS_HANDLER_C

#include "RS_Handler.h"
#include "SP_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;
using CIAO::Config_Handler::SP_Handler;

Deployment::RequirementSatisfier *
CIAO::Config_Handler::RS_Handler::process_RequirementSatisfier (DOMNodeIterator * iter)
{
  ::Deployment::RequirementSatisfier_var ret_struct = 0;
  ACE_NEW_THROW_EX (ret_struct,
                    ::Deployment::RequirementSatisfier,
                    CORBA::NO_MEMORY ());

  //Check if the Schema IDs for both the elements match
  DOMNode * node = iter->nextNode ();
  XStr name (node->getNodeName ());
  if (name != XStr (ACE_TEXT ("name")))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handlers::RS_Handler::process_RequirementSatisfier \
                   element mismatch expected <name>"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Populate the structure
  ret_struct->name = Utils::parse_string (iter);

  // Requirement: Atleast one <resourceType> definition present
  ::CORBA::StringSeq_var res_seq = 0;
  ACE_NEW_THROW_EX (res_seq,
                    ::CORBA::StringSeq (1),
                    CORBA::NO_MEMORY ());
  res_seq->length (1);

  // Process <resourceType> .. </resourceType> definitions
  ::CORBA::ULong index = 0;
  name = node->getNodeName ();
  for (node = iter->nextNode ();
       name == XStr (ACE_TEXT ("resourceType"));
       iter->nextNode (), index++)
    {
      res_seq [index] = Utils::parse_string (iter);
      name = node->getNodeName ();
      res_seq->length (res_seq->length () + 1);
    }

  // On exit go one step back to faciliate parsing next tag
  iter->previousNode ();

  // Assign this sequence to the Return Structure
  ret_struct->resourceType = res_seq;

  // Process <property> tags
  Deployment::SatisfierProperties_var property_seq = 0;
  ACE_NEW_THROW_EX (property_seq,
                    Deployment::SatisfierProperties (),
                    CORBA::NO_MEMORY ());

  property_seq->length (0);

  name = node->getNodeName ();
  for (index = 0, node = iter->nextNode ();
       name == XStr (ACE_TEXT ("property"));
       iter->nextNode (), index++)
    {
      // Increment length of sequence
      property_seq->length (property_seq->length () + 1);

      // Assign the Struct to temporary variable
      Deployment::SatisfierProperty_var temp =
        SP_Handler::process_SatisfierProperty (iter);

      // Perform deep copy
      property_seq [index] = temp;

      // Get next node
      name = node->getNodeName ();
    }

  // On exit go one step back to faciliate parsing next tag
  iter->previousNode ();

  // Deep Copy the property sequence
  ret_struct->property = property_seq;

  // Return structure
  return ret_struct._retn ();
}

#endif /* RS_HANDLER_C */
