set(kernel_source
  subgrid.cc
  grid.cc
  interpolator.cc
  lagrangeinterpolator.cc
  integrator.cc
  operator.cc
  expression.cc
  distribution.cc
  matchedevolution.cc
  tools.cc
  matrix.cc
  qgrid.cc
  convolutionmap.cc
  set.cc
  tabulateobject.cc
  dglap.cc
  observable.cc
  doubleobject.cc
  rotations.cc
  specialfunctions.cc
  messages.cc
  hplog.f
  )

add_library(kernel OBJECT ${kernel_source})
