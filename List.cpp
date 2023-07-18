/********************************************************************************* 
 * Keaton Shawhan, kshawhan
 * 2023 Spring CSE101 PA5
 * List.cpp
 * Main file for List ADT
*********************************************************************************/

#include"List.h"

// Node struct
List::Node::Node(ListElement x){
   data = x;
   next = nullptr;
   prev = nullptr;
}

// List constructor
List::List(){
    frontDummy = new Node(-1);
    backDummy = new Node(-1);
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
}

// List copy constructor
List::List(const List& L){
    frontDummy = new Node(-1);
    backDummy = new Node(-1);
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;

    Node* N = L.frontDummy->next;
    while (N->next != nullptr){
        this->insertBefore(N->data);
        N = N->next;
    }
}

// List destructor
List::~List(){
    moveFront();
    while(num_elements > 0){
        eraseAfter();
    }
    delete frontDummy;
    delete backDummy;
}

// Returns the length of this List.
int List::length() const{
    return num_elements;
}

// returns value of the front of the List
ListElement List::front() const{
    if (num_elements <= 0){
        throw std::length_error("List: front(): empty List");
    }
    return(frontDummy->next->data);
}

// returns value of the back of the List
ListElement List::back() const{
    if (num_elements <= 0){
        throw std::length_error("List: back(): empty List");
    }
    return(backDummy->prev->data);
}

// returns the position of the cursor in the list
int List::position() const{
    if (pos_cursor < 0 || pos_cursor > length()){
        throw std::range_error("List: position(): position out of range");
    }
    return (pos_cursor);
}

// returns the value of the node to the right of the cursor
ListElement List::peekNext() const{
    if (num_elements <= position()){
        throw std::range_error("List: peekNext(): position out of range");
    }
    return (afterCursor->data);
}

// returns the value of the node to the left of the cursor
ListElement List::peekPrev() const{
    if (position() <= 0){
        throw std::range_error("List: peekPrev(): position out of range");
    }
    return (beforeCursor->data);
}

// clears the entire list of non-sentinel nodes
void List::clear() {
    moveFront();
    while (length() > 0){
        eraseAfter();
    }
}

// moves the cursor to the 0th index
void List::moveFront() {
    pos_cursor = 0;
    afterCursor = frontDummy->next;
    beforeCursor = frontDummy;
}

// moves the cursor to the back of the list (length()th's index)
void List::moveBack() {
    pos_cursor = length();
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;
}

// moves the cursor 1 index forward
ListElement List::moveNext() {
    if (position() >= length()){
        throw std::range_error("List: moveNext(): position out of range");
    }
    beforeCursor = afterCursor;
    afterCursor = afterCursor->next;
    pos_cursor += 1;
    return beforeCursor->data;
}

// moves the cursor 1 index backwards
ListElement List::movePrev() {
    if (position() <= 0){
        throw std::range_error("List: movePrev(): position out of range");
    }
    afterCursor = beforeCursor;
    beforeCursor = beforeCursor->prev;
    pos_cursor -= 1;
    return afterCursor->data;
}

// Inserts a new node with the passed in value x after the cursor
void List::insertAfter(ListElement x) {
    Node* n = new Node(x);
    n->next = afterCursor;
    n->prev = beforeCursor;
    afterCursor->prev = n;
    beforeCursor->next = n;
    afterCursor = n;
    num_elements += 1;
}

// Inserts a new node with the passed in value x before the cursor
void List::insertBefore(ListElement x) {
    Node* n = new Node(x);
    n->prev = beforeCursor;
    n->next = afterCursor;
    beforeCursor->next = n;
    afterCursor->prev = n;
    beforeCursor = n;
    num_elements += 1;
    pos_cursor += 1;
}

// updates the value of the element to the right of the cursor with x
void List::setAfter(ListElement x) {
    if (position() >= length()){
        throw std::range_error("List: setAfter(): position out of range");
    }
    afterCursor->data = x;
}

// updates the value of the element to the left of the cursor with x
void List::setBefore(ListElement x) {
    if (position() <= 0){
        throw std::range_error("List: setBefore(): position out of range");
    }
    beforeCursor->data = x;
}

// deletes the node to the right of the cursor
void List::eraseAfter() {
    if (position() >= length()){
        throw std::range_error("List: eraseAfter(): position out of range");
    }
    beforeCursor->next = afterCursor->next;
    afterCursor->next->prev = beforeCursor;
    delete afterCursor;
    afterCursor = beforeCursor->next;
    num_elements -= 1;
}

