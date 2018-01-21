//
// APFEL++ 2017
//
// Authors: Valerio Bertone: valerio.bertone@cern.ch
//          Stefano Carrazza: stefano.carrazza@cern.ch
//

#include "apfel/matchingfunctions.h"
#include "apfel/constants.h"

#include <cmath>
#include <numeric>

using std::inner_product;

namespace apfel
{
  //_________________________________________________________________________________
  C1ns::C1ns():
    Expression()
  {
  }
  double C1ns::Regular(double const& x) const
  {
    return 2 * CF * ( 1 - x );
  }
  double C1ns::Local(double const&) const
  {
    return - CF * zeta2;
  }
  
  //_________________________________________________________________________________
  C1qg::C1qg():
    Expression()
  {
  }
  double C1qg::Regular(double const& x) const
  {
    return 8 * TR * x * ( 1 - x );
  }

  //_________________________________________________________________________________
  C1gq::C1gq():
    Expression()
  {
  }
  double C1gq::Regular(double const& x) const
  {
    return 2 * CF * x;
  }

  //_________________________________________________________________________________
  C1gg::C1gg():
    Expression()
  {
  }
  double C1gg::Local(double const&) const
  {
    return - CA * zeta2;
  }

  //_________________________________________________________________________________
  C2Vqq::C2Vqq(int const& nf):
    Expression(),
    _nf(nf)
  {
    _A2 = - 3232. / 27. + 112 * zeta3 + 448. * _nf / 81.;
    _A3 = 0.;
  }
  double C2Vqq::Regular(double const& x) const
  {
    const double fA4 = log(1-x);
    const double fA5 = fA4 * fA4;
    const double fA6 = fA5 * fA4;

    const double fB1 = log(x);
    const double fB2 = fB1 * fB1;
    const double fB3 = fB2 * fB1;
    const double fB4 = 1 / x;
    const double fB5 = fB1 * fB4;

    const double fc1 = 1;
    const double fc2 = x;
    const double fc3 = fc2 * fc2;
    const double fc4 = fc3 * fc2;
    const double fc5 = fA4 * fB1;
    const double fc6 = fc5 * fB1;

    const vector<double> fReg{fc1, fc2, fc3, fc4, fA4, fA5, fA6, fB1, fB2, fB3, fc5, fc6, fB4, fB5};
    const vector<double> CoeffReg{- 15.591812729537994 - 3.6403764478352265 * _nf,
	- 26.19077732354684 - 4.035868117241033 * _nf,
	61.362896852454675 - 0.6181383623814728 * _nf,
	- 18.603456697689623 - 0.19612093929051017 * _nf,
	200. / 9.,
	- 64. / 9.,
	0.,
	- 8. + 40. * _nf / 27.,
	- 2. + 4. * _nf / 9.,
	- 20. / 27.,
	32.70163574653397 - 0.069575874398736 * _nf,
	12.62916203247169 - 0.17334865938082786 * _nf,
	0.,
	0.};

    return inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }
  double C2Vqq::Singular(double const& x) const
  {
    const double fA2 = 1 / ( 1 - x );
    const double fA3 = fA2 * log(1-x);

    return _A2 * fA2 + _A3 * fA3;
  }
  double C2Vqq::Local(double const& x) const
  {
    const double ln1mx  = log(1-x);
    const double ln1mx2 = ln1mx * ln1mx;
    const double A1     = - 2416. / 81. - 67 * Pi2 / 9 + 448 * zeta3 / 9 + 20 * Pi2 * Pi2 / 81
      + _nf * ( 352. / 243. + 10 * Pi2 / 27 + 56 * zeta3 / 27 );

    return A1 + _A2 * ln1mx + _A3 * ln1mx2 / 2;
  }

  //_________________________________________________________________________________
  C2Vqqb::C2Vqqb()
  {
  }
  double C2Vqqb::Regular(double const& x) const
  {
    const double fA4 = log(1-x);
    const double fA5 = fA4 * fA4;
    const double fA6 = fA5 * fA4;

    const double fB1 = log(x);
    const double fB2 = fB1 * fB1;
    const double fB3 = fB2 * fB1;

    const double fc1 = 1;
    const double fc2 = x;
    const double fc3 = fc2 * fc2;
    const double fc4 = fc3 * fc2;
    const double fc5 = fA4 * fB1;
    const double fc6 = fc5 * fB1;

    const vector<double> fReg{fc1, fc2, fc3, fc4, fA4, fA5, fA6, fB1, fB2, fB3, fc5, fc6};

    const vector<double> CoeffReg{- 3.329144380448291,
	5.852825050121677,
	- 4.796244777535505,
	2.2557153775946435,
	0.,
	0.,
	0.,
	- 4. / 3.,
	0.,
	4. / 27.,
	0.505971782717726,
	0.08759292525297521};

    return inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }

