/**
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "mock_array.hpp"

my_array_slice *
TAO::Array_Traits<my_array,my_array_slice,my_array_tag>::alloc()
{
  return new my_array;
}

void
TAO::Array_Traits<my_array,my_array_slice,my_array_tag>::free(
    my_array_slice * _tao_slice)
{
  delete[] _tao_slice;
}

my_array_slice *
TAO::Array_Traits<my_array,my_array_slice,my_array_tag>::dup(
    my_array_slice const * _tao_source)
{
  // TODO exception safety
  my_array_slice * _tao_destination = alloc();
  copy(_tao_destination, _tao_source);
  return _tao_destination;
}

void
TAO::Array_Traits<my_array,my_array_slice,my_array_tag>::copy(
    my_array_slice * _tao_destination,
    my_array_slice const * _tao_source)
{
  std::copy(
      _tao_source, _tao_source + sizeof(my_array)/sizeof(my_array_slice),
      _tao_destination);
}

void
TAO::Array_Traits<my_array,my_array_slice,my_array_tag>::
zero(
    my_array_slice * _tao_slice)
{
  std::fill(
      _tao_slice, _tao_slice + sizeof(my_array)/sizeof(my_array_slice),
      0);
}
