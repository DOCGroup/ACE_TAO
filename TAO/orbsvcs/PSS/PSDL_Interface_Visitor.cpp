// $Id$

#include "PSDL_Interface_Visitor.h"
#include "PSDL_Stream.h"

TAO_PSDL_Interface_Visitor::TAO_PSDL_Interface_Visitor (void)
  : forward_dcl_ (0),
    interface_name_ ()
{
  // constructor.
}

TAO_PSDL_Interface_Visitor::~TAO_PSDL_Interface_Visitor (void)
{
  // destructor.
}

int
TAO_PSDL_Interface_Visitor::visit_identifier (TAO_PSDL_Identifier *identifier)
{
  this->identifiers_.size (1);
  this->count_ = 0;

  // Save the identifier.
  this->identifiers_ [this->count_] = identifier->value ();

  return 0;
}

int
TAO_PSDL_Interface_Visitor::visit_forward_dcl (TAO_PSDL_Forward_Dcl *forward_dcl)
{
  if (forward_dcl->identifier ()->accept (this) == -1)
    return -1;

  this->interface_name_ = this->identifiers_[this->count_];

  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  ps_sh->reset ();

  *ps_sh << "class " << this->interface_name_ << ";";
  ps_sh->nl ();

  this->forward_dcl_ = 1;
  this->print_for_forward_dcl ();

  return 0;
}

int
TAO_PSDL_Interface_Visitor::visit_interface_dcl (TAO_PSDL_Interface_Dcl *interface_dcl)
{
  if (interface_dcl->interface_header ()->accept (this) == -1)
    return -1;

  if (interface_dcl->interface_body () != 0)
    {
      if (interface_dcl->interface_body ()->accept (this) == -1)
        return -1;
    }

  // The generated code for interfaces.
  this->print_end_for_interface ();
  return 0;
}

int
TAO_PSDL_Interface_Visitor::visit_interface_header (TAO_PSDL_Interface_Header *interface_header)
{
  if (interface_header->identifier ()->accept (this) == -1)
    return -1;

  this->interface_name_ = this->identifiers_[this->count_];
  TAO_PSDL_Scope::instance ()->set_interface_name (this->interface_name_);
  this->print_class_for_interface ();

  if (interface_header->type () != 0)
    {
      if (interface_header->type ()->accept (this) == -1)
        return -1;
    }

  if (interface_header->interface_inheritance_spec () != 0)
    {
      if (interface_header->interface_inheritance_spec ()->accept (this) == -1)
        return -1;
    }

  return 0;
}

