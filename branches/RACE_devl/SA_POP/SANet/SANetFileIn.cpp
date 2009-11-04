// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SANetFileIn.cpp
 *
 * This file contains the SANetFileIn class implementation for the input adapter
 * that initializes a SANet object using an XML SANet file.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include "SANet_Types.h"
#include "SANetFileIn.h"

#if !defined (SANET_STANDALONE)
#include "Builder.h"
#include "SA_POP_XML_Typedefs.h"
#else
#include "SANet_XML_Typedefs.h"
#endif

#include "XML_SANet.hpp"

using namespace SANet;

// Constructor.
SANetFileIn::SANetFileIn (void)
{
  // Nothing to do.
};

// Destructor.
SANetFileIn::~SANetFileIn (void)
{
  // Nothing to do.
};

using SA_POP::XML::XML_Helper;

// Build network from XML file.
Network *SANetFileIn::build_net (std::string filename)
{
  if (!XML_Helper::XML_HELPER.is_initialized ())
    return 0;

  // Set path to XML schemas (XSD files).
  XML_Helper::_path_resolver.add_path (SA_POP::ROOT_PATH_STR.c_str (), SA_POP::XSD_REL_PATH_STR.c_str ());

  // Parse file with Xerces.
  XERCES_CPP_NAMESPACE::DOMDocument *dom =
//*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****
// No longer need different call to XML Helper with/without CIAO... I think...
//#if defined (SA_POP_HAS_CIAO)
    XML_Helper::XML_HELPER.create_dom (filename.c_str ());
//#else
//  XML_Helper::XML_HELPER.create_dom (filename.c_str (), "");
//#endif  /* SA_POP_HAS_CIAO */
  if (!dom)
    return 0;

  SANet::XML::Network xml_net = SANet::XML::reader::network (dom);

  SANet::Network *net = new SANet::Network ();

  // Get task nodes.
  for (SANet::XML::Network::taskNode_iterator iter = xml_net.begin_taskNode ();
    iter != xml_net.end_taskNode (); iter++)
  {
    ::SANet::XML::TaskNode node (*iter);
    TaskID nodeID = node.nodeID ();
    Probability priorProb = (*iter).priorProb ();
    MultFactor attenFactor = (*iter).attenFactor ();
    TaskCost cost = (*iter).cost ();
//    TimeValue dur = 
    char name[SANet::SANetFileIn::STR_BUF_SIZE];
    wcstombs (name, (*iter).name ().c_str (), SANet::SANetFileIn::STR_BUF_SIZE);
    
    net->add_task (nodeID, name, attenFactor, cost, priorProb);
  }

  // Get condition nodes.
  for (SANet::XML::Network::condNode_iterator iter = xml_net.begin_condNode ();
    iter != xml_net.end_condNode (); iter++)
  {
    CondID nodeID = (*iter).nodeID ();
    Probability probTrue = (*iter).probTrue ();
    Utility utility = (*iter).utility ();
    MultFactor attenFactor = (*iter).attenFactor ();
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
//    CondKind
    char name[SANet::SANetFileIn::STR_BUF_SIZE];
    wcstombs (name, (*iter).name ().c_str (), SANet::SANetFileIn::STR_BUF_SIZE);
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

	SANet::XML::CondKind cond_kind = (*iter).kind();
  ::SANet::CondKind cond;
#if defined (SANET_STANDALONE)
  if (cond_kind == cond_kind.ENVIRON)
    cond = ::SANet::ENVIRON;
  else if (cond_kind == cond_kind.SYSTEM)
    cond = ::SANet::SYSTEM;
  else
    cond = ::SANet::DATA;
#else  // SANET_STANDALONE not defined
  if (cond_kind == cond_kind.ENVIRON)
    cond = ::SA_POP::ENVIRON;
  else if (cond_kind == cond_kind.SYSTEM)
    cond = ::SA_POP::SYSTEM;
  else
    cond = ::SA_POP::DATA;
#endif  /* SANET_STANDALONE */

   
    net->add_cond (nodeID, name, attenFactor,
      probTrue, 1.0 - probTrue, utility, cond);
  }

  // Get precondition->task links.
  for (SANet::XML::Network::precondLink_iterator iter = xml_net.begin_precondLink ();
    iter != xml_net.end_precondLink (); iter++)
  {
    CondID condID = (*iter).condID ();
    TaskID taskID = (*iter).taskID ();
    Probability trueProb = (*iter).trueProb ();
    Probability falseProb = (*iter).falseProb ();

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    char portID[SANet::SANetFileIn::STR_BUF_SIZE];
    wcstombs (portID, (*iter).portID ().c_str (), SANet::SANetFileIn::STR_BUF_SIZE);
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

    net->add_precond_link (condID, taskID, trueProb, falseProb);
  }

  // Get task->effect links.
  for (SANet::XML::Network::effectLink_iterator iter = xml_net.begin_effectLink ();
    iter != xml_net.end_effectLink (); iter++)
  {
    TaskID taskID = (*iter).taskID ();
    CondID condID = (*iter).condID ();
    Probability weight = (*iter).weight ();

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    char portID[SANet::SANetFileIn::STR_BUF_SIZE];
    wcstombs (portID, (*iter).portID ().c_str (), SANet::SANetFileIn::STR_BUF_SIZE);
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

    net->add_effect_link (taskID, condID, weight);
  }

  return net;
};

