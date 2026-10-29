// file      : XSCRT/Elements.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#ifndef XSCRT_ELEMENTS_HPP
#define XSCRT_ELEMENTS_HPP

#include "ace/ace_wchar.h"
#include <limits>
#include <locale>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include <ace/XML_Utils/XSCRT/Parser.hpp>

namespace XSCRT
{
  namespace Details
  {
    template <typename C, typename X>
    X parse_numeric (std::basic_string<C> const &value)
    {
      std::basic_istringstream<C> stream (value);
      stream.imbue (std::locale::classic ());

      if constexpr (std::numeric_limits<X>::has_infinity && std::numeric_limits<X>::has_quiet_NaN)
      {
        std::basic_istringstream<C> special_stream (value);
        special_stream.imbue (std::locale::classic ());

        std::basic_string<C> special;
        special_stream >> std::ws >> special >> std::ws;

        if (special_stream.eof ())
        {
          if (special == std::basic_string<C> { C ('I'), C ('N'), C ('F') })
            return std::numeric_limits<X>::infinity ();
          if (special == std::basic_string<C> { C ('-'), C ('I'), C ('N'), C ('F') })
            return -std::numeric_limits<X>::infinity ();
          if (special == std::basic_string<C> { C ('N'), C ('a'), C ('N') })
            return std::numeric_limits<X>::quiet_NaN ();
        }
      }

      stream >> std::ws;
      bool const negative (stream.peek () == stream.widen ('-'));
      X result{};
      if (!(stream >> result))
        throw std::invalid_argument ("Invalid XML numeric value");
      if (std::numeric_limits<X>::is_integer && !std::numeric_limits<X>::is_signed && negative && result != 0)
        throw std::invalid_argument ("Invalid XML numeric value");
      stream >> std::ws;
      if (!stream.eof ())
        throw std::invalid_argument ("Invalid XML numeric value");
      return result;
    }
  }

  struct IdentityProvider
  {
    virtual bool before (IdentityProvider const &) const = 0;

    virtual ~IdentityProvider () = default;
  };

  class Type
  {
  public:
    virtual ~Type () = default;

  protected:
    Type () = default;

    template <typename C>
    explicit Type (XML::Element<C> const &)
    {
    }

    template <typename C>
    explicit Type (XML::Attribute<C> const &)
    {
    }

    // A copy gets fresh runtime bookkeeping; parent and ID registrations stay with their owner.
    Type (Type const &)
    {
    }

    // Assignment preserves the destination's runtime bookkeeping.
    Type &operator= (Type const &)
    {
      return *this;
    }

  public:
    Type const *container () const
    {
      return container_ ? container_ : this;
    }

    Type *container ()
    {
      return container_ ? container_ : this;
    }

    Type const *root () const
    {
      Type const *r = container ();

      Type const *c = r->container ();

      for (; c != r; c = c->container ())
        r = c;

      return r;
    }

    Type *root ()
    {
      Type *r = container ();

      for (Type *c = r->container (); c != r; c = c->container ())
        r = c;

      return r;
    }

  public:
    virtual void container (Type *c)
    {
      if (container_ == c)
        return;

      Map_::size_type registered{};

      // Revoke registrations from our old container.
      if (container_ != nullptr && map_.get ())
      {
        for (Map_::value_type const &entry : *map_)
        {
          // std::wcerr << "revoking " << entry.second
          //            << " to " << container_ << std::endl;

          container_->unregister_id (*entry.first);
        }
      }

      // Copy registrations to our new container.
      if (c != nullptr && map_)
      {
        try
        {
          for (Map_::value_type const &entry : *map_)
          {
            // std::wcerr << "copying " << entry.second
            //            << " to " << c << std::endl;

            c->register_id (*entry.first, entry.second);
            ++registered;
          }
        }
        catch (...)
        {
          Map_::const_iterator entry (map_->begin ());
          for (Map_::size_type i{}; i < registered; ++i, ++entry)
            c->unregister_id (*entry->first);

          if (container_ != nullptr)
          {
            for (Map_::value_type const &old_entry : *map_)
              container_->register_id (*old_entry.first, old_entry.second);
          }

          throw;
        }
      }

      container_ = c;
    }

  public:
    void register_id (IdentityProvider const &id, Type *t)
    {
      if (!map_)
      {
        map_ = std::unique_ptr<Map_> (new Map_);
      }

      std::pair<Map_::iterator, bool> const insertion (map_->try_emplace (std::addressof (id), t));
      if (!insertion.second)
      {
        throw std::logic_error ("XML ID is already registered");
      }

      try
      {
        if (container () != this)
          container ()->register_id (id, t);
      }
      catch (...)
      {
        map_->erase (insertion.first);
        throw;
      }
    }

    void unregister_id (IdentityProvider const &id)
    {
      if (map_.get ())
      {
        Map_::iterator const it (map_->find (std::addressof (id)));

        if (it != map_->end ())
        {
          map_->erase (it);

          if (container () != this)
            container ()->unregister_id (id);

          return;
        }
      }

      throw std::logic_error ("XML ID is not registered");
    }

    Type *lookup_id (IdentityProvider const &id) const
    {
      if (map_.get ())
      {
        Map_::const_iterator const it (map_->find (std::addressof (id)));

        if (it != map_->end ())
        {
          return it->second;
        }
      }

      return nullptr;
    }

