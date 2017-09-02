//
// APFEL++ 2017
//
// Authors: Valerio Bertone: valerio.bertone@cern.ch
//          Stefano Carrazza: stefano.carrazza@cern.ch
//

#include "apfel/grid.h"
#include "apfel/dglapbuilder.h"
#include "apfel/dglap.h"
#include "apfel/operator.h"
#include "apfel/set.h"
#include "apfel/timer.h"
#include "apfel/tools.h"
#include "apfel/splittingfunctions.h"
#include "apfel/matchingconditions.h"

using namespace std;

namespace apfel {
  //_____________________________________________________________________________
  map<int,DglapObjects> InitializeDglapObjectsQCD(Grid           const& g,
						  vector<double> const& Masses,
						  vector<double> const& Thresholds,
						  double         const& IntEps)
  {
    cout << "Initializing DglapObjects for QCD evolution... ";
    Timer t;
    t.start();

    // Compute initial and final number of active flavours according
    // to the vector of thresholds (it assumes that the thresholds
    // vector entries are ordered).
    int nfi = 0;
    int nff = Thresholds.size();
    for (auto const& v : Thresholds)
      if (v <= 0)
	nfi++;

    // Compute logs of muth2 / m2 needed for the the matching
    // conditions.
    vector<double> LogKth;
    for (int im = 0; im < (int) Thresholds.size(); im++)
      if (Thresholds[im] < eps12 || Masses[im] < eps12)
	LogKth.push_back(0);
      else
	LogKth.push_back(2 * log( Thresholds[im] / Masses[im] ));

    // Allocate needed operators (matching conditions and splitting
    // functions).  By now the code is fast enough to precompute
    // everything at all available perturbative orders and the current
    // perturbative order is accounted for only when the actual
    // splitting functions and matching conditions (lambda) functions
    // are defined.
    // ===============================================================
    // LO Matching conditions.
    map<int,Operator> MatchLO;
    const Operator Id  {g, Identity{}, IntEps};
    const Operator Zero{g, Null{},     IntEps};
    MatchLO.insert({MatchingBasisQCD::PNSP, Id});
    MatchLO.insert({MatchingBasisQCD::PNSM, Id});
    MatchLO.insert({MatchingBasisQCD::PNSV, Id});
    MatchLO.insert({MatchingBasisQCD::PQQ,  Id});
    MatchLO.insert({MatchingBasisQCD::PQG,  Zero});
    MatchLO.insert({MatchingBasisQCD::PGQ,  Zero});
    MatchLO.insert({MatchingBasisQCD::PGG,  Id});
    for (int i = MatchingBasisQCD::PT3Q; i <= MatchingBasisQCD::PT35Q; i++)
      MatchLO.insert({i, Id});
    for (int i = MatchingBasisQCD::PT3G; i <= MatchingBasisQCD::PT35G; i++)
      MatchLO.insert({i, Zero});

    // ===============================================================
    // LO splitting functions operators.
    map<int,map<int,Operator>> OpMapLO;
    const Operator O0ns{g, P0ns{}, IntEps};
    const Operator O0qg{g, P0qg{}, IntEps};
    const Operator O0gq{g, P0gq{}, IntEps};
    for (int nf = nfi; nf <= nff; nf++)
      {
	const Operator O0gg{g, P0gg{nf}, IntEps};
	const Operator O0qgnf = nf * O0qg;
	map<int,Operator> OM;
	OM.insert({EvolutionBasisQCD::PNSP, O0ns});
	OM.insert({EvolutionBasisQCD::PNSM, O0ns});
	OM.insert({EvolutionBasisQCD::PNSV, O0ns});
	OM.insert({EvolutionBasisQCD::PQQ,  O0ns});
	OM.insert({EvolutionBasisQCD::PQG,  O0qgnf});
	OM.insert({EvolutionBasisQCD::PGQ,  O0gq});
	OM.insert({EvolutionBasisQCD::PGG,  O0gg});
	OpMapLO.insert({nf,OM});
      }

    // ===============================================================
    // NLO splitting functions operators.
    map<int,map<int,Operator>> OpMapNLO;
    for (int nf = nfi; nf <= nff; nf++)
      {
	const Operator O1nsp{g, P1nsp{nf}, IntEps};
	const Operator O1nsm{g, P1nsm{nf}, IntEps};
	const Operator O1ps {g, P1ps{nf},  IntEps};
	const Operator O1qg {g, P1qg{nf},  IntEps};
	const Operator O1gq {g, P1gq{nf},  IntEps};
	const Operator O1gg {g, P1gg{nf},  IntEps};
	const Operator O1qq = O1nsp + O1ps;
	map<int,Operator> OM;
	OM.insert({EvolutionBasisQCD::PNSP, O1nsp});
	OM.insert({EvolutionBasisQCD::PNSM, O1nsm});
	OM.insert({EvolutionBasisQCD::PNSV, O1nsm});
	OM.insert({EvolutionBasisQCD::PQQ,  O1qq});
	OM.insert({EvolutionBasisQCD::PQG,  O1qg});
	OM.insert({EvolutionBasisQCD::PGQ,  O1gq});
	OM.insert({EvolutionBasisQCD::PGG,  O1gg});
	OpMapNLO.insert({nf,OM});
      }

    // ===============================================================
    // Allocate NNLO Matching conditions.
    map<int,map<int,Operator>> MatchNNLO;  
    const Operator APS2Hq {g, APS2Hq_0{},  IntEps};
    const Operator ANS2qqH{g, ANS2qqH_0{}, IntEps};
    const Operator AS2Hg  {g, AS2Hg_0{},   IntEps};
    const Operator AS2gqH {g, AS2gqH_0{},  IntEps};
    const Operator AS2ggH {g, AS2ggH_0{},  IntEps};
    const Operator AS2qqH = ANS2qqH + APS2Hq;
    for (int nf = nfi; nf <= nff; nf++)
      {
	map<int,Operator> OM;
	OM.insert({MatchingBasisQCD::PNSP, ANS2qqH});
	OM.insert({MatchingBasisQCD::PNSM, ANS2qqH});
	OM.insert({MatchingBasisQCD::PNSV, ANS2qqH});
	OM.insert({MatchingBasisQCD::PQQ,  AS2qqH});
	OM.insert({MatchingBasisQCD::PQG,  AS2Hg});
	OM.insert({MatchingBasisQCD::PGQ,  AS2gqH});
	OM.insert({MatchingBasisQCD::PGG,  AS2ggH});
	const Operator AS2TqH = ANS2qqH - nf * APS2Hq;
	const Operator AS2Tg  = - nf * AS2Hg;
	for (int i = MatchingBasisQCD::PT3Q; i <= MatchingBasisQCD::PT35Q; i++)
	  if (i > MatchingBasisQCD::PT3Q + nf - 1)
	    OM.insert({i, AS2qqH});
	  else if (i == MatchingBasisQCD::PT3Q + nf - 1)
	    OM.insert({i, AS2TqH});
	  else
	    OM.insert({i, Zero});
	for (int i = MatchingBasisQCD::PT3G; i <= MatchingBasisQCD::PT35G; i++)
	  if (i > MatchingBasisQCD::PT3G + nf - 1)
	    OM.insert({i, AS2Hg});
	  else if (i == MatchingBasisQCD::PT3G + nf - 1)
	    OM.insert({i, AS2Tg});
	  else
	    OM.insert({i, Zero});
	MatchNNLO.insert({nf,OM});
      }

    // ===============================================================
    // Allocate NNLO splitting functions operators.
    map<int,map<int,Operator>> OpMapNNLO;
    for (int nf = nfi; nf <= nff; nf++)
      {
	const Operator O2nsp{g, P2nsp{nf}, IntEps};
	const Operator O2nsm{g, P2nsm{nf}, IntEps};
	const Operator O2nss{g, P2nss{nf}, IntEps};
	const Operator O2ps {g, P2ps{nf},  IntEps};
	const Operator O2qg {g, P2qg{nf},  IntEps};
	const Operator O2gq {g, P2gq{nf},  IntEps};
	const Operator O2gg {g, P2gg{nf},  IntEps};
	const Operator O2qq  = O2nsp + O2ps;
	const Operator O2nsv = O2nsm + O2nss;
	map<int,Operator> OM;
	OM.insert({EvolutionBasisQCD::PNSP, O2nsp});
	OM.insert({EvolutionBasisQCD::PNSM, O2nsm});
	OM.insert({EvolutionBasisQCD::PNSV, O2nsv});
	OM.insert({EvolutionBasisQCD::PQQ,  O2qq});
	OM.insert({EvolutionBasisQCD::PQG,  O2qg});
	OM.insert({EvolutionBasisQCD::PGQ,  O2gq});
	OM.insert({EvolutionBasisQCD::PGG,  O2gg});
	OpMapNNLO.insert({nf,OM});
      }

    // Define object of the structure containing the DglapObjects
    map<int,DglapObjects> DglapObj;

    // Allocate convolution maps for evolution and matching, and set
    // of operators.
    for (auto nf = nfi; nf <= nff; nf++)
      {
	const EvolutionBasisQCD evb{nf};
	const MatchingBasisQCD  mtb{nf};
	DglapObjects obj;
	obj.Threshold = Thresholds[nf-1];
	obj.SplittingFunctions.insert({0, Set<Operator>{evb, OpMapLO.at(nf)}});
	obj.SplittingFunctions.insert({1, Set<Operator>{evb, OpMapNLO.at(nf)}});
	obj.SplittingFunctions.insert({2, Set<Operator>{evb, OpMapNNLO.at(nf)}});
	obj.MatchingConditions.insert({0, Set<Operator>{mtb, MatchLO}});
	obj.MatchingConditions.insert({2, Set<Operator>{mtb, MatchNNLO.at(nf)}});
	DglapObj.insert({nf,obj});
      }
    t.stop();

    return DglapObj;
  }

