// file      : XSCRT/OptionalArg.hpp
// author    : Johnny Willemsen <jwillemsen@remedy.nl>

#ifndef XSCRT_OPTIONAL_ARG_HPP
#define XSCRT_OPTIONAL_ARG_HPP

#include <memory>
#include <optional>

namespace XSCRT
{
  // A non-owning optional argument for generated constructors. The referenced
  // value only needs to live until the constructor call has completed.
  template <typename T>
  class OptionalArg
  {
  public:
    OptionalArg (std::nullopt_t) noexcept
    {
    }

    OptionalArg (T const &value) noexcept
      : value_ (std::addressof (value))
    {
    }

    OptionalArg (T const &&) = delete;

    explicit operator bool () const noexcept
    {
      return value_ != nullptr;
    }

    T const &operator* () const noexcept
    {
      return *value_;
    }

  private:
    T const *value_{};
  };
}

#endif // XSCRT_OPTIONAL_ARG_HPP
