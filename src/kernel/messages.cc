//
// APFEL++ 2017
//
// Authors: Valerio Bertone: valerio.bertone@cern.ch
//          Stefano Carrazza: stefano.carrazza@cern.ch
//

#include "apfel/messages.h"

namespace apfel
{
  //_________________________________________________________________________
  void SetVerbosityLevel(int const& vl)
  {
    VerbosityLevel = vl;
  }

  //_________________________________________________________________________
  int GetVerbosityLevel()
  {
    return VerbosityLevel;
  }

  //_________________________________________________________________________
  void info(string const& tag, string const& what)
  {
    if (VerbosityLevel > MEDIUM)
      cout << "\033[" << code::blue << "m[" << tag << "] Info: " << what << "\033[" << code::normal << "m\n";
  }

  //_________________________________________________________________________
  void warning(string const& tag, string const& what)
  {
    if (VerbosityLevel > LOW)
      cout << "\033[" << code::yellow << "m[" << tag << "] Warning: " << what << "\033[" << code::normal << "m\n";
  }

  //_________________________________________________________________________
  string error(string const& tag, string const& what)
  {
    stringstream ss;
    ss << "\033[" << code::red << "m[" << tag << "] Error: " << what << "\033[" << code::normal << "m\n";
    return ss.str();
  }
}