  //_____________________________________________________________________________
  map<int,DglapObjects> InitializeDglapObjectsQCD(Grid           const& g,
						  vector<double> const& Thresholds,
						  double         const& IntEps)
  {
    return InitializeDglapObjectsQCD(g, Thresholds, Thresholds, IntEps);
  }

  //_____________________________________________________________________________
  unique_ptr<Dglap> BuildDglap(map<int,DglapObjects>                                   const& DglapObj,
			       function<map<int,double>(double const&, double const&)> const& InDistFunc,
			       double                                                  const& MuRef,
			       int                                                     const& PerturbativeOrder,
			       function<double(double const&)>                         const& Alphas,
			       int                                                     const& nsteps)
  {
    // Compute coupling above and below the thresholds.
    map<int,double> asThUp;
    map<int,double> asThDown;
    vector<double> Thresholds;
    for(auto const& obj : DglapObj)
      {
	const int    nf  = obj.first;
	const double thr = obj.second.Threshold;
	asThDown.insert({nf,Alphas(thr)/FourPi});
	asThUp.insert({nf,Alphas(thr+eps8)/FourPi});
	if ((int) Thresholds.size() < nf)
	  Thresholds.resize(nf);
	Thresholds[nf-1] = thr;
      }

    // Create splitting functions and matching conditions lambda
    // functions according to the requested perturbative order.
    function<Set<Operator>(int const&, double const&)> SplittingFunctions;
    function<Set<Operator>(bool const&, int const&)>   MatchingConditions;
    if (PerturbativeOrder == 0)
      {
        SplittingFunctions = [=] (int const& nf, double const& mu) -> Set<Operator>
	  {
	    const auto cp = Alphas(mu) / FourPi;
	    return cp * DglapObj.at(nf).SplittingFunctions.at(0);
	  };
	MatchingConditions = [=] (bool const&, int const& nf) -> Set<Operator>
	  {
	    return DglapObj.at(nf).MatchingConditions.at(0);
	  };
      }
    else if (PerturbativeOrder == 1)
      {
        SplittingFunctions = [=] (int const& nf, double const& mu) -> Set<Operator>
	  {
	    const auto cp = Alphas(mu) / FourPi;
	    return cp * ( DglapObj.at(nf).SplittingFunctions.at(0) + cp * DglapObj.at(nf).SplittingFunctions.at(1) );
	  };
	MatchingConditions = [=] (bool const&, int const& nf) -> Set<Operator>
	  {
	    return DglapObj.at(nf).MatchingConditions.at(0);
	  };
      }
    else if (PerturbativeOrder == 2)
      {
        SplittingFunctions = [=] (int const& nf, double const& mu) -> Set<Operator>
	  {
	    const auto cp = Alphas(mu) / FourPi;
	    return cp * ( DglapObj.at(nf).SplittingFunctions.at(0)
			  + cp * ( DglapObj.at(nf).SplittingFunctions.at(1)
				   + cp * DglapObj.at(nf).SplittingFunctions.at(2) ) );
	  };
	MatchingConditions = [=] (bool const& Up, int const& nf) -> Set<Operator>
	  {
	    const auto cp = asThUp.at(nf+1);
	    return DglapObj.at(nf).MatchingConditions.at(0) + ( Up ? 1 : -1) * cp * cp * DglapObj.at(nf).MatchingConditions.at(2);
	  };
      }

    // Create set of initial distributions.
    const Set<Distribution> InPDFs{DglapObj.at(NF(MuRef, Thresholds)).SplittingFunctions.at(0).GetMap(),
	DistributionMap(DglapObj.begin()->second.SplittingFunctions.at(0).at(0).GetGrid(), InDistFunc, MuRef)};

    // Initialize DGLAP evolution.
    return unique_ptr<Dglap>(new Dglap{SplittingFunctions, MatchingConditions, InPDFs, MuRef, Thresholds, nsteps});
  }