void
TAO_PSDL_Interface_Visitor::print_for_forward_dcl (void)
{
  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  ps_sh->reset ();

  ps_sh->nl ();

  ACE_CString name_space = TAO_PSDL_Scope::instance ()->get_name_space ();

  *ps_sh << "#if !defined (_" << name_space << "_" << this->interface_name_
         << "___PTR_CH_)";  ps_sh->nl ();

  *ps_sh << "#define _" << name_space << "_"
         << this->interface_name_ << "___PTR_CH_";

  ps_sh->nl ();
  ps_sh->nl ();

  if (this->forward_dcl_ != 1)
    {
      *ps_sh << "class " << this->interface_name_ << ";";  ps_sh->nl ();
    }

  *ps_sh << "typedef " << this->interface_name_ << "*"
         << this->interface_name_ << "_ptr;";
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "#endif /* end #if !defined */" << "\n\n";  ps_sh->nl ();

  *ps_sh << "#if !defined (_" << name_space << "_"
         << this->interface_name_ << "___VAR_CH_)";
  ps_sh->nl ();

  *ps_sh << "#define _" << name_space << "_"
         << this->interface_name_ << "___VAR_CH_";
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "class " << this->interface_name_.c_str ()
         << "_var : public TAO_Base_Var";  ps_sh->nl ();

  *ps_sh << "{";
  ps_sh->nl ();
  ps_sh->incr_indent (0);

  *ps_sh << "public:";
  ps_sh->nl ();

  *ps_sh  <<  this->interface_name_ << "_var (void);"
          << "// default constructor";
  ps_sh->nl ();

  *ps_sh  <<  this->interface_name_ << "_var (" << this->interface_name_
          << "_ptr p) : ptr_ (p) {}";
  ps_sh->nl ();

  *ps_sh  <<  this->interface_name_ << "_var (const "
          << this->interface_name_
          << "_var &);" << "// copy constructor";
  ps_sh->nl ();

  *ps_sh << " ~" << this->interface_name_ << "_var (void"
         << "); // destructor";
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_var"
         << "&operator= (" << this->interface_name_
         << "_ptr);";  ps_sh->nl ();

  *ps_sh  << this->interface_name_ << "_var"
          << "&operator= (const " << this->interface_name_
          << "_var &);";
  ps_sh->nl ();

  *ps_sh  << this->interface_name_
          << "_ptr operator-> (void) const;";
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "operator const "
         << this->interface_name_ << "_ptr &() const;";  ps_sh->nl ();

  *ps_sh << "operator "
         << this->interface_name_ << "_ptr &();";

  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "// in, inout, out, _retn";  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_ptr in (void) const;";
  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_ptr &inout (void);";
  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_ptr &out (void);";
  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_ptr _retn (void);";
  ps_sh->nl ();

  *ps_sh <<  this->interface_name_ << "_ptr ptr (void) const;";
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "// Hooks used by template sequence and object manager classes";
  ps_sh->nl ();

  *ps_sh << "// for non-defined forward declared interfaces.";  ps_sh->nl ();

  *ps_sh << "static " << this->interface_name_ << "_ptr tao_duplicate ("
         << this->interface_name_ << "_ptr);";
  ps_sh->nl ();

  *ps_sh << "static void tao_release ("
         << this->interface_name_ << "_ptr);";
  ps_sh->nl ();

  *ps_sh << "static " << this->interface_name_
         << "_ptr tao_nil (void);";  ps_sh->nl ();

  *ps_sh << "static " << this->interface_name_ << "_ptr tao_narrow (";
  ps_sh->incr_indent (0);
  ps_sh->nl ();

  *ps_sh << "CORBA::Object *";
  ps_sh->nl ();
  *ps_sh << "";

  ps_sh->decr_indent (0);
  ps_sh->nl ();

  *ps_sh << ");";
  ps_sh->nl ();

  *ps_sh << "static CORBA::Object * tao_upcast (void *);";

  ps_sh->reset ();
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "private:";
  ps_sh->incr_indent (0);
  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_ptr ptr_;";
  ps_sh->nl ();

  *ps_sh << "// Unimplemented - prevents widening assignment.";
  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_var (const TAO_Base_var &rhs);";
  ps_sh->nl ();

  *ps_sh << this->interface_name_
         << "_var &operator= (const TAO_Base_var &rhs);";
  ps_sh->nl ();

  ps_sh->decr_indent (0);
  ps_sh->nl ();

  *ps_sh << "};";
  ps_sh->nl ();

  *ps_sh << "#endif /* end #if !defined */";
  ps_sh->nl ();
  ps_sh->nl ();

  ps_sh->reset ();

  *ps_sh << "#if !defined (_" << name_space
         << "_" << this->interface_name_ << "___OUT_CH_)";
  ps_sh->nl ();

  *ps_sh << "#define _" << name_space
         << "_" << this->interface_name_ << "___OUT_CH_";
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "class  " << this->interface_name_ << "_out";  ps_sh->nl ();
  *ps_sh << "{";
  ps_sh->nl ();
  ps_sh->incr_indent (0);

  *ps_sh << "public:";  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "_out (" << this->interface_name_
         << "_ptr &);";  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "_out (" << this->interface_name_
         << "_var &);";  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "_out (const "
         << this->interface_name_ << "_out &);";  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "_out &operator= (const "
         << this->interface_name_ << "_out &);";  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "_out &operator= (const "
         << this->interface_name_ << "_var &);";  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "_out &operator= ("
         << this->interface_name_ << "_ptr);";  ps_sh->nl ();
  *ps_sh << "operator " << this->interface_name_ << "_ptr &();";  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "_ptr &ptr (void);";  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_ptr operator-> (void);";

  ps_sh->reset ();
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "private:";
  ps_sh->incr_indent (0);
  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "_ptr &ptr_;";  ps_sh->nl ();

  ps_sh->decr_indent (0);
  ps_sh->nl ();

  *ps_sh << "};\n";

  *ps_sh << "#endif /* end #if !defined */";

  ps_sh->reset ();

  ps_sh->nl ();
  ps_sh->nl ();


  *ps_sh << "// External declarations for undefined interface";
  ps_sh->nl ();
  *ps_sh << "//" << this->interface_name_;
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_ptr";
  ps_sh->nl ();

  *ps_sh << "tao_" << this->interface_name_ << "_duplicate (";
  ps_sh->incr_indent (0);
  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_ptr";

  ps_sh->decr_indent (0);
  ps_sh->nl ();
  *ps_sh << ");";

  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "void";
  ps_sh->nl ();
  *ps_sh << "tao_" << this->interface_name_ << "_release (";
  ps_sh->incr_indent (0);
  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "_ptr";
  ps_sh->decr_indent (0);
  ps_sh->nl ();
  *ps_sh << ");";

  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_ptr";
  ps_sh->nl ();

  *ps_sh << "tao_" << this->interface_name_ << "_nil (";
  ps_sh->incr_indent (0);
  ps_sh->nl ();
  *ps_sh << "void";
  ps_sh->decr_indent (0);
  ps_sh->nl ();
  *ps_sh << ");";

  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "_ptr";
  ps_sh->nl ();

  *ps_sh << "tao_" << this->interface_name_ << "_narrow (";
  ps_sh->incr_indent (0);
  ps_sh->nl ();

  *ps_sh << "CORBA::Object *";
  ps_sh->nl ();

  *ps_sh << "";

  ps_sh->decr_indent (0);
  ps_sh->nl ();
  *ps_sh << ");";

  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "CORBA::Object *";
  ps_sh->nl ();
  *ps_sh << "tao_" << this->interface_name_ << "_upcast (";
  ps_sh->incr_indent (0);
  ps_sh->nl ();
  *ps_sh << "void *";
  ps_sh->decr_indent (0);
  ps_sh->nl ();
  *ps_sh << ");";

  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << "// Any operators for interface " << this->interface_name_;
  ps_sh->nl ();

  *ps_sh << "void operator<<= (CORBA::Any &, " << this->interface_name_ << "_ptr);";
  ps_sh->nl ();

  *ps_sh << "CORBA::Boolean operator>>= (const CORBA::Any &, " << this->interface_name_ << " *&);";
  ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "#ifndef __ACE_INLINE__";
  ps_sh->nl ();
  ps_sh->nl ();

  *ps_sh << " CORBA::Boolean operator<< (TAO_OutputCDR &, const " << this->interface_name_ << "_ptr );";
  ps_sh->nl ();
  *ps_sh << " CORBA::Boolean operator>> (TAO_InputCDR &, " << this->interface_name_ << "_ptr &);";
  ps_sh->nl ();
  ps_sh->nl ();
  *ps_sh << "#endif /* __ACE_INLINE__ */";
  ps_sh->nl ();

}

