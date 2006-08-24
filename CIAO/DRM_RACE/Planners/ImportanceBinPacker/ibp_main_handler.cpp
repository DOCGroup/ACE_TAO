// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "ibp_main_handler.h"
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <fstream>
#include <sstream>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include "tools/Config_Handlers/XML_File_Intf.h"
#include "tools/Config_Handlers/DP_Handler.h"
#include "tools/Config_Handlers/Utils/XML_Helper.h"
#include "tools/Config_Handlers/Deployment.hpp"
#include "tools/Config_Handlers/Package_Handlers/PCD_Handler.h"
#include "tools/Config_Handlers/Common.h"

class ScriptElement
{
private:
  std::multimap<std::string, std::string> attributes;
  std::string characters;
  std::vector<ScriptElement*> subElements;

protected:
  char * getAttribute(const char * key)
  {
    std::multimap<std::string, std::string>::iterator finder =
      attributes.find(std::string(key));

    if (finder == attributes.end())
      return 0;
    else
    {
      char * retval = new char[finder->second.size() + 1];
      std::strcpy (retval, finder->second.c_str ());
      return retval;
    }
  }

  std::string getCharacters()
  {
    return characters;
  }

public:

  void addAttribute(const char * key, const char * value)
  {
    attributes.insert(std::pair<std::string,std::string>
                      (std::string (key), std::string (value)));
  }

  void addCharacters(const char * characters)
  {
    this->characters += characters;
  }

  void addSubElement(ScriptElement* subElement)
  {
    subElements.push_back (subElement);
  }

  virtual void operator () (ScriptElement* parent);
  virtual void appString (const char * plan,
                          const char * package);
  virtual void initializeNode (const char * name,
                               int capacity);
  virtual void changeNode (const char * name,
                           Domain_Change_Enum change);
  virtual void outputStringsTo (const char * to);
  virtual void outputNodesTo (const char * to);
  virtual ~ScriptElement ();
};

void ScriptElement::operator () (ScriptElement* /*parent*/)
{
  for (std::vector<ScriptElement*>::iterator
         i (subElements.begin()),
         e (subElements.end());
       i != e;
       ++i)
  {
    (**i)(this);
  }
}

void ScriptElement::appString (const char * /*plan*/,
                               const char * /*package*/)
{}

void ScriptElement::initializeNode (const char * /*name*/,
                                    int /*capacity*/)
{}

void ScriptElement::changeNode (const char * /*name*/,
                                Domain_Change_Enum /*change*/)
{}

void ScriptElement::outputStringsTo (const char * /*to*/)
{}

void ScriptElement::outputNodesTo (const char * /*to*/)
{}

ScriptElement::~ScriptElement ()
{
  for (std::vector<ScriptElement*>::iterator
         i (subElements.begin()),
         e (subElements.end());
       i != e;
       ++i)
  {
    delete (*i);
  }
}

class ShellElement : public ScriptElement
{
public:
  void operator () (ScriptElement* parent);
};

void ShellElement::operator () (ScriptElement* parent)
{
  ScriptElement::operator () (parent);

  const std::string & characters = getCharacters();

  std::cerr << "Executing Shell Script: \"";
  for (std::string::size_type i = 0,
         l = (characters.size() > 12 ?
              12 :
              characters.size());
       i < l;
       ++i)
  {
    if (characters[i] == '\n')
      std::cerr << "\\n";
    else
      std::cerr << characters[i];
  }
  if (characters.size() > 12)
    std::cerr << "...";
  std::cerr << "\" (" << characters.size() << " chars)\n";

  std::system (characters.c_str());
}

class NodesElement : public ScriptElement
{
public:
  void operator () (ScriptElement* parent);
};

void NodesElement::operator () (ScriptElement* parent)
{
  ScriptElement::operator () (parent);

  char* to = getAttribute ("to");
  if (to)
    parent->outputNodesTo (to);

  delete [] to;
}