    /// Get and set methods for the idref_map_ data member
    Type *get_idref (const char *name) const
    {
      std::basic_string<ACE_TCHAR> const name_string (ACE_TEXT_CHAR_TO_TCHAR (name));
      std::map<std::basic_string<ACE_TCHAR>, XSCRT::Type *>::const_iterator const i =
        this->idref_map_.find (name_string);
      if (i != idref_map_.end ())
      {
        return i->second;
      }
      else
      {
        return nullptr;
      }
    }

    Type *get_idref (const std::basic_string<ACE_TCHAR> &name) const
    {
      std::map<std::basic_string<ACE_TCHAR>, XSCRT::Type *>::const_iterator const i = this->idref_map_.find (name);
      if (i != idref_map_.end ())
      {
        return i->second;
      }
      else
      {
        return nullptr;
      }
    }

    Type *get_idref (const wchar_t *name) const
    {
      std::basic_string<ACE_TCHAR> const name_string (ACE_TEXT_WCHAR_TO_TCHAR (name));
      std::map<std::basic_string<ACE_TCHAR>, XSCRT::Type *>::const_iterator const i =
        this->idref_map_.find (name_string);
      if (i != idref_map_.end ())
      {
        return i->second;
      }
      else
      {
        return nullptr;
      }
    }

    void set_idref (const std::basic_string<ACE_TCHAR> &name, Type *new_idref)
    {
      this->idref_map_.try_emplace (name, new_idref);
    }

  private:
    // Data member to handle unbounded IDREF attributes and elements
    std::map<std::basic_string<ACE_TCHAR>, XSCRT::Type *> idref_map_{};

    Type *container_{};

    struct IdentityComparator
    {
      bool operator() (IdentityProvider const *x, IdentityProvider const *y) const
      {
        return x->before (*y);
      }
    };

    using Map_ = std::map<IdentityProvider const *, Type *, IdentityComparator>;

    std::unique_ptr<Map_> map_{};
  };

  // Fundamental types template.
  template <typename X>
  class FundamentalType : public Type
  {
  public:
    // Trait for marshaling a FundamentalType X
    using CDR_Type__ = X;

    FundamentalType () = default;

    template <typename C>
    explicit FundamentalType (XML::Element<C> const &e)
      : x_ (Details::parse_numeric<C, X> (e.value ()))
    {
    }

    template <typename C>
    explicit FundamentalType (XML::Attribute<C> const &a)
      : x_ (Details::parse_numeric<C, X> (a.value ()))
    {
    }

    FundamentalType (X const &x)
      : x_ (x)
    {
    }

    FundamentalType &operator= (X const &x)
    {
      x_ = x;
      return *this;
    }

  public:
    operator X const & () const
    {
      return x_;
    }

    operator X & ()
    {
      return x_;
    }

  protected:
    X x_{};
  };

  // Specialization for `signed char'
  template <>
  template <typename C>
  inline FundamentalType<signed char>::FundamentalType (XML::Element<C> const &e)
  {
    short const t (Details::parse_numeric<C, short> (e.value ()));
    if (t < std::numeric_limits<signed char>::min () || t > std::numeric_limits<signed char>::max ())
      throw std::out_of_range ("XML byte value is out of range");
    x_ = static_cast<signed char> (t);
  }

  template <>
  template <typename C>
  inline FundamentalType<signed char>::FundamentalType (XML::Attribute<C> const &a)
  {
    short const t (Details::parse_numeric<C, short> (a.value ()));
    if (t < std::numeric_limits<signed char>::min () || t > std::numeric_limits<signed char>::max ())
      throw std::out_of_range ("XML byte value is out of range");
    x_ = static_cast<signed char> (t);
  }

  // Specialization for `unsigned char'
  template <>
  template <typename C>
  inline FundamentalType<unsigned char>::FundamentalType (XML::Element<C> const &e)
  {
    unsigned short const t (Details::parse_numeric<C, unsigned short> (e.value ()));
    if (t > std::numeric_limits<unsigned char>::max ())
      throw std::out_of_range ("XML unsignedByte value is out of range");
    x_ = static_cast<unsigned char> (t);
  }

  template <>
  template <typename C>
  inline FundamentalType<unsigned char>::FundamentalType (XML::Attribute<C> const &a)
  {
    unsigned short const t (Details::parse_numeric<C, unsigned short> (a.value ()));
    if (t > std::numeric_limits<unsigned char>::max ())
      throw std::out_of_range ("XML unsignedByte value is out of range");
    x_ = static_cast<unsigned char> (t);
  }

  // Specialization for bool.
  template <>
  template <>
  inline FundamentalType<bool>::FundamentalType (XML::Element<char> const &e)
  {
    x_ = (e.value () == "true") || (e.value () == "1");
  }

  template <>
  template <>
  inline FundamentalType<bool>::FundamentalType (XML::Element<wchar_t> const &e)
  {
    x_ = (e.value () == L"true") || (e.value () == L"1");
  }

  template <>
  template <>
  inline FundamentalType<bool>::FundamentalType (XML::Attribute<char> const &a)
  {
    x_ = (a.value () == "true") || (a.value () == "1");
  }

  template <>
  template <>
  inline FundamentalType<bool>::FundamentalType (XML::Attribute<wchar_t> const &a)
  {
    x_ = (a.value () == L"true") || (a.value () == L"1");
  }

}

#endif // XSCRT_ELEMENTS_HPP
