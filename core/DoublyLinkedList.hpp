// DoublyLinkedList.hpp
//
// ICS 46 Spring 2020
// Project #2: Time Waits for No One
//
// DoublyLinkedList<ValueType> is a class template that implements a
// doubly-linked list with head and tail pointers, including two kinds
// of iterators: One of them allows viewing and modifying the list's
// contents, while the other allows only viewing them.
//
// Your goal is to implement the entire public interface *exactly* as
// specified below.  Do not modify the signatures of any of the public
// member functions (including the public member functions of the various
// iterator classes) in any way.  We will be running extensive unit
// tests against your implementation, which will not compile unless
// things remain completely unchanged.  As we did in Project #0, we've
// provided you a basic set of unit tests that briefly demonstrate how
// each of the member functions is required to behave; you'll find
// those in the "gtest" directory.
//
// All of the public member functions listed with "noexcept" in their
// signature must be implemented in a way that they never throw exceptions.
// All of the others are expected to make the "strong" exception guarantee,
// which means two things: (1) no memory has leaked, and (2) the contents
// of the list/iterator will not have visibly changed in the event that
// an exception has been thrown.
//
// The entire C++ Standard Library is off-limits in your implementation
// of this class.  DO NOT submit a version of this file (or any file
// that it includes) that includes any C++ Standard Library headers.
// (This includes things like adding a print() member function that
// requires <iostream>.)
//
// As is good custom in class templates, keep the interface separate
// from the implementation.  This means the bodies of member functions
// should not be written in the class declaration, but should appear
// below it.  I've placed "dummy" implementations of every public
// member function, though, of course, most of them don't do the
// right thing; but they will save you some typing and demonstrate
// the structure of what you should be writing.

#ifndef DOUBLYLINKEDLIST_HPP
#define DOUBLYLINKEDLIST_HPP

#include "EmptyException.hpp"
#include "IteratorException.hpp"



template <typename ValueType>
class DoublyLinkedList
{
    // The forward declarations of these classes allows us to establish
    // that they exist, but delay displaying all of the details until
    // later in the file.
    //
    // (This is generally a good style, with the most important details
    // appearing earlier in the class declaration.  That's the same
    // reason why we're implementing the bodies of the member functions
    // outside of the class declaration.)
public:
    class Iterator;
    class ConstIterator;


private:
    struct Node;


public:
    // Initializes this list to be empty.
    DoublyLinkedList() noexcept;

    // Initializes this list as a copy of an existing one.
    DoublyLinkedList(const DoublyLinkedList& list);

    // Initializes this list from an expiring one.
    DoublyLinkedList(DoublyLinkedList&& list) noexcept;


    // Destroys the contents of this list.
    virtual ~DoublyLinkedList() noexcept;


    // Replaces the contents of this list with a copy of the contents
    // of an existing one.
    DoublyLinkedList& operator=(const DoublyLinkedList& list);

    // Replaces the contents of this list with the contents of an
    // expiring one.
    DoublyLinkedList& operator=(DoublyLinkedList&& list) noexcept;


    // addToStart() adds a value to the start of the list, meaning that
    // it will now be the first value, with all subsequent elements still
    // being in the list (after the new value) in the same order.
    void addToStart(const ValueType& value);

    // addToEnd() adds a value to the end of the list, meaning that
    // it will now be the last value, with all subsequent elements still
    // being in the list (before the new value) in the same order.
    void addToEnd(const ValueType& value);


    // removeFromStart() removes a value from the start of the list, meaning
    // that the list will now contain all of the values *in the same order*
    // that it did before, *except* that the first one will be gone.
    // In the event that the list is empty, an EmptyException will be thrown.
    void removeFromStart();

    // removeFromEnd() removes a value from the end of the list, meaning
    // that the list will now contain all of the values *in the same order*
    // that it did before, *except* that the last one will be gone.
    // In the event that the list is empty, an EmptyException will be thrown.
    void removeFromEnd();


    // first() returns the value at the start of the list.  In the event that
    // the list is empty, an EmptyException will be thrown.  There are two
    // variants of this member function: one for a const DoublyLinkedList and
    // another for a non-const one.
    const ValueType& first() const;
    ValueType& first();


    // last() returns the value at the end of the list.  In the event that
    // the list is empty, an EmptyException will be thrown.  There are two
    // variants of this member function: one for a const DoublyLinkedList and
    // another for a non-const one.
    const ValueType& last() const;
    ValueType& last();


