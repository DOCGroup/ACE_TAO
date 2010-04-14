// $Id$

#include "be_sunsoft.h"
#include "identifier_helper.h"

#include "ast_expression.h"
#include "utl_identifier.h"
#include "utl_idlist.h"
#include "utl_string.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_ctype.h"

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
  ACE_OS::fprintf (this->fp_,
                   id->get_string ());

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
          this->TAO_OutStream::print (ACE_INT32_FORMAT_SPECIFIER_ASCII, ev->u.sval);
          break;
        case AST_Expression::EV_ushort:
          this->TAO_OutStream::print (ACE_INT32_FORMAT_SPECIFIER_ASCII "%c", ev->u.usval, 'U');
          break;
        case AST_Expression::EV_long:
          this->TAO_OutStream::print (ACE_INT32_FORMAT_SPECIFIER_ASCII, ev->u.lval);
          break;
        case AST_Expression::EV_ulong:
          this->TAO_OutStream::print (ACE_UINT32_FORMAT_SPECIFIER_ASCII "%c", ev->u.ulval, 'U');
          break;
        /// The next 2 cases differ from the tao_idl backend - the
        /// ACE_(U)INT64_LITERAL macros have been removed since
        /// we are generating IDL rather than C++.
        case AST_Expression::EV_longlong:
          this->TAO_OutStream::print (ACE_INT64_FORMAT_SPECIFIER_ASCII,
                                      ev->u.llval);
          break;
        case AST_Expression::EV_ulonglong:
          this->TAO_OutStream::print (ACE_UINT64_FORMAT_SPECIFIER_ASCII,
                                      ev->u.ullval);
          break;
        case AST_Expression::EV_float:
          this->TAO_OutStream::print ("%f%c", ev->u.fval, 'F');
          break;
        case AST_Expression::EV_double:
          this->TAO_OutStream::print ("%24.16G", ev->u.dval);
          break;
        case AST_Expression::EV_longdouble:
          break;
        case AST_Expression::EV_char:
          // isprint() sees \ and ' as printable characters
          // so we have to test for them first.
          if (ev->u.cval == '\\')
            this->TAO_OutStream::print ("'\\\\'");
          else if (ev->u.cval == '\'')
            this->TAO_OutStream::print ("'\\''");

          // This handles hex and octal escape sequences
          // that would print out either as weird characters
          // or as an unsigned number too large for a char.
          else if ((unsigned char) ev->u.cval > ACE_CHAR_MAX)
            this->TAO_OutStream::print ("%hd", ev->u.cval);
          else if (ACE_OS::ace_isprint (ev->u.cval))
            this->TAO_OutStream::print ("'%c'", ev->u.cval);
          else if (ACE_OS::ace_iscntrl (ev->u.cval))
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
          this->TAO_OutStream::print ("L'%lc'", ev->u.wcval);
          break;
        case AST_Expression::EV_octet:
          this->TAO_OutStream::print ("%d", ev->u.oval);
          break;
        case AST_Expression::EV_bool:
          this->TAO_OutStream::print ("%s", ev->u.bval ? "true" : "false");
          break;
        case AST_Expression::EV_string:
          this->TAO_OutStream::print ("\"%s\"", ev->u.strval->get_string ());
          break;
        case AST_Expression::EV_wstring:
          this->TAO_OutStream::print ("L\"%s\"", ev->u.wstrval);
          break;
        case AST_Expression::EV_enum:
          *this << IdentifierHelper::orig_sn (expr->n ()).c_str ();
          break;
        default:
          break;
        }
    }
  else
    {
      // XXXASG: need to add code here
    }

  return *this;
}
