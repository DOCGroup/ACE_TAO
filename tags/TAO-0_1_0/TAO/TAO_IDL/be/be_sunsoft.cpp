#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"
#include        "be_sunsoft.h"

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
          break;
        case AST_Expression::EV_ulonglong:
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
          this->TAO_OutStream::print ("'%c'", ev->u.cval);
          break;
        case AST_Expression::EV_wchar:
          break;
        case AST_Expression::EV_octet:
          this->TAO_OutStream::print ("%c", ev->u.oval);
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
