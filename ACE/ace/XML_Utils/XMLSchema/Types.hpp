// file      : XMLSchema/Types.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#ifndef XMLSCHEMA_TYPES_HPP
#define XMLSCHEMA_TYPES_HPP

#include "ace/XML_Utils/XSCRT/Elements.hpp"
#include <string>

#include "ace/Basic_Types.h"
/*
 * MSVC warning C4355 is triggered by ID's member initializer passing `this`
 * to IdentityProvider. IdentityProvider stores the reference without using
 * the ID while it is being constructed.
 */
#ifdef _MSC_VER
#  pragma warning(disable : 4355)
#endif
namespace XMLSchema
{
  using byte = XSCRT::FundamentalType<ACE_INT8>;
  using unsignedByte = XSCRT::FundamentalType<ACE_UINT8>;

  using short_ = XSCRT::FundamentalType<ACE_INT16>;
  using unsignedShort = XSCRT::FundamentalType<ACE_UINT16>;

  using int_ = XSCRT::FundamentalType<ACE_INT32>;
  using unsignedInt = XSCRT::FundamentalType<ACE_UINT32>;

  using long_ = XSCRT::FundamentalType<ACE_INT64>;
  using unsignedLong = XSCRT::FundamentalType<ACE_UINT64>;

  //@@ It would be nice to use some arbitrary-length integer class.
  //
  using decimal = long_;
  using integer = decimal;
  using nonPositiveInteger = integer;
  using nonNegativeInteger = integer;
  using positiveInteger = nonNegativeInteger;
  using negativeInteger = nonPositiveInteger;

  using boolean = XSCRT::FundamentalType<bool>;

  using float_ = XSCRT::FundamentalType<float>;
  using double_ = XSCRT::FundamentalType<double>;

  template <typename C>
  class string
    : public XSCRT::Type
    , public std::basic_string<C>
  {
  protected:
    using Base__ = std::basic_string<C>;

  public:
    string () = default;

    explicit string (XSCRT::XML::Element<C> const &e)
      : Base__ (e.value ())
    {
    }

    explicit string (XSCRT::XML::Attribute<C> const &a)
      : Base__ (a.value ())
    {
    }

    string (Base__ const &x)
      : Base__ (x)
    {
    }

    string (C const *x)
      : Base__ (x)
    {
    }

    string &operator= (Base__ const &x)
    {
      static_cast<Base__ &> (*this) = x;
      return *this;
    }
  };

  template <typename C>
  class normalizedString : public string<C>
  {
  protected:
    using Base__ = typename string<C>::Base__;

  public:
    normalizedString () = default;

    explicit normalizedString (XSCRT::XML::Element<C> const &e)
      : string<C> (e)
    {
    }

    explicit normalizedString (XSCRT::XML::Attribute<C> const &a)
      : string<C> (a)
    {
    }

    normalizedString (Base__ const &x)
      : string<C> (x)
    {
    }

    normalizedString (C const *x)
      : string<C> (x)
    {
    }

    normalizedString &operator= (Base__ const &x)
    {
      static_cast<Base__ &> (*this) = x;
      return *this;
    }
  };

  template <typename C>
  class token : public normalizedString<C>
  {
  protected:
    using Base__ = typename normalizedString<C>::Base__;

  public:
    token () = default;

    explicit token (XSCRT::XML::Element<C> const &e)
      : normalizedString<C> (e)
    {
    }

    explicit token (XSCRT::XML::Attribute<C> const &a)
      : normalizedString<C> (a)
    {
    }

    token (Base__ const &x)
      : normalizedString<C> (x)
    {
    }

    token (C const *x)
      : normalizedString<C> (x)
    {
    }

    token &operator= (Base__ const &x)
    {
      static_cast<Base__ &> (*this) = x;
      return *this;
    }
  };

  template <typename C>
  class NMTOKEN : public token<C>
  {
  protected:
    using Base__ = typename token<C>::Base__;

  public:
    NMTOKEN () = default;

    explicit NMTOKEN (XSCRT::XML::Element<C> const &e)
      : token<C> (e)
    {
    }

    explicit NMTOKEN (XSCRT::XML::Attribute<C> const &a)
      : token<C> (a)
    {
    }

