
#pragma once

#include "AuxKernel.h"
#include "Assembly.h"

class ExplicitAccelAux : public AuxKernel
{
public:
  static InputParameters validParams();

  ExplicitAccelAux(const InputParameters & parameters);

  virtual ~ExplicitAccelAux() {}

protected:
  virtual Real computeValue();

  // Density
  Real _rho;

  //Internal Forces
  const VariableValue & _Fint;

  //External Forces
  const VariableValue & _Fext;

  //Contact Force
  const VariableValue & _Pc;

  //Nodal area
  const VariableValue & _nodal_area;
};