class OutputStringElement : public ScriptElement
{
  void operator () (ScriptElement* parent);
};

void OutputStringElement::operator () (ScriptElement* parent)
{
  ScriptElement::operator () (parent);

  char* to = getAttribute ("to");
  if (to)
    parent->outputStringsTo (to);

  delete [] to;
}

class DumpElement : public ScriptElement
{
  struct DumpAction
  {
    std::string destination;
    enum
      {
        OUTPUT_STRINGS,
        OUTPUT_NODES
      }
    action;
  };

  std::vector<DumpAction>  dumpActions;

  virtual void operator () (ScriptElement* parent);
  virtual void outputStringsTo (const char * to);
  virtual void outputNodesTo (const char * to);
};

void DumpElement::operator () (ScriptElement* parent)
{
  ScriptElement::operator () (parent);

  for (std::vector<DumpAction>::iterator i = dumpActions.begin(),
         e = dumpActions.end();
       i != e;
       ++i)
  {
    switch (i->action)
    {
    default:
    case DumpAction::OUTPUT_STRINGS:
      {
        std::cerr << "Dumping Strings to " << i->destination << "\n";
        std::string indexfile = i->destination + "/index.txt";
        std::system ((std::string("mkdir -p ") + i->destination).c_str());
        for (CORBA::ULong j = 0,
               l = IBP_Main_Parser::output->length();
             j < l;
             ++j)
        {
          // Output Plan
          ::Deployment::DeploymentPlan & plan = IBP_Main_Parser::output[j].plan;
          ::CIAO::Config_Handlers::DP_Handler reverse_handler (plan);
          ::CIAO::Config_Handlers::XML_Helper xml_helper;
          ::xercesc::DOMDocument *the_xsc (xml_helper.create_dom (0));

          ::CIAO::Config_Handlers::deploymentPlan (*reverse_handler.xsc (), the_xsc);

          std::string filename (plan.UUID.in ());
          filename += ".cdp";
          std::string dom_out =  i->destination + "/" + filename;
          xml_helper.write_DOM (the_xsc, dom_out.c_str());

          // Index Plan
          std::ofstream indexOut (indexfile.c_str(), std::ios_base::app);
          indexOut << "[" << j << "]  "
                   << (IBP_Main_Parser::output[j].command == Action_Types::DEPLOY ?
                       "Deploy          " :
                       IBP_Main_Parser::output[j].command == Action_Types::TEARDOWN ?
                       "Teardown        " :
                       IBP_Main_Parser::output[j].command == Action_Types::AUTO_TEARDOWN ?
                       "Swapout         " :
                       IBP_Main_Parser::output[j].command == Action_Types::REPLAN_DEPLOY ?
                       "Replan-Deploy   " :
                       IBP_Main_Parser::output[j].command == Action_Types::REPLAN_TEARDOWN ?
                       "Replan-Teardown " :
                       IBP_Main_Parser::output[j].command == Action_Types::REPLAN_AUTO_TEARDOWN ?
                       "Replan-Swapout  " :
                       "Other         ")
                   << filename
                   << "\n";
        }

        // Flush the cache of output plans
        IBP_Main_Parser::output->length(0);
      }
      break;
    case DumpAction::OUTPUT_NODES:
      std::cerr << "Dumping Nodes to " << i->destination << "\n";
      std::ofstream binsOut (i->destination.c_str());
      IBP_Main_Parser::ibp->print_bins (binsOut);
      break;
    }
  }
}

void DumpElement::outputStringsTo (const char * to)
{
  DumpAction dumpAction;
  dumpAction.destination = to;
  dumpAction.action = DumpAction::OUTPUT_STRINGS;
  dumpActions.push_back (dumpAction);
}

void DumpElement::outputNodesTo (const char * to)
{
  DumpAction dumpAction;
  dumpAction.destination = to;
  dumpAction.action = DumpAction::OUTPUT_NODES;
  dumpActions.push_back (dumpAction);
}

