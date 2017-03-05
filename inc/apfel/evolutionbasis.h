//
// APFEL++ 2017
//
// Authors: Valerio Bertone: valerio.bertone@cern.ch
//          Stefano Carrazza: stefano.carrazza@cern.ch
//

#pragma once

#include "apfel/convolutionmap.h"

using namespace std;

namespace apfel
{
  /**
   * @brief ConvolutionMap derivation for the QCD evolution basis.
   *
   * This class, following the derivation procedure from ConvolutionMap
   * implements the Basis enumerator with custom tags for the objects.
   */
  class EvolutionBasis: public ConvolutionMap
  {
  public:
    /**
     * @brief The map enums
     */
    enum Operand: int {PNSP, PNSM, PNSV, PQQ, PQG, PGQ, PGG};
    enum Object:  int {GLUON, SIGMA, VALENCE, T3, V3, T8, V8, T15, V15, T24, V24, T35, V35};

    /**
     * @brief The class constructor
     */
  EvolutionBasis(int const& nf):
    ConvolutionMap{"EvolutionBasis"}
    {
      // dg = Pgg * g + Pgq * Sigma
      _rules[GLUON] = { {PGG, GLUON, +1}, {PGQ, SIGMA, +1} };

      // dSigma = Pqg * g + ( Pnsp + Pps ) * Sigma
      _rules[SIGMA] = { {PQG, GLUON, +1}, {PQQ, SIGMA, +1} };

      // dV = Pnsv * V
      _rules[VALENCE] = { {PNSV, VALENCE, +1} };

      // d{T,V}3 = Pnsp * {T,V}3
      if (nf > 1)
	{
	  _rules[T3] = { {PNSP, T3, +1} };
	  _rules[V3] = { {PNSM, V3, +1} };
	}
      else
	{
	  _rules[T3] = _rules[SIGMA];
	  _rules[V3] = _rules[VALENCE];
	}

      // d{T,V}8 = Pnsp * {T,V}8
      if (nf > 2)
	{
	  _rules[T8] = { {PNSP, T8, +1} };
	  _rules[V8] = { {PNSM, V8, +1} };
	}
      else
	{
	  _rules[T8] = _rules[SIGMA];
	  _rules[V8] = _rules[VALENCE];
	}

      // d{T,V}15 = Pnsp * {T,V}15
      if (nf > 3)
	{
	  _rules[T15] = { {PNSP, T15, +1} };
	  _rules[V15] = { {PNSM, V15, +1} };
	}
      else
	{
	  _rules[T15] = _rules[SIGMA];
	  _rules[V15] = _rules[VALENCE];
	}

      // d{T,V}24 = Pnsp * {T,V}24
      if (nf > 4)
	{
	  _rules[T24] = { {PNSP, T24, +1} };
	  _rules[V24] = { {PNSM, V24, +1} };
	}
      else
	{
	  _rules[T24] = _rules[SIGMA];
	  _rules[V24] = _rules[VALENCE];
	}

      // d{T,V}35 = Pnsp * {T,V}35
      if (nf > 5)
	{
	  _rules[T35] = { {PNSP, T35, +1} };
	  _rules[V35] = { {PNSM, V35, +1} };
	}
      else
	{
	  _rules[T35] = _rules[SIGMA];
	  _rules[V35] = _rules[VALENCE];
	}
    };
  };

}