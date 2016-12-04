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
#include "openturns/RankMCovarianceModel.hxx"
#include "openturns/PersistentObjectFactory.hxx"
#include "openturns/Exception.hxx"
#include "openturns/ConstantBasisFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(RankMCovarianceModel);

static const Factory<RankMCovarianceModel> Factory_RankMCovarianceModel;


/* Default constructor */
RankMCovarianceModel::RankMCovarianceModel(const UnsignedInteger spatialDimension)
  : CovarianceModelImplementation(spatialDimension)
  , variance_(1, 1.0)
  , covariance_(0)
  , basis_()
  , functions_(0)
{
  // Use a constant basis for the default constructor
  setBasis(ConstantBasisFactory(1).build());
}

/* Parameters constructor */
RankMCovarianceModel::RankMCovarianceModel(const NumericalPoint & variance,
    const Basis & basis)
  : CovarianceModelImplementation()
  , variance_(variance)
  , covariance_(0)
  , basis_()
  , functions_(0)
{
  if (variance.getDimension() == 0) throw InvalidArgumentException(HERE) << "Error: expected a vector of marginal variances of positive dimension, got dimension=0.";
  setBasis(basis);
}

/* Parameters constructor */
RankMCovarianceModel::RankMCovarianceModel(const CovarianceMatrix & covariance,
    const Basis & basis)
  : CovarianceModelImplementation()
  , variance_(0)
  , covariance_(0)
  , basis_()
  , functions_(0)
{
  if (covariance.getDimension() == 0) throw InvalidArgumentException(HERE) << "Error: expected a covariance of positive dimension, got dimension=0.";
  // Check if the covariance is diagonal
  if (covariance.isDiagonal())
  {
    variance_ = NumericalPoint(covariance.getDimension());
    for (UnsignedInteger i = 0; i < variance_.getDimension(); ++i)
      variance_[i] = covariance(i, i);
  }
  else covariance_ = covariance;
  setBasis(basis);
}

/* Virtual constructor */
RankMCovarianceModel * RankMCovarianceModel::clone() const
{
  return new RankMCovarianceModel(*this);
}

/* Computation of the covariance function
 * Suppose that X(\omega,t)=\sum_{i=1}^M\xi_i(\omega)\phi_i(t)
 * with E[\xi_i]=0. Then:
 * C(s,t)=E[X(\omega,s)X(\omega,t)^t]
 *       =\sum_{i=1}^M\sum_{j=1}^M\phi_i(s)\phi_j(t)^tE[\xi_i\xi_j]
 * and when the components of (\xi_1,\dots,\xi_M) are decorrelated with unit variance, it reduces to:
 * C(s,t)=\sum_{i=1}^M\alpha_i^2\phi_i(s)\phi_i(t)^t
 */
CovarianceMatrix RankMCovarianceModel::operator() (const NumericalPoint & s,
    const NumericalPoint & t) const
{
  if (s.getDimension() != spatialDimension_) throw InvalidArgumentException(HERE) << "Error: the point s has dimension=" << s.getDimension() << ", expected dimension=" << spatialDimension_;
  if (t.getDimension() != spatialDimension_) throw InvalidArgumentException(HERE) << "Error: the point t has dimension=" << t.getDimension() << ", expected dimension=" << spatialDimension_;

  MatrixImplementation result(dimension_, dimension_);
  const UnsignedInteger size = functions_.getSize();
  // If the variance are uncorrelated
  if (covariance_.getDimension() == 0)
    for (UnsignedInteger i = 0; i < size; ++i)
    {
      const MatrixImplementation phiS(dimension_, 1, functions_[i](s));
      const MatrixImplementation phiT(1, dimension_, functions_[i](t) * variance_[i]);
      result += phiS.genProd(phiT);
    }
  else
  {
    for (UnsignedInteger i = 0; i < size; ++i)
    {
      const MatrixImplementation phiS(dimension_, 1, functions_[i](s));
      for (UnsignedInteger j = 0; j < size; ++j)
      {
        const MatrixImplementation phiT(1, dimension_, functions_[j](t));
        result += phiS.genProd(phiT) * covariance_(i, j);
      } // j
    } // i
  } // covariance dimension > 0
  return result;
}

/* Gradient */
Matrix RankMCovarianceModel::partialGradient(const NumericalPoint & s,
    const NumericalPoint & t) const
{
  if (s.getDimension() != spatialDimension_) throw InvalidArgumentException(HERE) << "Error: the point s has dimension=" << s.getDimension() << ", expected dimension=" << spatialDimension_;
  if (t.getDimension() != spatialDimension_) throw InvalidArgumentException(HERE) << "Error: the point t has dimension=" << t.getDimension() << ", expected dimension=" << spatialDimension_;

  throw NotYetImplementedException(HERE);
}

