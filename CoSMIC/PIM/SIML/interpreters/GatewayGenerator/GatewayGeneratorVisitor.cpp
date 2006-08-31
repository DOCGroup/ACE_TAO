#include "stdafx.h"
#include "GatewayGeneratorVisitor.h"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"
#include "CCF/CodeGenerationKit/IndentationCxx.hpp"
#include <set>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <algorithm>
#include <functional>

using std::set;
using std::vector;
using std::string;
using std::endl;
using std::ofstream;
using std::stringstream;
using std::toupper;
using std::isalnum;
using std::sort;
using std::copy;
using std::transform;
using std::binary_function;

namespace BON
{
  std::string strNamespacePref;
}

template <class T>
struct ParamSorter
  : public binary_function<T,T,bool>
{
  bool operator()(const T& x, const T& y)
  {
    BON::FCOExRegistryNode& lnode (x->getRegistry());
    BON::FCOExRegistryNode& rnode (y->getRegistry());
    BON::Point lhs = lnode->getLocation();
    BON::Point rhs = rnode->getLocation();
    return (lhs.first < rhs.first && lhs.second < rhs.second);
  }
};

GatewayGeneratorVisitor::GatewayGeneratorVisitor (BON::Project& project,
                                                  string outputFolder)
  : project_ (project),
    outputFolder_ (outputFolder)
{
}

GatewayGeneratorVisitor::~GatewayGeneratorVisitor()
{}

string GatewayGeneratorVisitor::idl_scoped_name (const BON::FCO& object)
{
  PredefinedType pdt (object);
  if (pdt) return this->basic_name (pdt);

  BON::Model parent (object->getParentModel ());

  // If we are a file, return an empty string.
  if (!parent) return "";

  string local_name (object->getName ());

  // If our parent is a file, don't tack on its name.
  if (!File (parent))
    {
      return this->idl_scoped_name (parent) + "::" + local_name;
    }

  return local_name;
}

string GatewayGeneratorVisitor::wsdl_scoped_name (const BON::FCO& object)
{
  PredefinedType pdt (object);
  if (pdt) return this->basic_name (pdt);

  BON::Model parent (object->getParentModel ());

  // If we are a file, return an empty string.
  if (!parent) return "";

  string local_name (object->getName ());

  if (Collection (object))
    {
      local_name = string ("_SE_") + local_name;
    }
  // If our parent is a file, don't tack on its name.
  if (!File (parent))
    {
      return this->wsdl_scoped_name (parent) + "." + local_name;
    }
  return local_name;
}

string GatewayGeneratorVisitor::basic_name (const PredefinedType& pdt)
{
  if (String (pdt))             return "string";
  if (LongInteger (pdt))        return "int";
  if (RealNumber (pdt))         return "double";
  if (ShortInteger (pdt))       return "short";
  if (Boolean (pdt))            return "boolean";
  if (Byte (pdt))               return "octet";
  if (GenericValue (pdt))       return "any";
  if (GenericObject (pdt))      return "Object";
  if (GenericValueObject (pdt)) return "ValueBase";
  if (TypeEncoding (pdt))       return "CORBA::TypeCode";
  if (TypeKind (pdt))           return "CORBA::TCKind";
  else                          return "";
}

string GatewayGeneratorVisitor::get_file_name (const BON::FCO& object)
{
  BON::Model parent (object->getParentModel ());
  if (!parent) return object->getName();
  else
    return this->get_file_name (parent);
}


const char *
GatewayGeneratorVisitor::utf8(char* t, const char *s)
{
  unsigned int c = 0;
  int c1, c2, c3, c4;
  c = (unsigned char)*s;
  if (c >= 0x80)
  {
    c1 = *++s;
    if (c1 < 0x80)
      s--;
    else
    {
      c1 &= 0x3F;
      if (c < 0xE0)
        c = ((c & 0x1F) << 6) | c1;
      else
      {
        c2 = *++s & 0x3F;
        if (c < 0xF0)
          c = ((c & 0x0F) << 12) | (c1 << 6) | c2;
        else
	{
          c3 = *++s & 0x3F;
          if (c < 0xF8)
            c = ((c & 0x07) << 18) | (c1 << 12) | (c2 << 6) | c3;
          else
	  {
            c4 = *++s & 0x3F;
            if (c < 0xFC)
              c = ((c & 0x03) << 24) | (c1 << 18) | (c2 << 12) | (c3 << 6) | c4;
            else
              c = ((c & 0x01) << 30) | (c1 << 24) | (c2 << 18) | (c3 << 12) | (c4 << 6) | *++s & 0x3F;
          }
        }
      }
    }
  }
  sprintf(t, "_x%.4x", c);
  return s;
}

string
GatewayGeneratorVisitor::transform_name (const char* name)
{
  string result;
  if (name && *name)
    {
      for (; *name; name++)
        {
          if (isalnum (*name))
            result += *name;
          else if (*name == '_')
            result += "_USCORE";
          else
            {
              char t[7] = {0};
              name = utf8 (&t[0], name);
              result.append (&t[0]);
            }
        }
    }
  return result;
}