class AppStringElement : public ScriptElement
{
public:
  virtual void operator() (ScriptElement * parent);
};

void AppStringElement::operator () (ScriptElement* parent)
{
  ScriptElement::operator () (parent);

  char* plan    = getAttribute ("plan");
  char* package = getAttribute ("package");
  parent->appString (plan, package);
  delete [] plan;
  delete [] package;
}

class ActionElement : public ScriptElement
{
protected:
  struct DeployAction
  {
    std::string plan;
    std::string package;
    CORBA::Long new_importance;
    int plan_ready;
    int package_ready;
    int new_importance_ready;
    enum
      {
        DEPLOY,
        TEARDOWN,
      }
    action;
  };

  std::vector<DeployAction> deployActions;

public:

  virtual void operator () (ScriptElement* parent);
};

void ActionElement::operator () (ScriptElement* parent)
{
  ScriptElement::operator () (parent);

  std::cerr << "Sending App Strings to RACE...\n";

  int count = 0;
  Plan_Seq::_var_type input(new Plan_Seq (0));
  input->length(0);
  for (std::vector<DeployAction>::iterator i = deployActions.begin(),
         e = deployActions.end();
       i != e;
       ++i, ++count)
  {
    switch (i->action)
    {
    default:
    case DeployAction::DEPLOY:
      std::cerr << "\t[" << count << "] {"
                << (i->plan_ready ? i->plan : std::string("0")) << ", "
                << (i->package_ready ? i->package : std::string("0")) << ", "
                << "DEPLOY}\n";
      break;
    case DeployAction::TEARDOWN:
      std::cerr << "\t[" << count << "] {"
                << (i->plan_ready ? i->plan : std::string("0")) << ", "
                << (i->package_ready ? i->package : std::string("0")) << ", "
                << "TEARDOWN}\n";
      break;
    }

    CORBA::ULong old_length = input->length();
    input->length (old_length+1);
    input[old_length].command = (i->action == DeployAction::DEPLOY ?
                                    Action_Types::DEPLOY :
                                    Action_Types::TEARDOWN);
    if (i->plan_ready)
    {
      // Read Plan
      ::CIAO::Config_Handlers::XML_File_Intf xfi (i->plan.c_str());
      const ::CIAO::Config_Handlers::XML_File_Intf & xfi_const = xfi;
      if (xfi_const.get_plan()) {
        input[old_length].plan = *(xfi_const.get_plan());
      }
    }

    if (i->package_ready)
    {
      // Read Package
      try
      {
        ::CIAO::Config_Handlers::Packaging::PCD_Handler::package_config
            (i->package.c_str(), input[old_length].package);
      }
      catch (::CIAO::Config_Handlers::Config_Error &ex)
      {
        ACE_ERROR ((LM_ERROR, "Config_Error exception caught "
                    "while parsing %s: %s, %s\n",
                    i->package.c_str(),
                    ex.error_.c_str (),
                    ex.name_.c_str ()));
      }
      catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Config_Error exception caught "
                    "while parsing %s.\n",
                    i->package.c_str()));
      }
    }

    if (i->new_importance_ready)
    {
      // Change the Importance
      ::Deployment::ComponentPackageDescriptions & basePackage =
          input[old_length].package.basePackage;
      if (basePackage.length()) {
        ::Deployment::ComponentPackageDescription & cpd = basePackage[0];
        ::Deployment::PackagedComponentImplementations & implementation = cpd.implementation;
        if (implementation.length()) {
          ::Deployment::PackagedComponentImplementation & first_impl = implementation[0];
          ::Deployment::ComponentImplementationDescription & cid = first_impl.referencedImplementation;
          ::Deployment::Properties & configProperties = cid.configProperty;
          for (CORBA::ULong j = 0, l = configProperties.length();
               j < l;
               ++j)
          {
            ::Deployment::Property & configProperty = configProperties[j];
            if (ACE_OS::strcmp(configProperty.name.in(), "Importance") == 0)
            {
              configProperty.value <<= i->new_importance;
            }
          }
        }
      }
    }
  }

  // Call the Importance Bin Packer on the input
  bool ret_val = IBP_Main_Parser::ibp->convert_input
    (input.in(),
     IBP_Main_Parser::output.inout());

  const char *msg = (ret_val)?"SUCCESS":"FAIL";
  std::cerr << "...Sent " << count << " App Strings (" << msg << ").\n";
}

