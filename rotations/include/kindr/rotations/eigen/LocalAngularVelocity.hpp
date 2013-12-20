/*
 * Copyright (c) 2013, Christian Gehring, Hannes Sommer, Paul Furgale, Remo Diethelm
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autonomous Systems Lab, ETH Zurich nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Christian Gehring, Hannes Sommer, Paul Furgale,
 * Remo Diethelm BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#ifndef KINDR_ROTATIONS_EIGEN_LOCALANGULARVELOCITY_HPP_
#define KINDR_ROTATIONS_EIGEN_LOCALANGULARVELOCITY_HPP_

#include <Eigen/Core>

#include "kindr/common/common.hpp"
#include "kindr/common/assert_macros.hpp"
#include "kindr/common/assert_macros_eigen.hpp"
#include "kindr/rotations/RotationDiffBase.hpp"
#include "kindr/rotations/RotationEigen.hpp"
#include "kindr/quaternions/QuaternionEigen.hpp"
#include "kindr/linear_algebra/LinearAlgebra.hpp"



namespace kindr {
namespace rotations {
namespace eigen_impl {



/*! \class LocalAngularVelocity
 * \brief Angular velocity in 3D-space expressed in local coordinates (frame fixed to the body).
 *
 * This class implements an angular velocity of a rigid body in 3D-space expressed in body fixed (local) frame.
 *
 * Note that only the version with the active usage type makes sense to represent a physical angular velocity of a body.
 * The angular velocity should represent the absolute rotational velocity of a rigid body with respect to an inertial (global) frame I
 * and its coordinates are expressed in the body fixed (local) frame (\f$\B_Omega = B_\omega_{IB}$\f)
 *
 * \tparam PrimType_  Primitive type of the coordinates.
 * \ingroup rotations
 */
template<typename PrimType_, enum RotationUsage Usage_>
class LocalAngularVelocity : public AngularVelocityBase<LocalAngularVelocity<PrimType_, Usage_>, Usage_>, private Eigen::Matrix<PrimType_, 3, 1> {
 private:
  /*! \brief The base type.
   */
  typedef Eigen::Matrix<PrimType_, 3, 1> Base;
 public:
  /*! \brief The implementation type.
   *
   *  The implementation type is always an Eigen object.
   */
  typedef Base Implementation;

  /*! \brief The primitive type of the velocities.
   */
  typedef PrimType_ Scalar;

  /*! \brief Default constructor initializes all velocities with zero.
   */
  LocalAngularVelocity()
    : Base(Base::Zero()) {
  }

  /*! Constructor with three components (x,y,z)
   * \param x   x-coordinate expressed in body fixed (local) frame
   * \param y   y-coordinate expressed in body fixed (local) frame
   * \param z   z-coordinate expressed in body fixed (local) frame
   */
  LocalAngularVelocity(Scalar x, Scalar y, Scalar z)
    : Base(x, y, z) {
  }


  /*! \brief Constructor using Eigen::Matrix<Scalar,3,1>.
   *  \param other   Eigen::Matrix<Scalar,3,1>
   */
  explicit LocalAngularVelocity(const Base& other)
    : Base(other) {
   }


  /*! Constructor with a time derivative of a rotation with a different parameterization
   *
   * \param rotation    rotation the time derivative is taken at
   * \param other       other time derivative
   */
  template<typename RotationDerived_, enum RotationUsage RotationUsage_, typename OtherDerived_, enum RotationUsage OtherUsage_>
  inline explicit LocalAngularVelocity(const RotationBase<RotationDerived_, RotationUsage_>& rotation, const RotationDiffBase<OtherDerived_, OtherUsage_>& other)
    : Base(internal::RotationDiffConversionTraits<LocalAngularVelocity, OtherDerived_, RotationDerived_>::convert(rotation.derived(), other.derived())){
  }

  /*! \brief Cast to the implementation type.
   *  \returns the implementation (recommended only for advanced users)
   */
  inline Implementation& toImplementation() {
    return static_cast<Implementation&>(*this);
  }

  /*! \brief Cast to the implementation type.
   *  \returns the implementation (recommended only for advanced users)
   */
  inline const Implementation& toImplementation() const {
    return static_cast<const Implementation&>(*this);
  }

