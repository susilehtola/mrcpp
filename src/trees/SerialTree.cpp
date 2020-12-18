/*
 * MRCPP, a numerical library based on multiresolution analysis and
 * the multiwavelet basis which provide low-scaling algorithms as well as
 * rigorous error control in numerical computations.
 * Copyright (C) 2020 Stig Rune Jensen, Jonas Juselius, Luca Frediani and contributors.
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

#include "SerialTree.h"
#include "MWTree.h"
#include "utils/Printer.h"
#include "utils/math_utils.h"

using namespace Eigen;

namespace mrcpp {

template <int D>
SerialTree<D>::SerialTree(MWTree<D> *tree, SharedMemory *mem)
        : nNodes(0)
        , maxNodesPerChunk(0)
        , sizeNodeCoeff(0)
        , coeffStack(nullptr)
        , maxNodes(0)
        , tree_p(tree)
        ,
#ifdef MRCPP_HAS_MPI
        shMem(mem) {
#else
        shMem(nullptr) {
#endif
}

template class SerialTree<1>;
template class SerialTree<2>;
template class SerialTree<3>;

} // namespace mrcpp
