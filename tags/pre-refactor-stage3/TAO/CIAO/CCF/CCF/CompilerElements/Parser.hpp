#ifndef PARSER_HPP
#define PARSER_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/CompilerElements/Token.hpp"

#include <boost/spirit.hpp>

namespace Details
{
  using namespace boost::spirit;

  typedef
  TokenStream::iterator
  Iterator;

  typedef
  scanner<Iterator, scanner_policies<> >
  Scanner;

  typedef
  rule<Scanner>
  Rule;


  //-------------------------------------------------------------

  template <typename TypePtr>
  struct IdentityParser : public parser <IdentityParser<TypePtr> >
  {
    typedef
    IdentityParser
    self_t;

    IdentityParser (TypePtr const& t)
        : t_ (t)
    {
    }

    typename parser_result<self_t, Scanner>::type
    parse(Scanner const& scan) const
    {
      if (!scan.at_end())
      {
        TokenPtr t = *scan;

        if(identical (t_, t))
        {
          Iterator save(scan.first);
          ++scan;
          return scan.create_match(1, t, save, scan.first);
        }
      }
      return scan.no_match();
    }

  private:
    TypePtr t_;
  };

  typedef
  IdentityParser<KeywordPtr>
  KeywordParser;

  typedef
  IdentityParser<PunctuationPtr>
  PunctuationParser;

  //----------------------------------------------------------------

  template <typename TypePtr>
  struct TypeParser : public parser <TypeParser<TypePtr> >
  {
    typedef
    TypeParser
    self_t;

    typename parser_result<self_t, Scanner>::type
    parse(Scanner const& scan) const
    {
      if (!scan.at_end())
      {
        TokenPtr t = *scan;

        if(typical<TypePtr>(t) != 0)
        {
          Iterator save(scan.first);
          ++scan;
          return scan.create_match(1, t, save, scan.first);
        }
      }
      return scan.no_match();
    }
  };

  typedef
  TypeParser<IdentifierPtr>
  IdentifierParser;

  typedef
  TypeParser<SimpleIdentifierPtr>
  SimpleIdentifierParser;

  typedef
  TypeParser<ScopedIdentifierPtr>
  ScopedIdentifierParser;

  typedef
  TypeParser<StringLiteralPtr>
  StringLiteralParser;

  //---------------------------------------------------------------

  bool parse (Iterator const& first_,
              Iterator const& last,
              Rule const& rule)
  {
    Iterator first = first_;
    Scanner scan(first, last);
    match<nil_t> hit = rule.parse(scan);

    bool result = parse_info<Iterator>(
      first, hit, hit && (first == last), hit.length()).full;

    if (!result)
    {
      if (first != last)
      {
        std::cerr << (*first)->line ()
                  << ": error: illegal syntactic construct" << std::endl;
      }
      else
      {
        //@@ what are the conditions when this happens?
        //@@ is I add EOS token to the grammar then I can avoid this?
        std::cerr << "error: illegal syntactic construct" << std::endl;
      }

    }

    return result;
  }
}

// Base class for CCF-compliant parser
class Parser
{
protected:

  typedef
  Details::Iterator
  Iterator;

protected:

  template <typename Obj>
  class ActionExecutor
  {
  public:

    typedef
    void (Obj::*SemanticAction)(Iterator, Iterator) const;

    ActionExecutor (Obj const* obj, SemanticAction action)
        : obj_ (obj), action_ (action)
    {
    }

    void operator () (Iterator begin, Iterator end) const
    {
      (obj_->*action_) (begin, end);
    }

  private:
    Obj const* obj_;
    SemanticAction action_;
  };

  template <typename Obj>
  class NoArgAction
  {
  public:

    typedef
    void (Obj::*Action)();

    NoArgAction (Obj& obj, Action action)
        : obj_ (obj), action_ (action)
    {
    }

    void operator () (Iterator, Iterator) const
    {
      (obj_.*action_) ();
    }

  private:
    Obj& obj_;
    Action action_;
  };

  template <typename TypePtr, typename Obj>
  class OneArgAction
  {
  public:

    typedef
    void (Obj::*Action)(TypePtr const&);

    OneArgAction (Obj& obj, Action action)
        : obj_ (obj), action_ (action)
    {
    }

    void operator () (Iterator begin, Iterator end) const
    {
      if (end - begin != 1 )
      {
        //@@ throw
      }

      //@@ error handling if typical fails
      (obj_.*action_) (typical<TypePtr> (*begin));
    }

  private:
    Obj& obj_;
    Action action_;
  };


  template <typename Arg1, typename Arg2, typename Obj>
  class TwoArgAction
  {
  public:

    typedef
    void (Obj::*Action)(Arg1 const&, Arg2 const&);

    TwoArgAction (Obj& obj, Action action)
        : obj_ (obj), action_ (action)
    {
    }

    void operator () (Iterator begin, Iterator end) const
    {
      if (end - begin != 2 )
      {
        //@@ throw
      }

      //@@ error handling if typical fails
      (obj_.*action_) (typical<Arg1> (*begin),
                       typical<Arg2> (*(begin + 1)));
    }

  private:
    Obj& obj_;
    Action action_;
  };
};


#endif // PARSER_HPP
