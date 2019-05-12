#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>


namespace aisdi
{

    template <typename Type>
    class LinkedList
    {
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
	//struktura wezla, pointer na value zeby odroznic sentinela
        struct Node {
            Type* value;
            Node* next;
            Node* prev;

            Node(Type* val)
	    {
		next = nullptr;
		prev = nullptr;
		value = val;
	    }
            ~Node() {
                delete value;
            }
        };
        using node_pointer = Node*;  //skrócenie zapisu w klasie iterator, nie trzeba pisać LinkedList<Type>::

        LinkedList() : size(0) {
            Node* sentinel = new Node(nullptr);
            first = last = sentinel;
            sentinel->next = nullptr;
            sentinel->prev = nullptr;
        }

        LinkedList(std::initializer_list<Type> l) : LinkedList() {
            for (const auto& val : l) {
                insert(end(), val);
	    
	    
            }
        }

        LinkedList(const LinkedList& other) : LinkedList() {
            for (auto it = other.begin(); it != other.end(); ++it) {
                insert(end(), *it);
            }
        }

        LinkedList(LinkedList&& other) {
            first = other.first;
            last = other.last;
            size = other.size;
            other.first = nullptr;
            other.last = nullptr;
        }

        ~LinkedList() {
            Node* next = first;
            while (next != nullptr) {
                Node* to_delete = next;
                next = to_delete->next;
                delete to_delete;
            }
        }

        LinkedList& operator=(const LinkedList& other) {
            if (this == &other) {
                return *this;
            }
            erase(cbegin(), cend()); //usuwanie elementow znajdujacych sie w liscie
            for (auto it = other.begin(); it != other.end(); ++it) {
                insert(end(), *it);
            }
            return *this;
        }

        LinkedList& operator=(LinkedList&& other) {
            if (this == &other) {
                return *this;
            }
            Node* next = first;
	    //jezeli lista byla, trzeba zdealokowac pamiec
            while (next != nullptr) {
                Node* to_delete = next;
                next = to_delete->next;
                delete to_delete;
            }
            first = other.first;
            last = other.last;
            size = other.size;
            other.first = nullptr;
            other.last = nullptr;
            return *this;
        }

        bool isEmpty() const {
            return size == 0;
        }

        size_type getSize() const {
            return size;
        }

        void append(const Type& item) {
            insert(end(), item);
        }

        void prepend(const Type& item) {
            insert(begin(), item);
        }
	//wstawianie elementu przed elementem wskazywanym przez iterator
        void insert(const const_iterator& insertPosition, const Type& item) {
            Node* new_element = new Node(new Type(item));
            new_element->next = insertPosition.get();
            new_element->prev = insertPosition.get()->prev;
            new_element->next->prev = new_element;

            if (new_element->prev != nullptr) {
                new_element->prev->next = new_element;
            }
            else {
                first = new_element;
            }
            ++size;
        }

        Type popFirst() {
            if (isEmpty()) {
                throw std::logic_error("List is empty");
            }
            Type ret_val = *begin();
            erase(begin());
            return ret_val;
        }

        Type popLast() {
            if (isEmpty()) {
                throw std::logic_error("List is empty");
            }
            Type ret_val = *(--end());
            erase(--end());
            return ret_val;
        }

        void erase(const const_iterator& position) {
            if (position == end()) {
                throw std::out_of_range("Iterator out of range");
            }
            Node* to_delete = position.get();
            to_delete->next->prev = to_delete->prev;
            if (to_delete->prev != nullptr) {
                to_delete->prev->next = to_delete->next;
            }
            else {
                first = to_delete->next;
            }
            delete to_delete;
            --size;
        }

        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded) {
            
	    Node* beg_el = firstIncluded.get();
            Node* end_el = lastExcluded.get();
            end_el->prev = beg_el->prev;
            if (beg_el->prev != nullptr) {
                beg_el->prev->next = end_el;
            }
            else {
                first = end_el;
            }
            Node* next = beg_el;
            while (next != end_el) {
                Node* to_delete = next;
                next = to_delete->next;
                delete to_delete;
                --size;
            }
        }

        iterator begin()
        {
            return iterator(first, *this);
        }

        iterator end()
        {
            return iterator(last, *this);
        }

        const_iterator cbegin() const
        {
            return const_iterator(first, *this);
        }

        const_iterator cend() const
        {
            return const_iterator(last, *this);
        }

        const_iterator begin() const
        {
            return cbegin();
        }

        const_iterator end() const
        {
            return cend();
        }

    private:
        Node* first;
        Node* last;
        size_type size;
    };

    template <typename Type>
    class LinkedList<Type>::ConstIterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

        using node_pointer = typename LinkedList::node_pointer;

        explicit ConstIterator(node_pointer el, const LinkedList& parent) : node_ptr(el), parent(parent) {}

        reference operator*() const {
            if (*this == parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            return *(node_ptr->value);
        }

        node_pointer get() const {
            return node_ptr;
        }

        ConstIterator& operator++() {
            if (*this == parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            node_ptr = node_ptr->next;
            return *this;
        }

        ConstIterator operator++(int) {
            if (*this == parent.end()) {
                throw std::out_of_range("Iterator out of range");
            }
            ConstIterator result = *this;
            node_ptr = node_ptr->next;
            return result;
        }

        ConstIterator& operator--() {
            if (*this == parent.begin()) {
                throw std::out_of_range("Iterator out of range");
            }
            node_ptr = node_ptr->prev;
            return *this;
        }

        ConstIterator operator--(int) {
            if (*this == parent.begin()) {
                throw std::out_of_range("Iterator out of range");
            }
            ConstIterator result = *this;
            node_ptr = node_ptr->prev;
            return result;
        }

        ConstIterator& operator+=(difference_type d) {
            for (int i = 0; i < d; ++i)
		++(*this);
            return *this;
        }

        ConstIterator& operator-=(difference_type d) {
            for (int i = 0; i < d; ++i)
		--(*this);
            return *this;
        }

        ConstIterator operator+(difference_type d) const {
            ConstIterator new_iter = *this;
            new_iter += d;
            return new_iter;
        }

        ConstIterator operator-(difference_type d) const {
            ConstIterator new_iter = *this;
            new_iter -= d;
            return new_iter;
        }

        bool operator==(const ConstIterator& other) const {
            return node_ptr == other.node_ptr;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }

    private:
	//referencja do listy, mozliwe bylo skojarzenie iteratora z konkretna lista np begin()
        node_pointer node_ptr;
        const LinkedList& parent;
    };

    template <typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
    {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator(node_pointer el, const LinkedList& parent) : ConstIterator(el, parent) {}

        Iterator(const ConstIterator& other)
                : ConstIterator(other)
        {}

        Iterator& operator++()
        {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int)
        {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator& operator--()
        {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int)
        {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const
        {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const
        {
            return ConstIterator::operator-(d);
        }

        reference operator*() const
        {
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_LINKEDLIST_H
