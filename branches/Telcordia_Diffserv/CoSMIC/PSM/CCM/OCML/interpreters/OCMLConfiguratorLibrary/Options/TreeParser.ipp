// $Id$

/*
 *  Exception class OptionTreeParser::UnknownOptionKind implementation.
 */

namespace OCML
{

  inline
  OptionTreeParser::UnknownOptionKind::UnknownOptionKind(const std::string& msg)
    : msg_(std::string("OptionTreeParser:UnknownOptionKind ") +
           std::string("\"") +
           msg +
           std::string("\""))
  {
  }

  inline
  OptionTreeParser::UnknownOptionKind::~UnknownOptionKind() throw()
  {
  }

  inline const char*
  OptionTreeParser::UnknownOptionKind::what() const throw()
  {
    return msg_.c_str();
  }

} // namespace OCML