void
GatewayGeneratorVisitor::get_namespace(const Module& module)
{
  BON::Folder rootFolder = this->project_->getRootFolder();
  set<BON::Object> defs = rootFolder->getChildObjects ("WSML::Definitions");
  bool serviceFound = false;
  for (set<BON::Object>::iterator iter = defs.begin();
       iter != defs.end();
       ++iter)
    {
      Definitions def (*iter);
      if (!def)
        {
          stringstream msg;
          msg << "Unable to create Definition corresponding to module "
              << module->getName() << endl;
          this->project_->consoleMsg (msg.str(), MSG_ERROR);
          throw GatewayGeneratorException();
        }
      set<Service> services = def->getService();
      for (set<Service>::iterator iter1 = services.begin();
           iter1 != services.end();
           ++iter1)
        {
          Service service (*iter1);
          if (!service)
            {
              stringstream msg;
              msg << "Unable to create Service corresponding to Definitions "
                  << def->getName() << endl;
              this->project_->consoleMsg (msg.str(), MSG_ERROR);
              throw GatewayGeneratorException();
            }
          if (service->getName() == module->getName())
            {
              serviceFound = true;
              this->targetNamespace_ = def->gettargetNamespace();
              this->defName_ = def->getName();
              this->serviceName_
                = this->transform_name (service->getName().c_str());
              return;
            }
        }

    }
  if (!serviceFound)
    {
      stringstream msg;
      msg << "Unable to locate Service corresponding to module "
          << module->getName() << endl;
      this->project_->consoleMsg (msg.str(), MSG_ERROR);
      throw GatewayGeneratorException();
    }
}

void
GatewayGeneratorVisitor::visitSystem(const System& object)
{
  set<Module> modules = object->getModule();
  for (set<Module>::iterator iter = modules.begin();
       iter != modules.end();
       ++iter)
    {
      Module module (*iter);
      module->accept (this);
    }
}

void
GatewayGeneratorVisitor::visitModule(const Module& object)
{
  string moduleName (this->transform_name (object->getName().c_str()));
  set<PortProxy> delegates = object->getOutPortProxyLinks();
  if (!delegates.empty())
    {
      this->get_namespace (object);
      string upperModuleName;
      transform (moduleName.begin(),
                      moduleName.end(),
                      back_inserter(upperModuleName),
                      toupper);
      string path (this->outputFolder_);
      path += "\\";
      path += moduleName.c_str();
      string fileName (path + ".h");
      this->header_.open (fileName.c_str());
      if (!this->header_.is_open())
        {
          stringstream msg;
          msg << "Error in opening file " << fileName << endl;
          this->project_->consoleMsg (msg.str(), MSG_ERROR);
          throw GatewayGeneratorException();
        }
      Indentation::Implanter <Indentation::Cxx> header (this->header_);
      this->header_ << "#ifndef " << upperModuleName << "_H" << endl;
      this->header_ << "#define " << upperModuleName << "_H" << endl << endl;

      fileName = path + ".cpp";
      this->source_.open (fileName.c_str());
      if (!this->source_.is_open())
        {
          stringstream msg;
          msg << "Error in opening file " << fileName << endl;
          this->project_->consoleMsg (msg.str(), MSG_ERROR);
          throw GatewayGeneratorException();
        }
      Indentation::Implanter <Indentation::Cxx> source (this->source_);
      this->source_ << "#include \"" << moduleName << ".h\"" << endl;
      this->source_ << "#include <string>" << endl;
      this->source_ << "using std::string;" << endl;

      fileName = path + "Service.cpp";
      this->driver_.open (fileName.c_str());
      if (!this->driver_.is_open())
        {
          stringstream msg;
          msg << "Error in opening file " << fileName << endl;
          this->project_->consoleMsg (msg.str(), MSG_ERROR);
          throw GatewayGeneratorException();
        }
      Indentation::Implanter <Indentation::Cxx> driver (this->driver_);
      this->driver_ << "#include \"ace/Get_Opt.h\"" << endl;
      this->driver_ << "#include \"" << moduleName << ".h\"" << endl;

      for (set<PortProxy>::iterator iter = delegates.begin();
           iter != delegates.end();
           ++iter)
        {
          PortProxy proxy (*iter);
          proxy->accept (this);
        }
      this->generate_header();

      this->generate_source();

      this->generate_driver();

      this->header_ << "#endif /* " << upperModuleName << "_H" << " */" << endl;
    }
}

