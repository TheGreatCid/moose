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
    _nonlinear_implicit_system(dynamic_cast<NonlinearImplicitSystem *>(&_sys.system()))
{
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
  _nonlinear_implicit_system->get_system_matrix()(1, 1);
  //   auto mass_tag = _fe_problem.currentNonlinearSystem().systemMatrixTag();
  //   auto mass_tag = _sys.subproblem().getMatrixTagID("mass");
  _mass_matrix.print();
  //   _fe_problem.computeJacobianTag(
  //       _fe_problem.currentNonlinearSystem().solution(), auto & mass_mat, mass_tag);
  //   auto test = _fe_problem.currentNonlinearSystem().getSharedTimeIntegrator();

  return _u[_qp] - computeQpValue();
}
