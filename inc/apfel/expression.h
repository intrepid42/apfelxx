//
// APFEL++ 2017
//
// Authors: Valerio Bertone: valerio.bertone@cern.ch
//

#pragma once

namespace apfel
{
  /**
   * @brief The Expression class encapsulates in a proper form a given
   * analystic expression in such a way that it can be transformed
   * into an operator.
   */
  class Expression
  {
  public:
    /**
     * @name Constructors
     * List of constructors.
     */
    ///@{
    /**
     * @brief The constructor in the presence of a phase-space
     * reduction given by eta.
     * @param eta: upper limit of the convolution integral (default: 1)
     */
    Expression(double const& eta = 1);
    ///@}

    /**
     * @name Expression components
     * The three different possible components of an expression:
     * regular, singular, and local.
     */
    ///@{
    /**
     * @brief Virtual regular term.
     * @param x: the integration variable
     * @return The regular term at x
     */
    virtual double Regular(double const&) const { return 0; }

    /**
     * @brief Virtual singular term.
     * @param x: the integration variable
     * @return The singular term at x
     */
    virtual double Singular(double const&) const { return 0; }

    /**
     * @brief Virtual local term.
     * @param x: the physical variable
     * @return The local term at x
     */
    virtual double Local(double const&) const { return 0; }
    ///@}

    /**
     * @brief Function that returns the value of the scaling parameter
     * eta.
     */
    double eta() const { return _eta; }

    /**
     * @brief Function that sets the value of a possible external
     * variable.
     */
    void SetExternalVariable(double const& extvar) const { _extvar = extvar; }

  protected:
    double const   _eta;     //!< Mass parameter
    double mutable _extvar;  //!< External kinematic variable
  };

  /**
   * @defgroup RecExprs Recurrent expressions
   * Collection of recurrent expressions. This includes the identity
   * and the null expressions.
   */
  ///@{
  /**
   * @brief Derived class from Expression to implement the Identity
   * operator (delta function).
   */
  class Identity: public Expression
  {
  public:
    Identity(): Expression() { }
    double Local(double const&) const { return 1; }
  };

  /**
   * @brief Derived class from Expression to implement the Null
   * operator (zero).
   */
  class Null: public Expression
  {
  public:
    Null(): Expression() { }
  };
  ///@}
}