class DeployElement : public ActionElement
{
public:
  virtual void appString (const char * plan,
                          const char * package);
};

void DeployElement::appString (const char * plan,
                               const char * package)
{
  DeployAction da;

  if (plan) {
    da.plan = plan;
    da.plan_ready = 1;
  } else {
    da.plan_ready = 0;
  }

  if (package) {
    da.package = package;
    da.package_ready = 1;
  } else {
    da.package_ready = 0;
  }

  da.action = DeployAction::DEPLOY;
  da.new_importance_ready = 0;
  deployActions.push_back(da);
}

class ChangeElement : public DeployElement
{

public:
  virtual void appString (const char * plan,
                          const char * package);
};

void ChangeElement::appString (const char * plan,
                               const char * package)
{
  DeployElement::appString (plan, package);

  int last_app = ((int) (deployActions.size())) - 1;
  CORBA::Long importance;
  char* importance_as_char = getAttribute("importance");

  if (importance_as_char != 0 && last_app >= 0) {
    std::string importance_as_string(importance_as_char);
    std::istringstream converter(importance_as_string);
    if (converter >> importance) {
      DeployAction & da = deployActions[last_app];
      da.new_importance = importance;
      da.new_importance_ready = 1;
    }
  }

  delete [] importance_as_char;
}

class TeardownElement : public ActionElement
{
public:
  virtual void appString (const char * plan,
                          const char * package);
};

void TeardownElement::appString (const char * plan,
                                 const char * package)
{
  DeployAction da;

  if (plan) {
    da.plan = plan;
    da.plan_ready = 1;
  } else {
    da.plan_ready = 0;
  }

  if (package) {
    da.package = package;
    da.package_ready = 1;
  } else {
    da.package_ready = 0;
  }

  da.action = DeployAction::TEARDOWN;
  da.new_importance_ready = 0;
  deployActions.push_back(da);
}

class InitializeElement : public ScriptElement
{
private:
  struct NodeRecord
  {
    std::string name;
    int capacity;
  };

  std::vector <NodeRecord> node_records;
public:
  virtual void operator () (ScriptElement* parent);
  virtual void initializeNode (const char * name,
                               int capacity);
};

void InitializeElement::operator () (ScriptElement* parent)
{
  ScriptElement::operator () (parent);
  std::vector<NodeRecord> ::size_type num_nodes = node_records.size();

  char ** bin_names = new char*[num_nodes + 1];
  bin_names[num_nodes] = 0;
  double ** bin_capacities = new double*[num_nodes + 1];
  bin_capacities[num_nodes] = 0;

  std::vector<NodeRecord> ::size_type index = 0;
  for (std::vector<NodeRecord> ::iterator i = node_records.begin(),
         e = node_records.end();
       i != e;
       ++index, ++i)
  {
    std::string & bin_name = i->name;
    std::string::size_type bin_name_length = bin_name.size();
    bin_names[index] = new char[bin_name_length + 1];
    ACE_OS::strcpy(bin_names[index], bin_name.c_str());
    bin_capacities[index] = new double(i->capacity);
  }

  IBP_Main_Parser::ibp.reset(new ImportanceBinPacker(bin_names, bin_capacities));

  for (double** i = bin_capacities; *i != 0; ++i)
    delete *i;
  delete [] bin_capacities;

  for (char** i = bin_names; *i != 0; ++i)
    delete [] *i;
  delete [] bin_names;
}

void InitializeElement::initializeNode (const char * name,
                                        int capacity)
{
  NodeRecord node;
  node.name = name;
  node.capacity = capacity;
  node_records.push_back(node);
}