    NMTOKEN (Base__ const &x)
      : token<C> (x)
    {
    }

    NMTOKEN (C const *x)
      : token<C> (x)
    {
    }

    NMTOKEN &operator= (Base__ const &x)
    {
      static_cast<Base__ &> (*this) = x;
      return *this;
    }
  };

  template <typename C>
  class Name : public token<C>
  {
  protected:
    using Base__ = typename token<C>::Base__;

  public:
    Name () = default;

    explicit Name (XSCRT::XML::Element<C> const &e)
      : token<C> (e)
    {
    }

    explicit Name (XSCRT::XML::Attribute<C> const &a)
      : token<C> (a)
    {
    }

    Name (Base__ const &x)
      : token<C> (x)
    {
    }

    Name (C const *x)
      : token<C> (x)
    {
    }

    Name &operator= (Base__ const &x)
    {
      static_cast<Base__ &> (*this) = x;
      return *this;
    }
  };

  template <typename C>
  class NCName : public Name<C>
  {
  protected:
    using Base__ = typename Name<C>::Base__;

  public:
    NCName () = default;

    explicit NCName (XSCRT::XML::Element<C> const &e)
      : Name<C> (e)
    {
    }

    explicit NCName (XSCRT::XML::Attribute<C> const &a)
      : Name<C> (a)
    {
    }

    NCName (Base__ const &x)
      : Name<C> (x)
    {
    }

    NCName (C const *x)
      : Name<C> (x)
    {
    }

    NCName &operator= (Base__ const &x)
    {
      static_cast<Base__ &> (*this) = x;
      return *this;
    }
  };

  template <typename C>
  class QName : public Name<C>
  {
  protected:
    using Base__ = typename Name<C>::Base__;

  public:
    QName () = default;

    explicit QName (XSCRT::XML::Element<C> const &e)
      : Name<C> (e)
    {
    }

    explicit QName (XSCRT::XML::Attribute<C> const &a)
      : Name<C> (a)
    {
    }

    QName (Base__ const &x)
      : Name<C> (x)
    {
    }

    QName (C const *x)
      : Name<C> (x)
    {
    }

    QName &operator= (Base__ const &x)
    {
      static_cast<Base__ &> (*this) = x;
      return *this;
    }
  };

  template <typename C>
  struct IdentityProvider : XSCRT::IdentityProvider
  {
    explicit IdentityProvider (NCName<C> const &id)
      : id_ (id)
    {
    }

    ~IdentityProvider () override = default;

    bool before (XSCRT::IdentityProvider const &y) const override
    {
      return id_ < dynamic_cast<IdentityProvider const &> (y).id_;
    }

  private:
    NCName<C> const &id_;

  private:
    IdentityProvider (IdentityProvider const &) = delete;
    IdentityProvider (IdentityProvider &&) = delete;

    IdentityProvider &operator= (IdentityProvider const &) = delete;
    IdentityProvider &operator= (IdentityProvider &&) = delete;
  };

  template <typename C>
  class ID : public NCName<C>
  {
  protected:
    using Base__ = typename NCName<C>::Base__;

  public:
    ~ID () override
    {
      unregister_id ();
    }

    ID ()
      : id_provider_ (*this)
    {
    }

    explicit ID (XSCRT::XML::Element<C> const &e)
      : NCName<C> (e)
      , id_provider_ (*this)
    {
    }

    explicit ID (XSCRT::XML::Attribute<C> const &a)
      : NCName<C> (a)
      , id_provider_ (*this)
    {
    }

    ID (ID const &x)
      : NCName<C> (x)
      , id_provider_ (*this)
    {
    }

    ID (Base__ const &x)
      : NCName<C> (x)
      , id_provider_ (*this)
    {
    }

    ID (C const *x)
      : NCName<C> (x)
      , id_provider_ (*this)
    {
    }

    ID &operator= (Base__ const &x)
    {
      assign_id_ (x);
      return *this;
    }

    ID &operator= (ID const &x)
    {
      if (this != std::addressof (x))
        assign_id_ (static_cast<Base__ const &> (x));
      return *this;
    }

  public:
    using NCName<C>::container;

    void container (XSCRT::Type *c) override
    {
      unregister_id ();

      NCName<C>::container (c);

      register_id ();
    }