  //_____________________________________________________________________________
  unique_ptr<Dglap> BuildDglap(map<int,DglapObjects>                                      const& DglapObj,
			       function<double(int const&, double const&, double const&)> const& InDistFunc,
			       double                                                     const& MuRef,
			       int                                                        const& PerturbativeOrder,
			       function<double(double const&)>                            const& Alphas,
			       int                                                        const& nsteps)
  {
    const auto InDistFuncMap = [=] (double const& x, double const& Q) -> map<int,double>
      {
	map<int,double> DistMap;
	for (int i = EvolutionBasisQCD::GLUON; i <= EvolutionBasisQCD::V35; i++)
	  DistMap.insert({i,InDistFunc(i, x, Q)});
	return DistMap;
      };
    return BuildDglap(DglapObj, InDistFuncMap, MuRef, PerturbativeOrder, Alphas, nsteps);
  }

  //_____________________________________________________________________________
  unique_ptr<Dglap> BuildDglap(function<DglapObjects(double const&)>                   const& DglapObj,
			       vector<double>                                          const& Thresholds,
			       function<map<int,double>(double const&, double const&)> const& InDistFunc,
			       double                                                  const& MuRef,
			       int                                                     const& PerturbativeOrder,
			       function<double(double const&)>                         const& Alphas,
			       int                                                     const& nsteps)
  {
    // Compute initial and final number of active flavours according
    // to the vector of thresholds (it assumes that the thresholds
    // vector entries are ordered).
    int nfi = 0;
    int nff = Thresholds.size();
    for (auto const& v : Thresholds)
      if (v <= 0)
	nfi++;

    // Compute coupling above and below the thresholds.
    map<int,double> asThUp;
    map<int,double> asThDown;
    for (auto nf = nfi + 1; nf <= nff; nf++)
      {
	asThDown.insert({nf,Alphas(Thresholds[nf-1])/FourPi});
	asThUp.insert({nf,Alphas(Thresholds[nf-1]+eps8)/FourPi});
      }

    // Create splitting functions and matching conditions lambda
    // functions according to the requested perturbative order.
    function<Set<Operator>(int const&, double const&)> SplittingFunctions;
    function<Set<Operator>(bool const&, int const&)>   MatchingConditions;
    if (PerturbativeOrder == 0)
      {
        SplittingFunctions = [=] (int const&, double const& mu) -> Set<Operator>
	  {
	    const auto cp = Alphas(mu) / FourPi;
	    return cp * DglapObj(mu).SplittingFunctions.at(0);
	  };
	MatchingConditions = [=] (bool const&, int const& nf) -> Set<Operator>
	  {
	    return DglapObj(Thresholds[nf-1]).MatchingConditions.at(0);
	  };
      }
    else if (PerturbativeOrder == 1)
      {
        SplittingFunctions = [=] (int const&, double const& mu) -> Set<Operator>
	  {
	    const auto cp = Alphas(mu) / FourPi;
	    return cp * ( DglapObj(mu).SplittingFunctions.at(0) + cp * DglapObj(mu).SplittingFunctions.at(1) );
	  };
	MatchingConditions = [=] (bool const&, int const& nf) -> Set<Operator>
	  {
	    return DglapObj(Thresholds[nf-1]).MatchingConditions.at(0);
	  };
      }
    else if (PerturbativeOrder == 2)
      {
        SplittingFunctions = [=] (int const&, double const& mu) -> Set<Operator>
	  {
	    const auto cp = Alphas(mu) / FourPi;
	    return cp * ( DglapObj(mu).SplittingFunctions.at(0)
			  + cp * ( DglapObj(mu).SplittingFunctions.at(1)
				   + cp * DglapObj(mu).SplittingFunctions.at(2) ) );
	  };
	MatchingConditions = [=] (bool const& Up, int const& nf) -> Set<Operator>
	  {
	    const auto cp = asThUp.at(nf+1);
	    return DglapObj(Thresholds[nf-1]).MatchingConditions.at(0)
	    + ( Up ? 1 : -1) * cp * cp * DglapObj(Thresholds[nf-1]).MatchingConditions.at(2);
	  };
      }

    // Create set of initial distributions.
    const Set<Distribution> InPDFs{DglapObj(MuRef).SplittingFunctions.at(0).GetMap(),
	DistributionMap(DglapObj(MuRef).SplittingFunctions.at(0).at(0).GetGrid(), InDistFunc, MuRef)};

    // Initialize DGLAP evolution.
    return unique_ptr<Dglap>(new Dglap{SplittingFunctions, MatchingConditions, InPDFs, MuRef, Thresholds, nsteps});
  }

  //_____________________________________________________________________________
  unique_ptr<Dglap> BuildDglap(function<DglapObjects(double const&)>                      const& DglapObj,
			       vector<double>                                             const& Thresholds,
			       function<double(int const&, double const&, double const&)> const& InDistFunc,
			       double                                                     const& MuRef,
			       int                                                        const& PerturbativeOrder,
			       function<double(double const&)>                            const& Alphas,
			       int                                                        const& nsteps)
  {
    const auto InDistFuncMap = [=] (double const& x, double const& Q) -> map<int,double>
      {
	map<int,double> DistMap;
	for (int i = EvolutionBasisQCD::GLUON; i <= EvolutionBasisQCD::V35; i++)
	  DistMap.insert({i,InDistFunc(i, x, Q)});
	return DistMap;
      };
    return BuildDglap(DglapObj, Thresholds, InDistFuncMap, MuRef, PerturbativeOrder, Alphas, nsteps);
  }
}
