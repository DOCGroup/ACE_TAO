// file      : XSCRT/Writer.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef XSCRT_WRITER_HPP
#define XSCRT_WRITER_HPP

#include <stack>
#include <string>

#include <XSCRT/XML.hpp>

namespace XSCRT
{
  template <typename C>
  class Writer
  {
  public:
    Writer (XML::Element<C>& e)
        : attr__ (0)
    {
      push_ (e);
    }

  protected:
    // This c-tor should never be called.
    //
    Writer ()
    {
      abort ();
    }

  public:
    void
    push_ (XML::Element<C> const& e)
    {
      stack_.push (e);
    }

    void
    pop_ ()
    {
      stack_.pop ();
    }

    XML::Element<C>&
    top_ ()
    {
      return stack_.top ();
    }

  public:
    XML::Attribute<C>*
    attr_ ()
    {
      return attr__;
    }

    void
    attr_ (XML::Attribute<C>* a)
    {
      attr__ = a;
    }

  private:
    std::stack<XML::Element<C> > stack_;

    XML::Attribute<C>* attr__;

  private:
    Writer (Writer const&);

    void
    operator= (Writer const&);
  };
}

#include <XSCRT/Writer.ipp>
#include <XSCRT/Writer.tpp>

#endif  // XSCRT_WRITER_HPP
