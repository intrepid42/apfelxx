//
// APFEL++ 2017
//
// Authors: Valerio Bertone: valerio.bertone@cern.ch
//

#include <math.h>
#include <LHAPDF/LHAPDF.h>
#include <apfel/apfelxx.h>
#include <yaml-cpp/yaml.h>
#include <stdlib.h>
#include <algorithm>

// b* prescription
double bstar(double const& b, double const& kappa, bool const& css)
{
  const double b0 = 2 * exp( - apfel::emc);
  const double bmax = b0 / kappa;
  if (css)
    return b / sqrt( 1 + pow(b / bmax, 2) );
  else
    return b;
}

// mu* prescription
double mustar(double const& mu, double const& kappa, bool const& css)
{
  const double b0 = 2 * exp( - apfel::emc);
  return b0 / bstar(b0 / mu, kappa, !css);
}

// Electroweak charges (local function to use the correct values of
// the physical parameters)
std::vector<double> EWCharges(double const& Q)
{
  // Relevant constants
  const double Q2    = Q * Q;
  const double MZ2   = pow(91.15348061918276, 2);
  const double GmZ2  = pow(2.4942663787728243, 2);
  const double S2ThW = 0.22283820939806087;
  const double VD    = - 0.5 + 2 * S2ThW / 3;
  const double VU    = + 0.5 - 4 * S2ThW / 3;
  const double AD    = - 0.5;
  const double AU    = + 0.5;

  const double Ve    = - 0.5 + 2 * S2ThW;
  const double Ae    = - 0.5;
  const std::vector<double> Vq = {VD, VU, VD, VU, VD, VU};
  const std::vector<double> Aq = {AD, AU, AD, AU, AD, AU};

  // Propagator and its square
  double PZ;
  double PZ2;
  PZ  = Q2 * ( Q2 -  MZ2 ) / ( pow(Q2 - MZ2,2) + MZ2 * GmZ2 ) / ( 4 * S2ThW * ( 1 - S2ThW ) );
  PZ2 = pow(Q2,2) / ( pow(Q2 - MZ2,2) + MZ2 * GmZ2 ) / pow(4 * S2ThW * ( 1 - S2ThW ),2);

  // Build electroweak charges
  std::vector<double> Charges(6, 0.);
  for (auto i = 0; i < 6; i++)
    Charges[i] = apfel::QCh2[i]
      - 2 * apfel::QCh[i] * Vq[i] * Ve * PZ
      + ( Ve * Ve + Ae * Ae ) * ( Vq[i] * Vq[i] + Aq[i] * Aq[i] ) * PZ2;

  return Charges;
}