void
GatewayGeneratorVisitor::generate_driver()
{
  // Generate the component #includes
  string idlFile = this->get_file_name (this->comp_);
  string stubFile (idlFile + "C.h");
  this->driver_ << "#include \"" << stubFile << "\"" << endl;

  // Generate the include for the .nsmap file
  this->driver_ << "#include \"" << this->serviceName_ << ".nsmap\"" << endl;
  // Generate the port #includes
  for (map<string, Object>::iterator iter = this->interfaces_.begin();
       iter != this->interfaces_.end();
       ++iter)
    {
      Object object (iter->second);
      idlFile = this->get_file_name (object);
      stubFile = idlFile + "C.h";
      this->driver_ << "#include \"" << stubFile << "\"" << endl;
    }
  this->driver_ << endl;
  this->driver_ << "const char *bdc_ior = \"file://BDC.ior\";"
                << "int port = 8080;\n"
                << "\n"
                << "int\n"
                << "parse_args (int argc, char *argv[])\n"
                << "{"
                << "ACE_Get_Opt get_opts (argc, argv, \"k:p:\");\n"
                << "int c;\n"
                << "\n"
                << "while ((c = get_opts ()) != -1)\n"
                << "{"
                << "switch (c)\n"
                << "{"
                << "case 'k':\n"
                << "bdc_ior = get_opts.opt_arg ();\n"
                << "break;\n"
                << "\n"
                << "case 'p':\n"
                << "port = atoi (get_opts.opt_arg ());\n"
                << "break;\n"
                << "\n"
                << "case '?':  // display help for use of the server.\n"
                << "default:\n"
                << "ACE_ERROR_RETURN ((LM_ERROR,\n"
                << "\"usage:  %s\\n\"\n"
                << "\"-k <Benchmark Data Collector IOR> (default is file://BDC.ior)\\n\"\n"
                << "\"-p <Port to listen for requests (default is 8080)\\n\"\n"
                << "\"\\n\",\n"
                << "argv [0]),\n"
                << "-1);\n"
                << "}\n"
                << "}\n"
                << "return 0;\n"
                << "}\n";
  this->driver_ << "int\n"
                << "main (int argc, char *argv[])\n"
                << "{"
                << "try\n"
                << "{"
                << "// Initialize orb\n"
                << "CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, \"\");\n"
                << "if (parse_args (argc, argv) != 0)\n"
                << "{"
                << "  return -1;\n"
                << "}"
                << "CORBA::Object_var obj = orb->string_to_object (bdc_ior);\n";
  this->driver_ << this->idl_scoped_name (this->comp_) << "_var comp = "
                << this->idl_scoped_name (this->comp_)
                << "::_narrow (obj.in());";
  this->driver_ << "if (CORBA::is_nil (comp.in ()))" << endl
                << "{"
                << "ACE_ERROR_RETURN ((LM_ERROR, \"Unable to acquire "
                << this->comp_->getName() << " objref\\n\"), -1);" << endl
                << "}";
  for (map<string, Object>::iterator iter = this->interfaces_.begin();
       iter != this->interfaces_.end();
       ++iter)
    {
      Object object (iter->second);
      this->driver_ << this->idl_scoped_name (object) << "_var "
                    << iter->first << " = comp->provide_"
                    << iter->first << "();";
      this->driver_ << "if (CORBA::is_nil (" << iter->first << ".in()))" << endl
                    << "{"
                    << "ACE_ERROR_RETURN ((LM_ERROR, \"Unable to acquire "
                    << iter->first << " objref\\n\"), -1);" << endl
                    << "}";
    }
  this->driver_ << this->serviceName_ << " service (";
  size_t count = this->interfaces_.size() - 1;
  for (map<string, Object>::iterator iter = this->interfaces_.begin();
       iter != this->interfaces_.end();
       ++iter, --count)
    {
      Object object (iter->second);
      this->driver_ << iter->first << ".in()";
      if (count > 0)
        this->driver_ << ", ";
    }
  this->driver_ << ");" << endl;
  this->driver_ << "int m, s; // master and slave sockets\n"
                << "\n"
                << "m = soap_bind(&service, \"localhost\", port, 100);\n"
                << "if (m < 0)\n"
                << "  soap_print_fault(&service, stderr);\n"
                << "else\n"
                << "{"
                << "fprintf(stderr, \"Listening for SOAP requests on port = %d\\n\", port);\n"
                << "for (int i = 1; ; i++)"
                << "{"
                << "s = soap_accept(&service);"
                << "if (s < 0)"
                << "{"
                << "soap_print_fault(&service, stderr);"
                << "break;\n"
                << "}"
                << "fprintf(stderr, \"%d: Accepted connection from IP=%d.%d.%d.%d socket=%d\\n\", i,\n"
                << "(service.ip >> 24)&0xFF,\n"
                << "(service.ip >> 16)&0xFF,\n"
                << "(service.ip >> 8)&0xFF,\n"
                << "service.ip&0xFF, s);\n"
                << "if (service.serve() != SOAP_OK) // process RPC request\n"
                << "soap_print_fault(&service, stderr); // print error\n"
                << "fprintf(stderr, \"SOAP Request served\\n\");\n"
                << "soap_destroy(&service); // clean up class instances\n"
                << "soap_end(&service); // clean up everything and close socket\n"
                << "}"
                << "}"
                << "}"
                << "catch (CORBA::Exception& ex)\n"
                << "{"
                << "ACE_PRINT_EXCEPTION (ex, \"CORBA::Exception\\n\");\n"
                << "ACE_ERROR_RETURN ((LM_ERROR, \"Uncaught CORBA exception\\n\"), 1);\n"
                << "}"
                << "return 0;\n"
                << "}";
}

void
GatewayGeneratorVisitor::generate_header()
{
  // Generate the #includes
  for (map<string, Object>::iterator iter = this->interfaces_.begin();
       iter != this->interfaces_.end();
       ++iter)
    {
      Object object (iter->second);
      string idlFile = this->get_file_name (object);
      string stubFile (idlFile + "C.h");
      this->header_ << "#include \"" << stubFile << "\"" << endl;
    }
  // Generate the soap Header include
  this->header_ << "#include \"" << this->defName_ << "H.h\"" << endl;
  this->header_ << endl;

  this->header_ << "class SOAP_CMAC " << this->serviceName_ << " : public soap"
                << endl;
  this->header_ << "{" << endl;
  this->header_ << "private:" << endl << endl;

  // Generate _vars to hold all the interfaces
  for (map<string, Object>::iterator iter = this->interfaces_.begin();
       iter != this->interfaces_.end();
       ++iter)
    {
      Object object (iter->second);
      this->header_ << this->idl_scoped_name (object) + "_var "
                    << iter->first << "_;" << endl;
    }

  // Generate the member functions

  this->header_ << "public:" << endl << endl;

  // Generate the multiple interface argument constructor
  this->header_ << this->serviceName_ << " (";
  size_t count = this->interfaces_.size() - 1;
  for (map<string, Object>::iterator iter = this->interfaces_.begin();
       iter != this->interfaces_.end();
       ++iter, --count)
    {
      Object object (iter->second);
      this->header_ << this->idl_scoped_name (object) + "_ptr "
                    << iter->first;
      if (count > 0)
        this->header_ << ", ";
    }
  this->header_ << ");" << endl;

  // Generate the soap template constructors
  this->header_ << this->serviceName_ <<  "();" << endl;
  this->header_ << this->serviceName_ <<  "(soap_mode iomode);" << endl;
  this->header_ << this->serviceName_
                <<  "(soap_mode imode, soap_mode omode);" << endl;

  // Generate the destructors
  this->header_ << "virtual ~" << this->serviceName_ << "();" << endl;

  // Generate the init function
  this->header_ << "virtual void " << this->serviceName_
                << "_init (soap_mode imode, soap_mode omode);" << endl;

  // Generate the soap template functions
  this->header_ << "virtual int serve();" << endl
                << "virtual int dispatch();" << endl;

  for (set<TwowayOperation>::iterator iter  = this->operations_.begin();
       iter != this->operations_.end();
       ++iter)
    {
      TwowayOperation oper (*iter);
      string operationName = this->transform_name (oper->getName().c_str());
      this->header_ << "virtual int " << operationName << " (";
      oper->accept (this);
      this->header_ << ";" << endl;
    }
  this->header_ << "};" << endl;
}