void
TAO_PSDL_Interface_Visitor::print_class_for_interface (void)
{
  this->print_for_forward_dcl ();

  ACE_CString name_space = TAO_PSDL_Scope::instance ()->get_name_space ();

  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  *ps_sh << "#if !defined (_" << name_space << "_" << this->interface_name_
         << "_CH_)";  ps_sh->nl ();
  *ps_sh << "#define _" << name_space << this->interface_name_
         << "_CH_";  ps_sh->nl ();

  *ps_sh << "// Forward Classes Declaration.";  ps_sh->nl ();
  *ps_sh << "class _TAO_" << this->interface_name_ << "_Proxy_Impl;";  ps_sh->nl ();
  *ps_sh << "class _TAO_" << this->interface_name_ << "_Remote_Proxy_Impl;";  ps_sh->nl ();
  *ps_sh << "class _TAO_" << this->interface_name_ << "_Proxy_Broker;";  ps_sh->nl ();
  *ps_sh << "class _TAO_" << this->interface_name_ << "_Remote_Proxy_Broker;";  ps_sh->nl ();


  *ps_sh << "class  " << this->interface_name_;
  *ps_sh << ": public virtual CORBA::Object";  ps_sh->nl ();

  *ps_sh << "{";
  ps_sh->nl ();
  ps_sh->incr_indent (0);

  *ps_sh << "public:";  ps_sh->nl ();

  *ps_sh << "typedef " << this->interface_name_ << "_ptr _ptr_type;";  ps_sh->nl ();
  *ps_sh << "typedef " << this->interface_name_ << "_var _var_type;";  ps_sh->nl ();

  *ps_sh << "static int _tao_class_id;";  ps_sh->nl ();

  *ps_sh << "// The static operations.";  ps_sh->nl ();
  *ps_sh << "static " << this->interface_name_
         << "_ptr _duplicate ("
         << this->interface_name_ << "_ptr obj);"
    ;

  *ps_sh << "static " << this->interface_name_ << "_ptr _narrow (";  ps_sh->nl ();
  *ps_sh << "CORBA::Object_ptr obj";  ps_sh->nl ();
  *ps_sh << "";  ps_sh->nl ();
  *ps_sh << ");";  ps_sh->nl ();

  *ps_sh << "static " << this->interface_name_
         << "_ptr _unchecked_narrow (";  ps_sh->nl ();
  *ps_sh << "CORBA::Object_ptr obj";  ps_sh->nl ();
  *ps_sh << "";  ps_sh->nl ();
  *ps_sh << ");";  ps_sh->nl ();

  *ps_sh << "static " << this->interface_name_ << "_ptr _nil (void)";  ps_sh->nl ();
  *ps_sh << "{";  ps_sh->nl ();
  *ps_sh << "return (" << this->interface_name_ << "_ptr)0;";

  ps_sh->decr_indent (0);
  ps_sh->nl ();

  *ps_sh << "}";  ps_sh->nl ();

  *ps_sh << "static void _tao_any_destructor (void*);";  ps_sh->nl ();


}

