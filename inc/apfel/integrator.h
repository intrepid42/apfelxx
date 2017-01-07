//
// APFEL++ 2017
//
// Authors: Valerio Bertone: valerio.bertone@cern.ch
//          Stefano Carrazza: stefano.carrazza@cern.ch
//

#pragma once

namespace apfel
{
  /**
   * @brief The Integrator class which uses Adaptative Gaussian Quadrature.
   *
   * This class takes as input the integrand function and provides
   * the integrate method which performs the integration.
   */
  class Integrator
  {
  public:
    Integrator();  //!< The class constructor

    /**
     * @brief Integrates the integrand passed during initialization
     * between xmin and xmax with tolerance eps.
     *
     * @param xmin the lower bound integration value.
     * @param xmax the upper bound integration value.
     * @param eps the required relative error.
     * @return the integral value.
     */
    double integrate(double const& xmin, double const& xmax, double const& eps) const;

    /**
     * @brief Integrates the integrand passed during initialization
     * between xmin and xmax with m points.
     *
     * @param xmin the lower bound integration value.
     * @param xmax the upper bound integration value.
     * @param m the number of points of the Gauss quadrature.
     * @return the integral value.
     */
    double integrate(double const& xmin, double const& xmax, int const& m) const;

  protected:
    /**
     * @brief Protected virtual integrand function.
     *
     * @param x the integration variable.
     * @return the integrand evaluated at x.
     */
    virtual double integrand(double const& x) const = 0;

  };
}