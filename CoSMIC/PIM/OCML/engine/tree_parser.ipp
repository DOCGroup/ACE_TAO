// $Id$

/*
 *  Exception class Option_Tree_Parser::UnknownOptionKind implementation.
 */

namespace OCML
{

  inline
  Option_Tree_Parser::UnknownOptionKind::
  UnknownOptionKind(const std::string& msg)
    : msg_(std::string("Option_Tree_Parser:UnknownOptionKind ") +
           std::string("\"") +
           msg +
           std::string("\""))
  {
  }

  inline
  Option_Tree_Parser::UnknownOptionKind::
  ~UnknownOptionKind() throw()
  {
  }

  inline const char*
  Option_Tree_Parser::UnknownOptionKind::
  what() const throw()
  {
    return msg_.c_str();
  }

} // namespace OCML