void
GatewayGeneratorVisitor::generate_source()
{
  // Generate multiple argument constructor
  this->source_ << this->serviceName_ << "::" << this->serviceName_ << " (";
  size_t count = this->interfaces_.size() - 1;
  for (map<string, Object>::iterator iter = this->interfaces_.begin();
       iter != this->interfaces_.end();
       ++iter, --count)
    {
      Object object (iter->second);
      this->source_ << this->idl_scoped_name (object) + "_ptr "
                    << iter->first;
      if (count > 0)
        this->source_ << ", " << endl;
    }
  this->source_ << ")" << endl;
  this->source_ << ": ";
  count = this->interfaces_.size() - 1;
  for (map<string, Object>::iterator iter = this->interfaces_.begin();
       iter != this->interfaces_.end();
       ++iter, --count)
    {
      Object object (iter->second);
      this->source_ << iter->first << "_" << "("
                    << this->idl_scoped_name (object) << "::_duplicate ("
                    << iter->first << "))";
      if (count > 0)
        this->source_ << ", " << endl;
    }
  this->source_ << endl;
  this->source_ << "{";
  this->source_ << this->serviceName_ << "_init"
                << "(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);" << endl;
  this->source_ << "}";

  // Generate standard GSOAP constructors
  //
  // FooService()
  this->source_ << this->serviceName_ << "::" << this->serviceName_
                << " ()" << endl;
  this->source_ << "{";
  this->source_ << this->serviceName_ << "_init"
                << "(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);" << endl;
  this->source_ << "}";

  // FooService(soap_mode iomode)
  this->source_ << this->serviceName_ << "::" << this->serviceName_
                << " (soap_mode iomode)" << endl;
  this->source_ << "{";
  this->source_ << this->serviceName_ << "_init" << "(iomode, iomode);" << endl;
  this->source_ << "}";

  // FooService(soap_mode imode, soap_mode omode)
  this->source_ << this->serviceName_ << "::" << this->serviceName_
                << " (soap_mode imode, soap_mode omode)" << endl;
  this->source_ << "{";
  this->source_ << this->serviceName_ << "_init" << "(imode, omode);" << endl;
  this->source_ << "}";

  // FooService_init (soap_mode imode, soap_mode omode)
  this->source_ << "void " << this->serviceName_  << "::" << this->serviceName_
                << "_init"
                << " (soap_mode imode, soap_mode omode)" << endl;
  this->source_ << "{";
  this->source_ << "static const struct Namespace namespaces[] = \n"
                << "{"
                << "{\"SOAP-ENV\", \"http://schemas.xmlsoap.org/soap/envelope/\", \"http://www.w3.org/*/soap-envelope\", NULL},"
                << "{\"SOAP-ENC\", \"http://schemas.xmlsoap.org/soap/encoding/\", \"http://www.w3.org/*/soap-encoding\", NULL},"
                << "{\"xsi\", \"http://www.w3.org/2001/XMLSchema-instance\", \"http://www.w3.org/*/XMLSchema-instance\", NULL},"
                << "{\"xsd\", \"http://www.w3.org/2001/XMLSchema\", \"http://www.w3.org/*/XMLSchema\", NULL},"
                << "{\"CORBA\", \"http://www.omg.org/IDL-WSDL/1.0/\", NULL, NULL},"
                << "{\"" << this->defName_ << "\"" << "," <<  "\"" << this->targetNamespace_ << "\", NULL, NULL},"
                << "{NULL, NULL, NULL, NULL}"
                << "};"
                << "soap_init2(this, imode, omode);"
                << "if (!this->namespaces)\n"
                << "this->namespaces = namespaces;\n"
                << "};\n";
  // ~FooService()
  this->source_ << this->serviceName_ << "::~" << this->serviceName_ << "()"
                << endl;
  this->source_ << "{";
  this->source_ << "soap_destroy(this);";
  this->source_ << "soap_end(this);";
  this->source_ << "soap_done(this);";
  this->source_ << "}";

  // FooService::serve()
  this->source_ << "int " << this->serviceName_ << "::serve()\n"
                << "{"
                << "#ifndef WITH_FASTCGI\n"
                << "unsigned int k = this->max_keep_alive;\n"
                << "#endif\n"
                << "do\n"
                << "{"
                << "soap_begin(this);\n"
                << "#ifdef WITH_FASTCGI\n"
                << "if (FCGI_Accept() < 0)\n"
                << "{"
                << "  this->error = SOAP_EOF;\n"
                << "  return soap_send_fault(this);\n"
                << "}"
                << "#endif\n"
                << "\n"
                << "soap_begin(this);\n"
                << "\n"
                << "#ifndef WITH_FASTCGI\n"
                << "if (this->max_keep_alive > 0 && !--k)\n"
                << "this->keep_alive = 0;\n"
                << "#endif\n"
                << "\n"
                << "if (soap_begin_recv(this))\n"
                << "{"
                << "if (this->error < SOAP_STOP)\n"
                << "{"
                << "#ifdef WITH_FASTCGI\n"
                << "soap_send_fault(this);\n"
                << "#else\n"
                << "return soap_send_fault(this);\n"
                << "#endif\n"
                << "}"
                << "soap_closesock(this);\n"
                << "\n"
                << "continue;\n"
                << "}"
                << "\n"
                << "if (soap_envelope_begin_in(this)\n"
                << "|| soap_recv_header(this)\n"
                << "|| soap_body_begin_in(this)\n"
                << "|| dispatch() || (this->fserveloop && this->fserveloop(this)))\n"
                << "{"
                << "#ifdef WITH_FASTCGI\n"
                << "soap_send_fault(this);\n"
                << "#else\n"
                << "return soap_send_fault(this);\n"
                << "#endif\n"
                << "}"
                << "\n"
                << "#ifdef WITH_FASTCGI\n"
                << "} while (1);\n"
                << "#else\n"
                << "} while (this->keep_alive);\n"
                << "#endif\n"
                << "return SOAP_OK;\n"
                << "}";
  // Generate static function forward declarations
  for (set<TwowayOperation>::iterator iter  = this->operations_.begin();
       iter != this->operations_.end();
       ++iter)
    {
      this->source_ << "static int serve_" << this->defName_ << "__";
      TwowayOperation oper (*iter);
      string operationName = this->transform_name (oper->getName().c_str());
      this->source_ << operationName << "(" << this->serviceName_ << "*);\n";
    }
  // Generate FooService::dispatch()
  this->source_ << "int " << this->serviceName_ << "::dispatch()" << endl;
  this->source_ << "{";
  this->source_ << "if (soap_peek_element(this))" << endl
                << "return this->error;" << endl;
  for (set<TwowayOperation>::iterator iter  = this->operations_.begin();
       iter != this->operations_.end();
       ++iter)
    {
      TwowayOperation oper (*iter);
      string operationName = this->transform_name (oper->getName().c_str());
      this->source_ << "if (!soap_match_tag(this, this->tag, ";
      this->source_ << "\"" << this->defName_ << ":"
                    << oper->getName() << "\"))" << endl
                    << "return serve_" << this->defName_ << "__"
                    << operationName << "(this);" << endl;
    }
  this->source_ << "return this->error = SOAP_NO_METHOD;";
  this->source_ << "}";

  // Generate serve_FooServiceMethod
  for (set<TwowayOperation>::iterator iter  = this->operations_.begin();
       iter != this->operations_.end();
       ++iter)
    {
      TwowayOperation oper (*iter);
      string operationName = this->transform_name (oper->getName().c_str());
      this->source_ << "static int serve_" << this->defName_ << "__";
      this->source_ << operationName << "(" << this->serviceName_ << "*soap)";
      this->source_ << "{";
      this->source_ << "struct " << this->defName_ << "__"
                    << operationName << " request;";
      this->source_ << "struct " << this->defName_ << "__"
                    << operationName << "Response response;";
      this->source_ << "soap_default_" << this->defName_ << "__"
                    << operationName << "Response (soap, &response);";
      this->source_ << "soap_default_" << this->defName_ << "__"
                    << operationName << "(soap, &request);";
      this->source_ << "soap->encodingStyle = \"http://schemas.xmlsoap.org/soap/encoding/\";";
      this->source_ << "if (!soap_get_" << this->defName_ << "__"
                    << operationName << "(soap, &request, "
                    << "\"" << this->defName_ << ":" << oper->getName()
                    << "\", NULL))" << endl;
      this->source_ << "return soap->error;" << endl;
      this->source_ << "if (soap_body_end_in(soap)\n"
                    << "|| soap_envelope_end_in(soap)\n"
                    << "|| soap_end_recv(soap))\n"
                    << "return soap->error;" << endl
                    << "soap->error = soap->"
                    << operationName << "(";
      set<InParameter> inputParams = oper->getInParameter();
      for (set<InParameter>::iterator iter = inputParams.begin();
           iter != inputParams.end();
           ++iter)
        {
          InParameter inputParam (*iter);
          this->source_ << "request."
                        << this->transform_name (inputParam->getName().c_str())
                        << ", " << endl;
        }
      string response;
      set<ReturnType> retTypes = oper->getReturnType();
      for (set<ReturnType>::iterator returnIter = retTypes.begin();
           returnIter != retTypes.end();
           ++returnIter)
        {
          ReturnType retType (*returnIter);
          MemberType returnType = retType->getMemberType();
          string retTypeName = this->wsdl_scoped_name (returnType);
          if (PredefinedType (returnType))
            {
              response = "response.";
              response += this->transform_name ("_return");
            }
        }
      if (!response.empty())
        this->source_ << response << ");";
      else
        this->source_ << "response);";
      this->source_ << "if (soap->error)\n"
                    << "return soap->error;\n"
                    << "soap_serializeheader(soap);\n";
      this->source_ << "soap_serialize_" << this->defName_ << "__"
                    << operationName << "Response (soap, &response);\n";
      this->source_ << "if (soap_begin_count(soap))\n"
                    << "return soap->error;\n"
                    << "  if (soap->mode & SOAP_IO_LENGTH)\n"
                    << "{"
                    << "if (soap_envelope_begin_out(soap)\n"
                    << "|| soap_putheader(soap)\n"
                    << "|| soap_body_begin_out(soap)\n"
                    << "|| soap_put_" << this->defName_ << "__"
                    << operationName << "Response (soap, &response, "
                    << "\"" << this->defName_ << ":" << oper->getName()
                    << "Response\",\"\")" << endl
                    << "|| soap_body_end_out(soap)\n"
                    << "|| soap_envelope_end_out(soap))\n"
                    << "return soap->error;\n"
                    << "}";
      this->source_ << "if (soap_end_count(soap)\n"
                    << "|| soap_response(soap, SOAP_OK)\n"
                    << "|| soap_envelope_begin_out(soap)\n"
                    << "|| soap_putheader(soap)\n"
                    << "|| soap_body_begin_out(soap)\n"
                    << "|| soap_put_" << this->defName_ << "__"
                    << operationName << "Response (soap, &response, "
                    << "\"" << this->defName_ << ":" << oper->getName()
                    << "Response\",\"\")" << endl
                    << "|| soap_body_end_out(soap)\n"
                    << "|| soap_envelope_end_out(soap)\n"
                    << "|| soap_end_send(soap))\n"
                    << "return soap->error;\n"
                    << "return soap_closesock(soap);"
                    << "}";
    }

  // Generate the actual skeleton implementations...

  // Generate the function signature
  for (set<TwowayOperation>::iterator iter  = this->operations_.begin();
       iter != this->operations_.end();
       ++iter)
    {
      TwowayOperation oper (*iter);
      string operationName = this->transform_name (oper->getName().c_str());
      this->source_ << "int " << this->serviceName_ << "::"
                    << operationName << " (";
      set<InParameter> inputParams = oper->getInParameter();
      vector<InParameter> sortedParams;
      copy (inputParams.begin(),
            inputParams.end(),
            back_inserter (sortedParams));
      sort(sortedParams.begin(),
           sortedParams.end(),
           ParamSorter<InParameter>());
      for (vector<InParameter>::iterator paramIter = sortedParams.begin();
           paramIter != sortedParams.end();
           ++paramIter)
        {
          InParameter inputParam (*paramIter);
          MemberType paramType = inputParam->getMemberType();
          string paramTypeName = this->wsdl_scoped_name (paramType);
          if (PredefinedType (paramType))
            {
              if (ShortInteger (paramType))
                this->source_ << "unsigned ";
              this->source_ << this->transform_name (paramTypeName.c_str()) << " ";
            }
          else
            {
              this->source_ << this->transform_name (paramTypeName.c_str())
                            << "* ";
            }
          this->source_ << this->transform_name (inputParam->getName().c_str())
                        << ", " << endl;
        }
      string returntypes;
      set<ReturnType> retTypes = oper->getReturnType();
      for (set<ReturnType>::iterator returnIter = retTypes.begin();
           returnIter != retTypes.end();
           ++returnIter)
        {
          ReturnType retType (*returnIter);
          MemberType returnType = retType->getMemberType();
          string retTypeName = this->wsdl_scoped_name (returnType);
          if (PredefinedType (returnType))
            {
              if (ShortInteger (returnType))
                returntypes += "unsigned ";
              returntypes += this->transform_name (retTypeName.c_str());
              returntypes += "& ";
              returntypes += this->transform_name ("_return");
              returntypes += ")";
            }
        }
      // Generate the response struct only if no basic type is returned
      if (returntypes.empty())
        {
          this->source_ << "struct " << this->defName_ << "__" << operationName
                        << "Response& _param_1)";
        }
      else
        {
          this->source_ << returntypes << endl;
        }
      this->source_ << "{";
      // Generate the function body
      this->source_ << "try" << endl;
      this->source_ << "{";
      vector<string> arguments;
      string retval;
      MemberType returnType;
      for (vector<InParameter>::iterator iter1 = sortedParams.begin();
           iter1 != sortedParams.end();
           ++iter1)
        {
          InParameter inputParam (*iter1);
          string soapName = this->transform_name (inputParam->getName().c_str());
          string corbaName = inputParam->getName();
          MemberType paramType = inputParam->getMemberType();
          string paramTypeName = this->wsdl_scoped_name (paramType);
          if (Collection (paramType))
            {
              Collection sequence = Collection (paramType);
              corbaName +=  "Seq";
              this->soap_seq_to_corba_seq (sequence,
                                           soapName,
                                           corbaName,
                                           this->source_);
            }
          arguments.push_back (corbaName);
        }
      retTypes = oper->getReturnType();
      for (set<ReturnType>::iterator iter1 = retTypes.begin();
           iter1 != retTypes.end();
           ++iter1)
        {
          ReturnType retType (*iter1);
          returnType = retType->getMemberType();
          string retTypeName = this->wsdl_scoped_name (returnType);
          if (PredefinedType (returnType))
            {
              this->source_ << this->transform_name ("_return") << " = ";
            }
          else if (Aggregate (returnType))
            {
              Aggregate agg = Aggregate (returnType);
              retval = this->idl_scoped_name (agg) + " _return = ";
            }
        }
      if (!retval.empty())
        {
          this->source_ << retval;
        }
      string portName = this->port_[oper] + "_";
      this->source_ << "this->" << portName << "->"
                    << oper->getName() << " (";
      size_t count = arguments.size() - 1;
      for (vector<string>::iterator argIter = arguments.begin();
           argIter != arguments.end();
           ++argIter, --count)
        {
          this->source_ << this->transform_name ((*argIter).c_str());
          if (count > 0)
            this->source_ << ", ";
        }
      this->source_ << ");";
      if (returnType)
        {
          if (Aggregate (returnType))
            {
              Aggregate agg = Aggregate (returnType);
              this->corba_struct_to_soap_struct (agg,
                                                 "_param_1._USCOREreturn",
                                                 "_return",
                                                 this->source_);
            }
        }
      this->source_ << "}";
      set<ExceptionRef> exceptions = oper->getExceptionRef();
      for (set<ExceptionRef>::iterator exIter = exceptions.begin();
           exIter != exceptions.end();
           ++exIter)
        {
          ExceptionRef exceptionRef (*exIter);
          Exception exception (exceptionRef->getException());
          string exceptioName = this->idl_scoped_name (exception);
          this->source_ << "catch (" << exceptioName << "& ex)" << endl;
          this->source_ << "{";
          this->source_ << "ACE_PRINT_EXCEPTION (ex, "
                        << "\"" << oper->getName() << "\");";
          this->source_ << "string msg (ex._info().c_str());";
          this->source_ << "return soap_receiver_fault (this,"
                        << "\"An exception occurred: \", msg.c_str());";
          this->source_ << "}";
        }
      this->source_ << "catch (CORBA::Exception& ex)" << endl;
      this->source_ << "{";
      this->source_ << "ACE_PRINT_EXCEPTION (ex, "
                    << "\"" << oper->getName() << "\"" << ");";
      this->source_ << "string msg (ex._info().c_str());";
      this->source_ << "return soap_receiver_fault (this,"
                    << "\"An exception occurred: \", msg.c_str());";
      this->source_ << "}";
      this->source_ << "return SOAP_OK;";
      this->source_ << "}" << endl;
    }
}


