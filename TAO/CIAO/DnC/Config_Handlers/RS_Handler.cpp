//$Id$

#ifndef RS_HANDLER_C
#define RS_HANDLER_C

#include "RS_Handler.h"
#include "SP_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

void
CIAO::Config_Handler::RS_Handler::
process_RequirementSatisfier (DOMNodeIterator * iter,
                              Deployment::RequirementSatisfier &ret_struct)
{
  for (DOMNode * node = iter->nextNode (); ; node = iter->nextNode ())
    {
      XStr name (node->getNodeName ());

      if (name == XStr (ACE_TEXT ("name")))
        ret_struct.name = CIAO::Config_Handler::Utils::parse_string (iter);

      else if (name == XStr (ACE_TEXT ("resourceType")))
        {
          // Requirement: Atleast one <resourceType> definition present
          ::CORBA::StringSeq_var res_seq = 0;
          ACE_NEW_THROW_EX (res_seq,
                            ::CORBA::StringSeq (1),
                            CORBA::NO_MEMORY ());
          res_seq->length (0);

          while (name == XStr (ACE_TEXT ("resourceType")))
            {
              ::CORBA::ULong index = res_seq->length ();
              res_seq->length (res_seq->length () + 1);
              res_seq [index] = CIAO::Config_Handler::Utils::parse_string (iter);
              name = node->getNodeName ();
            }

          // On exit go one step back to faciliate parsing next tag
          iter->previousNode ();

          // Assign this sequence to the Return Structure
          ret_struct.resourceType = res_seq;
        }

      else if (name == XStr (ACE_TEXT ("property")))
        {
          Deployment::SatisfierProperties_var property_seq = 0;
          ACE_NEW_THROW_EX (property_seq,
                            Deployment::SatisfierProperties,
                            CORBA::NO_MEMORY ());
          property_seq->length (0);

          while (name == XStr (ACE_TEXT ("property")))
            {

              ::CORBA::ULong index = property_seq->length ();

              // Increment length of sequence
              property_seq->length (property_seq->length () + 1);

              CIAO::Config_Handler::SP_Handler::process_SatisfierProperty (iter,
                                                                           property_seq [index]);
              // Get next node
              name = node->getNodeName ();
            }

          // On exit go one step back to faciliate parsing next tag
          iter->previousNode ();

          // Copy the sequence to return structure
          ret_struct.property = property_seq;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Config_Handlers::RS_Handler::process_ \
                      RequirementSatisfier element mismatch expected <name>,\
                      <resourceType> <property>\n"));
          // On exit go one step back as one might have processed the next valid
          // tag
          iter->previousNode ();

          // Exit out of the for loop
          break;
        }
    }
}

#endif /* RS_HANDLER_C */
