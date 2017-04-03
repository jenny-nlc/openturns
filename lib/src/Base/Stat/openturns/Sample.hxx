//                                               -*- C++ -*-
/**
 *  @brief The class Sample implements blank free samples
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
#ifndef OPENTURNS_SAMPLE_HXX
#define OPENTURNS_SAMPLE_HXX

#include <iostream>              // for std::ostream
#include "openturns/TypedInterfaceObject.hxx"
#include "openturns/Point.hxx"
#include "openturns/Description.hxx"
#include "openturns/Pointer.hxx"
#include "openturns/SampleImplementation.hxx"
#include "openturns/ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Sample
 */

class OT_API Sample
  : public TypedInterfaceObject<SampleImplementation>
{
  CLASSNAME;

public:

  /* Some typedefs for easy reading */

public:

  /** Factory of Sample from CSV file */
  static Sample ImportFromCSVFile(const FileName & fileName,
      const String & csvSeparator = ResourceMap::Get( "csv-file-separator" ));

  /** Factory of Sample from Text file */
  static Sample ImportFromTextFile(const FileName & fileName,
      const String & separator = " ");

  /** Export Sample into CSV file */
  void exportToCSVFile(const FileName & fileName,
                       const String & csvSeparator = ResourceMap::Get( "csv-file-separator" )) const;

  /** Export a sample as a matrix, one row by realization, in a format suitable to exchange with R. */
  String streamToRFormat() const;


public:

  /**
   * Default constructor
   * Build a Sample of 1 dimension and with size equal to 0
   */
  Sample();

  /** Constructor with size and dimension */
  Sample(const UnsignedInteger size,
                  const UnsignedInteger dim);

  /** Constructor from a Point (all elements are equal to the Point) */
  Sample(const UnsignedInteger size,
                  const Point & point);

  /** Partial copy constructor */
  Sample(const Sample other,
                  const UnsignedInteger first,
                  const UnsignedInteger last);

#ifndef SWIG
  /** Constructor from a collection of Point */
  Sample(const Collection<Point> & coll);

  /** Constructor from a collection of Indices */
  Sample(const Collection<Indices> & coll);
#endif

  /** Constructor from implementation */
  Sample(const SampleImplementation & implementation);

private:

  /** Constructor from implementation */
  Sample(const Implementation & implementation);

public:

  /** Comparison operator */
  Bool operator ==(const Sample & other) const;
  using TypedInterfaceObject<SampleImplementation>::operator ==;

  /** Comparison operator */
  Bool operator !=(const Sample & other) const;

#ifndef SWIG
  NSI_point operator [] (const UnsignedInteger index);
  NSI_const_point operator [] (const UnsignedInteger index) const;
  NSI_point at (const UnsignedInteger index);
  NSI_const_point at (const UnsignedInteger index) const;
  NumericalScalar & operator () (const UnsignedInteger i,
                                 const UnsignedInteger j);
  const NumericalScalar & operator () (const UnsignedInteger i,
                                       const UnsignedInteger j) const;
  NumericalScalar & at (const UnsignedInteger i,
                        const UnsignedInteger j);
  const NumericalScalar & at (const UnsignedInteger i,
                              const UnsignedInteger j) const;

  void erase(SampleImplementation::iterator first, SampleImplementation::iterator last);
#endif
  // These functions are only intended to be used by SWIG, DO NOT use them for your own purpose !
  // INTENTIONALY NOT DOCUMENTED
  const NumericalScalar * __baseaddress__ () const;
  UnsignedInteger __elementsize__ () const;

  /** Whether the list contains the value val */
  Bool contains(const Point & val) const;

  void erase(const UnsignedInteger first,
             const UnsignedInteger last);
  void erase(const UnsignedInteger index);

  /** erase the whole sample */
  void clear();

  /**
   * String converter
   * This method shows human readable information on the
   * internal state of an Sample. It is used when streaming
   * the Sample or for user information.
   */
  String __repr__() const;

  String __str__(const String & offset = "") const;

  /** Description accessor */
  void setDescription(const Description & description);
  Description getDescription() const;

  /** Dimension accessor */
  UnsignedInteger getDimension() const;

  /** Size accessor */
  UnsignedInteger getSize() const;

  /** Maximum accessor */
  Point getMax() const;

  /** Minimum accessor */
  Point getMin() const;

  /** Method add() appends an element to the collection */
  void add(const Point & point);

  /** Method add() appends another sample to the collection */
  void add(const Sample & sample);

  /** Stack the given sample to the current one */
  void stack(const Sample & sample);

  /**
   * Method split() trunk the sample before the index passed as argument
   * and returns the remainder as new sample. This method tries its best not for doubling
   * memory usage.
   */
  Sample split(const UnsignedInteger index);

  /**
   * Method computeMean() gives the mean of the sample, based on the formula
   * mean = sum of the elements in the sample / size of the sample
   */
  Point computeMean() const;

  /**
   * Method computeCovariance() gives the covariance of the sample
   */
  CovarianceMatrix computeCovariance() const;

  /**
   * Method computeStandardDeviation() gives the standard deviation of the sample
   */
  TriangularMatrix computeStandardDeviation() const;

  /**
   * Method computeVariance() gives the variance of the sample (by component)
   */
  Point computeVariance() const;

  /**
   * Method computeStandardDeviationPerComponent() gives the standard deviation of each component of the sample
   */
  Point computeStandardDeviationPerComponent() const;

  /**
   * Method computePearsonCorrelation() gives the Pearson correlation matrix of the sample
   */
  CorrelationMatrix computeLinearCorrelation() const;
  CorrelationMatrix computePearsonCorrelation() const;

  /**
   * Method computeSpearmanCorrelation() gives the Spearman correlation matrix of the sample
   */
  CorrelationMatrix computeSpearmanCorrelation() const;

  /**
   * Gives the Kendall correlation matrix of the sample
   */
  CorrelationMatrix computeKendallTau() const;

  /**
   * Method computeRange gives the range of the sample (by component)
   */
  Point computeRange() const;

  /**
   * Method computeMedian() gives the median of the sample (by component)
   */
  Point computeMedian() const;

  /**
   * Method computeSkewness() gives the skewness of the sample (by component)
   */
  Point computeSkewness() const;

  /**
   * Method computeKurtosis() gives the kurtosis of the sample (by component)
   */
  Point computeKurtosis() const;

  /**
   * Gives the centered moment of order k of the sample (by component)
   */
  Point computeCenteredMoment(const UnsignedInteger k) const;

  /**
   * Gives the raw moment of order k of the sample (by component)
   */
  Point computeRawMoment(const UnsignedInteger k) const;

  /**
   * Method computeQuantilePerComponent() gives the quantile per component of the sample
   */
  Point computeQuantilePerComponent(const NumericalScalar prob) const;

  /**
   * Method computeQuantile() gives the N-dimension quantile of the sample
   */
  Point computeQuantile(const NumericalScalar prob) const;

  /**
   * Get the empirical CDF of the sample
   */
  NumericalScalar computeEmpiricalCDF(const Point & point,
                                      const Bool tail = false) const;

  /**
   * Get the position of a point in the sample.
   * Returns size if the point does not belong to the sample.
   */
  UnsignedInteger find(const Point & point) const;

  /**
   * Translate realizations in-place
   */
  Sample & operator += (const NumericalScalar translation);
  Sample & operator += (const Point & translation);
  Sample & operator += (const Sample & translation);
  Sample & operator -= (const NumericalScalar translation);
  Sample & operator -= (const Point & translation);
  Sample & operator -= (const Sample & translation);

  /** Translate/scale realizations */
  Sample operator + (const NumericalScalar translation) const;
  Sample operator + (const Point & translation) const;
  Sample operator + (const Sample & translation) const;
  Sample operator - (const NumericalScalar translation) const;
  Sample operator - (const Point & translation) const;
  Sample operator - (const Sample & translation) const;
  Sample operator * (const NumericalScalar scaling) const;
  Sample operator * (const Point & scaling) const;
  Sample operator * (const SquareMatrix & scaling) const;
  Sample operator / (const NumericalScalar scaling) const;
  Sample operator / (const Point & scaling) const;
  Sample operator / (const SquareMatrix & scaling) const;

  /**
   * Scale realizations componentwise in-place
   */
  Sample & operator *= (const NumericalScalar scaling);
  Sample & operator *= (const Point & scaling);
  Sample & operator *= (const SquareMatrix & scaling);
  Sample & operator /= (const NumericalScalar scaling);
  Sample & operator /= (const Point & scaling);
  Sample & operator /= (const SquareMatrix & scaling);

  /** Ranked sample */
  Sample rank() const;

  /** Ranked component */
  Sample rank(const UnsignedInteger index) const;

  /** Sorted sample */
  Sample sort() const;

  /** Sorted component */
  Sample sort(const UnsignedInteger index) const;

  /** Sorted according a component */
  Sample sortAccordingToAComponent(const UnsignedInteger index) const;

  /* Sorted and duplicated points removed */
  Sample sortUnique() const;

  /** Store a sample in a temporary text file, one realization by line. Returns the file name. */
  virtual String storeToTemporaryFile() const;

  /** Get the i-th marginal sample */
  Sample getMarginal(const UnsignedInteger index) const;

  /** Get the marginal sample corresponding to indices dimensions */
  Sample getMarginal(const Indices & indices) const;

  /** Returns a pointer to the underlying implementation object */
  virtual ImplementationAsPersistentObject getImplementationAsPersistentObject() const;

  /** Sets the pointer to the underlying implementation object */
  virtual void setImplementationAsPersistentObject(const ImplementationAsPersistentObject & obj);

}; /* class Sample */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SAMPLE_HXX */