  //_________________________________________________________________________________
  C2ps::C2ps():
    Expression()
  {
  }
  double C2ps::Regular(double const& x) const
  {
    const double fA4 = log(1-x);
    const double fA5 = fA4 * fA4;
    const double fA6 = fA5 * fA4;

    const double fB1 = log(x);
    const double fB2 = fB1 * fB1;
    const double fB3 = fB2 * fB1;
    const double fB4 = 1 / x;
    const double fB5 = fB1 * fB4;

    const double fc1 = 1;
    const double fc2 = x;
    const double fc3 = fc2 * fc2;
    const double fc4 = fc3 * fc2;
    const double fc5 = fA4 * fB1;
    const double fc6 = fc5 * fB1;

    const vector<double> fReg{fc1, fc2, fc3, fc4, fA4, fA5, fA6, fB1, fB2, fB3, fc5, fc6, fB4, fB5};
    const vector<double> CoeffReg{- 3.54792360390173,
	4.949994905325898,
	- 11.357230229521692,
	7.2245197835090105,
	0.,
	0.,
	0.,
	8. / 3.,
	- 2. / 3.,
	4. / 9.,
	2.255041649827519,
	0.7663091186642608,
	688. / 81. - 32 * zeta2 / 9,
	0.};

    return inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }

  //_________________________________________________________________________________
  C2qg::C2qg():
    Expression()
  {
  }
  double C2qg::Regular(double const& x) const
  {
    const double fA4 = log(1-x);
    const double fA5 = fA4 * fA4;
    const double fA6 = fA5 * fA4;

    const double fB1 = log(x);
    const double fB2 = fB1 * fB1;
    const double fB3 = fB2 * fB1;
    const double fB4 = 1 / x;
    const double fB5 = fB1 * fB4;

    const double fc1 = 1;
    const double fc2 = x;
    const double fc3 = fc2 * fc2;
    const double fc4 = fc3 * fc2;
    const double fc5 = fA4 * fB1;
    const double fc6 = fc5 * fB1;

    const vector<double> fReg{fc1, fc2, fc3, fc4, fA4, fA5, fA6, fB1, fB2, fB3, fc5, fc6, fB4, fB5};
    const vector<double> CoeffReg{5.32456247529479,
	- 37.8039790325823,
	55.939541231907775,
	- 44.592546819513025,
	- 5. / 3.,
	0.,
	5. / 9.,
	34. / 3.,
	- 7. / 6.,
	7. / 9.,
	- 10.961736574029366,
	- 6.522774793663304,
	172. / 9. - 4 * Pi2 / 3,
	0.};

    return 2 * inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }

  //_________________________________________________________________________________
  C2gq::C2gq(int const& nf):
    Expression(),
    _nf(nf)
  {
  }
  double C2gq::Regular(double const& x) const
  {
    const double fA4 = log(1-x);
    const double fA5 = fA4 * fA4;
    const double fA6 = fA5 * fA4;

    const double fB1 = log(x);
    const double fB2 = fB1 * fB1;
    const double fB3 = fB2 * fB1;
    const double fB4 = 1 / x;
    const double fB5 = fB1 * fB4;

    const double fc1 = 1;
    const double fc2 = x;
    const double fc3 = fc2 * fc2;
    const double fc4 = fc3 * fc2;
    const double fc5 = fA4 * fB1;
    const double fc6 = fc5 * fB1;

    const vector<double> fReg{fc1, fc2, fc3, fc4, fA4, fA5, fA6, fB1, fB2, fB3, fc5, fc6, fB4, fB5};
    const vector<double> CoeffReg{- 43.40171996161091 - 16.984343881161237 * _nf,
	173.18374973344203 + 9.595722778694926 * _nf,
	- 88.4323955556243 - 2.1972189030825384 * _nf,
	- 8.718852731902624 + 1.1549244843223576 * _nf,
	- 184. / 9. + 32. * _nf / 27.,
	- 44. / 9. + 8. * _nf / 9.,
	- 40. / 27.,
	- 200. / 3., // !!!!!! - 200. / 9. in the paper  !!!!!!!
	112. / 9.,
	- 112. / 27.,
	- 40.28067793284879 - 0.552346106244215 * _nf,
	- 35.36530647197912 - 0.0627184829983808 * _nf,
	- 12640. / 27. + 352 * zeta2 / 3 + 192 * zeta3 + 896. * _nf / 81.,
	  0.};

    return inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }

  //_________________________________________________________________________________
  C2gg::C2gg(int const& nf):
    Expression(),
    _nf(nf)
  {
    _A2 = - 808. / 3. + 252 * zeta3 + 112. * _nf / 9.;
    _A3 = 0.;
  }
  double C2gg::Regular(double const& x) const
  {
    const double fA4 = log(1-x);
    const double fA5 = fA4 * fA4;
    const double fA6 = fA5 * fA4;

    const double fB1 = log(x);
    const double fB2 = fB1 * fB1;
    const double fB3 = fB2 * fB1;
    const double fB4 = 1 / x;
    const double fB5 = fB1 * fB4;

    const double fc1 = 1;
    const double fc2 = x;
    const double fc3 = fc2 * fc2;
    const double fc4 = fc3 * fc2;
    const double fc5 = fA4 * fB1;
    const double fc6 = fc5 * fB1;

    const vector<double> fReg{fc1, fc2, fc3, fc4, fA4, fA5, fA6, fB1, fB2, fB3, fc5, fc6, fB4, fB5};
    const vector<double> CoeffReg{160.3174084388 + 1.8798838185664 * _nf,
	235.25958439812186 - 13.729196847420008 * _nf,
	- 77.67188779845978 - 0.6784851744787738 * _nf,
	51.61266721115288 - 1.3169890096131303 * _nf,
	2. - 2. * _nf / 3.,
	- 12.,
	0.,
	- 293. / 3. + 74. * _nf / 9.,
	1. + 2. * _nf,
	- 4. + 8. * _nf / 27.,
	24.137288673188998 - 1.0158393615503878 * _nf,
	- 22.406633639406444 + 1.090527996849969 * _nf,
	- 3160. / 9. + 264 * zeta2 / 3 + 432 * zeta3 / 3 + 226. * _nf / 27.,
	0.};

    return 3 * inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }
  double C2gg::Singular(double const& x) const
  {
    const double fA2 = 1 / ( 1 - x );
    const double fA3 = fA2 * log(1-x);

    return _A2 * fA2 + _A3 * fA3;
  }
  double C2gg::Local(double const& x) const
  {
    const double ln1mx  = log(1-x);
    const double ln1mx2 = ln1mx * ln1mx;
    const double A1     = - 112. - 201 * zeta2 / 2 + 154 * zeta3 + 225 * zeta4 / 4
      + _nf * ( 548. / 27. + 5 * zeta2 - 28 * zeta3 / 3 )
      - 56.  *_nf * _nf / 81.;

    return A1 + _A2 * ln1mx + _A3 * ln1mx2 / 2;
  }

  //_________________________________________________________________________________
  C1nsff::C1nsff():
    Expression()
  {
  }
  double C1nsff::Regular(double const& x) const
  {
    return CF * ( 2 * ( 1 - x ) + 4 * ( 1 + x * x ) * log(x) / ( 1 - x ) );
  }
  double C1nsff::Local(double const&) const
  {
    return - CF * zeta2;
  }

  //_________________________________________________________________________________
  C1qgff::C1qgff():
    Expression()
  {
  }
  double C1qgff::Regular(double const& x) const
  {
    return 2 * CF * ( 2 * x + 4 * ( 1 + pow(1 - x, 2) ) * log(x) / x );
  }

  //_________________________________________________________________________________
  C1gqff::C1gqff():
    Expression()
  {
  }
  double C1gqff::Regular(double const& x) const
  {
    return TR * ( 4 * x * ( 1 - x ) + 4 * ( 1 - 2 * x * ( 1 - x ) ) * log(x) );
  }

  //_________________________________________________________________________________
  C1ggff::C1ggff():
    Expression()
  {
  }
  double C1ggff::Regular(double const& x) const
  {
    return 8 * CA * pow(1 - x * ( 1 - x ), 2) * log(x) / x / ( 1 - x );
  }
  double C1ggff::Local(double const&) const
  {
    return - CA * zeta2;
  }