    // isEmpty() returns true if the list has no values in it, false
    // otherwise.
    bool isEmpty() const noexcept;


    // size() returns the number of values in the list.
    unsigned int size() const noexcept;



    // There are two kinds of iterators supported: Iterators and
    // ConstIterators.  They have similar characteristics; they both
    // allow you to see what values are in the list and move back and
    // forth between them.  The difference is that ConstIterators allow
    // you to see the elements but not modify them, while Iterators also
    // support modification of the list (both by modifying the elements
    // directly, and also by inserting or removing values at arbitrary
    // locations).
    //
    // At any given time, an iterator refers to a value in the list.
    // There are also two additional places it can refer: "past start"
    // and "past end", which are the positions directly before the
    // first value and directly after the last one, respectively.
    // Except with respect to those boundaries, they can be moved
    // both forward and backward.
    //
    // Note, too, that the reason we have a ConstIterator class instead
    // of just saying "const Iterator" is because a "const Iterator"
    // is something different: It's an Iterator object that you can't
    // modify (i.e., you can't move it around).  What a ConstIterator
    // holds constant isn't the iterator; it's the list that's protected
    // by it.


    // iterator() creates a new Iterator over this list.  It will
    // initially be referring to the first value in the list, unless the
    // list is empty, in which case it will be considered both "past start"
    // and "past end".
    Iterator iterator();


    // constIterator() creates a new ConstIterator over this list.  It will
    // initially be referring to the first value in the list, unless the
    // list is empty, in which case it will be considered both "past start"
    // and "past end".
    ConstIterator constIterator() const;


public:
    // The IteratorBase class is the base class for our two kinds of
    // iterators.  Because there are so many similarities between them,
    // we write those similarities in a base class, then inherit from
    // that base class to specify only the differences.
    class IteratorBase
    {
    public:
        // Initializes a newly-constructed IteratorBase to operate on
        // the given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        IteratorBase(const DoublyLinkedList& list) noexcept;


        // moveToNext() moves this iterator forward to the next value in
        // the list.  If the iterator is refrering to the last value, it
        // moves to the "past end" position.  If it is already at the
        // "past end" position, an IteratorException will be thrown.
        void moveToNext();


        // moveToPrevious() moves this iterator backward to the previous
        // value in the list.  If the iterator is refrering to the first
        // value, it moves to the "past start" position.  If it is already
        // at the "past start" position, an IteratorException will be thrown.
        void moveToPrevious();


        // isPastStart() returns true if this iterator is in the "past
        // start" position, false otherwise.
        bool isPastStart() const noexcept;


        // isPastEnd() returns true if this iterator is in the "past end"
        // position, false otherwise.
        bool isPastEnd() const noexcept;
    
    protected:
        // You may want protected member variables and member functions,
        // which will be accessible to the derived classes.
        Node * pointer;
    };


    class ConstIterator : public IteratorBase
    {
    public:
        // Initializes a newly-constructed ConstIterator to operate on
        // the given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        ConstIterator(const DoublyLinkedList& list) noexcept;


        // value() returns the value that the iterator is currently
        // referring to.  If the iterator is in the "past start" or
        // "past end" positions, an IteratorException will be thrown.
        const ValueType& value() const;

    private:
        // You may want private member variables and member functions.
    };


    class Iterator : public IteratorBase
    {
    public:
        // Initializes a newly-constructed Iterator to operate on the
        // given list.  It will initially be referring to the first
        // value in the list, unless the list is empty, in which case
        // it will be considered to be both "past start" and "past end".
        Iterator(DoublyLinkedList& list) noexcept;


        // value() returns the value that the iterator is currently
        // referring to.  If the iterator is in the "past start" or
        // "past end" positions, an IteratorException will be thrown.
        ValueType& value() const;


        // insertBefore() inserts a new value into the list before
        // the one to which the iterator currently refers.  If the
        // iterator is in the "past start" position, an IteratorException
        // is thrown.
        void insertBefore(const ValueType& value);


        // insertAfter() inserts a new value into the list after
        // the one to which the iterator currently refers.  If the
        // iterator is in the "past end" position, an IteratorException
        // is thrown.
        void insertAfter(const ValueType& value);


        // remove() removes the value to which this iterator refers,
        // moving the iterator to refer to either the value after it
        // (if moveToNextAfterward is true) or before it (if
        // moveToNextAfterward is false).  If the iterator is in the
        // "past start" or "past end" position, an IteratorException
        // is thrown.
        void remove(bool moveToNextAfterward = true);

