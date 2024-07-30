//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "DirectDirichletBCBase.h"
#include "NonlinearSystemBase.h"

InputParameters
DirectDirichletBCBase::validParams()
{
  InputParameters params = NodalBC::validParams();
  params.addParam<TagName>("mass_matrix_tag", "mass", "The tag for the mass matrix");

  return params;
}

DirectDirichletBCBase::DirectDirichletBCBase(const InputParameters & parameters)
  : NodalBC(parameters),
    _preset(false),
    _nonlinear_implicit_system(dynamic_cast<NonlinearImplicitSystem *>(&_sys.system())),
    _mass_matrix()
{
}

void
DirectDirichletBCBase::initialSetup()
{
  _mass_matrix = &_nonlinear_implicit_system->get_system_matrix();
}

void
DirectDirichletBCBase::computeValue(NumericVector<Number> & current_solution)
{
  mooseAssert(_preset, "BC is not preset");

  if (_var.isNodalDefined())
  {
    const dof_id_type & dof_idx = _var.nodalDofIndex();
    current_solution.set(dof_idx, computeQpValue());
  }
}

Real
DirectDirichletBCBase::computeQpResidual()
{
  // Get dof for current var
  auto dofnum =
      _current_node->dof_number(_sys.number(), variable().number(), _sys.dofMap().sys_number());

  // Get lumped component of mass matrix
  Real masslump = 0;
  for (int j = 0; j < int(_mass_matrix->n()); j++)
    masslump += (*_mass_matrix)(dofnum, j);

  // Compute residual to enforce BC
  // This is the force required to enforce the BC in a central difference scheme
  Real resid = (computeQpValue() - _u[_qp]) / (_dt * _dt) - (*_sys.solutionUDotOld())(dofnum) / _dt;
  resid *= -masslump;

  return resid;
}
