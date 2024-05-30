#include "ExplicitAccelAux.h"

registerMooseObject("ContactApp", ExplicitAccelAux);

InputParameters
ExplicitAccelAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Computes acceleration for use in explicit contact dynamics");
  return params;
}

ExplicitAccelAux::ExplicitAccelAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _rho(getParam<Real>("rho")),
    _Fint(getMaterialProperty<RankTwoTensor>("internal_force")),
    _Fext(getMaterialProperty<RankTwoTensor>("external_force")),
    _Fc(getMaterialProperty<RankTwoTensor>("contact_force"))
{
}

Real
ExplicitAccelAux::computeValue()
{
  return 0;
}
