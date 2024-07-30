//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ExplicitTimeIntegrator.h"
#include "MooseError.h"
#include "libmesh/sparse_matrix.h"

class MassMatrixHelper;
/**
 * Implements a truly explicit (no nonlinear solve) first-order, forward Euler
 * time integration scheme.
 */
class CentralDifferenceDirect : public ExplicitTimeIntegrator
{
public:
  static InputParameters validParams();

  CentralDifferenceDirect(const InputParameters & parameters);

  virtual int order() override { return 1; }
  virtual void computeTimeDerivatives() override;

  virtual void solve() override;
  virtual void postResidual(NumericVector<Number> & residual) override;

  virtual bool performExplicitSolve(SparseMatrix<Number> & mass_matrix) override;

  void computeADTimeDerivatives(ADReal & ad_u_dot,
                                const dof_id_type & dof,
                                ADReal & ad_u_dotdot) const override
  {
    ad_u_dotdot.value();
    ad_u_dot.value();
    return mooseError("On %d - NOT SUPPORTED", dof);
  }

protected:
  const bool & _constant_mass;

  const TagName & _mass_matrix;
};
