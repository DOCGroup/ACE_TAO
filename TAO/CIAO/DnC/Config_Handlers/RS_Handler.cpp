//$Id$

#ifndef RS_HANDLER_C
#define RS_HANDLER_C

#include "RS_Handler.h"
#include "SP_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;
using CIAO::Config_Handler::SP_Handler;

void
CIAO::Config_Handler::RS_Handler::
process_RequirementSatisfier (DOMNodeIterator * iter,
                              Deployment::RequirementSatisfier &ret_struct)
{
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
  ret_struct.name = Utils::parse_string (iter);

  // Requirement: Atleast one <resourceType> definition present
  ::CORBA::StringSeq_var res_seq = 0;
  ACE_NEW_THROW_EX (res_seq,
                    ::CORBA::StringSeq (1),
                    CORBA::NO_MEMORY ());
  res_seq->length (0);

  // Process <resourceType> .. </resourceType> definitions
  name = node->getNodeName ();
  for (node = iter->nextNode ();
       name == XStr (ACE_TEXT ("resourceType"));
       iter->nextNode ())
    {
      ::CORBA::ULong index = res_seq->length ();
      res_seq->length (res_seq->length () + 1);
      res_seq [index] = Utils::parse_string (iter);
      name = node->getNodeName ();
    }

  // On exit go one step back to faciliate parsing next tag
  iter->previousNode ();

  // Assign this sequence to the Return Structure
  ret_struct.resourceType = res_seq;

  // Process <property> tags
  Deployment::SatisfierProperties_var property_seq = 0;
  ACE_NEW_THROW_EX (property_seq,
                    Deployment::SatisfierProperties,
                    CORBA::NO_MEMORY ());

  property_seq->length (0);

  name = node->getNodeName ();
  for (node = iter->nextNode ();
       name == XStr (ACE_TEXT ("property"));
       iter->nextNode ())
    {
      ::CORBA::ULong index = property_seq->length ();

      // Increment length of sequence
      property_seq->length (property_seq->length () + 1);

      SP_Handler::process_SatisfierProperty (iter,
                                             property_seq [index]);
      // Get next node
      name = node->getNodeName ();
    }

  // On exit go one step back to faciliate parsing next tag
  iter->previousNode ();

  // Copy the sequence to return structure
  ret_struct.property = property_seq;
}

#endif /* RS_HANDLER_C */