class InitializeNodeElement : public ScriptElement
{
public:
  virtual void operator() (ScriptElement * parent);
};

void InitializeNodeElement::operator () (ScriptElement* parent)
{
  ScriptElement::operator () (parent);

  char* name             = getAttribute ("name");
  int   capacity         = 0;
  char* capacity_as_char = getAttribute ("capacity");

  if (capacity_as_char) {
    std::string capacity_as_string (capacity_as_char);
    std::istringstream converter (capacity_as_string);
    converter >> capacity;
  }

  parent->initializeNode (name, capacity);

  delete [] name;
  delete [] capacity_as_char;
}

class ChangeDomainElement : public ScriptElement
{
private:
  struct ChangeRecord
  {
    std::string node_name;
    Domain_Change_Enum change;
  };

  std::vector <ChangeRecord> change_records;

public:
  virtual void operator () (ScriptElement* parent);
  virtual void changeNode (const char * node_name,
                           Domain_Change_Enum change);
};

void ChangeDomainElement::operator () (ScriptElement* parent)
{
  ScriptElement::operator () (parent);

  CORBA::ULong index = 0,
    num_domain_changes = CORBA::ULong (change_records.size());

  Domain_Change_List::_var_type domain_changes
      (new Domain_Change_List (num_domain_changes));

  domain_changes->length (num_domain_changes);

  for (std::vector<ChangeRecord> ::iterator i (change_records.begin()),
         e (change_records.end());
       i != e;
       ++i, ++index)
  {
    domain_changes[index].hostname = CORBA::string_dup (i->node_name.c_str());
    domain_changes[index].change   = i->change;
  }

  // Call the Importance Bin Packer on the input
  bool ret_val = IBP_Main_Parser::ibp->domain_change
    (domain_changes,
     0, 0,
     IBP_Main_Parser::output.inout());

  const char *msg = (ret_val)?"SUCCESS":"FAIL";
  std::cerr << "...Received "
            << IBP_Main_Parser::output->length()
            << " App Strings (" << msg << ").\n";
}

void ChangeDomainElement::changeNode (const char * node_name,
                                      Domain_Change_Enum change)
{
  ChangeRecord change_record;
  change_record.node_name = node_name;
  change_record.change    = change;
  change_records.push_back(change_record);
}

class FailDomainElement : public ScriptElement
{
public:
  virtual void operator () (ScriptElement* parent);
};

void FailDomainElement::operator () (ScriptElement* parent)
{
  ScriptElement::operator () (parent);
  const std::string & characters = getCharacters();
  parent->changeNode (characters.c_str(), Domain_Change_Types::NODE_FAILED);
}

// ---------------------------------------------------------------------------
//  SAX2CountHandlers: Constructors and Destructor
// ---------------------------------------------------------------------------
SAX2CountHandlers::SAX2CountHandlers() :

    fAttrCount(0)
    , fCharacterCount(0)
    , fElementCount(0)
    , fSpaceCount(0)
    , fSawErrors(false)
{
  std::memset(scriptElements, 0, sizeof(scriptElements));
  numElements = 0;
}

SAX2CountHandlers::~SAX2CountHandlers()
{
}

