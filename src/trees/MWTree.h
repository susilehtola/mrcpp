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

#pragma once

#include <Eigen/Core>

#include "MRCPP/mrcpp_declarations.h"
#include "utils/omp_utils.h"

#include "MultiResolutionAnalysis.h"
#include "NodeAllocator.h"
#include "NodeBox.h"

namespace mrcpp {

template <int D> class MWTree {
public:
    MWTree(const MultiResolutionAnalysis<D> &mra, const std::string &n);
    MWTree(const MWTree<D> &tree) = delete;
    MWTree<D> &operator=(const MWTree<D> &tree) = delete;
    virtual ~MWTree();

    void setZero();
    void clear();

    /** @returns Squared L2 norm of the function */
    double getSquareNorm() const { return this->squareNorm; }
    void calcSquareNorm();
    void clearSquareNorm() { this->squareNorm = -1.0; }

    int getOrder() const { return this->order; }
    int getKp1() const { return this->order + 1; }
    int getKp1_d() const { return this->kp1_d; }
    int getDim() const { return D; }
    int getTDim() const { return (1 << D); }
    int getNNodes() const { return getNodeAllocator().getNNodes(); }
    int getNNegScales() const { return this->nodesAtNegativeDepth.size(); }
    int getRootScale() const { return this->rootBox.getScale(); }
    int getDepth() const { return this->nodesAtDepth.size(); }
    int getNNodesAtDepth(int i) const;
    int getSizeNodes() const;

    NodeBox<D> &getRootBox() { return this->rootBox; }
    const NodeBox<D> &getRootBox() const { return this->rootBox; }
    const MultiResolutionAnalysis<D> &getMRA() const { return this->MRA; }

    void mwTransform(int type, bool overwrite = true);

    void setName(const std::string &n) { this->name = n; }
    const std::string &getName() const { return this->name; }

    int getRootIndex(Coord<D> r) const { return this->rootBox.getBoxIndex(r); }
    int getRootIndex(NodeIndex<D> nIdx) const { return this->rootBox.getBoxIndex(nIdx); }

    MWNode<D> *findNode(NodeIndex<D> nIdx);
    const MWNode<D> *findNode(NodeIndex<D> nIdx) const;

    MWNode<D> &getNode(NodeIndex<D> nIdx);
    MWNode<D> &getNodeOrEndNode(NodeIndex<D> nIdx);
    const MWNode<D> &getNodeOrEndNode(NodeIndex<D> nIdx) const;

    MWNode<D> &getNode(Coord<D> r, int depth = -1);
    MWNode<D> &getNodeOrEndNode(Coord<D> r, int depth = -1);
    const MWNode<D> &getNodeOrEndNode(Coord<D> r, int depth = -1) const;

    int getNEndNodes() const { return this->endNodeTable.size(); }
    int getNRootNodes() const { return this->rootBox.size(); }
    MWNode<D> &getEndMWNode(int i) { return *this->endNodeTable[i]; }
    MWNode<D> &getRootMWNode(int i) { return this->rootBox.getNode(i); }
    const MWNode<D> &getEndMWNode(int i) const { return *this->endNodeTable[i]; }
    const MWNode<D> &getRootMWNode(int i) const { return this->rootBox.getNode(i); }

    bool isPeriodic() const { return this->MRA.getWorldBox().isPeriodic(); }

    MWNodeVector<D> *copyEndNodeTable();
    MWNodeVector<D> *getEndNodeTable() { return &this->endNodeTable; }

    void deleteRootNodes();
    void resetEndNodeTable();
    void clearEndNodeTable() { this->endNodeTable.clear(); }

    int countBranchNodes(int depth = -1);
    int countLeafNodes(int depth = -1);
    int countAllocNodes(int depth = -1);
    int countNodes(int depth = -1);

    void makeMaxSquareNorms(); // sets values for maxSquareNorm and maxWSquareNorm in all nodes

    NodeAllocator<D> &getNodeAllocator() { return *this->nodeAllocator_p; }
    const NodeAllocator<D> &getNodeAllocator() const { return *this->nodeAllocator_p; }

    friend std::ostream &operator<<(std::ostream &o, const MWTree<D> &tree) { return tree.print(o); }

    friend class MWNode<D>;
    friend class FunctionNode<D>;
    friend class OperatorNode;
    friend class TreeBuilder<D>;
    friend class NodeAllocator<D>;

protected:
    // Parameters that are set in construction and should never change
    const MultiResolutionAnalysis<D> MRA;

    // Constant parameters that are derived internally
    const int order;
    const int kp1_d;

    // Parameters that are dynamic and can be set by user
    std::string name;

    std::unique_ptr<NodeAllocator<D>> nodeAllocator_p{nullptr};

    // Tree data
    double squareNorm;
    NodeBox<D> rootBox;                    ///< The actual container of nodes
    MWNodeVector<D> endNodeTable;          ///< Final projected nodes
    std::vector<int> nodesAtDepth;         ///< Node counter
    std::vector<int> nodesAtNegativeDepth; ///< Node counter

    virtual void mwTransformDown(bool overwrite);
    virtual void mwTransformUp();

    void incrementNodeCount(int scale);
    void decrementNodeCount(int scale);

    virtual std::ostream &print(std::ostream &o) const;
};

} // namespace mrcpp