void
TAO_PSDL_Interface_Visitor::print_end_for_interface (void)
{
  ACE_CString name_space = TAO_PSDL_Scope::instance ()->get_name_space ();

  // Get a pointer to PSDL_Stream of the stub header.
  TAO_PSDL_Stream *ps_sh = TAO_PSDL_Scope::instance ()->get_sh ();

  *ps_sh << "virtual CORBA::Boolean _is_a (";  ps_sh->nl ();
  *ps_sh << "const CORBA::Char *type_id";  ps_sh->nl ();
  *ps_sh << "";  ps_sh->nl ();
  *ps_sh << ");";  ps_sh->nl ();

  *ps_sh << "virtual void *_tao_QueryInterface (ptrdiff_t type);";  ps_sh->nl ();

  *ps_sh << "virtual const char* _interface_repository_id (void) const;";  ps_sh->nl ();

  *ps_sh << "private:";  ps_sh->nl ();
  *ps_sh << "_TAO_" << this->interface_name_ << "_Proxy_Broker *the_TAO_" << this->interface_name_ << "_Proxy_Broker_;";  ps_sh->nl ();

  *ps_sh << "protected:";  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "(int collocated = 0);";  ps_sh->nl ();

  *ps_sh << "// These methods travese the inheritance tree and set the";  ps_sh->nl ();
  *ps_sh << "// parents piece of the given class in the right mode";  ps_sh->nl ();
  *ps_sh << "virtual void " << name_space << "_" << this->interface_name_
         << "_setup_collocation (int collocated);";  ps_sh->nl ();

  *ps_sh << this->interface_name_ << "(";  ps_sh->nl ();
  *ps_sh << "TAO_Stub *objref, ";  ps_sh->nl ();
  *ps_sh << "CORBA::Boolean _tao_collocated = 0,";  ps_sh->nl ();
  *ps_sh << "TAO_Abstract_ServantBase *servant = 0";  ps_sh->nl ();
  *ps_sh << ");";  ps_sh->nl ();

  *ps_sh << "friend class _TAO_" << this->interface_name_ << "_Remote_Proxy_Impl;";  ps_sh->nl ();
  *ps_sh << "friend class _TAO_" << this->interface_name_ << "_ThruPOA_Proxy_Impl;";  ps_sh->nl ();
  *ps_sh << "friend class _TAO_" << this->interface_name_ << "_Direct_Proxy_Impl;";  ps_sh->nl ();

  *ps_sh << "virtual ~" << this->interface_name_ << "(void);";  ps_sh->nl ();

  *ps_sh << "private:";  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "(const " << this->interface_name_ << "&);";  ps_sh->nl ();
  *ps_sh << "void operator= (const " << this->interface_name_ << "&);";  ps_sh->nl ();
  *ps_sh << "};";  ps_sh->nl ();

  *ps_sh << "// The Proxy Implementations are used by each interface to";  ps_sh->nl ();
  *ps_sh << "// perform a call. Each different implementation encapsulate";  ps_sh->nl ();
  *ps_sh << "// an invocation logics.";  ps_sh->nl ();


  *ps_sh << "///////////////////////////////////////////////////////////////////////";  ps_sh->nl ();
  *ps_sh << "//                    Base Proxy Impl. Declaration";  ps_sh->nl ();
  *ps_sh << "//";  ps_sh->nl ();

  *ps_sh << "class  _TAO_" << this->interface_name_ << "_Proxy_Impl";  ps_sh->nl ();
  *ps_sh << ": public virtual TAO_Object_Proxy_Impl";  ps_sh->nl ();

  *ps_sh << "{";
  ps_sh->nl ();
  ps_sh->incr_indent (0);

  *ps_sh << "public:";  ps_sh->nl ();
  *ps_sh << "virtual ~_TAO_" << this->interface_name_ << "_Proxy_Impl (void) { }";  ps_sh->nl ();

  *ps_sh << "protected:";  ps_sh->nl ();
  *ps_sh << "_TAO_" << this->interface_name_ << "_Proxy_Impl (void);";  ps_sh->nl ();
  *ps_sh << "};";  ps_sh->nl ();

  *ps_sh << "//";  ps_sh->nl ();
  *ps_sh << "//               End Base Proxy Impl. Declaration";  ps_sh->nl ();
  *ps_sh << "///////////////////////////////////////////////////////////////////////";  ps_sh->nl ();


  *ps_sh << "///////////////////////////////////////////////////////////////////////";  ps_sh->nl ();
  *ps_sh << "//                Remote Proxy Impl. Declaration";  ps_sh->nl ();
  *ps_sh << "//";  ps_sh->nl ();

  *ps_sh << "class  _TAO_" << this->interface_name_ << "_Remote_Proxy_Impl";  ps_sh->nl ();
  *ps_sh << ": public virtual _TAO_" << this->interface_name_ << "_Proxy_Impl,";  ps_sh->nl ();
  *ps_sh << "public virtual TAO_Remote_Object_Proxy_Impl";  ps_sh->nl ();

  *ps_sh << "{";
  ps_sh->nl ();
  ps_sh->incr_indent (0);

  *ps_sh << "public:";
  ps_sh->nl ();

  *ps_sh << "_TAO_" << this->interface_name_
         << "_Remote_Proxy_Impl (void);";
  ps_sh->nl ();

  *ps_sh << "virtual ~_TAO_" << this->interface_name_
         << "_Remote_Proxy_Impl (void) { }";

  ps_sh->decr_indent (0);
  ps_sh->nl ();

  *ps_sh << "};";  ps_sh->nl ();

  *ps_sh << "//";  ps_sh->nl ();
  *ps_sh << "//             End Remote Proxy Impl. Declaration";  ps_sh->nl ();
  *ps_sh << "///////////////////////////////////////////////////////////////////////";  ps_sh->nl ();

  *ps_sh << "// The Proxy Brokers are used by each interface to get";  ps_sh->nl ();
  *ps_sh << "// the right proxy for performing a call. In the new ";  ps_sh->nl ();
  *ps_sh << "// collocation scheme, the proxy to be used can vary on";  ps_sh->nl ();
  *ps_sh << "// a call by call basis.";  ps_sh->nl ();


  *ps_sh << "///////////////////////////////////////////////////////////////////////";  ps_sh->nl ();
  *ps_sh << "//                 Base Proxy Broker Declaration ";  ps_sh->nl ();
  *ps_sh << "//";  ps_sh->nl ();

  *ps_sh << "class  _TAO_" << this->interface_name_ << "_Proxy_Broker";
  ps_sh->nl ();

  *ps_sh << "{";
  ps_sh->incr_indent (0);
  ps_sh->nl ();

  *ps_sh << "public:";
  ps_sh->nl ();
  *ps_sh << "virtual ~_TAO_" << this->interface_name_ << "_Proxy_Broker (void);";  ps_sh->nl ();
  *ps_sh << "virtual _TAO_" << this->interface_name_ << "_Proxy_Impl &select_proxy (";  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "*object";  ps_sh->nl ();
  *ps_sh << "";  ps_sh->nl ();
  *ps_sh << ") = 0;";  ps_sh->nl ();

  *ps_sh << "protected:";  ps_sh->nl ();
  *ps_sh << "_TAO_" << this->interface_name_ << "_Proxy_Broker (void);";
  ps_sh->nl ();

  *ps_sh << "};";  ps_sh->nl ();

  *ps_sh << "//";  ps_sh->nl ();
  *ps_sh << "//              End Base Proxy Broker Declaration ";  ps_sh->nl ();
  *ps_sh << "///////////////////////////////////////////////////////////////";
  ps_sh->nl ();


  *ps_sh << "///////////////////////////////////////////////////////////////";
  ps_sh->nl ();
  *ps_sh << "//                 Remote Proxy Broker Declaration ";
  ps_sh->nl ();
  *ps_sh << "//";
  ps_sh->nl ();

  *ps_sh << "class  _TAO_" << this->interface_name_ << "_Remote_Proxy_Broker";
  ps_sh->nl ();
  *ps_sh << ": public virtual _TAO_" << this->interface_name_
         << "_Proxy_Broker";
  ps_sh->nl ();

  *ps_sh << "{";
  ps_sh->incr_indent (0);
  ps_sh->nl ();

  *ps_sh << "public: ";  ps_sh->nl ();
  *ps_sh << "_TAO_" << this->interface_name_ << "_Remote_Proxy_Broker (void);";  ps_sh->nl ();

  *ps_sh << "virtual ~_TAO_" << this->interface_name_ << "_Remote_Proxy_Broker (void);";  ps_sh->nl ();

  *ps_sh << "virtual _TAO_" << this->interface_name_ << "_Proxy_Impl &select_proxy (";  ps_sh->nl ();
  *ps_sh << this->interface_name_ << "*object";  ps_sh->nl ();
  *ps_sh << "";  ps_sh->nl ();
  *ps_sh << ");";  ps_sh->nl ();

  *ps_sh << "private:";  ps_sh->nl ();
  *ps_sh << "_TAO_" << this->interface_name_ << "_Remote_Proxy_Impl remote_proxy_impl_;";  ps_sh->nl ();

  *ps_sh << "public:";  ps_sh->nl ();
  *ps_sh << "// This member function is used to get an handle to the unique instance";  ps_sh->nl ();
  *ps_sh << "// of the Remote Proxy Broker that is available for a given";  ps_sh->nl ();
  *ps_sh << "// interface.";  ps_sh->nl ();
  *ps_sh << "static _TAO_" << this->interface_name_ << "_Remote_Proxy_Broker *the_TAO_" << this->interface_name_ << "_Remote_Proxy_Broker (void);";  ps_sh->nl ();
  *ps_sh << "};";  ps_sh->nl ();

  *ps_sh << "//";  ps_sh->nl ();
  *ps_sh << "//              End Remote Proxy Broker Declaration ";  ps_sh->nl ();
  *ps_sh << "//////////////////////////////////////////////////////////////";
  ps_sh->nl ();

  *ps_sh << "#endif /* end #if !defined */";  ps_sh->nl ();

  *ps_sh << "TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_"
         << this->interface_name_ << ";";
  ps_sh->nl ();
}

