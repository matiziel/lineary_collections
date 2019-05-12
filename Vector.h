#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>

namespace aisdi {

    template <typename Type>
    class Vector {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;
        using const_pointer = const Type*;
        using const_reference = const Type&;

        class ConstIterator;
        class Iterator;
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        Vector() {
            array_begin = new Type[FIRST_SIZE];
            alloc_size = FIRST_SIZE;
            current_size = 0;
        }

        Vector(std::initializer_list<Type> l) {
            current_size = alloc_size = l.size();
            array_begin = new Type[alloc_size];
            std::copy(l.begin(), l.end(), array_begin);
        }

        Vector(const Vector& other) {
            current_size = alloc_size = other.getSize();
            array_begin = new Type[alloc_size];
            std::copy(other.begin(), other.end(), array_begin);
        }

        Vector(Vector&& other) {
            array_begin = other.array_begin;
            other.array_begin = nullptr;
            current_size = other.current_size;
            alloc_size = other.alloc_size;
        }

        ~Vector() {
            delete[] array_begin;
        }

        Vector& operator=(const Vector& other) {
            if (this == &other) {
                return *this;
            }
            delete[] array_begin;
            array_begin = new Type[other.alloc_size];
            std::copy(other.cbegin(), other.cend(), begin());
            alloc_size = other.alloc_size;
            current_size = other.current_size;
            return *this;
        }

        Vector& operator=(Vector&& other) {
            if (this == &other) {
                return *this;
            }
            delete[] array_begin;
            array_begin = other.array_begin;
            other.array_begin = nullptr;
            alloc_size = other.alloc_size;
            current_size = other.current_size;
            return *this;
        }

        bool isEmpty() const {
            return current_size == 0;
        }

        size_type getSize() const {
            return current_size;
        }

        void append(const Type& item) {
            insert(end(), item);
        }

        void prepend(const Type& item) {
            insert(begin(), item);
        }
	//przesuwa elementy o jeden od podanego iteratora i wstawia w wolne miejsce
        void insert(const const_iterator& insertPosition, const Type& item) {
            difference_type distance =  insertPosition - cbegin();
            if (current_size == alloc_size) {
                new_allocate();
            }
            for (pointer it = array_begin + current_size - 1; it >= array_begin + distance; --it) {
                *(it + 1) = *it;
            }
            ++current_size;
            *(begin() + distance) = item;
        }

        Type popFirst() {
            if (isEmpty()) {
                throw std::logic_error("Empty collection");
            }
            Type val = array_begin[0];
            std::copy(++begin(), end(), begin());
            --current_size;
            return val;
        }

        Type popLast() {
            if (isEmpty()) {
                throw std::logic_error("Empty collection");
            }
            --current_size;
            return array_begin[current_size];
        }

        void erase(const const_iterator& position) {
            std::copy(position + 1, cend(), iterator(position));
            --current_size;
        }

        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded) {
            std::copy(lastExcluded, cend(), iterator(firstIncluded));
            current_size -= lastExcluded - firstIncluded;
        }


        iterator begin() {
            return iterator(array_begin, *this);
        }

        iterator end() {
            return iterator(array_begin + current_size, *this);
        }

        const_iterator cbegin() const {
            return const_iterator(array_begin, *this);
        }

        const_iterator cend() const {
            return const_iterator(array_begin + current_size, *this);
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

    private:
        void new_allocate() {
            Type* new_array = new Type[alloc_size * 2];
            std::copy(begin(), end(), new_array);
            alloc_size *= 2;
            delete[] array_begin;
            array_begin = new_array;
        }

        pointer array_begin;
        size_type current_size;
        size_type alloc_size;

        size_type FIRST_SIZE = 10;
    };

    template <typename Type>
    class Vector<Type>::ConstIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename Vector::value_type;
        using difference_type = typename Vector::difference_type;
        using pointer = typename Vector::const_pointer;
        using reference = typename Vector::const_reference;

        explicit ConstIterator(pointer ptr, const Vector<Type>& parent) : current_pointer(ptr), parent(parent) {}

        reference operator*() const {
            if (*this < parent.begin() || *this >= parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            return *(current_pointer);
        }

        ConstIterator& operator++() {
            if (*this >= parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            ++current_pointer;
            return *this;
        }

        ConstIterator operator++(int) {
            if (*this >= parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            ConstIterator result = *this;
            ++current_pointer;
            return result;
        }

        ConstIterator& operator--() {
            if (*this <= parent.begin()) {
                throw std::out_of_range("Iterator out of range");
            }
            --current_pointer;
            return *this;
        }

        ConstIterator operator--(int) {
            if (*this <= parent.begin()) {
                throw std::out_of_range("Iterator out of range");
            }
            ConstIterator result = *this;
            --current_pointer;
            return result;
        }

        ConstIterator& operator+=(difference_type d) {
            current_pointer += d;
            return *this;
        }

        ConstIterator& operator-=(difference_type d) {
            current_pointer -= d;
            return *this;
        }

        ConstIterator operator+(difference_type d) const {
            ConstIterator new_iter = *this;
            new_iter += d;
            return new_iter;
        }

        difference_type operator-(const ConstIterator &other) const {
            return current_pointer - other.current_pointer;
        }

        ConstIterator operator-(difference_type d) const {
            ConstIterator new_iter = *this;
            new_iter -= d;
            return new_iter;
        }

        bool operator==(const ConstIterator& other) const {
            return current_pointer == other.current_pointer;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }

        bool operator<=(const ConstIterator &other) const {
            return current_pointer <= other.current_pointer;
        }

        bool operator>=(const ConstIterator &other) const {
            return current_pointer >= other.current_pointer;
        }

        bool operator<(const ConstIterator &other) const {
            return !(*this >= other);
        }

        bool operator>(const ConstIterator &other) const {
            return !(*this <= other);
        }

    protected:
        pointer current_pointer;
        const Vector<Type>& parent;
    };

    template <typename Type>
    class Vector<Type>::Iterator : public Vector<Type>::ConstIterator {
    public:
        using pointer = typename Vector::pointer;
        using reference = typename Vector::reference;

        explicit Iterator(pointer ptr, Vector<Type>& parent) : ConstIterator(ptr, parent) {}

        Iterator(const ConstIterator& other)
                : ConstIterator(other) {}

        Iterator& operator++() {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int) {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator& operator--() {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int) {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const {
            return ConstIterator::operator-(d);
        }

        reference operator*() const {
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_VECTOR_H