  //_________________________________________________________________________________
  C2Vqqff::C2Vqqff(int const& nf):
    Expression(),
    _nf(nf)
  {
    _A2 = - 3232. / 27. + 112 * zeta3 + 448. * _nf / 81.;
    _A3 = 0.;
  }
  double C2Vqqff::Regular(double const& x) const
  {
    // 1)  Log[1-z],
    // 2)  log[1-z]^2
    // 3)  log[1-z]^3
    // 4)  1/z
    // 5)  log[z]/z
    // 6)  Log[z]^2/z
    // 7)  Log[z]^3/z
    // 8)  Log[z]
    // 9)  log[z]^2
    // 10) Log[z]^3
    // 11) 1
    // 12) z
    // 13) z^2
    // 14) zLog[z]/(1-z)
    // 15) z Log[z]
    // 16) z^2 Log[z]
    // 17) z Log[z]^2/(1-z)
    // 18) z Log[z]^2
    // 19) z Log[z]^3
    // 20) (Log[z]/(1-z)+1)Log[1-z]
    // 21) Log[z]Log[1-z]
    // 22) zLog[z]Log[1-z]
    // 23) (1-z)Log[z]/z
    // 24) (1-z)Log[1-z]
    // 25) (1-z) Log[1-z]^2

    const double f1  = log(1-x);
    const double f2  = f1 * f1;
    const double f3  = f2 * f1;

    const double f8  = log(x);
    const double f9  = f8 * f8;
    const double f10 = f9 * f8;

    const double f11 = 1;
    const double f12 = x;
    const double f13 = x * x;

    const double f4  = 1 / x;
    const double f5  = f8 * f4;
    const double f6  = f9 * f4;
    const double f7  = f10 * f4;

    const double f15 = x * f8;
    const double f16 = x * f15;

    const double f18 = f15 * f8;
    const double f19 = f18 * f8;

    const double f14 = f15 / ( 1 - x );
    const double f17 = f14 * f8;

    const double f20 = ( f8 / ( 1 - x ) + 1 ) * f1;
    const double f21 = f8 * f1;
    const double f22 = x * f21;
    const double f23 = ( 1 - x ) * f5;
    const double f24 = ( 1 - x ) * f1;
    const double f25 = ( 1 - x ) * f2;

    const vector<double> fReg{f1, f2, f3, f4, f5, f6, f7, f8, f9, f10,
	f11, f12, f13, f14, f15, f16, f17, f18, f19, f20,
	f21, f22, f23, f24, f25};

    const vector<double> CoeffReg{
      - 200. / 9.,
	64. / 9.,
	0.,
	0.,
	0.,
	0.,
	0.,
	1496. / 9. - 8. * _nf-560. * zeta2 / 9.,
	- 130. / 9. + 4. * _nf / 9.,
	- 140. / 27.,
	- 296. * _nf / 81. - 301.03247439776976,
	- 152. * _nf / 81. - 989.2167272286393,
	82.59056818996338,
	- 80. * _nf / 9. - 1063.98482846164,
	8. * _nf + 206.28577290245227,
	-18.651271690975136,
	8. * _nf / 9. + 83.00296625888389,
	- 4. * _nf / 9. - 70.52319745715631,
	- 4.911975080877064,
	- 1105.7693500845382,
	327.376932797376,
	- 109.45363459015105,
	- 174.6471655693391,
	- 112.83673919345797,
	- 3.5294575557396084};

    return inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }
  double C2Vqqff::Singular(double const& x) const
  {
    const double fA2 = 1 / ( 1 - x );
    const double fA3 = fA2 * log(1-x);

    return _A2 * fA2 + _A3 * fA3;
  }
  double C2Vqqff::Local(double const& x) const
  {
    const double ln1mx  = log(1-x);
    const double ln1mx2 = ln1mx * ln1mx;
    const double A1     = - 2416. / 81. - 67 * Pi2 / 9 + 448 * zeta3 / 9 + 20 * Pi2 * Pi2 / 81
      + _nf * ( 352. / 243. + 10 * Pi2 / 27 + 56 * zeta3 / 27 );

    return A1 + _A2 * ln1mx + _A3 * ln1mx2 / 2;
  }

