// $Id$

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"
#include        "be_sunsoft.h"

ACE_RCSID(be, be_sunsoft, "$Id$")


TAO_SunSoft_OutStream::TAO_SunSoft_OutStream (void)
  : TAO_OutStream ()
{
}

TAO_SunSoft_OutStream::~TAO_SunSoft_OutStream (void)
{
}

TAO_OutStream & 
TAO_SunSoft_OutStream::print (Identifier *id)
{
  ACE_OS::fprintf (this->fp_, id->get_string ());
  return *this;
}

TAO_OutStream & 
TAO_SunSoft_OutStream::print (UTL_IdList *idl)
{
  UTL_IdListActiveIterator	*i = new UTL_IdListActiveIterator (idl);
  long				first = I_TRUE;
  long				second = I_FALSE;

  while (!(i->is_done ())) 
    {
      if (!first)
        *this << "::";
      else if (second)
        first = second = I_FALSE;
      // print the identifier
      *this << i->item ();
      if (first) 
        {
          if (ACE_OS::strcmp (i->item ()->get_string (), "") != 0)
            // does not start with a ""
            first = I_FALSE;
          else
            second = I_TRUE;
        }
      i->next ();
  }
  return *this;
}

TAO_OutStream&
TAO_SunSoft_OutStream::print (AST_Expression *expr)
{
  AST_Expression::AST_ExprValue *ev = expr->ev ();

  if (ev)
    {
      switch (ev->et)
        {
        case AST_Expression::EV_short:
          this->TAO_OutStream::print ("%hd", ev->u.sval);
          break;
        case AST_Expression::EV_ushort:
          this->TAO_OutStream::print ("%hu", ev->u.usval);
          break;
        case AST_Expression::EV_long:
          this->TAO_OutStream::print ("%ld", ev->u.lval);
          break;
        case AST_Expression::EV_ulong:
          this->TAO_OutStream::print ("%ld", ev->u.ulval);
          break;
        case AST_Expression::EV_longlong:
          this->TAO_OutStream::print ("%ld", ev->u.llval);
          break;
        case AST_Expression::EV_ulonglong:
          this->TAO_OutStream::print ("%ld", ev->u.ullval);
          break;
        case AST_Expression::EV_float:
          this->TAO_OutStream::print ("%f", ev->u.fval);
          break;
        case AST_Expression::EV_double:
          this->TAO_OutStream::print ("%f", ev->u.dval);
          break;
        case AST_Expression::EV_longdouble:
          break;
        case AST_Expression::EV_char:
          if (isprint (ev->u.cval))
	          this->TAO_OutStream::print ("'%c'", ev->u.cval);
	        else if (iscntrl (ev->u.cval))
            switch (ev->u.cval) 
              {
                case '\n':
                  this->TAO_OutStream::print ("'\\n'");
                  break;
                case '\t':
                  this->TAO_OutStream::print ("'\\t'");
                  break;
                case '\r':
                  this->TAO_OutStream::print ("'\\r'");
                  break;
                case '\v':
                  this->TAO_OutStream::print ("'\\v'");
                  break;
                case '\f':
                  this->TAO_OutStream::print ("'\\f'");
                  break;
                case '\b':
                  this->TAO_OutStream::print ("'\\b'");
                  break;
                case '\a':
                  this->TAO_OutStream::print ("'\\a'");
                  break;
                case '\\':
                  this->TAO_OutStream::print ("'\\'");
                  break;
                case '\?':
                  this->TAO_OutStream::print ("'?'");
                  break;
              default:
                this->TAO_OutStream::print ("'\\x%x'", ev->u.cval);
              }
 	        else 
	          this->TAO_OutStream::print ("'\\x%x'", ev->u.cval);
          break;
        case AST_Expression::EV_wchar:
          this->TAO_OutStream::print ("%ld", ev->u.wcval);
          break;
        case AST_Expression::EV_octet:
          this->TAO_OutStream::print ("%d", ev->u.oval);
          break;
        case AST_Expression::EV_bool:
          this->TAO_OutStream::print ("%ld", ev->u.bval);
          break;
        case AST_Expression::EV_string:
          this->TAO_OutStream::print ("\"%s\"", ev->u.strval->get_string ());
          break;
        case AST_Expression::EV_wstring:
          break;
        default:
          break;
        }
    }
  else // 
    {
      // XXXASG: need to add code here
    }
  return *this;
}
