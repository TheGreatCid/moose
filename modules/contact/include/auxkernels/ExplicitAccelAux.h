
#pragma once

#include "AuxKernel.h"

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
  const MaterialProperty<RankTwoTensor> & _Fint;
  const MaterialProperty<RankTwoTensor> & _Fext;
  const MaterialProperty<RankTwoTensor> & _Fc;
};
