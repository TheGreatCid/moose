#include "ExplicitAccelAux.h"

registerMooseObject("ContactApp", ExplicitAccelAux);

InputParameters
ExplicitAccelAux::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Computes acceleration for use in explicit contact dynamics");
  params.addRequiredParam<Real>("rho", "The density");
  params.addRequiredCoupledVar("Fext", "External Force");
  params.addRequiredCoupledVar("Fint", "Internal Force");
  params.addRequiredCoupledVar("Pc", "Contact pressure");
  params.addRequiredCoupledVar("nodal_area", "The nodal area");
  return params;
}

ExplicitAccelAux::ExplicitAccelAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _rho(getParam<Real>("rho")),
    _Fint(coupledValue("Fext")),
    _Fext(coupledValue("Fint")),
    _Pc(coupledValue("Pc")),
    _nodal_area(coupledValue("nodal_area"))
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
  Real _Fc = 0;
  if (_Pc[_qp] != 0)
    _Fc = _Pc[_qp] / _nodal_area[_qp];

  // Computing mass at node
  Real mass = (_rho * vol) / 8;

  // if (_nodal_area[_qp] != 0)
  // {
  //   std::cout << _nodal_area[_qp] << std::endl;
  //   mooseError("good error");
  // }

  if (isnan(vol))
  {
    mooseError("Vol is NaN!");
  }
  else if (isnan(_nodal_area[_qp]))
  {
    mooseError("nodal_area[_qp] is NaN!");
  }

  return (1 / mass) * (_Fext[_qp] - _Fint[_qp] * mass - _Fc);
}
