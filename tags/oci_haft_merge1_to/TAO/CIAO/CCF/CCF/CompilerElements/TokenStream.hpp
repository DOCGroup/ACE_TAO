#ifndef TOKEN_STREAM_HPP
#define TOKEN_STREAM_HPP

#include <string>
#include <istream>

namespace CCF
{
  template <typename Token>
  class TokenStream
  {
  public:
    virtual Token
    next () = 0;
  };


  template <>
  class TokenStream<char>
  {
  public:
    typedef std::char_traits<char> traits;

    typedef traits::int_type int_type;
    typedef traits::char_type char_type;

  public:
    virtual int_type
    next () = 0;

    char_type
    to_char_type (int_type i)
    {
      return traits::to_char_type (i);
    }

    bool
    eos (int_type i)
    {
      return i == traits::eof ();
    }
  };

  class InputStreamAdapter : public TokenStream<char>
  {
  public:
    InputStreamAdapter (std::istream& is)
        : is_ (is)
    {
    }

  public:

    virtual int_type
    next ()
    {
      return is_.get ();
    }

  private:
    std::istream& is_;
  };
}

#endif //TOKEN_STREAM_HPP