void
GatewayGeneratorVisitor::corba_struct_to_soap_struct (const Aggregate& agg,
                                                      const string& soapName,
                                                      const string& corbaName,
                                                      ofstream& os)
{
  string scopedName = this->wsdl_scoped_name (agg);
  string aggName = this->transform_name (scopedName.c_str());
  os << soapName << " = new " << this->defName_ << "__" << aggName << "();";
  set<Member> members = agg->getMember();
  for (set<Member>::iterator iter = members.begin();
       iter != members.end();
       ++iter)
    {
      Member member (*iter);
      MemberType memberType = member->getMemberType();
      string soap_hack = this->transform_name (member->getName().c_str());
      if (soap_hack == "max" || soap_hack == "min")
        {
          soap_hack += "_";
        }
      string lhs = soapName + "->" + soap_hack;
      string rhs = corbaName + "." + member->getName();
      if (PredefinedType (memberType))
        {
          if (String (memberType))
            {
              os << lhs << " = soap_strdup (this, " << rhs << ");";
            }
          else
            {
              os << lhs << " = " << rhs << ";";
            }
        }
      else if (Aggregate (memberType))
        {
          Aggregate child (memberType);
          this->corba_struct_to_soap_struct (child, lhs, rhs, os);
        }
      else if (Alias (memberType))
        {
          Alias typeDef (memberType);
          MemberType memberType = typeDef->getMemberType();
          if (PredefinedType (memberType))
            {
              if (String (memberType))
                {
                  os << lhs << " = soap_strdup (this, " << rhs << ");";
                }
              else
                {
                  os << lhs << " = " << rhs << ";";
                }
            }
        }
      else
        {
          stringstream msg;
          msg << "Unhandled CORBA type " << memberType->getName()
              << " in struct " << agg->getName() << endl;
          this->project_->consoleMsg (msg.str(), MSG_ERROR);
          throw GatewayGeneratorException();
        }
    }
}

