/*
 *    This file is part of CasADi.
 *
 *    CasADi -- A symbolic framework for dynamic optimization.
 *    Copyright (C) 2010 by Joel Andersson, Moritz Diehl, K.U.Leuven. All rights reserved.
 *
 *    CasADi is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    CasADi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with CasADi; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef CRS_SPARSITY_HPP
#define CRS_SPARSITY_HPP

#include "../shared_object.hpp"
#include <vector>
#include <list>

namespace CasADi{

// Forward declaration
class CRSSparsityNode;
  
/** \brief General sparsity class
 * 
 * The storage format is a (modified) compressed row storage (CRS) format. This way, a vector element can always be accessed in constant time.\n
 * 
  The following indices exist. All start with 0.\n
  (i) row index \n
  (j) column index \n
  (k) structural non-zero elements index \n
 *
 *
 *  col(k)=j \n
 *  rowind(i)<=k<=rowind(i+1) \n
 *  getRow()[k]=i \n
 * 
 * The actual col and rowind vectors are stored inside CRSSparsityNode \n
 * 
 * \see Matrix
 *
 * \author Joel Andersson 
 * \date 2010	
*/
class CRSSparsity : public SharedObject{
  public:
  
    /// Default constructor, null pointer
    CRSSparsity();
    
    /// Construct a sparsity pattern (sparse/dense)
    CRSSparsity(int nrow, int ncol, bool dense=false);

    /// Construct a sparsity pattern from vectors
    CRSSparsity(int nrow, int ncol, std::vector<int> col, std::vector<int> rowind);
    
    /// Access a member function or object
    CRSSparsityNode* operator->();

    /// Const access a member function or object
    const CRSSparsityNode* operator->() const;
  
    /// Check if the node is pointing to the right type of object
    virtual bool checkNode() const;

    /// \name Size and element counting
    /// @{
    /// Get the number of rows
    int size1() const;
    
    /// Get the number of columns
    int size2() const;

    /** \brief Get the number of elements, including structural zeros.
     * \see size()
    */
    int numel() const;
    
    /** \brief Get the number of (structural) non-zeros
     * \see numel()
     */
    int size() const;

    /** \brief Number of non-zeros in the upper triangular half
    *
    * Counts (i,j) if j>=i
    * 
    */
    int sizeU() const;

    /** \brief Number of non-zeros in the lower triangular half
     * Counts (i,j) if j<=i
    */
    int sizeL() const;
    /// @}

    /// @{
    /** \brief Get a reference to the columns of all non-zero element (copy if not unique!)
     * The length of the returned vector is exactly size()
     *  \post col()[k]=j
     */
    std::vector<int>& col();
    /// Get a const reference to the columns of all non-zero element
    const std::vector<int>& col() const;
    /** \brief Get the column of a non-zero element
     *  \post col(k)=j
     */
    int col(int k) const;
    /// @}
    
    /// @{
    /** \brief Get a reference to the rowindex of all row element (copy if not unique!)
     * The length of the returned vector is exactly size2()+1
     *  \post rowind()[i]<=k<=rowind()[i+1]
    */
    std::vector<int>& rowind();
    /// Get a const reference to the rowindex of all row element
    const std::vector<int>& rowind() const;
    /** \brief Get the index of the first non-zero element in a row
     * \post rowind(i)<=k<=rowind(i+)
     */
    int rowind(int row) const;
    /// @}

    /** \brief Get the row for each non-zero entry
     * \post getRow()[k]=i
     * 
     * 
     *  This method relies on rowind().
     *  \see rowind()
     */
    std::vector<int> getRow() const;
    
    /// Resize
    void resize(int nrow, int ncol);
    
    /// @{
    /** \brief Get the index of a non-zero element (copy object if necessary)
     * \post getNZ(i,j)=k
     */
    int getNZ(int i, int j);
    
    /// Get the index of a non-zero element (return -1 if not exists)
    int getNZ(int i, int j) const;
    /// @}

    /// Get the sparsity in CRS format
    void getSparsityCRS(std::vector<int>& rowind, std::vector<int> &col) const;

    /// Get the sparsity in sparse triplet format
    void getSparsity(std::vector<int>& row, std::vector<int> &col) const;
    
    /// Bucket sort the elements by column
    void bucketSort(std::vector<std::list<int> >& buckets, std::vector<int>& row) const;

    /// Transpose the matrix and get the reordering of the non-zero entries, i.e. the non-zeros of the original matrix for each non-zero of the new matrix
    CRSSparsity transpose(std::vector<int>& mapping) const;
    
    /// Reserve space
    void reserve(int nnz, int nrow);

};

#ifdef SWIG
%extend CRSSparsity {
std::string __repr__() { return $self->getRepresentation(); }
}
#endif // SWIG

#ifndef SWIG
class CRSSparsityNode : public SharedObjectNode{
  public:
    /// Construct a sparsity pattern from vectors
    CRSSparsityNode(int nrow, int ncol, std::vector<int> col, std::vector<int> rowind) : nrow_(nrow), ncol_(ncol), col_(col), rowind_(rowind){}

    /// Clone
    virtual CRSSparsityNode* clone() const{ return new CRSSparsityNode(*this); }

    /// Print representation
    virtual void repr(std::ostream &stream) const;

    /// Print description
    virtual void print(std::ostream &stream) const;

    /// Number of rows
    int nrow_;
    
    /// Number of columns
    int ncol_;
    
    /// vector of length nnz containing the columns for all the indices of the non-zero elements
    std::vector<int> col_;
    
    /// vector of length n+1 containing the index of the last non-zero element up till each row 
    std::vector<int> rowind_;
};
#endif // SWIG

} // namespace CasADi

#endif // CRS_SPARSITY_HPP