    private:
        // You may want private member variables and member functions.
    };


private:
    // A structure that contains the vital parts of a Node in a
    // doubly-linked list, the value and two pointers: one pointing
    // to the previous node (or nullptr if there isn't one) and
    // one pointing to the next node (or nullptr if there isn't
    // one).
    struct Node
    {
        ValueType value;
        Node* prev;
        Node* next;
    };
    Node* head;
    // You can feel free to add private member variables and member
    // functions here; there's a pretty good chance you'll need some.
};



template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList() noexcept
{
    head = nullptr;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList(const DoublyLinkedList& list)
{
    if (list.head == nullptr){
        head = nullptr;
    }
    else{
        Node * temp = list.head;
        Node *current = new Node();
        current ->prev = nullptr;
        current -> value = temp->value;
        current -> next = nullptr;
        head = current;
        while (temp->next != nullptr){
            temp = temp-> next;
            Node *nextnode = new Node();
            nextnode ->value = temp->value;
            nextnode->prev = current;
            nextnode->next = temp->next;
            current->next = nextnode;
            current = current->next; 
        }

    }
}


template <typename ValueType>
DoublyLinkedList<ValueType>::DoublyLinkedList(DoublyLinkedList&& list) noexcept
{
    head = list.head;
    list.head = nullptr;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::~DoublyLinkedList() noexcept
{
    if (head != nullptr){
        Node *current = head;
        while (current!= nullptr){
            Node *del = current;
            current = current->next;
            delete(del);
        } 
    }
}


template <typename ValueType>
DoublyLinkedList<ValueType>& DoublyLinkedList<ValueType>::operator=(const DoublyLinkedList& list)
{
    if (!isEmpty()){
        Node *current = head;
        while (current!= nullptr){
            Node *del = current;
            current = current->next;
            delete(del);
        } 
    }
    if (list.head == nullptr){
        head = nullptr;
    }
    else{
        Node * temp = list.head;
        Node *current = new Node();
        current ->prev = nullptr;
        current -> value = temp->value;
        current -> next = nullptr;
        head = current;
        while (temp->next != nullptr){
            temp = temp-> next;
            Node *nextnode = new Node();
            nextnode ->value = temp->value;
            nextnode->prev = current;
            nextnode->next = temp->next;
            current->next = nextnode;
            current = current->next; 
        }
    }

    return *this;
}


template <typename ValueType>
DoublyLinkedList<ValueType>& DoublyLinkedList<ValueType>::operator=(DoublyLinkedList&& list) noexcept
{
    Node *temp = head;
    head = list.head;
    list.head = temp;
    return *this;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::addToStart(const ValueType& value)
{
    if (isEmpty()){
        Node *new_node = new Node();
        new_node->prev = nullptr;
        new_node->next = nullptr;
        new_node->value = value;
        head = new_node;
    }
    else{
        Node *new_node = new Node();
        new_node->value = value;
        new_node->prev = nullptr;
        new_node->next = head;
        head = new_node;
    }
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::addToEnd(const ValueType& value)
{
    if (isEmpty()){
        Node *new_node = new Node();
        new_node->prev = nullptr;
        new_node->next = nullptr;
        new_node->value = value;
        head = new_node;
    }
    else{
        Node *new_node = new Node();
        new_node->value = value;
        Node *current = head;
        while(current->next != nullptr) current = current->next;
        current->next = new_node;
        new_node->prev = current;
        new_node->next = nullptr;
    }
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::removeFromStart()
{
    if(isEmpty()){
        throw EmptyException();
    }
    if (head->next == nullptr){
        delete head;
        head = nullptr;
    }
    else{
        Node *temp = head;
        head = head->next;
        head->prev = nullptr;
        delete temp; 
    }

}


template <typename ValueType>
void DoublyLinkedList<ValueType>::removeFromEnd()
{
    try{
        if(isEmpty()){
        throw EmptyException();
        }
        if (head->next == nullptr){
            delete head;
            head = nullptr;
        }
        else{
            Node* current = head;
            while(current->next != nullptr) current = current->next;
            current->prev->next = nullptr;
            delete current;
        }
    }
    catch(...){
        Node *current = head;
        while (current!= nullptr){
            Node *del = current;
            current = current->next;
            delete del;
        } 
        throw;
    }
    
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::first() const
{
    if (isEmpty()){
        throw EmptyException();
    }
    else{
        return head->value;
    }
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::first()
{
    if (isEmpty()){
        throw EmptyException();
    }
    else{
        return head->value;
    }
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::last() const
{
    if (isEmpty()){
        throw EmptyException();
    }
    else{
        Node *current = head;
        while(current->next != nullptr) current = current->next;
        return current->value;
    }
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::last()
{
    if (isEmpty()){
        throw EmptyException();
    }
    else{
        Node *current = head;
        while(current->next != nullptr) current = current->next;
        return current->value;
    }
}


template <typename ValueType>
unsigned int DoublyLinkedList<ValueType>::size() const noexcept
{
    if (head == nullptr) return 0;
    int result = 1;
    Node *current = head;
    while (current->next != nullptr){
        result++;
        current = current->next;
    }
    return result;
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::isEmpty() const noexcept
{
    return head == nullptr;
}


template <typename ValueType>
typename DoublyLinkedList<ValueType>::Iterator DoublyLinkedList<ValueType>::iterator()
{
    return Iterator{*this};
}


template <typename ValueType>
typename DoublyLinkedList<ValueType>::ConstIterator DoublyLinkedList<ValueType>::constIterator() const
{
    return ConstIterator{*this};
}


template <typename ValueType>
DoublyLinkedList<ValueType>::IteratorBase::IteratorBase(const DoublyLinkedList& list) noexcept
{
    if (list.isEmpty()) {
        pointer = nullptr;
    }
    else{
        pointer = list.head;
    }
    
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::IteratorBase::moveToNext()
{
    if(!isPastEnd()){
        pointer = pointer->next;
    }
    else{
        throw IteratorException();
    }
    
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::IteratorBase::moveToPrevious()
{
    if(!isPastStart()){
        pointer = pointer->prev;
    }
    else{
        throw IteratorException();
    }
    
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::IteratorBase::isPastStart() const noexcept
{
    if (pointer == nullptr) return true;
    return false;
}


template <typename ValueType>
bool DoublyLinkedList<ValueType>::IteratorBase::isPastEnd() const noexcept
{
    if (pointer == nullptr) return true;
    return false;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::ConstIterator::ConstIterator(const DoublyLinkedList& list) noexcept
    : IteratorBase{list}
{
}


template <typename ValueType>
const ValueType& DoublyLinkedList<ValueType>::ConstIterator::value() const
{
    // Note that this is an awful thing I'm doing here, but I needed
    // something that would make this code compile.  You're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.  This
    // version *will* leak memory and *will* return an undefined value.
    if (this->pointer == nullptr) throw IteratorException();
    return this->pointer->value;
}


template <typename ValueType>
DoublyLinkedList<ValueType>::Iterator::Iterator(DoublyLinkedList& list) noexcept
    : IteratorBase{list}
{
}


template <typename ValueType>
ValueType& DoublyLinkedList<ValueType>::Iterator::value() const
{
    // Note that this is an awful thing I'm doing here, but I needed
    // something that would make this code compile.  You're definitely
    // going to want to replace this with something else before you
    // ever call this member function and expect it to work.  This
    // version *will* leak memory and *will* return an undefined value.
    if (this->pointer == nullptr) throw IteratorException();
    return this->pointer->value;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::insertBefore(const ValueType& value)
{
    if (this->pointer == nullptr) throw IteratorException();
    Node *new_node = new Node;
    new_node->value = value;
    new_node->prev = this->pointer->prev;
    new_node->next = this->pointer;
    if (this->pointer->prev != nullptr) this->pointer->prev->next = new_node;
    this->pointer->prev = new_node;

}


template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::insertAfter(const ValueType& value)
{
    if (this->pointer == nullptr) throw IteratorException();
    Node *new_node = new Node;
    new_node->value = value;
    new_node->prev = this->pointer;
    new_node->next = this->pointer->next;
    this->pointer->next = new_node;
    if (this->pointer->next != nullptr) this->pointer->next->prev = new_node;
}


template <typename ValueType>
void DoublyLinkedList<ValueType>::Iterator::remove(bool moveToNextAfterward)
{
    if (this->pointer == nullptr) throw IteratorException();
    if (moveToNextAfterward){
        Node * temp = this->pointer;
        this->pointer = this->pointer->next;
        if (this->pointer != nullptr) this->pointer->prev = temp->prev;
        temp->prev->next = this->pointer;
        delete temp; 
    }
    else{
        Node * temp = this->pointer;
        this->pointer = this->pointer->prev;
        if (this->pointer != nullptr) this->pointer->next = temp->next;
        temp->next->prev = this->pointer;
        delete temp;
    }
}



#endif