// Main program
int main()
{
  // Which regularisation prescription
  const bool css = false;
  const double kappa = 1;

  // Open LHAPDF set
  const std::string pdfset = "NNPDF31_nnlo_as_0118_luxqed";
  LHAPDF::PDF* distpdf = LHAPDF::mkPDF(pdfset, 0);

  // Heavy-quark thresholds
  std::vector<double> Thresholds{0, 0, 0, 0, 0};
  //for (auto const& v : distpdf->flavors())
  //  if (v > 0 && v < 7)
  //    Thresholds.push_back(distpdf->quarkThreshold(v));

  // x-space grid (to setup APFEL++ computation)
  const apfel::Grid g{{{100, 1e-6, 3}, {60, 1e-1, 3}, {50, 6e-1, 3}, {50, 8e-1, 3}}};

  // Rotate PDF set into the QCD evolution basis
  const auto RotPDFs = [=] (double const& x, double const& mu) -> std::map<int,double>{ return apfel::PhysToQCDEv(distpdf->xfxQ(x, mu)); };

  // Construct set of distributions as a function of the scale to be
  // tabulated
  const auto EvolvedPDFs = [=,&g] (double const& mu) -> apfel::Set<apfel::Distribution>
    {
      return apfel::Set<apfel::Distribution>{apfel::EvolutionBasisQCD{apfel::NF(mu, Thresholds)}, DistributionMap(g, RotPDFs, mu)};
    };

  // Tabulate collinear PDFs
  const apfel::TabulateObject<apfel::Set<apfel::Distribution>> TabPDFs{EvolvedPDFs, 100, 1, distpdf->qMax(), 3, Thresholds};
  const auto CollPDFs = [&] (double const& mu) -> apfel::Set<apfel::Distribution> { return TabPDFs.Evaluate(mustar(mu, kappa, css)); };

  // Initialize TMD objects
  const auto TmdObj = apfel::InitializeTmdObjects(g, Thresholds);

  // Alpha_em
  const double aref = 0.007555310522369057;
  const double Qref = 91.15348061918276;
  const bool   arun = false;
  const apfel::AlphaQED alphaem{aref, Qref, Thresholds, {0, 0, 1.777}, 0};

  // Perturbative order
  const int pt = 3;

  // Kinematics
  const double Vs = 13000;
  const double yb = 0;
  const double Qb = 91.15348061918276;

  // Double exponential quadrature
  apfel::DoubleExponentialQuadrature DEObj{};

  // Timer
  apfel::Timer t;

  // Alpha_s
  const double asref = 0.118;
  const double Qsref = 91.1876;
  apfel::AlphaQCD as{asref, Qsref, Thresholds, std::max(std::abs(pt), 0)};
  const apfel::TabulateObject<double> tabas{as, 100, 0.9, 1001, 3};
  const auto Alphas = [&] (double const& mu) -> double{ return tabas.Evaluate(mustar(mu, kappa, css)); };

  // Build evolved TMD PDFs
  const auto EvTMDPDFs = BuildTmdPDFs(TmdObj, CollPDFs, Alphas, pt, 1);

  // Renormalisation scales
  const double muf   = Qb;
  const double zetaf = Qb * Qb;
	  
  // Number of active flavours at 'muf'
  const int nf = apfel::NF(muf, Thresholds);

  // EW charges
  //const std::vector<double> Bq = apfel::ElectroWeakCharges(Qb, true);
  const std::vector<double> Bq = EWCharges(Qb);
	  
  // Compute Bjorken 'x1' and 'x2'
  const double xi = exp(yb);
  const double x1 = Qb * xi / Vs;
  const double x2 = Qb / xi / Vs;
	  
  // Electromagnetic coupling squared
  const double aem2 = pow((arun ? alphaem.Evaluate(Qb) : aref), 2);
	  
  // Compute the hard factor
  const double hcs = apfel::HardFactorDY(pt, Alphas(muf), nf, 1);
	  
  // Construct the TMD luminosity in b space to be fed to be
  // trasformed in qT space.
  const auto TMDLumib = [=] (double const& b) -> double
    {
      // Get Evolved TMD PDFs and rotate them into the physical
      // basis
      const std::map<int,apfel::Distribution> xF = QCDEvToPhys(EvTMDPDFs(bstar(b, kappa, css), muf, zetaf).GetObjects());
	      
      // Combine TMDs through the EW charges
      double lumi = 0;
      for (int i = 1; i <= nf; i++)
	lumi += Bq[i-1] * ( xF.at(i).Evaluate(x1) * xF.at(-i).Evaluate(x2) + xF.at(-i).Evaluate(x1) * xF.at(i).Evaluate(x2) );
	      
      // Combine all pieces and return
      return b * lumi;
    };

  // Linear scale in qT
  std::cout << "======================================\n";
  const int nqT = 50;
  const double qTmin = 0.001;
  const double qTmax = 20;
  const double qTstp = ( qTmax - qTmin ) / ( nqT - 1 );
  for (double qT = qTmin; qT <= qTmax; qT += qTstp)
    {
      // Perform Fourier transform and obtain cross section
      const auto prefactor = [=] (double const& qTc) -> double { return apfel::ConvFact * qTc * 8 * M_PI * aem2 * hcs / pow(Qb, 3) / 9; };

      // Integration
      const double de = prefactor(qT) * DEObj.transform(TMDLumib, qT);

      // Print results
      std::cout << std::scientific << Qb << "  " << yb << "  " << qT << "  " << de << std::endl;
    }

  std::cout << "======================================\n";
  const int nbT = 100;
  const double bTmin = 1e-5;
  const double bTmax = 1.5;
  const double bTstp = ( bTmax - bTmin ) / ( nbT - 1 );
  for (double bT = bTmin; bT <= bTmax; bT += bTstp)
    {
      // Perform Fourier transform and obtain cross section
      const auto prefactor = apfel::ConvFact * 8 * M_PI * aem2 * hcs / pow(Qb, 3) / 9;

      // Integration
      const double de = prefactor * TMDLumib(bT);

      // Print results
      std::cout << std::scientific << Qb << "  " << yb << "  " << bT << "  " << de << std::endl;
    }
  std::cout << std::endl;
  t.stop();
  
  delete distpdf;
  return 0;
}
