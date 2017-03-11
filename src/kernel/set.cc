//
// APFEL++ 2017
//
// Authors: Valerio Bertone: valerio.bertone@cern.ch
//          Stefano Carrazza: stefano.carrazza@cern.ch
//

#include "apfel/set.h"
#include "apfel/distribution.h"
#include "apfel/operator.h"
#include "apfel/tools.h"

namespace apfel {

  //_________________________________________________________________________
  template<class T>
  Set<T>::Set(ConvolutionMap const& map, unordered_map<int,T> const& in):
    _map(map),
    _objects(in)
  {
  }

  //_________________________________________________________________________
  template<class T>
  template<class V> Set<V> Set<T>::operator *= (Set<V> const& d) const
  {
    if (_map.GetName() != d.GetMap().GetName())
      throw runtime_exception("Set::operator *=", "Convolution Map does not match (1)");

    unordered_map<int,V> mmap;
    for (auto item = _map.GetRules().begin(); item != _map.GetRules().end(); item++)
      {
	// If an element of the map with the same rules has already been computed,
	// retrieve it and use it.
	bool cycle = false;
	for (auto it = _map.GetRules().begin(); it != item; it++)
	  if(it->second == item->second)
	    {
	      mmap.insert({item->first,mmap.at(it->first)});
	      cycle = true;
	      break;
	    }
	if (cycle) continue;

	// Start with the first object of the vector or rules
        auto o = std::begin(item->second);
        V result = _objects.at((*o).operand) * d.GetObjects().at((*o).object);

	// Multiply by the numerical coefficient only if it is different from one
	if((*o).coefficient != 1)
	  result *= (*o).coefficient;
        o++;

	// Continue with the following objects of the vector of rules
        for (auto end = std::end(item->second); o != end; o++)
	  // Multiply by the numerical coefficient only if it is different from one
	  if((*o).coefficient != 1)
	    result += (*o).coefficient * _objects.at((*o).operand) * d.GetObjects().at((*o).object);
	  else
	    result += _objects.at((*o).operand) * d.GetObjects().at((*o).object);

        mmap.insert({item->first,result});
      }

    return Set<V>{d.GetMap(),mmap};
  }

  //_________________________________________________________________________
  template<class T>
  Set<T>& Set<T>::operator *= (double const& s)
  {
    for (auto& v: _objects)
      v.second *= s;
    return *this;
  }

  //_________________________________________________________________________
  template<class T>
  Set<T>& Set<T>::operator /= (int const& s)
  {
    const double r = 1.0/static_cast<double>(s);
    for (auto& v: _objects)
      v.second *= r;
    return *this;
  }

  //_________________________________________________________________________
  template<class T>
  Set<T>& Set<T>::operator *= (Set<T> const& d)
  {
    if (_map.GetName() != d.GetMap().GetName())
      throw runtime_exception("Set::operator *=", "Convolution Map does not match (2)");

    for (auto& v: _objects)
      v.second *= d.at(v.first);

    return *this;
  }

  //_________________________________________________________________________
  template<class T>
  Set<T>& Set<T>::operator += (Set<T> const& d)
  {
    if (_map.GetName() != d.GetMap().GetName())
      throw runtime_exception("Set::operator +=", "Convolution Map does not match");

    for (auto& v: _objects)
      v.second += d.at(v.first);

    return *this;
  }

  template class Set<Distribution>;
  template class Set<Operator>;
  template Set<Distribution> Set<Operator>::operator *= (Set<Distribution> const&) const;
}