// ---------------------------------------------------------------------------
//  SAX2CountHandlers: Implementation of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void SAX2CountHandlers::startElement(const XMLCh* const /*uri*/
                                   , const XMLCh* const localname
                                     , const XMLCh* const /*qname*/
                                   , const Attributes& attrs)
{
    fElementCount++;
    fAttrCount += attrs.getLength();

    char * asChar = XMLString::transcode(localname);
    if (numElements < sizeof(scriptElements))
    {
      scriptElements[numElements] =
        !std::strcmp (asChar, "dump") ? new DumpElement :
        (!std::strcmp (asChar, "outputStrings") ? new OutputStringElement :
         (!std::strcmp (asChar, "nodes") ? new NodesElement :
          (!std::strcmp (asChar, "string") ? new AppStringElement :
           (!std::strcmp (asChar, "deploy") ? new DeployElement :
            (!std::strcmp (asChar, "teardown") ? new TeardownElement :
             (!std::strcmp (asChar, "change") ? new ChangeElement :
              (!std::strcmp (asChar, "shell") ? new ShellElement :
               (!std::strcmp (asChar, "initialize") ? new InitializeElement :
                (!std::strcmp (asChar, "node") ? new InitializeNodeElement :
                 (!std::strcmp (asChar, "changeDomain") ? new ChangeDomainElement :
                  (!std::strcmp (asChar, "fail") ? new FailDomainElement :
                   (new ScriptElement)
                   )
                  )
                 )
                )
               )
              )
             )
            )
           )
          )
         )
        ;
    }
    XMLString::release(&asChar);
    ++numElements;

    for (unsigned int i = 0, l = attrs.getLength();
         i < l;
         ++i)
    {
      char * name  =
        XMLString::transcode(attrs.getLocalName(i));
      char * value =
        XMLString::transcode(attrs.getValue(i));

      if (numElements <= sizeof(scriptElements))
        scriptElements[numElements-1]->addAttribute(name, value);

      XMLString::release(&value);
      XMLString::release(&name);
    }

#if 0
    char names [3][16] = {"uri", "localname", "qname"};
    const XMLCh * const values [3] = {uri, localname, qname};
    for (int i = 0; i < 3; ++i)
    {
      if (values[i])
      {
        char * asChar = XMLString::transcode(values[i]);
        XERCES_STD_QUALIFIER cerr << names[i] << " is " << asChar << "\n";
        XMLString::release(&asChar);
      }
    }
#endif
}

void SAX2CountHandlers::characters(  const   XMLCh* const chars
                                                                    , const unsigned int    length)
{
    fCharacterCount += length;

    char * asChar = XMLString::transcode(chars);
    if (numElements <= sizeof(scriptElements))
      scriptElements[numElements-1]->addCharacters(asChar);

#if 0
    XERCES_STD_QUALIFIER cerr << '"' << asChar << "\"\n";
#endif
    XMLString::release(&asChar);
}

void SAX2CountHandlers::endElement(const XMLCh* const /*uri*/
                                , const XMLCh* const /*localname*/
                                , const XMLCh* const /*qname*/)
{
  ScriptElement *parent = 0, *child = 0;

  if (numElements <= sizeof(scriptElements) &&
      numElements >= 2)
  {
    parent = scriptElements[numElements-2];
    child  = scriptElements[numElements-1];
    parent->addSubElement(child);
  }

  else if (numElements == 1)
  {
    child  = scriptElements[numElements-1];
    (*child)(0);
    delete child;
  }

  --numElements;
}

void SAX2CountHandlers::ignorableWhitespace( const   XMLCh* const /* chars */
                                                                                    , const unsigned int length)
{
    fSpaceCount += length;
}

void SAX2CountHandlers::resetDocument()
{
    fAttrCount = 0;
    fCharacterCount = 0;
    fElementCount = 0;
    fSpaceCount = 0;
}


// ---------------------------------------------------------------------------
//  SAX2CountHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void SAX2CountHandlers::error(const SAXParseException& e)
{
    fSawErrors = true;
    XERCES_STD_QUALIFIER cerr << "\nError at file " << StrX(e.getSystemId())
                 << ", line " << e.getLineNumber()
                 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SAX2CountHandlers::fatalError(const SAXParseException& e)
{
    fSawErrors = true;
    XERCES_STD_QUALIFIER cerr << "\nFatal Error at file " << StrX(e.getSystemId())
                 << ", line " << e.getLineNumber()
                 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SAX2CountHandlers::warning(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nWarning at file " << StrX(e.getSystemId())
                 << ", line " << e.getLineNumber()
                 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void SAX2CountHandlers::resetErrors()
{
    fSawErrors = false;
}