  //_________________________________________________________________________________
  C2Vqqbff::C2Vqqbff()
  {
  }
  double C2Vqqbff::Regular(double const& x) const
  {
    const double f1  = log(1-x);
    const double f2  = f1 * f1;
    const double f3  = f2 * f1;

    const double f8  = log(x);
    const double f9  = f8 * f8;
    const double f10 = f9 * f8;

    const double f11 = 1;
    const double f12 = x;
    const double f13 = x * x;

    const double f4  = 1 / x;
    const double f5  = f8 * f4;
    const double f6  = f9 * f4;
    const double f7  = f10 * f4;

    const double f15 = x * f8;
    const double f16 = x * f15;

    const double f18 = f15 * f8;
    const double f19 = f18 * f8;

    const double f14 = f15 / ( 1 - x );
    const double f17 = f14 * f8;

    const double f20 = ( f8 / ( 1 - x ) + 1 ) * f1;
    const double f21 = f8 * f1;
    const double f22 = x * f21;
    const double f23 = ( 1 - x ) * f5;
    const double f24 = ( 1 - x ) * f1;
    const double f25 = ( 1 - x ) * f2;

    const vector<double> fReg{f1, f2, f3, f4, f5, f6, f7, f8, f9, f10,
	f11, f12, f13, f14, f15, f16, f17, f18, f19, f20,
	f21, f22, f23, f24, f25};

    const vector<double> CoeffReg{
      0.,
	0.,
	0.,
	0.,
	0.,
	0.,
	0.,
	- 76. / 9.,
	- 32. / 9.,
	- 4. / 3.,
	- 448.1996646927744,
	- 5647.490349524448,
	- 257.33492226817515,
	- 6353.024936485396,
	3394.3630072013384,
	84.62666948172131,
	483.57763340201967,
	- 518.3011843768235,
	- 0.3446236997186985,
	- 6484.394475423013,
	3796.7810643424796,
	339.26036997443003,
	- 436.86534933603923,
	1330.4397468097895,
	- 0.02218996661410778};

    return inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }

  //_________________________________________________________________________________
  C2psff::C2psff():
    Expression()
  {
  }
  double C2psff::Regular(double const& x) const
  {
    const double f1  = log(1-x);
    const double f2  = f1 * f1;
    const double f3  = f2 * f1;

    const double f8  = log(x);
    const double f9  = f8 * f8;
    const double f10 = f9 * f8;

    const double f11 = 1;
    const double f12 = x;
    const double f13 = x * x;

    const double f4  = 1 / x;
    const double f5  = f8 * f4;
    const double f6  = f9 * f4;
    const double f7  = f10 * f4;

    const double f15 = x * f8;
    const double f16 = x * f15;

    const double f18 = f15 * f8;
    const double f19 = f18 * f8;

    const double f14 = f15 / ( 1 - x );
    const double f17 = f14 * f8;

    const double f20 = ( f8 / ( 1 - x ) + 1 ) * f1;
    const double f21 = f8 * f1;
    const double f22 = x * f21;
    const double f23 = ( 1 - x ) * f5;
    const double f24 = ( 1 - x ) * f1;
    const double f25 = ( 1 - x ) * f2;

    const vector<double> fReg{f1, f2, f3, f4, f5, f6, f7, f8, f9, f10,
	f11, f12, f13, f14, f15, f16, f17, f18, f19, f20,
	f21, f22, f23, f24, f25};

    const vector<double> CoeffReg{
      0.,
	0.,
	0.,
	- 592. / 81. + 32. * zeta2 / 9.,
	32. / 9.,
	64. / 9.,
	0.,
	- 48.,
	- 22. / 3.,
	44. / 9.,
	- 28.813571629909163,
	206.17553030550255,
	76.0334481394452,
	251.93541929963473,
	- 169.05906218222225,
	- 32.29013619101719,
	- 10.685799944808078,
	3.7898590887852626,
	4.909581801691148,
	188.58291934528984,
	- 90.34188300607897,
	- 1.4634823045099683,
	18.62607672661471,
	- 16.127886782439663,
	0.0009797967055855182};

    return inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }

