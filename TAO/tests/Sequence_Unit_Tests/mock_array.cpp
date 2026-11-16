/**
 * @author Carlos O'Ryan
 */
#include "mock_array.hpp"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

my_array_slice *
TAO::Array_Traits<TAO::my_array_forany>::alloc()
{
  return new my_array;
}

void
TAO::Array_Traits<TAO::my_array_forany>::free(
    my_array_slice * _tao_slice)
{
  delete[] _tao_slice;
}

my_array_slice *
TAO::Array_Traits<TAO::my_array_forany>::dup(
    my_array_slice const * _tao_source)
{
  // TODO exception safety
  my_array_slice * _tao_destination = alloc();
  copy(_tao_destination, _tao_source);
  return _tao_destination;
}

void
TAO::Array_Traits<TAO::my_array_forany>::copy(
    my_array_slice * _tao_destination,
    my_array_slice const * _tao_source)
{
  std::copy(
      _tao_source, _tao_source + sizeof(my_array)/sizeof(my_array_slice),
      _tao_destination);
}

void
TAO::Array_Traits<TAO::my_array_forany>::
zero(
    my_array_slice * _tao_slice)
{
  std::fill(
      _tao_slice, _tao_slice + sizeof(my_array)/sizeof(my_array_slice),
      0);
}

TAO_END_VERSIONED_NAMESPACE_DECL