  private:
    using NCName<C>::empty;
    using NCName<C>::root;

    void assign_id_ (Base__ const &value)
    {
      Base__ next (value);

      unregister_id ();
      static_cast<Base__ &> (*this).swap (next);

      try
      {
        register_id ();
      }
      catch (...)
      {
        static_cast<Base__ &> (*this).swap (next);
        register_id ();
        throw;
      }
    }

    void register_id ()
    {
      if (NCName<C>::container () != this && !empty ())
      {
        // std::wcerr << "registering " << container ()
        //            << " as '" << *this
        //            << "' on " << container () << std::endl;
        NCName<C>::container ()->register_id (id_provider_, NCName<C>::container ());
        registered_ = true;
      }
    }

    void unregister_id ()
    {
      if (registered_)
      {
        // std::wcerr << "un-registering " << container ()
        //            << " as '" << *this
        //            << "' on " << container () << std::endl;
        NCName<C>::container ()->unregister_id (id_provider_);
        registered_ = false;
      }
    }

  private:
    IdentityProvider<C> id_provider_;
    bool registered_{};
  };

  struct IDREF_Base : public XSCRT::Type
  {
    virtual XSCRT::Type const *get () const = 0;

    virtual XSCRT::Type *get () = 0;
  };

  template <typename C>
  class IDREF : public IDREF_Base
  {
  public:
    using CDR_Type__ = C;
    IDREF ()
      : id_provider_ (id_)
    {
    }

    explicit IDREF (XSCRT::XML::Element<C> const &e)
      : id_ (e)
      , id_provider_ (id_)
    {
    }

    explicit IDREF (XSCRT::XML::Attribute<C> const &a)
      : id_ (a)
      , id_provider_ (id_)
    {
    }

    IDREF (IDREF const &x)
      : XMLSchema::IDREF_Base ()
      , id_ (x.id_)
      , id_provider_ (id_)
    {
    }

    IDREF (std::basic_string<C> const &id)
      : id_ (id)
      , id_provider_ (id_)
    {
    }

    IDREF (C const *id)
      : id_ (id)
      , id_provider_ (id_)
    {
    }

    IDREF &operator= (IDREF const &x)
    {
      id_ = x.id_;
      return *this;
    }

    IDREF &operator= (std::basic_string<C> const &x)
    {
      id_ = x;
      return *this;
    }

  public:
    NCName<C> id () const
    {
      return id_;
    }

  public:
    XSCRT::Type const *operator->() const
    {
      return get ();
    }

    XSCRT::Type *operator->()
    {
      return get ();
    }

    XSCRT::Type const &operator* () const
    {
      return *(get ());
    }

    XSCRT::Type &operator* ()
    {
      return *(get ());
    }

    XSCRT::Type const *get () const override
    {
      if (!id_.empty () && container () != this)
      {
        return root ()->lookup_id (id_provider_);
      }
      else
      {
        return nullptr;
      }
    }

    XSCRT::Type *get () override
    {
      if (!id_.empty () && container () != this)
      {
        return root ()->lookup_id (id_provider_);
      }
      else
      {
        return nullptr;
      }
    }

    explicit operator bool () const
    {
      return get () != nullptr;
    }

  private:
    NCName<C> id_;
    IdentityProvider<C> id_provider_;
  };

  template <typename C>
  class anyURI
    : public XSCRT::Type
    , public std::basic_string<C>
  {
  protected:
    using Base__ = std::basic_string<C>;

  public:
    // Trait for marshaling string
    using CDR_Type__ = C;

    anyURI () = default;

    explicit anyURI (XSCRT::XML::Element<C> const &e)
      : Base__ (e.value ())
    {
    }

    explicit anyURI (XSCRT::XML::Attribute<C> const &a)
      : Base__ (a.value ())
    {
    }

    anyURI (Base__ const &x)
      : Base__ (x)
    {
    }

    anyURI (C const *x)
      : Base__ (x)
    {
    }

    anyURI (anyURI const &) = default;

    anyURI &operator= (Base__ const &x)
    {
      static_cast<Base__ &> (*this) = x;
      return *this;
    }
  };
}

#endif // XMLSCHEMA_TYPES_HPP
