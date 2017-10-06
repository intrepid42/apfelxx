//
// APFEL++ 2017
//
// Authors: Valerio Bertone: valerio.bertone@cern.ch
//          Stefano Carrazza: stefano.carrazza@cern.ch
//

#pragma once

#include "apfel/set.h"
#include "apfel/distribution.h"

#include <functional>

using std::function;

namespace apfel
{
  /**
   * @brief Functions that return the fully differential Drell-Yan
   * cross section as function of the transverse momentum on the
   * vector boson qT, taking as inputs the evolved TMD PDFs, the
   * center-of-mass energy Vs, the vector-boson virtuality Q and
   * rapidity, the final TMD scales muf and zetaf.
   */
  function<double(double const&)> TmdCrossSectionDY(double                                                                   const& Vs,
						    double                                                                   const& Q,
						    double                                                                   const& y,
						    function<Set<Distribution>(double const&, double const&, double const&)> const& EvolvedTMDPDFs,
						    function<double(double const&)>                                          const& Alphas,
						    function<vector<double>(double const&)>                                  const& fEWCharges,
						    int                                                                      const& PerturbativeOrder,
						    vector<double>                                                           const& Thresholds,
						    double                                                                   const& muf,
						    double                                                                   const& zetaf);

  function<double(double const&)> TmdCrossSectionDY(double                                                                   const& Vs,
						    double                                                                   const& Q,
						    double                                                                   const& y,
						    function<Set<Distribution>(double const&, double const&, double const&)> const& EvolvedTMDPDFs,
						    function<double(double const&)>                                          const& Alphas,
						    function<vector<double>(double const&)>                                  const& fEWCharges,
						    int                                                                      const& PerturbativeOrder,
						    vector<double>                                                           const& Thresholds);

 /**
   * @brief Perturbative hard cross section for Drell-Yann.
   */
  double HardCrossSectionDY(int const& pt, double const& Alphas, int const& nf, double const& kappa);
}