#if !defined (SANET_STANDALONE)
// Build network from XML file.
void SANetFileIn::build_net (std::string filename, SA_POP::Builder *builder)
{
  if (!XML_Helper::XML_HELPER.is_initialized ())
    return;

  // Set path to XML schemas (XSD files).
  XML_Helper::_path_resolver.add_path (SA_POP::ROOT_PATH_STR.c_str (), SA_POP::XSD_REL_PATH_STR.c_str ());

  // Parse file with Xerces.
  XERCES_CPP_NAMESPACE::DOMDocument *dom =
//*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****
// No longer need different call to XML Helper with/without CIAO... I think...
//#if defined (SA_POP_HAS_CIAO)
    XML_Helper::XML_HELPER.create_dom (filename.c_str ());
//#else
//  XML_Helper::XML_HELPER.create_dom (filename.c_str (), "");
//#endif  /* SA_POP_HAS_CIAO */
  if (!dom)
    return;

  SANet::XML::Network xml_net = SANet::XML::reader::network (dom);

  // Get task nodes.
  for (SANet::XML::Network::taskNode_iterator iter = xml_net.begin_taskNode ();
    iter != xml_net.end_taskNode (); iter++)
  {
    TaskID nodeID = (*iter).nodeID ();
    Probability priorProb = (*iter).priorProb ();
    MultFactor attenFactor = (*iter).attenFactor ();
    TaskCost cost = (*iter).cost ();

    char name[SANet::SANetFileIn::STR_BUF_SIZE];
    wcstombs (name, (*iter).name ().c_str (), SANet::SANetFileIn::STR_BUF_SIZE);
    
    builder->add_task (nodeID, priorProb, name);
  }
  
 // builder->add_task(SA_POP::INIT_TASK_ID, .5, "initact");

  // Get condition nodes.
  for (SANet::XML::Network::condNode_iterator iter = xml_net.begin_condNode ();
    iter != xml_net.end_condNode (); iter++)
  {
    CondID nodeID = (*iter).nodeID ();
    Probability probTrue = (*iter).probTrue ();
    Utility utility = (*iter).utility ();
    MultFactor attenFactor = (*iter).attenFactor ();

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
//    CondKind
    char name[SANet::SANetFileIn::STR_BUF_SIZE];
    wcstombs (name, (*iter).name ().c_str (), SANet::SANetFileIn::STR_BUF_SIZE);
	SANet::XML::CondKind cond_kind = (*iter).kind();
	SA_POP::CondKind cond;
	if(cond_kind==cond_kind.ENVIRON) cond=::SA_POP::ENVIRON;
	else if(cond_kind==cond_kind.SYSTEM) cond=::SA_POP::SYSTEM;
  else cond=::SA_POP::DATA;
	builder->add_cond (nodeID, utility, probTrue, name, cond);

//	builder->set_effect(SA_POP::INIT_TASK_ID, nodeID, "", (probTrue-.5)*2);
  }

  // Get precondition->task links.
  for (SANet::XML::Network::precondLink_iterator iter = xml_net.begin_precondLink ();
    iter != xml_net.end_precondLink (); iter++)
  {
    CondID condID = (*iter).condID ();
    TaskID taskID = (*iter).taskID ();
    Probability trueProb = (*iter).trueProb ();
    Probability falseProb = (*iter).falseProb ();

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    char portID[SANet::SANetFileIn::STR_BUF_SIZE];
    wcstombs (portID, (*iter).portID ().c_str (), SANet::SANetFileIn::STR_BUF_SIZE);

    builder->set_precond (condID, taskID, portID, trueProb, falseProb);
  }

  // Get task->effect links.
  for (SANet::XML::Network::effectLink_iterator iter = xml_net.begin_effectLink ();
    iter != xml_net.end_effectLink (); iter++)
  {
    TaskID taskID = (*iter).taskID ();
    CondID condID = (*iter).condID ();
    Probability weight = (*iter).weight ();

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    char portID[SANet::SANetFileIn::STR_BUF_SIZE];
    wcstombs (portID, (*iter).portID ().c_str (), SANet::SANetFileIn::STR_BUF_SIZE);

    builder->set_effect (taskID, condID, portID, weight);
  }
};
#endif /* SANET_STANDALONE */