CovarianceMatrix RankMCovarianceModel::discretize(const NumericalSample & vertices) const
{
  if (vertices.getDimension() != spatialDimension_) throw InvalidArgumentException(HERE) << "Error: the given sample has a dimension=" << vertices.getDimension() << " different from the input dimension=" << spatialDimension_;
  const UnsignedInteger size = vertices.getSize();
  const UnsignedInteger fullSize = size * dimension_;
  MatrixImplementation covarianceMatrix(fullSize, fullSize);
  // Precompute the discretizations of the functions over the vertices
  const UnsignedInteger basisSize = functions_.getSize();
  Collection<MatrixImplementation> basisDiscretization(basisSize);
  // If the covariance is diaonal
  if (covariance_.getDimension() == 0)
    {
      for (UnsignedInteger i = 0; i < basisSize; ++i)
	{
	  const NumericalSample data(functions_[i](vertices));
	  basisDiscretization[i] = MatrixImplementation(fullSize, 1, data.getImplementation()->getData()) * std::sqrt(variance_[i]);
	}
      for (UnsignedInteger i = 0; i < basisSize; ++i)
	  covarianceMatrix += basisDiscretization[i].genProd(basisDiscretization[i], false, true);
      return covarianceMatrix;
    }
  const TriangularMatrix cholesky(const_cast<CovarianceMatrix&>(covariance_).computeCholesky());
  for (UnsignedInteger i = 0; i < basisSize; ++i)
    basisDiscretization[i] = MatrixImplementation(fullSize, 1, (functions_[i](vertices) * cholesky).getImplementation()->getData());
  for (UnsignedInteger i = 0; i < basisSize; ++i)
    for (UnsignedInteger j = 0; j < basisSize; ++j)
      covarianceMatrix += basisDiscretization[i].genProd(basisDiscretization[j], false, true);
  return covarianceMatrix;
}

/* Is it a stationary model ? */
Bool RankMCovarianceModel::isStationary() const
{
  return false;
}

/* Covariance accessor */
CovarianceMatrix RankMCovarianceModel::getCovariance() const
{
  return covariance_;
}

NumericalPoint RankMCovarianceModel::getVariance() const
{
  return variance_;
}

/* Basis accessor */
Basis RankMCovarianceModel::getBasis() const
{
  return basis_;
}

Basis::NumericalMathFunctionCollection RankMCovarianceModel::getFunctions() const
{
  return functions_;
}

/* Basis accessor */
void RankMCovarianceModel::setBasis(const Basis & basis)
{
  const UnsignedInteger size = std::max(variance_.getSize(), covariance_.getDimension());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: there must be at least one coefficient";
  functions_ = Basis::NumericalMathFunctionCollection(size);
  for (UnsignedInteger i = 0; i < size; ++i)
    functions_[i] = basis.build(i);
  scale_ = NumericalPoint(functions_[0].getInputDimension(), 1.0);
  amplitude_ = NumericalPoint(functions_[0].getOutputDimension(), 1.0);
  basis_ = basis;
}

/* String converter */
String RankMCovarianceModel::__repr__() const
{
  OSS oss;
  oss << "class=" << RankMCovarianceModel::GetClassName()
      << ", variance=" << variance_
      << ", covariance=" << covariance_
      << ", basis=" << basis_
      << ", functions=" << functions_;
  return oss;
}

/* String converter */
String RankMCovarianceModel::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset << "class=" << RankMCovarianceModel::GetClassName();
  if (covariance_.getDimension() > 0) oss << ", covariance=" << covariance_;
  else oss << ", variance=" << variance_;
  oss << ", basis=" << basis_
      << ", functions=" << functions_;
  return oss;
}

/* Method save() stores the object through the StorageManager */
void RankMCovarianceModel::save(Advocate & adv) const
{
  CovarianceModelImplementation::save(adv);
  adv.saveAttribute("variance_", variance_);
  adv.saveAttribute("covariance_", covariance_);
  adv.saveAttribute("basis_", basis_);
  adv.saveAttribute("functions_", functions_);
}

/* Method load() reloads the object from the StorageManager */
void RankMCovarianceModel::load(Advocate & adv)
{
  CovarianceModelImplementation::load(adv);
  adv.loadAttribute("variance_", variance_);
  adv.loadAttribute("covariance_", covariance_);
  adv.loadAttribute("basis_", basis_);
  adv.loadAttribute("functions_", functions_);
}

END_NAMESPACE_OPENTURNS
