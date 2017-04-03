//                                               -*- C++ -*-
/**
 *
 *  Copyright 2005-2017 Airbus-EDF-IMACS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "openturns/InverseBoxCoxTransform.hxx"
#include "openturns/InverseBoxCoxEvaluation.hxx"
#include "openturns/InverseBoxCoxGradient.hxx"
#include "openturns/InverseBoxCoxHessian.hxx"
#include "openturns/BoxCoxTransform.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(InverseBoxCoxTransform);

/* Default constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform()
  : Function(InverseBoxCoxEvaluation())
{
  // Nothing to do
}

/* Standard parameter constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform(const Point & lambda)
  : Function()
{
  const InverseBoxCoxEvaluation evaluation(lambda);
  setEvaluation(evaluation.clone());
  setGradient(InverseBoxCoxGradient(evaluation).clone());
  setHessian(InverseBoxCoxHessian(evaluation).clone());
}

/* NumericalScalarCollection parameter constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform(const Point & lambda,
    const Point & shift)
  : Function()
{
  const InverseBoxCoxEvaluation evaluation(lambda, shift);
  setEvaluation(evaluation.clone());
  setGradient(InverseBoxCoxGradient(evaluation).clone());
  setHessian(InverseBoxCoxHessian(evaluation).clone());
}

/* 1D NumericalScalar parameter constructor */
InverseBoxCoxTransform::InverseBoxCoxTransform(const NumericalScalar & lambda)
  : Function()
{
  const InverseBoxCoxEvaluation evaluation(Point(1, lambda));
  setEvaluation(evaluation.clone());
  setGradient(InverseBoxCoxGradient(evaluation).clone());
  setHessian(InverseBoxCoxHessian(evaluation).clone());
}

InverseBoxCoxTransform::InverseBoxCoxTransform(const NumericalScalar & lambda,
    const NumericalScalar & shift)
  : Function()
{
  const InverseBoxCoxEvaluation evaluation(Point(1, lambda), Point(1, shift));
  setEvaluation(evaluation.clone());
  setGradient(InverseBoxCoxGradient(evaluation).clone());
  setHessian(InverseBoxCoxHessian(evaluation).clone());
}

/* Virtual constructor */
InverseBoxCoxTransform * InverseBoxCoxTransform::clone() const
{
  return new InverseBoxCoxTransform(*this);
}

/* Lambda accessor */
Point InverseBoxCoxTransform::getLambda() const
{
  return dynamic_cast<InverseBoxCoxEvaluation *>(getEvaluation().get())->getLambda();
}

/* Shift accessor */
Point InverseBoxCoxTransform::getShift() const
{
  return dynamic_cast<InverseBoxCoxEvaluation *>(getEvaluation().get())->getShift();
}

/* Inverse accessor */
BoxCoxTransform InverseBoxCoxTransform::getInverse() const
{
  return BoxCoxTransform(getLambda(), getShift());
}

END_NAMESPACE_OPENTURNS