  /*!\brief Get x-coordinate of the angular velocity expressed in body fixed (local) frame
   * \returns the x-coordinate of the angular velocity
   */
  using Base::x;

  /*!\brief Get y-coordinate of the angular velocity expressed in body fixed (local) frame
   * \returns the y-coordinate of the angular velocity
   */
  using Base::y;

  /*!\brief Get z-coordinate of the angular velocity expressed in body fixed (local) frame
   * \returns the z-coordinate of the angular velocity
   */
  using Base::z;

  /*! \brief Addition of two angular velocities.
   */
  using AngularVelocityBase<LocalAngularVelocity<PrimType_, Usage_>,Usage_>::operator+; // otherwise ambiguous PositionBase and Eigen

  /*! \brief Subtraction of two angular velocities.
   */
  using AngularVelocityBase<LocalAngularVelocity<PrimType_, Usage_>, Usage_>::operator-; // otherwise ambiguous PositionBase and Eigen

  /*! \brief Addition of two angular velocities.
   * \param other   other angular velocity
   */
  template<typename Other_>
  LocalAngularVelocity& operator +=(const Other_& other) {
    this->toImplementation() += other.toImplementation();
    return *this;
  }

  /*! \brief Subtraction of two angular velocities.
   * \param other   other angular velocity
   */
  template<typename Other_>
  LocalAngularVelocity& operator -=(const Other_& other) {
    this->toImplementation() -= other.toImplementation();
    return *this;
  }

  /*! \brief Sets all components of the angular velocity to zero.
   * \returns reference
   */
  LocalAngularVelocity& setZero() {
    Base::setZero();
    return *this;
  }

  /*! \brief Used for printing the object with std::cout.
   *  \returns std::stream object
   */
  friend std::ostream& operator << (std::ostream& out, const LocalAngularVelocity& velocity) {
    out << velocity.toImplementation().transpose();
    return out;
  }
};

//! \brief 3D angular velocity with primitive type double
typedef LocalAngularVelocity<double, RotationUsage::PASSIVE>  LocalAngularVelocityPD;
//! \brief 3D angular velocity with primitive type float
typedef LocalAngularVelocity<float, RotationUsage::PASSIVE>  LocalAngularVelocityPF;
//! \brief 3D angular velocity with primitive type double
typedef LocalAngularVelocity<double, RotationUsage::ACTIVE>  LocalAngularVelocityAD;
//! \brief 3D angular velocity with primitive type float
typedef LocalAngularVelocity<float, RotationUsage::ACTIVE>  LocalAngularVelocityAF;

} // namespace eigen_impl