void
GatewayGeneratorVisitor::soap_seq_to_corba_seq (const Collection& seq,
                                                const string& soapName,
                                                const string& corbaName,
                                                ofstream& os)
{
  MemberType seqType = seq->getMemberType();
  string scopedName = this->wsdl_scoped_name (seqType);
  string seqTypeName = this->transform_name (scopedName.c_str());
  os << this->defName_ << "__" << seqTypeName << "** array = "
     << soapName << "->__ptritem;";
  os << "int array_len = " << soapName << "->__size;";
  os << this->idl_scoped_name (seq) << " " << corbaName << ";";
  os << "for (int i = 0; i < array_len; ++i)" << endl;
  os << "{";
  os << this->defName_ << "__" << seqTypeName << " element = *(*(array+i));";
  os << this->idl_scoped_name (seqType) << " "
     << corbaName << "Element;";
  Aggregate agg = Aggregate (seqType);
  if (!agg)
    {
      stringstream msg;
      msg << "Unhandled CORBA type " << seqType->getName() << "in sequence "
          << seq->getName() << endl;
      this->project_->consoleMsg (msg.str(), MSG_ERROR);
      throw GatewayGeneratorException();
    }
  this->soap_struct_to_corba_struct (agg, "element", corbaName + "Element",
                                     os);
  os << "CORBA::ULong len = " << corbaName << ".length();";
  os << corbaName << ".length(len + 1);";
  os << corbaName << "[len] = " << corbaName << "Element;";
  os << "}";
}

