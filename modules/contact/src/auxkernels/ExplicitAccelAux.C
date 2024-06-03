#include "ExplicitAccelAux.h"

registerMooseObject("ContactApp", ExplicitAccelAux);

InputParameters
ExplicitAccelAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Computes acceleration for use in explicit contact dynamics");
  params.addRequiredParam<Real>("rho", "The density");
  params.addRequiredCoupledVar("Fext", "External Force");
  params.addRequiredCoupledVar("Fint", "InternalForce");
  params.addRequiredCoupledVar("Fc", "Contact Force");
  return params;
}

ExplicitAccelAux::ExplicitAccelAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _rho(getParam<Real>("rho")),
    _Fint(coupledValue("internal_force")),
    _Fext(coupledValue("external_force")),
    _Fc(coupledValue("contact_force"))
{
}

// METHOD ONLY WORKS FOR HEX8 ELEMENTS
Real
ExplicitAccelAux::computeValue()
{
  // Need to calculate acceleration
  // a_n = (rho*volume)^-1(Fext-Fint-Fc)

  // Computing volume of element at current _qp and surrounding elements
  Real vol = _assembly.elemVolume();
  vol += _assembly.neighborVolume();

  // Computing mass at node
  Real mass = (_rho * vol) / 8;

  return (1 / mass) * (_Fext[_qp] - _Fint[_qp] - _Fc[_qp]);
}