namespace internal {


template<typename PrimType_>
class RotationDiffConversionTraits<eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>, eigen_impl::RotationQuaternionDiff<PrimType_, RotationUsage::ACTIVE>, eigen_impl::RotationQuaternion<PrimType_, RotationUsage::ACTIVE>> {
 public:
  inline static eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE> convert(const eigen_impl::RotationQuaternion<PrimType_, RotationUsage::ACTIVE>& rquat, const eigen_impl::RotationQuaternionDiff<PrimType_, RotationUsage::ACTIVE>& rquatdiff) {
    Eigen::Matrix<PrimType_,3,4> H_bar;
    H_bar << -rquat.toUnitQuaternion().x(),  rquat.toUnitQuaternion().w(),  rquat.toUnitQuaternion().z(), -rquat.toUnitQuaternion().y(),
             -rquat.toUnitQuaternion().y(), -rquat.toUnitQuaternion().z(),  rquat.toUnitQuaternion().w(),  rquat.toUnitQuaternion().x(),
             -rquat.toUnitQuaternion().z(),  rquat.toUnitQuaternion().y(), -rquat.toUnitQuaternion().x(),  rquat.toUnitQuaternion().w();
    return eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>(2.0*H_bar*rquatdiff.toQuaternion().vector());
  }
};

//! B_\hat{w}_IB = (R_IB*dR_IB')
template<typename PrimType_>
class RotationDiffConversionTraits<eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>, eigen_impl::RotationMatrixDiff<PrimType_, RotationUsage::ACTIVE>, eigen_impl::RotationMatrix<PrimType_, RotationUsage::ACTIVE>> {
 public:
  inline static eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE> convert(const eigen_impl::RotationMatrix<PrimType_, RotationUsage::ACTIVE>& rotationMatrix, const eigen_impl::RotationMatrixDiff<PrimType_, RotationUsage::ACTIVE>& rotationMatrixDiff) {
    return eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>(linear_algebra::getVectorFromSkewMatrix<PrimType_>(rotationMatrix.toImplementation()*rotationMatrixDiff.toImplementation().transpose()));
  }
};


//! B_\hat{w}_IB = (C_IB'*dC_IB)
template<typename PrimType_>
class RotationDiffConversionTraits<eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>, eigen_impl::RotationMatrixDiff<PrimType_, RotationUsage::PASSIVE>, eigen_impl::RotationMatrix<PrimType_, RotationUsage::PASSIVE>> {
 public:
  inline static eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE> convert(const eigen_impl::RotationMatrix<PrimType_, RotationUsage::PASSIVE>& rotationMatrix, const eigen_impl::RotationMatrixDiff<PrimType_, RotationUsage::PASSIVE>& rotationMatrixDiff) {
    return eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>(linear_algebra::getVectorFromSkewMatrix<PrimType_>(rotationMatrix.inverted().toImplementation()*rotationMatrixDiff.toImplementation()));
  }
};

//! B_\hat{w}_IB = B_n \dot{\theta} + \dot{B_n}*sin(\theta) + B_\hat{n}(1-cos(\theta))
template<typename PrimType_>
class RotationDiffConversionTraits<eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>, eigen_impl::AngleAxisDiff<PrimType_, RotationUsage::ACTIVE>, eigen_impl::AngleAxis<PrimType_, RotationUsage::ACTIVE>> {
 public:
  inline static eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE> convert(const eigen_impl::AngleAxis<PrimType_, RotationUsage::ACTIVE>& angleAxis, const eigen_impl::AngleAxisDiff<PrimType_, RotationUsage::ACTIVE>& angleAxisDiff) {
    return eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>(angleAxis.axis()*angleAxisDiff.angle() + angleAxisDiff.axis()*sin(angleAxis.angle()) + linear_algebra::getSkewMatrixFromVector(angleAxis.axis())*angleAxisDiff.axis()*(1-cos(angleAxis.angle())));
  }
};

template<typename PrimType_>
class RotationDiffConversionTraits<eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>, eigen_impl::RotationVectorDiff<PrimType_, RotationUsage::ACTIVE>, eigen_impl::RotationVector<PrimType_, RotationUsage::ACTIVE>> {
 public:
  inline static eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE> convert(const eigen_impl::RotationVector<PrimType_, RotationUsage::ACTIVE>& rotationVector, const eigen_impl::RotationVectorDiff<PrimType_, RotationUsage::ACTIVE>& rotationVectorDiff) {
    typedef typename eigen_impl::RotationVector<PrimType_, RotationUsage::ACTIVE>::Implementation Vector;
    typedef PrimType_ Scalar;
    typedef typename Eigen::Matrix<PrimType_, 3, 3> Matrix3x3;


    // not tested:
//    const Vector rv = rotationVector.toImplementation();
//    const Vector rvDiff = rotationVectorDiff.toImplementation();
//    const Matrix3x3 rv_hat = linear_algebra::getSkewMatrixFromVector(rv);
//    const Scalar angle = rv.norm();
//    const Vector angularVelocity = rvDiff-rv_hat*rvDiff*(1-cos(angle)/(angle*angle)) + rv_hat*rv_hat*rvDiff*((angle-sin(angle))/(angle*angle*angle));
//    return eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>(angularVelocity);

    const PrimType_ v = rotationVector.vector().norm();
    const PrimType_ v1 = rotationVector.x();
    const PrimType_ v2 = rotationVector.y();
    const PrimType_ v3 = rotationVector.z();
    const PrimType_ dv1 = rotationVectorDiff.x();
    const PrimType_ dv2 = rotationVectorDiff.y();
    const PrimType_ dv3 = rotationVectorDiff.z();

    const PrimType_ t2 = 1.0/(v*v*v);
    const PrimType_ t3 = cos(v);
    const PrimType_ t4 = sin(v);
    const PrimType_ t5 = v1*v1;
    const PrimType_ t6 = v1*v2;
    const PrimType_ t7 = v*v;
    const PrimType_ t8 = t4*t7;
    const PrimType_ t9 = v2*v2;
    const PrimType_ t10 = v2*v3;
    const PrimType_ t11 = v1*v3;
    const PrimType_ t12 = t3*v2;
    const PrimType_ t13 = v3*v3;
    const PrimType_ w1 = dv3*t2*(v*(t11+t12-v2)-t4*v1*v3)+dv1*t2*(t8-t4*t5+t5*v)+dv2*t2*(v*(t6+v3-t3*v3)-t4*v1*v2);
    const PrimType_ w2 = dv1*t2*(v*(t6-v3+t3*v3)-t4*v1*v2)+dv2*t2*(t8-t4*t9+t9*v)+dv3*t2*(v*(t10+v1-t3*v1)-t4*v2*v3);
    const PrimType_ w3 = dv2*t2*(v*(t10-v1+t3*v1)-t4*v2*v3)+dv1*t2*(v*(t11-t12+v2)-t4*v1*v3)+dv3*t2*(t8-t4*t13+t13*v);

    return eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>(w1, w2, w3);

  }
};


template<typename PrimType_>
class RotationDiffConversionTraits<eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>, eigen_impl::EulerAnglesZyxDiff<PrimType_, RotationUsage::ACTIVE>, eigen_impl::EulerAnglesZyx<PrimType_, RotationUsage::ACTIVE>> {
 public:
  inline static eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE> convert(const eigen_impl::EulerAnglesZyx<PrimType_, RotationUsage::ACTIVE>& eulerAngles, const eigen_impl::EulerAnglesZyxDiff<PrimType_, RotationUsage::ACTIVE>& eulerAnglesDiff) {
    const PrimType_ phi = eulerAngles.roll();
    const PrimType_ theta = eulerAngles.pitch();
    const PrimType_ dphi = eulerAnglesDiff.roll();
    const PrimType_ dtheta = eulerAnglesDiff.pitch();
    const PrimType_ dpsi = eulerAnglesDiff.yaw();
    const PrimType_ t2 = sin(phi);
    const PrimType_ t3 = cos(phi);
    const PrimType_ t4 = cos(theta);
    return eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>(dphi-dpsi*sin(theta), dtheta*t3+dpsi*t2*t4, -dtheta*t2+dpsi*t3*t4);
  }
};

template<typename PrimType_>
class RotationDiffConversionTraits<eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>, eigen_impl::EulerAnglesXyzDiff<PrimType_, RotationUsage::ACTIVE>, eigen_impl::EulerAnglesXyz<PrimType_, RotationUsage::ACTIVE>> {
 public:
  inline static eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE> convert(const eigen_impl::EulerAnglesXyz<PrimType_, RotationUsage::ACTIVE>& eulerAngles, const eigen_impl::EulerAnglesXyzDiff<PrimType_, RotationUsage::ACTIVE>& eulerAnglesDiff) {
    const PrimType_ beta = eulerAngles.pitch();
    const PrimType_ gamma = eulerAngles.yaw();
    const PrimType_ dalpha = eulerAnglesDiff.roll();
    const PrimType_ dbeta = eulerAnglesDiff.pitch();
    const PrimType_ dgamma = eulerAnglesDiff.yaw();
    const PrimType_ t2 = cos(gamma);
    const PrimType_ t3 = cos(beta);
    const PrimType_ t4 = sin(gamma);
    return eigen_impl::LocalAngularVelocity<PrimType_, RotationUsage::ACTIVE>(dbeta*t4+dalpha*t2*t3, dbeta*t2-dalpha*t3*t4, dgamma+dalpha*sin(beta));
  }
};

} // namespace internal
} // namespace rotations
} // namespace kindr


#endif /* KINDR_ROTATIONS_EIGEN_LOCALANGULARVELOCITY_HPP_ */