void
GatewayGeneratorVisitor::corba_seq_to_soap_seq (const Collection& seq,
                                                const string& soapName,
                                                const string& corbaName,
                                                ofstream& os)
{

}

void
GatewayGeneratorVisitor::soap_struct_to_corba_struct (const Aggregate& agg,
                                                      const string& soapName,
                                                      const string& corbaName,
                                                      ofstream& os)
{
  set<Member> members = agg->getMember();
  for (set<Member>::iterator iter = members.begin();
       iter != members.end();
       ++iter)
    {
      Member member (*iter);
      MemberType memberType = member->getMemberType();
      string lhs = corbaName + "." + member->getName();
      string rhs = soapName + "." + member->getName();
      if (PredefinedType (memberType))
        {
          if (String (memberType))
            {
              os << lhs << " = " << "CORBA::string_dup (" << rhs << ");";
            }
          else
            {
              os << lhs << " = " << rhs << ";";
            }
        }
      else
        {
          stringstream msg;
          msg << "Unhandled CORBA type " << memberType->getName() << "in struct "
              << agg->getName() << endl;
          this->project_->consoleMsg (msg.str(), MSG_ERROR);
          throw GatewayGeneratorException();
        }
    }
}


void
GatewayGeneratorVisitor::visitPortProxy(const PortProxy& object)
{
  Module target = object->getDst();
  string dstPortName = object->getdstPortName();
  string srcPortName = object->getsrcPortName();
  bool portFound = false;
  bool assemblyFound = false;
  set<BON::Object> impls = this->project_->findByKind ("PICML::ComponentAssembly");
  for (set<BON::Object>::iterator iter = impls.begin();
       iter != impls.end();
       ++iter)
    {
      ComponentAssembly assembly (*iter);
      if (!assembly)
        {
          stringstream msg;
          msg << "Unable to create assembly corresponding to module "
              << target->getName() << endl;
          this->project_->consoleMsg (msg.str(), MSG_ERROR);
          throw GatewayGeneratorException();
        }
      if (assembly->getName() == target->getName())
        {
          set<ProvidedRequestPort> facets = assembly->getProvidedRequestPort();
          for (set<ProvidedRequestPort>::iterator iter1 = facets.begin();
               iter1 != facets.end();
               ++iter1)
            {
              ProvidedRequestPort facet (*iter1);
              if (!facet)
                {
                  stringstream msg;
                  msg << "Unable to get facets of assembly "
                      << assembly->getName() << endl;
                  this->project_->consoleMsg (msg.str(), MSG_ERROR);
                  throw GatewayGeneratorException();
                }
              if (facet->getName() == dstPortName)
                {
                  portFound = true;
                  Object iface = facet->getReferred();
                  if (!iface)
                    {
                      stringstream msg;
                      msg << "Unable to get referred interface of facet "
                          << facet->getName() << endl;
                      this->project_->consoleMsg (msg.str(),
                                                  MSG_ERROR);
                      throw GatewayGeneratorException();
                    }
                  this->interfaces_[facet->getName()] = iface;
                  set<TwowayOperation>
                    operations = iface->getTwowayOperation();
                  this->operations_.insert (operations.begin(),
                                            operations.end());
                  for (set<TwowayOperation>::iterator op = operations.begin();
                       op != operations.end();
                       ++op)
                    {
                      TwowayOperation oper (*op);
                      this->port_[oper] = facet->getName();
                    }
                  set<FacetDelegate> fdels = facet->getFacetDelegateLinks();
                  for (set<FacetDelegate>::iterator iter2 = fdels.begin();
                       iter2 != fdels.end();
                       ++iter2)
                    {
                      FacetDelegate del (*iter2);
                      ProvidedRequestPort compFacet (del->getSrc());
                      Component comp (compFacet->getParentModel("PICML::Component"));
                      if (!comp)
                        {
                          compFacet = del->getDst();
                          comp = compFacet->getParentModel ("PICML::Component");
                          if (!comp)
                            {
                              stringstream msg;
                              msg << "Unable to find destination component "
                                  << "in assembly "
                                  << assembly->getName() << endl;
                              this->project_->consoleMsg (msg.str(), MSG_ERROR);
                              throw GatewayGeneratorException();
                            }
                        }
                      if (comp->getTypeInhObject()->isInstance() == false)
                        {
                          stringstream msg;
                          msg << "Unable to find destination component "
                              << "in assembly "
                              << assembly->getName() << endl;
                          this->project_->consoleMsg (msg.str(), MSG_ERROR);
                          throw GatewayGeneratorException();
                        }
                      this->comp_ = comp->getTypeInhObject()->getType()->getFCO();
                      return;
                    }
                }
            }
          if (!portFound)
            {
              stringstream msg;
              msg << "Unable to find destination port "
                  << dstPortName << " in assembly " << assembly->getName() << endl;
              this->project_->consoleMsg (msg.str(), MSG_ERROR);
              throw GatewayGeneratorException();
            }
        }
    }
  if (!assemblyFound)
    {
      stringstream msg;
      msg << "Unable to find destination port "
          << dstPortName << " in assembly " << target->getName() << endl;
      this->project_->consoleMsg (msg.str(), MSG_ERROR);
      throw GatewayGeneratorException();
    }
}

