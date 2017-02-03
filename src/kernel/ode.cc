//
// APFEL++ 2017
//
// Authors: Valerio Bertone: valerio.bertone@cern.ch
//          Stefano Carrazza: stefano.carrazza@cern.ch
//

#include "apfel/ode.h"

namespace apfel
{
  function<double (double const&, double const&, double const&)>
  rk4(function<double(double const&,double const&)> const& f)
  {
    return
      [       f            ](double const& t, double const& y, double const& dt ) -> double{ return
      [t,y,dt,f            ](                                  double const& dy1) -> double{ return
      [t,y,dt,f,dy1        ](                                  double const& dy2) -> double{ return
      [t,y,dt,f,dy1,dy2    ](                                  double const& dy3) -> double{ return
      [t,y,dt,f,dy1,dy2,dy3](                                  double const& dy4) -> double{ return
      ( dy1 + 2*dy2 + 2*dy3 + dy4 ) / 6   ;} (
      dt * f( t+dt  , y+dy3   )          );} (
      dt * f( t+dt/2, y+dy2/2 )          );} (
      dt * f( t+dt/2, y+dy1/2 )          );} (
      dt * f( t     , y       )          );} ;
  }
}