  //_________________________________________________________________________________
  C2qgff::C2qgff():
    Expression()
  {
  }
  double C2qgff::Regular(double const& x) const
  {
    const double f1  = log(1-x);
    const double f2  = f1 * f1;
    const double f3  = f2 * f1;

    const double f8  = log(x);
    const double f9  = f8 * f8;
    const double f10 = f9 * f8;

    const double f11 = 1;
    const double f12 = x;
    const double f13 = x * x;

    const double f4  = 1 / x;
    const double f5  = f8 * f4;
    const double f6  = f9 * f4;
    const double f7  = f10 * f4;

    const double f15 = x * f8;
    const double f16 = x * f15;

    const double f18 = f15 * f8;
    const double f19 = f18 * f8;

    const double f14 = f15 / ( 1 - x );
    const double f17 = f14 * f8;

    const double f20 = ( f8 / ( 1 - x ) + 1 ) * f1;
    const double f21 = f8 * f1;
    const double f22 = x * f21;
    const double f23 = ( 1 - x ) * f5;
    const double f24 = ( 1 - x ) * f1;
    const double f25 = ( 1 - x ) * f2;

    const vector<double> fReg{f1, f2, f3, f4, f5, f6, f7, f8, f9, f10,
	f11, f12, f13, f14, f15, f16, f17, f18, f19, f20,
	f21, f22, f23, f24, f25};

    const vector<double> CoeffReg{
      - 40./9. - 80. * zeta2 / 3.,
	- 40. /9.,
	40. / 27.,
	12512. / 27. - 352. * zeta2 / 3. - 352. * zeta3,
	400. / 3.,
	- 848. / 3.,
	- 320. / 3.,
	1144. / 3.,
	64. / 3.,
	- 1232. / 27.,
	- 11519.346897372414,
	- 34241.466106099186,
	5326.770104932414,
	- 40601.87518176106,
	4178.463030904903,
	- 1705.6350033291087,
	- 966.5754106411847,
	1144.6267544753136,
	- 84.66732541780037,
	- 11393.035115581788,
	- 25857.49295712562,
	- 10601.55795204891,
	- 12368.214954397781,
	- 29991.60756399795,
	- 5.282535747460972};

    return 2 * inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }

  //_________________________________________________________________________________
  C2gqff::C2gqff(int const& nf):
    Expression(),
    _nf(nf)
  {
  }
  double C2gqff::Regular(double const& x) const
  {
    const double f1  = log(1-x);
    const double f2  = f1 * f1;
    const double f3  = f2 * f1;

    const double f8  = log(x);
    const double f9  = f8 * f8;
    const double f10 = f9 * f8;

    const double f11 = 1;
    const double f12 = x;
    const double f13 = x * x;

    const double f4  = 1 / x;
    const double f5  = f8 * f4;
    const double f6  = f9 * f4;
    const double f7  = f10 * f4;

    const double f15 = x * f8;
    const double f16 = x * f15;

    const double f18 = f15 * f8;
    const double f19 = f18 * f8;

    const double f14 = f15 / ( 1 - x );
    const double f17 = f14 * f8;

    const double f20 = ( f8 / ( 1 - x ) + 1 ) * f1;
    const double f21 = f8 * f1;
    const double f22 = x * f21;
    const double f23 = ( 1 - x ) * f5;
    const double f24 = ( 1 - x ) * f1;
    const double f25 = ( 1 - x ) * f2;

    const vector<double> fReg{f1, f2, f3, f4, f5, f6, f7, f8, f9, f10,
	f11, f12, f13, f14, f15, f16, f17, f18, f19, f20,
	f21, f22, f23, f24, f25};

    const vector<double> CoeffReg{
      - 44. / 3. + 10. * _nf / 9. + 10. * zeta2,
	- 7. / 2. + _nf / 3.,
	- 5. / 9.,
	- 148. / 9. + 8. * zeta2,
	8.,
	16.,
	0.,
	- 157. / 3. - 10. * _nf / 3. - 100. * zeta2 / 3.,
	- 73. / 3. + _nf / 3.,
	77. / 9.,
	- 5042.153939120427 - 66.62886623235278 * _nf,
	303.86339171965324 - 317.3462434009907 * _nf,
	3703.409480057778 + 21.346846003738534 * _nf,
	- 1059.0639607339292 - 361.41246956998253 * _nf,
	- 8193.44131612091 + 116.14537890481989 * _nf,
	- 1046.4745476064809 - 18.841069748305667 * _nf,
	- 1458.9834262434276 + 5.214301688311585 * _nf,
	1590.9730212100676 - 6.201059346425027 * _nf,
	73.07631646309086 - 0.014948523947447324 * _nf,
	10420.770382128236 - 200.7638366136303 * _nf,
	- 20512.88777168424 - 49.38978687382705 * _nf,
	- 4683.056024325436 - 58.30447555159878 * _nf,
	- 4929.425576232434 - 65.41307217273041 * _nf,
	- 15198.304463044708 - 144.13788267277235 * _nf,
	- 11.384648952243019 - 0.6611076391799198 * _nf};

    return inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }

  //_________________________________________________________________________________
  C2ggff::C2ggff(int const& nf):
    Expression(),
    _nf(nf)
  {
    _A2 = - 808. / 3. + 252 * zeta3 + 112. * _nf / 9.;
    _A3 = 0.;
  }
  double C2ggff::Regular(double const& x) const
  {
    const double f1  = log(1-x);
    const double f2  = f1 * f1;
    const double f3  = f2 * f1;

    const double f8  = log(x);
    const double f9  = f8 * f8;
    const double f10 = f9 * f8;

    const double f11 = 1;
    const double f12 = x;
    const double f13 = x * x;

    const double f4  = 1 / x;
    const double f5  = f8 * f4;
    const double f6  = f9 * f4;
    const double f7  = f10 * f4;

    const double f15 = x * f8;
    const double f16 = x * f15;

    const double f18 = f15 * f8;
    const double f19 = f18 * f8;

    const double f14 = f15 / ( 1 - x );
    const double f17 = f14 * f8;

    const double f20 = ( f8 / ( 1 - x ) + 1 ) * f1;
    const double f21 = f8 * f1;
    const double f22 = x * f21;
    const double f23 = ( 1 - x ) * f5;
    const double f24 = ( 1 - x ) * f1;
    const double f25 = ( 1 - x ) * f2;

    const vector<double> fReg{f1, f2, f3, f4, f5, f6, f7, f8, f9, f10,
	f11, f12, f13, f14, f15, f16, f17, f18, f19, f20,
	f21, f22, f23, f24, f25};

    const vector<double> CoeffReg{
      - 6. + 2. * _nf,
	36.,
	0.,
	3268. / 3. + 278. * _nf / 81. - 264. * zeta2 - 792. * zeta3,
	536. - 244. * _nf / 9. - 360. * zeta2,
	- 660. - 8. * _nf / 9.,
	- 240.,
	819. - 50. * _nf + 360. * zeta2,
	33. + 22. * _nf / 3.,
	- 132. + 88. * _nf / 9.,
	- 4536.42238065816 - 163.95224892156722 * _nf,
	18080.936974539778 - 625.9193713983447 * _nf,
	14139.810571485807 - 184.19497231902477 * _nf,
	27937.822349639453 - 975.5233827623935 * _nf,
	- 35293.57801079519 + 498.8898228158924 * _nf,
	- 5939.483006875214 + 81.15085632871367 * _nf,
	- 6775.737946774227 + 30.343871838863492 * _nf,
	7209.990192354363 - 51.50813149633906 * _nf,
	- 720.1068548927861 + 9.691339026103627 * _nf,
	62244.278348304506 - 725.7040599397907 * _nf,
	- 73593.14246325135 + 340.92281843959506 * _nf,
	- 19274.85766344364 + 2.9241023778057107 * _nf,
	- 6776.731748013898 - 73.730026699345 * _nf,
	- 54745.95834168829 + 54.675824792462755 * _nf,
	- 17.510572839038723 - 0.003977517619150036 * _nf};

    return inner_product(fReg.begin(), fReg.end(), CoeffReg.begin(), 0.);
  }
  double C2ggff::Singular(double const& x) const
  {
    const double fA2 = 1 / ( 1 - x );
    const double fA3 = fA2 * log(1-x);

    return _A2 * fA2 + _A3 * fA3;
  }
  double C2ggff::Local(double const& x) const
  {
    const double ln1mx  = log(1-x);
    const double ln1mx2 = ln1mx * ln1mx;
    const double A1     = - 112. - 201 * zeta2 / 2 + 154 * zeta3 + 2385 * zeta4 / 4
      + _nf * ( 548. / 27. + 5 * zeta2 - 28 * zeta3 / 3 )
      - 56.  *_nf * _nf / 81.;

    return A1 + _A2 * ln1mx + _A3 * ln1mx2 / 2;
  }
}