void
GatewayGeneratorVisitor::visitObject (const Object& object)
{

}


void
GatewayGeneratorVisitor::visitTwowayOperation(const TwowayOperation& object)
{
  string operationName = this->transform_name (object->getName().c_str());
  set<InParameter> inputParams = object->getInParameter();
  vector<InParameter> sortedParams;
  copy (inputParams.begin(), inputParams.end(), back_inserter (sortedParams));
  sort(sortedParams.begin(), sortedParams.end(), ParamSorter<InParameter>());
  for (vector<InParameter>::iterator iter = sortedParams.begin();
       iter != sortedParams.end();
       ++iter)
    {
      InParameter inputParam (*iter);
      MemberType paramType = inputParam->getMemberType();
      string paramTypeName = this->wsdl_scoped_name (paramType);
      if (PredefinedType (paramType))
        {
          if (ShortInteger (paramType))
            this->header_ << "unsigned ";
          this->header_ << this->transform_name (paramTypeName.c_str()) << " ";
        }
      else
        {
          this->header_ << this->transform_name (paramTypeName.c_str())
                        << "* ";
        }
      this->header_ << this->transform_name (inputParam->getName().c_str())
                    << ", " << endl;
    }
  set<ReturnType> retTypes = object->getReturnType();
  for (set<ReturnType>::iterator iter = retTypes.begin();
       iter != retTypes.end();
       ++iter)
    {
      ReturnType retType (*iter);
      MemberType returnType = retType->getMemberType();
      string retTypeName = this->wsdl_scoped_name (returnType);
      if (PredefinedType (returnType))
        {
          if (ShortInteger (returnType))
            this->header_ << "unsigned ";
          this->header_ << this->transform_name (retTypeName.c_str()) << "& ";
          this->header_ << this->transform_name ("_return") << ");" << endl;
          return;
        }
    }
  // Generate the response struct
  this->header_ << "struct " << this->defName_ << "__" << operationName
                << "Response& _param_1)";
}
