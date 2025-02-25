/*
 * MRCPP, a numerical library based on multiresolution analysis and
 * the multiwavelet basis which provide low-scaling algorithms as well as
 * rigorous error control in numerical computations.
 * Copyright (C) 2021 Stig Rune Jensen, Jonas Juselius, Luca Frediani and contributors.
 *
 * This file is part of MRCPP.
 *
 * MRCPP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MRCPP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with MRCPP.  If not, see <https://www.gnu.org/licenses/>.
 *
 * For information on the complete list of contributors to MRCPP, see:
 * <https://mrcpp.readthedocs.io/>
 */

#include "HelmholtzOperator.h"
#include "HelmholtzKernel.h"
#include "utils/Printer.h"

namespace mrcpp {

/** @returns New HelmholtzOperator object
 *  @param[in] mra: Which MRA the operator is defined
 *  @param[in] m: Exponential parameter of the operator
 *  @param[in] pr: Build precision, closeness to exp(-mu*r)/r
 *  @details This will construct a gaussian expansion to approximate
 *  exp(-mu*r)/r, and project each term into a one-dimensional MW operator.
 *  Subsequent application of this operator will apply each of the terms to
 *  the input function in all Cartesian directions.
 */
HelmholtzOperator::HelmholtzOperator(const MultiResolutionAnalysis<3> &mra, double m, double prec)
        : ConvolutionOperator<3>(mra, prec, prec / 10.0)
        , mu(m) {
    int oldlevel = Printer::setPrintLevel(0);
    double r_min = calcMinDistance(mra, this->kern_prec);
    double r_max = calcMaxDistance(mra);
    HelmholtzKernel helmholtz_kernel(this->mu, this->kern_prec, r_min, r_max);
    // Rescale for application in 3D
    helmholtz_kernel.rescale(3);
    initializeOperator(helmholtz_kernel);
    Printer::setPrintLevel(oldlevel);
}

HelmholtzOperator::HelmholtzOperator(const MultiResolutionAnalysis<3> &mra, double m, double prec, int root, int reach)
        : ConvolutionOperator<3>(mra, prec, prec / 100.0, root, reach)
        , mu(m) {
    int oldlevel = Printer::setPrintLevel(0);
    double r_min = calcMinDistance(mra, this->kern_prec);
    double r_max = calcMaxDistance(mra);
    HelmholtzKernel helmholtz_kernel(this->mu, this->kern_prec, r_min, r_max);
    // Rescale for application in 3D
    helmholtz_kernel.rescale(3);
    initializeOperator(helmholtz_kernel);
    Printer::setPrintLevel(oldlevel);
}

} // namespace mrcpp
