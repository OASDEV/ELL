////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     SparseBinaryDataVector.h (dataset)
//  Authors:  Ofer Dekel
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DataVector.h"

// utilities
#include "CompressedIntegerList.h"
#include "IntegerList.h"

// linear
#include "IndexValue.h"

// stl
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <initializer_list>
#include <cmath>

namespace emll
{
namespace dataset
{
    /// <summary> Implements a sparse binary vector as an increasing list of the coordinates where the
    /// value is 1.0. </summary>
    ///
    /// <typeparam name="tegerListType"> Type of the teger list type. </typeparam>
    template <typename IntegerListType>
    class SparseBinaryDataVectorBase : public DataVectorBase<SparseBinaryDataVectorBase<IntegerListType>>
    {
    public:
        /// <summary> A read-only forward iterator for the sparse binary vector. </summary>
        class Iterator : public linear::IIndexValueIterator
        {
        public:
            Iterator(const Iterator&) = default;

            Iterator(Iterator&&) = default;

            /// <summary> Returns true if the iterator is currently pointing to a valid iterate. </summary>
            ///
            /// <returns> true if it succeeds, false if it fails. </returns>
            bool IsValid() const { return _list_iterator.IsValid(); }

            /// <summary> Proceeds to the Next iterate. </summary>
            void Next() { _list_iterator.Next(); }

            /// <summary> Returns The current value. </summary>
            ///
            /// <returns> An IndexValue. </returns>
            linear::IndexValue Get() const { return linear::IndexValue{ _list_iterator.Get(), 1.0 }; }

        private:
            /// <summary> define typename to improve readability. </summary>
            using IndexIteratorType = typename IntegerListType::Iterator;

            /// <summary> private ctor, can only be called from SparseBinaryDataVectorBase class. </summary>
            ///
            /// <param name="list_iterator"> The list iterator. </param>
            Iterator(const typename IntegerListType::Iterator& list_iterator);
            friend SparseBinaryDataVectorBase<IntegerListType>;

            // members
            IndexIteratorType _list_iterator;
        };

        SparseBinaryDataVectorBase() = default;

        /// <summary> Converting constructor. </summary>
        ///
        /// <typeparam name="dexValueIteratorType"> Type of the dex value iterator type. </typeparam>
        /// <typeparam name="IIndexValueIterator">  Type of the index value iterator. </typeparam>
        /// <typeparam name="dexValueIteratorType"> Type of the dex value iterator type. </typeparam>
        /// <param name="IndexValueIterator"> The index value iterator. </param>
        template <typename IndexValueIteratorType, typename concept = linear::IsIndexValueIterator<IndexValueIteratorType>>
        SparseBinaryDataVectorBase(IndexValueIteratorType indexValueIterator);

        SparseBinaryDataVectorBase(SparseBinaryDataVectorBase<IntegerListType>&& other) = default;

        SparseBinaryDataVectorBase(const SparseBinaryDataVectorBase<IntegerListType>& other) = default;

        /// <summary> Constructs a data vector from an initializer list of index value pairs. </summary>
        ///
        /// <param name="list"> The initializer list. </param>
        SparseBinaryDataVectorBase(std::initializer_list<linear::IndexValue> list);

        /// <summary> Constructs a data vector from an initializer list of values. </summary>
        ///
        /// <param name="list"> The initializer list of values. </param>
        SparseBinaryDataVectorBase(std::initializer_list<double> list);

        /// <summary> Returns a Iterator that traverses the non-zero entries of the sparse vector. </summary>
        ///
        /// <returns> The iterator. </returns>
        Iterator GetIterator() const { return Iterator(_indices.GetIterator()); }

        /// <summary> Sets the element at the given index to 1.0. Calls to this function must have a
        /// monotonically increasing argument. The value argument must equal 1.0. </summary>
        ///
        /// <param name="index"> Zero-based index of the. </param>
        /// <param name="value"> The value. </param>
        virtual void AppendEntry(size_t index, double value = 1.0) override;

        /// <summary> Returns The largest index of a non-zero entry plus one. </summary>
        ///
        /// <returns> An size_t. </returns>
        virtual size_t Size() const override;

        /// <summary> Computes the vector squared 2-norm. </summary>
        ///
        /// <returns> A double. </returns>
        virtual double Norm2() const override { return std::sqrt(_indices.Size()); }

        /// <summary> Computes the Dot product. </summary>
        ///
        /// <param name="p_other"> The other. </param>
        ///
        /// <returns> A double. </returns>
        virtual double Dot(const double* p_other) const override;

        /// <summary> Performs (*p_other) += scalar * (*this), where other a dense vector. </summary>
        ///
        /// <param name="p_other"> [in,out] If non-null, the other. </param>
        /// <param name="scalar">  The scalar. </param>
        virtual void AddTo(double* p_other, double scalar = 1.0) const override;

    private:
        IntegerListType _indices;
    };

    // friendly name
    using SparseBinaryDataVector = SparseBinaryDataVectorBase<utilities::CompressedIntegerList>;
    using UncompressedSparseBinaryDataVector = SparseBinaryDataVectorBase<utilities::IntegerList>;
}
}

#include "../tcc/SparseBinaryDataVector.tcc"