void
TAO_PSDL_Interface_Visitor::gen_code_for_si (void)
{
  TAO_PSDL_Stream *ps_si = TAO_PSDL_Scope::instance ()->get_si ();

  ps_si->reset ();
  ps_si->indent (); // start from current indentation level

  *ps_si << "int " << this->interface_name_ << "::_tao_class_id = 0;";
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_ptr";
  ps_si->nl ();
  *ps_si << "tao_" << this->interface_name_ << "_duplicate (";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << this->interface_name_ << "_ptr p";
  ps_si->nl ();
  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "return " << this->interface_name_ << "::_duplicate (p);";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";
  ps_si->nl ();

  ps_si->nl ();

  *ps_si << "void";
  ps_si->nl ();

  *ps_si << "tao_" << this->interface_name_ << "_release (";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_ptr p";
  ps_si->nl ();

  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "{";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "CORBA::release (p);";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_ptr";
  ps_si->nl ();
  *ps_si << "tao_" << this->interface_name_ << "_nil (";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "void";
  ps_si->nl ();
  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "return " << this->interface_name_ << "::_nil ();";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";
  ps_si->nl ();

  ps_si->nl ();

  *ps_si << this->interface_name_ << "_ptr";
  ps_si->nl ();
  *ps_si << "tao_" << this->interface_name_ << "_narrow (";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "CORBA::Object *p";
  ps_si->nl ();

  *ps_si << "";   ps_si->nl ();
  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "{";

  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return " << this->interface_name_
         << "::_narrow (p);";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "CORBA::Object *";   ps_si->nl ();
  *ps_si << "tao_" << this->interface_name_ << "_upcast (";

  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "void *src";   ps_si->nl ();
  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << this->interface_name_ << " **tmp =";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "ACE_static_cast (" << this->interface_name_ << " **, src);";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "return *tmp;";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "// ***********************************************************";
  ps_si->nl ();
  *ps_si << "// " << this->interface_name_ << "_var";   ps_si->nl ();
  *ps_si << "// ***********************************************************";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_var::"
         << this->interface_name_
         << "_var (void) // default constructor";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << ": ptr_ (" << this->interface_name_ << "::_nil ())";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "{}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_ptr";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::ptr (void) const";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "return this->ptr_;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_var::"
         << this->interface_name_ << "_var (const ::"
         << this->interface_name_ << "_var &p) // copy constructor";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << ": TAO_Base_var (),";

  ps_si->incr_indent (0); ps_si->nl ();
  *ps_si << "ptr_ (" << this->interface_name_
         << "::_duplicate (p.ptr ()))";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "{}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_var::~"
         << this->interface_name_ << "_var (void) // destructor";

  ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "CORBA::release (this->ptr_);";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_var &";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::operator= ("
         << this->interface_name_ << "_ptr p)";

  ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "this->ptr_ = p;";   ps_si->nl ();
  *ps_si << "return *this;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_var &";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::operator= (const ::"
         << this->interface_name_ << "_var &p)";
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "if (this != &p)";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "{";

  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "CORBA::release (this->ptr_);";   ps_si->nl ();
  *ps_si << "this->ptr_ = ::" << this->interface_name_
         << "::_duplicate (p.ptr ());";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "return *this;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_var::operator const ::"
         << this->interface_name_ << "_ptr &() const // cast";   ps_si->nl ();
  *ps_si << "{";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "return this->ptr_;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_var::operator ::"
         << this->interface_name_ << "_ptr &() // cast ";   ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return this->ptr_;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_ptr";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::operator-> (void) const";
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return this->ptr_;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_ptr";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::in (void) const";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return this->ptr_;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_ptr &";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::inout (void)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return this->ptr_;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_ptr &";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::out (void)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "CORBA::release (this->ptr_);";   ps_si->nl ();
  *ps_si << "this->ptr_ = ::" << this->interface_name_ << "::_nil ();";
  ps_si->nl ();
  *ps_si << "return this->ptr_;";

  ps_si->nl ();
  ps_si->decr_indent (0);
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_ptr";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::_retn (void)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "// yield ownership of managed obj reference";   ps_si->nl ();
  *ps_si << "::" << this->interface_name_ << "_ptr val = this->ptr_;";
  ps_si->nl ();
  *ps_si << "this->ptr_ = ::" << this->interface_name_ << "::_nil ();";
  ps_si->nl ();
  *ps_si << "return val;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_ptr";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::tao_duplicate ("
         << this->interface_name_ << "_ptr p)";
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return ::" << this->interface_name_ << "::_duplicate (p);";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "void";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::tao_release ("
         << this->interface_name_ << "_ptr p)";
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "CORBA::release (p);";

  ps_si->nl ();
  ps_si->decr_indent (0);
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_ptr";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::tao_nil (void)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return ::" << this->interface_name_ << "::_nil ();";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_ptr";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::tao_narrow (";

  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "CORBA::Object *p";   ps_si->nl ();
  *ps_si << "";   ps_si->nl ();
  *ps_si << ")";

  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return ::" << this->interface_name_
         << "::_narrow (p);";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "CORBA::Object *";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_var::tao_upcast (void *src)";
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << this->interface_name_ << " **tmp =";   ps_si->nl ();
  *ps_si << "ACE_static_cast (" << this->interface_name_ << " **, src);";
  ps_si->nl ();
  *ps_si << "return *tmp;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "// **********************************************************";
  ps_si->nl ();
  *ps_si << "// " << this->interface_name_ << "_out";   ps_si->nl ();
  *ps_si << "// **********************************************************";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_out::"
         << this->interface_name_ << "_out ("
         << this->interface_name_ << "_ptr &p)";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << ": ptr_ (p)";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "this->ptr_ = ::" << this->interface_name_ << "::_nil ();";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_out::"
         << this->interface_name_ << "_out ("
         << this->interface_name_
         << "_var &p) // constructor from _var";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ": ptr_ (p.out ())";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "CORBA::release (this->ptr_);";   ps_si->nl ();
  *ps_si << "this->ptr_ = ::" << this->interface_name_ << "::_nil ();";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_out::"
         << this->interface_name_ << "_out (const ::"
         << this->interface_name_ << "_out &p) // copy constructor";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ": ptr_ (ACE_const_cast ("
         << this->interface_name_
         << "_out &, p).ptr_)";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_out &";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_out::operator= (const ::"
         << this->interface_name_ << "_out &p)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "this->ptr_ = ACE_const_cast ("
         << this->interface_name_ << "_out&, p).ptr_;";   ps_si->nl ();
  *ps_si << "return *this;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_out &";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_out::operator= (const ::"
         << this->interface_name_ << "_var &p)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "this->ptr_ = ::" << this->interface_name_
         << "::_duplicate (p.ptr ());";   ps_si->nl ();
  *ps_si << "return *this;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_out &";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_out::operator= ("
         << this->interface_name_ << "_ptr p)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "this->ptr_ = p;";   ps_si->nl ();
  *ps_si << "return *this;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_out::operator ::"
         << this->interface_name_ << "_ptr &() // cast";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
   ps_si->nl ();

  *ps_si << "return this->ptr_;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_ptr &";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_out::ptr (void) // ptr";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return this->ptr_;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << "_ptr";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_out::operator-> (void)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return this->ptr_;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "#if (TAO_HAS_INTERCEPTORS == 1)";   ps_si->nl ();
  *ps_si << "#endif /* TAO_HAS_INTERCEPTORS */";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "///////////////////////////////////////////////////////////////";   ps_si->nl ();
  *ps_si << "//                Base & Remote Proxy  Implementation. ";
  ps_si->nl ();
  *ps_si << "//";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "_TAO_" << this->interface_name_ << "_Proxy_Impl::_TAO_"
         << this->interface_name_ << "_Proxy_Impl (void)";   ps_si->nl ();
  *ps_si << "{}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "_TAO_" << this->interface_name_
         << "_Remote_Proxy_Impl::_TAO_" << this->interface_name_
         << "_Remote_Proxy_Impl (void)";   ps_si->nl ();
  *ps_si << "{}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "// Remote Implementation of the IDL interface methods";
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "//";   ps_si->nl ();
  *ps_si << "//            End  Base & Remote  Proxy Implemeentation. ";
  ps_si->nl ();

  *ps_si << "///////////////////////////////////////////////////////////////";
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "///////////////////////////////////////////////////////////////";   ps_si->nl ();
  *ps_si << "//           Remote & Base  Proxy Broker Implementation";
  ps_si->nl ();
  *ps_si << "//";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "_TAO_" << this->interface_name_ << "_Proxy_Broker::_TAO_"
         << this->interface_name_ << "_Proxy_Broker (void)";   ps_si->nl ();
  *ps_si << "{";   ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "_TAO_" << this->interface_name_ << "_Proxy_Broker::~_TAO_"
         << this->interface_name_ << "_Proxy_Broker (void)";   ps_si->nl ();
  *ps_si << "{";   ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "_TAO_" << this->interface_name_
         << "_Proxy_Broker * (*_TAO_"
         << this->interface_name_
         << "_Proxy_Broker_Factory_function_pointer) (";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "CORBA::Object_ptr obj";

  ps_si->nl ();
  *ps_si << ") = 0;";

  ps_si->decr_indent (0);

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "// Factory Member function Implementation.";   ps_si->nl ();
  *ps_si << "_TAO_" << this->interface_name_ << "_Remote_Proxy_Broker *";
  ps_si->nl ();
  *ps_si << "_TAO_" << this->interface_name_
         << "_Remote_Proxy_Broker::the_TAO_"
         << this->interface_name_
         << "_Remote_Proxy_Broker (void)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "static ::_TAO_" << this->interface_name_
         << "_Remote_Proxy_Broker remote_proxy_broker;";   ps_si->nl ();
  *ps_si << "return &remote_proxy_broker;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "_TAO_" << this->interface_name_
         << "_Remote_Proxy_Broker::_TAO_"
         << this->interface_name_ << "_Remote_Proxy_Broker (void)";   ps_si->nl ();
  *ps_si << "{";   ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "_TAO_" << this->interface_name_
         << "_Remote_Proxy_Broker::~_TAO_"
         << this->interface_name_ << "_Remote_Proxy_Broker (void)";   ps_si->nl ();
  *ps_si << "{";   ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "_TAO_" << this->interface_name_ << "_Proxy_Impl&";   ps_si->nl ();
  *ps_si << "_TAO_" << this->interface_name_
         << "_Remote_Proxy_Broker::select_proxy (";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "::" << this->interface_name_ << " *";   ps_si->nl ();
  *ps_si << "";   ps_si->nl ();
  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return this->remote_proxy_impl_;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "//";   ps_si->nl ();
  *ps_si << "//           End Remote & Base Proxy Broker Implementation";   ps_si->nl ();
  *ps_si << "///////////////////////////////////////////////////////////////";   ps_si->nl ();
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "::"
         << this->interface_name_ << " (int collocated)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "this->" << this->interface_name_
         << "_setup_collocation (collocated);";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "::~"
         << this->interface_name_ << " (void)";   ps_si->nl ();
  *ps_si << "{}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "void";   ps_si->nl ();
  *ps_si << this->interface_name_ << "::"
         << this->interface_name_
         << "_setup_collocation (int collocated)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "if (collocated)";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "this->the_TAO_"
         << this->interface_name_ << "_Proxy_Broker_ =";

  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "::_TAO_" << this->interface_name_
         << "_Proxy_Broker_Factory_function_pointer (this);";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "else";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "this->the_TAO_"
         << this->interface_name_
         << "_Proxy_Broker_ =";

  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "::_TAO_" << this->interface_name_
         << "_Remote_Proxy_Broker::the_TAO_"
         << this->interface_name_
         << "_Remote_Proxy_Broker ();";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->decr_indent (0);

  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "void " << this->interface_name_
         << "::_tao_any_destructor (void *_tao_void_pointer)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << this->interface_name_
         << " *tmp = ACE_static_cast ("
         << this->interface_name_ << "*, _tao_void_pointer);";   ps_si->nl ();
  *ps_si << "CORBA::release (tmp);";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_ptr "
         << this->interface_name_ << "::_narrow (";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "CORBA::Object_ptr obj";   ps_si->nl ();
  *ps_si << "";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ")";
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "if (CORBA::is_nil (obj))";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return " << this->interface_name_ << "::_nil ();";

  ps_si->decr_indent (0);
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "if (! obj->_is_local ())";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "CORBA::Boolean is_a = obj->_is_a (\"IDL:"
         << this->interface_name_ << ":1.0\");";
  ps_si->nl ();

  *ps_si << "if (is_a == 0)";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "return " << this->interface_name_ << "::_nil ();";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "return " << this->interface_name_
         << "::_unchecked_narrow (obj);";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_ptr ";   ps_si->nl ();
  *ps_si << this->interface_name_ << "::_unchecked_narrow (";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "CORBA::Object_ptr obj";   ps_si->nl ();
  *ps_si << "";   ps_si->nl ();
  *ps_si << ")";

  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "if (CORBA::is_nil (obj))";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "return " << this->interface_name_ << "::_nil ();";

  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "if (! obj->_is_local ())";   ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "TAO_Stub* stub = obj->_stubobj ();";   ps_si->nl ();

  *ps_si << "if (stub)";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << " stub->_incr_refcnt ();";

  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_ptr default_proxy = "
         << this->interface_name_ << "::_nil ();";   ps_si->nl ();
  ps_si->nl ();
  *ps_si << "if (";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "!CORBA::is_nil (stub->servant_orb_var ().ptr ()) &&";   ps_si->nl ();
  *ps_si << "stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects () &&";   ps_si->nl ();
  *ps_si << "obj->_is_collocated () &&";   ps_si->nl ();
  *ps_si << "_TAO_" << this->interface_name_
         << "_Proxy_Broker_Factory_function_pointer != 0";   ps_si->nl ();
  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << " {";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << " ACE_NEW_RETURN (";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "default_proxy,";   ps_si->nl ();
  *ps_si << "::" << this->interface_name_ << " (";
  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "stub,";   ps_si->nl ();
  *ps_si << "1,";   ps_si->nl ();
  *ps_si << "obj->_servant ()";   ps_si->nl ();
  *ps_si << "),";
  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << this->interface_name_ << "::_nil ()";   ps_si->nl ();

  *ps_si << ");";
  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << " }";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "if (CORBA::is_nil (default_proxy))";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "{";

  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "ACE_NEW_RETURN (";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "default_proxy,";   ps_si->nl ();

  *ps_si << "::" << this->interface_name_ << " (";
  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "stub,";   ps_si->nl ();
  *ps_si << "0,";   ps_si->nl ();
  *ps_si << "obj->_servant ()";   ps_si->nl ();
  *ps_si << "),";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << this->interface_name_ << "::_nil ()";   ps_si->nl ();
  *ps_si << ");";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << " }";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "return default_proxy;";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "}";   ps_si->nl ();
  *ps_si << "else";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "return";

  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "ACE_reinterpret_cast";   ps_si->nl ();
  *ps_si << "(";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << this->interface_name_ << "_ptr,";   ps_si->nl ();
  *ps_si << "obj->_tao_QueryInterface";   ps_si->nl ();
  *ps_si << "(";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "ACE_reinterpret_cast";   ps_si->nl ();
  *ps_si << "(";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "ptrdiff_t,";   ps_si->nl ();
  *ps_si << "&" << this->interface_name_ << "::_tao_class_id";
  ps_si->nl ();

  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << " );";
  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << this->interface_name_ << "_ptr";   ps_si->nl ();
  *ps_si << this->interface_name_ << "::_duplicate ("
         << this->interface_name_ << "_ptr obj)";   ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "if (!CORBA::is_nil (obj))";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "obj->_add_ref ();";

  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "return obj;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "CORBA::Boolean " << this->interface_name_
         << "::_is_a (const CORBA::Char *value)";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "if (";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "(!ACE_OS::strcmp ((char *)value, \"IDL:"
         << this->interface_name_ << ":1.0\")) ||";   ps_si->nl ();
  *ps_si << "(!ACE_OS::strcmp ((char *)value,"
         << "\"IDL:omg.org/CORBA/Object:1.0\")))";   ps_si->nl ();

  *ps_si << "return 1; // success using local knowledge";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "else";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "return this->Object::_is_a (value);";
  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "void *" << this->interface_name_
         << "::_tao_QueryInterface (ptrdiff_t type)";   ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "void *retv = 0;";   ps_si->nl ();
  *ps_si << "if (type == ACE_reinterpret_cast";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "(ptrdiff_t,";   ps_si->nl ();
  *ps_si << "&" << this->interface_name_ << "::_tao_class_id))";   ps_si->nl ();
  *ps_si << "retv = reinterpret_cast<void*> (this);";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "else if (type == ACE_reinterpret_cast "
         << "(ptrdiff_t, &CORBA::Object::_tao_class_id))";

  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "retv = ACE_reinterpret_cast (void *,";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "static_cast<CORBA::Object_ptr> (this));";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);

  ps_si->nl ();
  ps_si->nl ();
  *ps_si << "if (retv)";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "this->_add_ref ();";

  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "return retv;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "const char* " << this->interface_name_
         << "::_interface_repository_id (void) const";   ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "return \"IDL:" << this->interface_name_ << ":1.0\";";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "static const CORBA::Long _oc_"
         << this->interface_name_ << "[] =";   ps_si->nl ();
  *ps_si << "{";   ps_si->nl ();
  *ps_si << "// typecode related information goes here";

  ps_si->nl ();
  *ps_si << "};";

  ps_si->nl ();
  ps_si->nl ();


  *ps_si << "static CORBA::TypeCode _tc_TAO_tc_"
         << this->interface_name_ << " (";

  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "CORBA::tk_objref,";   ps_si->nl ();
  *ps_si << "sizeof (_oc_" << this->interface_name_ << "),";   ps_si->nl ();
  *ps_si << "(char *) &_oc_" << this->interface_name_ << ",";   ps_si->nl ();
  *ps_si << "0,";   ps_si->nl ();
  *ps_si << "sizeof (" << this->interface_name_ << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ");";

  ps_si->decr_indent (0);
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "::CORBA::TypeCode_ptr _tc_" << this->interface_name_ << " =";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "&_tc_TAO_tc_" << this->interface_name_ << ";";
  ps_si->decr_indent (0);

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "void operator<<= (";
  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "CORBA::Any &_tao_any,";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_ptr _tao_elem";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "TAO_OutputCDR stream;";
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "if (stream << _tao_elem)";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "_tao_any._tao_replace (";
  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "_tc_" << this->interface_name_ << ", ";   ps_si->nl ();
  *ps_si << "TAO_ENCAP_BYTE_ORDER,";   ps_si->nl ();
  *ps_si << "stream.begin (),";   ps_si->nl ();
  *ps_si << "1,";   ps_si->nl ();
  *ps_si << this->interface_name_ << "::_duplicate (_tao_elem),";   ps_si->nl ();
  *ps_si << this->interface_name_ << "::_tao_any_destructor";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ");";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "void operator<<= (";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "CORBA::Any &_tao_any,";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_ptr *_tao_elem";
  ps_si->nl ();

  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "TAO_OutputCDR stream;";
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "if (stream << *_tao_elem)";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "_tao_any._tao_replace (";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "_tc_" << this->interface_name_ << ", ";   ps_si->nl ();
  *ps_si << "TAO_ENCAP_BYTE_ORDER,";   ps_si->nl ();
  *ps_si << "stream.begin (),";   ps_si->nl ();
  *ps_si << "1,";   ps_si->nl ();
  *ps_si << "*_tao_elem,";   ps_si->nl ();
  *ps_si << this->interface_name_ << "::_tao_any_destructor";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ");";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "CORBA::Boolean operator>>= (";
  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "const CORBA::Any &_tao_any,";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_ptr &_tao_elem";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ")";

  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "ACE_TRY_NEW_ENV";   ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "_tao_elem = " << this->interface_name_ << "::_nil ();";
  ps_si->nl ();
  *ps_si << "CORBA::TypeCode_var type = _tao_any.type ();";
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "CORBA::Boolean result =";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "type->equivalent (";
  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "_tc_" << this->interface_name_ << "";   ps_si->nl ();
  *ps_si << "";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ");";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();
;

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "if (!result)";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "return 0; // not equivalent";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->decr_indent (0);
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "TAO_InputCDR stream (";
  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "_tao_any._tao_get_cdr (),";
  ps_si->nl ();
  *ps_si << "_tao_any._tao_byte_order ()";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ");";

  ps_si->decr_indent (0);
  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "if (stream >> _tao_elem)";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "((CORBA::Any *)&_tao_any)->_tao_replace (";
  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "_tc_" << this->interface_name_ << ",";   ps_si->nl ();
  *ps_si << "1,";   ps_si->nl ();
  *ps_si << "_tao_elem,";   ps_si->nl ();
  *ps_si << this->interface_name_ << "::_tao_any_destructor";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ");";

  ps_si->decr_indent (0);
  ps_si->nl ();
  ps_si->nl ();
  *ps_si << "return 1;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";
  ps_si->nl ();

  *ps_si << "ACE_CATCHANY";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "{";   ps_si->nl ();
  *ps_si << "}";
  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "ACE_ENDTRY;";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "_tao_elem = " << this->interface_name_ << "::_nil ();";
  ps_si->nl ();
  *ps_si << "return 0;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "CORBA::Boolean operator<< (";

  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "TAO_OutputCDR &strm,";   ps_si->nl ();
  *ps_si << "const " << this->interface_name_ << "_ptr _tao_objref";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ")";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "CORBA::Object_ptr _tao_corba_obj = _tao_objref;";   ps_si->nl ();
  *ps_si << "return (strm << _tao_corba_obj);";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

  *ps_si << "CORBA::Boolean operator>> (";
  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "TAO_InputCDR &strm,";   ps_si->nl ();
  *ps_si << this->interface_name_ << "_ptr &_tao_objref";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << ")";

  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "ACE_TRY_NEW_ENV";   ps_si->nl ();
  *ps_si << "{";
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "CORBA::Object_var obj;";   ps_si->nl ();
  *ps_si << "if ((strm >> obj.inout ()) == 0)";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "return 0;";
  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "// narrow to the right type";   ps_si->nl ();
  *ps_si << "_tao_objref =";
  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << this->interface_name_ << "::_unchecked_narrow (";

  ps_si->incr_indent (0);
  ps_si->incr_indent (0);
  ps_si->nl ();

  *ps_si << "obj.in ()";   ps_si->nl ();
  *ps_si << "";

  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << ");";

  ps_si->decr_indent (0);
  ps_si->decr_indent (0);
  ps_si->nl ();

;   ps_si->nl ();
  *ps_si << "return 1;";

  ps_si->decr_indent (0);
  ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  *ps_si << "ACE_CATCHANY";   ps_si->nl ();
  *ps_si << "{";

  ps_si->incr_indent (0);
  ps_si->nl ();
  *ps_si << "// do nothing";

  ps_si->decr_indent (0);ps_si->nl ();
  *ps_si << "}";

  ps_si->nl ();
  *ps_si << "ACE_ENDTRY;";   ps_si->nl ();
  *ps_si << "return 0;";

  ps_si->decr_indent (0);
  ps_si->nl ();

  *ps_si << "}";

  ps_si->nl ();
  ps_si->nl ();

}