// deletes the node to the left of the cursor
void List::eraseBefore() {
    if (position() <= 0){
        throw std::range_error("List: eraseBefore(): position out of range");
    }
    afterCursor->prev = beforeCursor->prev;
    beforeCursor->prev->next = afterCursor;
    delete beforeCursor;
    beforeCursor = afterCursor->prev;
    num_elements -= 1;
    pos_cursor -= 1;
}

// finds the next occurrence of the value x in the list while iterating forward
// returns the position of the cursor
int List::findNext(ListElement x) {
    while (position() < length()){
        if (peekNext() == x){
            moveNext();
            return position();
        }
        moveNext();
    }
    moveBack();
    return -1;
}

// finds the next occurrence of the value x in the list while iterating backwards
// returns the position of the cursor
int List::findPrev(ListElement x) {
    while (position() > 0){
        if (peekPrev() == x){
            movePrev();
            return position();
        }
        movePrev();
    }
    moveFront();
    return -1;
}


// Removes any repeated elements in this List, leaving only unique elements.
// The order of the remaining elements is obtained by retaining the frontmost 
// occurrance of each element, and removing all other occurances. The cursor 
// is not moved with respect to the retained elements, i.e. it lies between 
// the same two retained elements that it did before cleanup() was called.
void List::cleanup() {
    long myNum[length()];
    for (int j = 0; j < length(); j++){
        myNum[j] = -1;
    }
    bool front = false;
    bool back = false;
    int runs = 0;
    int start_pos = position();
    int new_pos = position();
    if (position() == 0){
        front = true;
    }else if (position() == length()){
        back = true;
    }
    int added_elements = 0;
    moveFront();
    while (position() < length()){
        bool found = false;
        for (int i = 0; i < added_elements; i++){ 
            if (myNum[i] == peekNext()){
                found = true;
                eraseAfter();
                if (runs < start_pos){
                    new_pos -= 1;
                }
                break;
            }
        }
        if (found == false){
            myNum[position()] = peekNext();
            added_elements += 1;
            moveNext();
        }
        runs += 1;
    }
    moveFront();
    if (front == true){
        return;
    }
    if (back == true){
        moveBack();
        return;
    }
    moveNext();
    while (position() != new_pos){
        moveNext();
    }
}

// Returns a new List consisting of the elements of this List, followed by
// the elements of L. The cursor in the returned List will be at postion 0.
List List::concat(const List& L) const{
    List original = *this;
    List passed = L;
    original.moveBack();
    passed.moveFront();
    while (passed.position() < passed.length()){
        original.insertBefore(passed.peekNext());
        passed.moveNext();
    }
    original.moveFront();
    return original;
}

// Returns a string representation of this List consisting of a comma 
// separated sequence of elements, surrounded by parentheses.
std::string List::to_string() const{
    std::string total = "(";
    List copy = *this;

    copy.moveFront();
    while(copy.position() < copy.length()){
        if (copy.position() == copy.length()-1){
            total += std::to_string(copy.peekNext());
            copy.moveNext();
        }else{
            total += std::to_string(copy.peekNext())+", ";
            copy.moveNext();
        }
    }
    total += ")";
    return total;
}

// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List& R) const{
    List original = *this;
    List passed = R;
    if (original.length() != passed.length()){
        return false;
    }
    original.moveFront();
    passed.moveFront();
    for (int i = 0; i < original.length(); i ++){
        if (original.peekNext() != passed.peekNext()){
            return false;
        }
        original.moveNext();
        passed.moveNext();
    }
    return true;
}

// Inserts string representation of L into stream.
std::ostream& operator<<(std::ostream& stream, const List& L){
    return stream << L.List::to_string();
}

// Returns true if and only if A is the same integer sequence as B. The 
// cursors in both Lists are unchanged.
bool operator==(const List& A, const List& B){
    return A.List::equals(B);
}

// Overwrites the state of this List with state of L.
List& List::operator=(const List& L) {
    if( this != &L ){ // not self assignment
        // make a copy of Q
        List temp = L;

        // then swap the copy's fields with fields of this
        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(num_elements, temp.num_elements);
        std::swap(pos_cursor, temp.pos_cursor);
        std::swap(beforeCursor, temp.beforeCursor);
        std::swap(afterCursor, temp.afterCursor);
    }

    // return this with the new data installed
    return *this;

    // the copy, if there is one, is deleted upon return
}
