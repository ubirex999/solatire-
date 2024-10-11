#include<iostream>
#include<iomanip>
#include<cstdlib>
#include <cstring>
#include <sstream>
using namespace std;

/////////////////////////////////// cards and its initializations ////////////////////////////
class card {
    const char* suit;// h or s or c or d
    const char* color; //r or b
    const char* rank;
    bool hide;
public:
    //constructor
    card() : suit(""), color(""), rank(""), hide(false) {}

    bool getHide() {
        return hide;
    }

    void setCard(const char* ch1, const char* ch2, const char* ch3) {
        suit = ch1;
        color = ch2;
        rank = ch3;
    }

    void toggleHide() {
        if(this)
            hide = !hide;
    }

    friend void initializeCards(card**& cardArray);

    friend ostream& operator<<(ostream& out, card* d);

    friend bool conditionsForCardsInFoundations(card*& f, card*& c);
    friend bool conditionsForCardsInLists(card*& c1, card*& c2);
};
void initializeCards(card**& cardArray) {
    const char* rankArray[] = { "A","2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    cardArray = new card * [52];//hardcode as there only exists 52 cards

    int j = 0;
    for (int i = 0; i < 52; i++) {
        if (i < 13) {
            cardArray[i] = new card();
            cardArray[i]->setCard("heart", "red", rankArray[j]);
        }
        else if (i < 26) {
            cardArray[i] = new card();
            cardArray[i]->setCard("spade", "black", rankArray[j]);
        }
        else if (i < 39) {
            cardArray[i] = new card();
            cardArray[i]->setCard("club", "black", rankArray[j]);
        }
        else {
            cardArray[i] = new card();
            cardArray[i]->setCard("diamond", "red", rankArray[j]);
        }
        if (j == 12)
            j = 0;
        else
            j++;
    }
}
ostream& operator<<(ostream& out, card* d) {
    if (d == nullptr)
        out << "[    empty    ]";
    else {
        if (d->hide == false) {
            out << "[" << d->suit << " ";
            out << d->color << " ";
            out << setw(2) << d->rank << "]";
        }
        else {
            out << "[   ]";
        }
    }

    return out;
}
bool conditionsForCardsInFoundations(card*& f, card*& c) {
    const char* rankArray[] = { "A","2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    if (f && c) {
        if (f->suit == c->suit && f->color == c->color) {
            for (int i = 0; i < 13; i++) {
                if (rankArray[i] == f->rank && rankArray[i + 1] == c->rank) {
                    return true;
                }
            }
        }
    }
    else if (c && !f) {
        if (c->rank == "A")
            return true;
        return false;
    }

    return false;
}
bool conditionsForCardsInLists(card*& c1, card*& c2) {
    if (c1 == nullptr)
        return false;
    if (c2 == nullptr)
        if (c1->rank == "K")
            return true;
        else
            return false;
    if (c1->hide || c2->hide)
        return false;
    const char* rankArray[] = { "A","2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    if (c1->color != c2->color) {
        for (int i = 0; i < 13; i++)
            if (rankArray[i] == c1->rank && rankArray[i + 1] == c2->rank)
                return true;
    }
    return false;
}

///////////////////////////////////// list implementations //////////////////////////////////
template<class T>
class list
{
    class node {

    public:
        T data;
        node* next;
        node* prev;

        //constructor
        node(T d) : data(d), next(nullptr), prev(nullptr) {}

    };

    node* head;
    node* tail;
    int size;

public:
    //constructor
    list() {

        //we made dumb nodes of head and tail in doubly linked list
        head = new node(T());
        tail = new node(T());

        head->next = tail;
        tail->prev = head;

        size = 0;
    }

    class iterator {
        node* currentNode;

    public:
        iterator(node* n = nullptr) : currentNode(n) {}

        iterator& operator=(node* n) {
            currentNode = n;
            return *this;
        }

        iterator& operator=(iterator& rhs) {
            if (this->currentNode != rhs.currentNode) {
                currentNode = rhs.currentNode;
            }
            return *this;
        }

        T& operator*() {
            return currentNode->data;
        }

        iterator& operator++() {
            if (currentNode) {
                currentNode = currentNode->next;
            }
            return *this;
        }

        iterator& operator++(int) {
            iterator temp = *this;
            if (currentNode) {
                currentNode = currentNode->next;
            }
            return temp;
        }

        iterator& operator--() {
            if (currentNode) {
                currentNode = currentNode->prev;
            }
            return *this;
        }

        iterator& operator--(int) {
            iterator temp = *this;
            if (currentNode) {
                currentNode = currentNode->prev;
            }
            return temp;
        }

        bool operator!=(const iterator& other) {
            return currentNode != other.currentNode;
        }

        void moveNodesToAnotherList(list<T>& dest, list<T>& source, int& n) {
            currentNode->prev->next = source.tail;
            node* temp = currentNode->prev;
            currentNode->prev = dest.tail->prev;
            dest.tail->prev->next = currentNode;
            source.tail->prev->next = dest.tail;
            dest.tail->prev = source.tail->prev;
            source.tail->prev = temp;
            temp->data->toggleHide();
            source.size = source.size - n;
            dest.size = dest.size + n;
        }
        void moveNodesToAnotherListFromFoundations(list<T>& dest, list<T>& source, int& n) {
            currentNode->prev->next = source.tail;
            node* temp = currentNode->prev;
            currentNode->prev = dest.tail->prev;
            dest.tail->prev->next = currentNode;
            source.tail->prev->next = dest.tail;
            dest.tail->prev = source.tail->prev;
            source.tail->prev = temp;
            source.size = source.size - n;
            dest.size = dest.size + n;
        }
        void moveNodesToAnotherListFromFoundationsReverse(list<T>& dest, list<T>& source, int& n) {
            node* destTemp = dest.tail->prev;
            currentNode->prev->next = source.tail;
            node* temp = currentNode->prev;
            currentNode->prev = dest.tail->prev;
            dest.tail->prev->next = currentNode;
            source.tail->prev->next = dest.tail;
            dest.tail->prev = source.tail->prev;
            source.tail->prev = temp;
            destTemp->data->toggleHide();
            source.size = source.size - n;
            dest.size = dest.size + n;
        }
    };

    node* begin() {
        return head;
    }

    node* end() {
        return tail->prev;
    }

    //remove from end not delete
    void pop() {
        if (size) {
            node* temp = tail->prev;
            tail->prev = tail->prev->prev;
            tail->prev->next = tail;
            size--;
        }
    }

    void insertAtStart(T d) {
        node* t = new node(d);
        t->next = head->next;
        t->prev = head;
        head->next->prev = t;
        head->next = t;
        size++;
    }

    void insertAtEnd(T d) {
        node* t = new node(d);
        t->prev = tail->prev;
        t->next = tail;
        tail->prev->next = t;
        tail->prev = t;
        size++;
    }

    bool deleteStart() {
        if (size) {
            node* temp = head->next;
            head->next = head->next->next;
            head->next->prev = head;
            delete temp;
            size--;
            return true;
        }
        return false;
    }

    bool deleteEnd() {
        if (size) {
            node* temp = tail->prev;
            tail->prev = tail->prev->prev;
            tail->prev->next = tail;
            delete temp;
            size--;
            return true;
        }
        return false;
    }

    void print() {
        node* temp = head->next;
        while (temp->next) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    node* findElement(T val) {
        node* temp = head->next;
        while (temp->next) {
            if (temp->data == val) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    bool deleteElement(T val) {
        node* temp = findElement(val);
        if (temp) {
            temp->next->prev = temp->prev;
            temp->prev->next = temp->next;
            size--;
            return true;
        }
        return false;
    }

    int getSize() {
        return size;
    }

    T& getTail() {
        return tail->prev->data;
    }

    T& gethead() {
        return head->next->data;
    }

    void movelistToFoundation(list<card*>& foundation) {
        node* temp = tail->prev->prev;
        temp->next = tail;
        tail->prev->prev = foundation.tail->prev;
        foundation.head->next = tail->prev;
        tail->prev->next = foundation.tail;
        foundation.tail->prev = tail->prev;
        tail->prev = temp;
        foundation.size = foundation.size + 1;
        size = size - 1;
    }
};

///////////////////////////////////// stack implementations /////////////////////////////////
template<class T>
class stack {
    list<T> s;
public:
    //constructor
    stack() {}

    void push(T d) {
        s.insertAtEnd(d);
    }

    bool pop() {
        if (s.deleteEnd())
            return true;
        return false;
    }

    bool isEmpty() {
        if (s.getSize())
            return false;
        return true;
    }

    T top() {
        if (isEmpty())
            return T();// will return nullptr
        else
            return s.getTail();
    }

    int size() {
        return s.getSize();
    }

    void print() {
        s.print();
    }

    list<T>& getList() {
        return s;
    }
};

class solitaire {
    card** cardArray;
    string command;
    stack<card*> stackPile;
    stack<card*> wastePile;
    stack<card*> f1;
    stack<card*> f2;
    stack<card*> f3;
    stack<card*> f4;
    list<card*>* columnLists;
    stack<string> commands;
public:
    //constructor
    solitaire() : cardArray(nullptr), columnLists(nullptr) {}

    //////////////////////////////  the overall globl game functions ////////////////////////////
    void initializeStackPile() {
        for (int i = 0; i < 24; i++) {
            stackPile.push(cardArray[i]);
        }
    }
    void initializeColumnLists() {
        columnLists = new list<card*>[7];
        int j = 24;//after stack pile remaining cards
        for (int i = 0; i < 7; i++) {
            for (int k = 0; k <= i; k++) {
                columnLists[i].insertAtStart(cardArray[j]);
                if (k)
                    columnLists[i].gethead()->toggleHide();
                j++;
            }
        }
    }
    void shuffleCards() {
        srand(time(0));
        card* temp = nullptr;
        for (int i = 0; i < 25; i++) {
            int j = rand() % 52;
            //random swapping
            temp = cardArray[i];
            cardArray[i] = cardArray[j];
            cardArray[j] = temp;
        }
    }
    void initialization() {
        initializeCards(cardArray);
        shuffleCards();
        initializeStackPile();
        initializeColumnLists();
    }
    void display() {
        cout << "Stock\t\tWaste\t\t\t\t\t" << "\tFoundation 1\t\tFoundation 2\t\tFoundation 3\t\tFoundation 4" << endl;
        cout << "[   ]\t\t";
        cout << wastePile.top() << "\t\t\t\t";
        cout << f1.top() << "  \t";
        cout << f2.top() << "  \t";
        cout << f3.top() << "  \t";
        cout << f4.top() << "  \t" << endl;
        cout << stackPile.size() << " cards \t " << wastePile.size() << " cards" << endl << endl;

        for (int i = 0; i < 7; i++) {
            list<card*> ::iterator it = columnLists[i].begin();
            list<card*> ::iterator end = columnLists[i].end();
            end++;
            it++;
            cout << "Column " << i + 1 << ": ";
            while (it != end) {
                cout << *it;
                it++;
            }
            cout << endl;
        }

        cout << "Commands: " << endl;
        commands.print();
    }
    void input() {
        cout << "Enter command: ";
        getline(cin, command);
    }
    void movetoFoundation(stack<card*>& foundation, string source) {
        card* f = foundation.top();
        if (source == "c1") {
            card* c = columnLists[0].getTail();
            if (conditionsForCardsInFoundations(f, c)) {
                columnLists[0].movelistToFoundation(foundation.getList());
                commands.push(command);
                c = columnLists[1].getTail();
                if (c)
                    if (c->getHide())
                        c->toggleHide();
            }
        }
        else if (source == "c2") {
            card* c = columnLists[1].getTail();
            if (conditionsForCardsInFoundations(f, c)) {
                columnLists[1].movelistToFoundation(foundation.getList());
                commands.push(command);
                c = columnLists[1].getTail();
                if (c)
                    if (c->getHide())
                        c->toggleHide();
            }
        }
        else if (source == "c3") {
            card* c = columnLists[2].getTail();
            if (conditionsForCardsInFoundations(f, c)) {
                columnLists[2].movelistToFoundation(foundation.getList());
                commands.push(command);
                c = columnLists[2].getTail();
                if (c)
                    if (c->getHide())
                        c->toggleHide();
            }
        }
        else if (source == "c4") {
            card* c = columnLists[3].getTail();
            if (conditionsForCardsInFoundations(f, c)) {
                columnLists[3].movelistToFoundation(foundation.getList());
                commands.push(command);
                c = columnLists[3].getTail();
                if (c)
                    if (c->getHide())
                        c->toggleHide();
            }
        }
        else if (source == "c5") {
            card* c = columnLists[4].getTail();
            if (conditionsForCardsInFoundations(f, c)) {
                columnLists[4].movelistToFoundation(foundation.getList());
                commands.push(command);
                c = columnLists[4].getTail();
                if (c)
                    if (c->getHide())
                        c->toggleHide();
            }
        }
        else if (source == "c6") {
            card* c = columnLists[5].getTail();
            if (conditionsForCardsInFoundations(f, c)) {
                columnLists[5].movelistToFoundation(foundation.getList());
                commands.push(command);
                c = columnLists[5].getTail();
                if (c)
                    if (c->getHide())
                        c->toggleHide();
            }
        }
        else if (source == "c7") {
            card* c = columnLists[6].getTail();
            if (conditionsForCardsInFoundations(f, c)) {
                columnLists[6].movelistToFoundation(foundation.getList());
                commands.push(command);
                c = columnLists[6].getTail();
                if (c)
                    if (c->getHide())
                        c->toggleHide();
            }
        }
        else if (source == "w") {
            card* c = wastePile.top();
            if (conditionsForCardsInFoundations(f, c)) {
                wastePile.getList().movelistToFoundation(foundation.getList());
                commands.push(command);
            }
        }
    }
    void forFoundationsDestination(string source, string dest, int number) {
        if (source != "c1" && source != "c2" && source != "c3" && source != "c4" && source != "c5" && source != "c6" && source != "c7" && source != "w") {
            cout << "Source not possible" << endl;
            return;
        }
        if (number > 1) {
            cout << "Cannot move cards" << endl;
            return;
        }
        if (dest == "f1")
            movetoFoundation(f1, source);
        else if (dest == "f2")
            movetoFoundation(f2, source);
        else if (dest == "f3")
            movetoFoundation(f3, source);
        else if (dest == "f4")
            movetoFoundation(f4, source);
        else {
            cout << "Given Destination not exists" << endl;
        }
    }
    void MoveWasteToList(list<card*>& dest, int& number) {
        card* c1 = wastePile.top();
        card* c2 = dest.getTail();
        if (!conditionsForCardsInLists(c1, c2))
            cout << "Invalid Command!" << endl;
        else {
            list<card*> l = wastePile.getList();
            list<card*> ::iterator it = l.end();
            it.moveNodesToAnotherListFromFoundations(dest, l, number);
            commands.push(command);
        }
    }
    void MoveListToList(list<card*>& source, list<card*>& dest, int& number) {
        if (number > source.getSize()) {
            cout << "Invalid Command!" << endl;
            return;
        }
        else {
            list<card*> ::iterator it = source.end();
            list<card*> ::iterator it2 = source.end();
            it2--;
            for (int i = 1; i < number; i++) {
                if (!conditionsForCardsInLists(*it, *it2)) {
                    cout << "Invalid Command!" << endl;
                    return;
                }
                it--;
                it2--;
            }
            if (!conditionsForCardsInLists(*it, dest.getTail())) {
                cout << "Invalid Command" << endl;
                return;
            }
            //if conditions are true
            it.moveNodesToAnotherList(dest, source, number);
            commands.push(command);
        }

    }
    void MoveFundationToList(list<card*>& source, list<card*>& dest, int& number) {
        if (number > source.getSize()) {
            cout << "Invalid Command!" << endl;
            return;
        }
        else {
            list<card*> ::iterator it = source.end();
            list<card*> ::iterator it2 = source.end();
            it2--;
            for (int i = 1; i < number; i++) {
                if (!conditionsForCardsInLists(*it, *it2)) {
                    cout << "Invalid Command!" << endl;
                    return;
                }
                it--;
                it2--;
            }
            if (!conditionsForCardsInLists(*it, dest.getTail())) {
                cout << "Invalid Command" << endl;
                return;
            }
            //if conditions are true
            it.moveNodesToAnotherListFromFoundations(dest, source, number);
            commands.push(command);
        }
    }
    void movetoList(list<card*>& dest, string source, int number) {
        if (source == "c1")
            MoveListToList(columnLists[0], dest, number);
        else if (source == "c2")
            MoveListToList(columnLists[1], dest, number);
        else if (source == "c3")
            MoveListToList(columnLists[2], dest, number);
        else if (source == "c4")
            MoveListToList(columnLists[3], dest, number);
        else if (source == "c5")
            MoveListToList(columnLists[4], dest, number);
        else if (source == "c6")
            MoveListToList(columnLists[5], dest, number);
        else if (source == "c7")
            MoveListToList(columnLists[6], dest, number);
        else if (source == "w")// undo mai w kabhi ho ga hi nhi source
            MoveWasteToList(dest, number);
        else if (source == "f1")
            MoveFundationToList(f1.getList(), dest, number);
        else if (source == "f2")
            MoveFundationToList(f2.getList(), dest, number);
        else if (source == "f3")
            MoveFundationToList(f3.getList(), dest, number);
        else if (source == "f4")
            MoveFundationToList(f4.getList(), dest, number);
    }
    void forListsDestination(string source, string dest, int number) {
        if (source != "c1" && source != "c2" && source != "c3" && source != "c4" && source != "c5" && source != "c6" && source != "c7" && source != "w" && source != "f1" && source != "f2" && source != "f3" && source != "f4") {
            cout << "Source not possible" << endl;
            return;
        }
        if (source == "w" || source == "f1" || source == "f2" || source == "f3" || source == "f4") {
            if (number > 1) {
                cout << "Cannot move cards" << endl;
                return;
            }
        }
        if (dest == "c1")
            movetoList(columnLists[0], source, number);
        else if (dest == "c2")
            movetoList(columnLists[1], source, number);
        else if (dest == "c3")
            movetoList(columnLists[2], source, number);
        else if (dest == "c4")
            movetoList(columnLists[3], source, number);
        else if (dest == "c5")
            movetoList(columnLists[4], source, number);
        else if (dest == "c6")
            movetoList(columnLists[5], source, number);
        else if (dest == "c7")
            movetoList(columnLists[6], source, number);
    }
    //for undo functions

    void moveToWasteReverse(list<card*>& dest, string& source, int& number) {
        if (source == "c1")
            MoveFundationToWasteReverse(columnLists[0], dest, number);
        else if (source == "c2")
            MoveFundationToWasteReverse(columnLists[1], dest, number);
        else if (source == "c3")
            MoveFundationToWasteReverse(columnLists[2], dest, number);
        else if (source == "c4")
            MoveFundationToWasteReverse(columnLists[3], dest, number);
        else if (source == "c5")
            MoveFundationToWasteReverse(columnLists[4], dest, number);
        else if (source == "c6")
            MoveFundationToWasteReverse(columnLists[5], dest, number);
        else if (source == "c7")
            MoveFundationToWasteReverse(columnLists[6], dest, number);
        else if (source == "f1")
            MoveFundationToWasteReverse(f1.getList(), dest, number);
        else if (source == "f2")
            MoveFundationToWasteReverse(f2.getList(), dest, number);
        else if (source == "f3")
            MoveFundationToWasteReverse(f3.getList(), dest, number);
        else if (source == "f4")
            MoveFundationToWasteReverse(f4.getList(), dest, number);
    }
    void movetoFoundationReverse(stack<card*>& foundation, string source) {
        card* f = foundation.top();
        if (source == "c1") {
            card* c = columnLists[0].getTail();
            columnLists[0].movelistToFoundation(foundation.getList());
        }
        else if (source == "c2") {
            card* c = columnLists[1].getTail();
            columnLists[1].movelistToFoundation(foundation.getList());
        }
        else if (source == "c3") {
            card* c = columnLists[2].getTail();
            columnLists[2].movelistToFoundation(foundation.getList());
        }
        else if (source == "c4") {
            card* c = columnLists[3].getTail();
            columnLists[3].movelistToFoundation(foundation.getList());
        }
        else if (source == "c5") {
            card* c = columnLists[4].getTail();
            columnLists[4].movelistToFoundation(foundation.getList());
        }
        else if (source == "c6") {
            card* c = columnLists[5].getTail();
            columnLists[5].movelistToFoundation(foundation.getList());
        }
        else if (source == "c7") {
            card* c = columnLists[6].getTail();
            columnLists[6].movelistToFoundation(foundation.getList());
        }
    }
    void movetoListReverse(list<card*>& dest, string& source, int& number) {
        if (source == "c1")
            MoveListToListReverse(columnLists[0], dest, number);
        else if (source == "c2")
            MoveListToListReverse(columnLists[1], dest, number);
        else if (source == "c3")
            MoveListToListReverse(columnLists[2], dest, number);
        else if (source == "c4")
            MoveListToListReverse(columnLists[3], dest, number);
        else if (source == "c5")
            MoveListToListReverse(columnLists[4], dest, number);
        else if (source == "c6")
            MoveListToListReverse(columnLists[5], dest, number);
        else if (source == "c7")
            MoveListToListReverse(columnLists[6], dest, number);
        else if (source == "f1")
            MoveFoundationToListReverse(f1.getList(), dest, number);
        else if (source == "f2")
            MoveFoundationToListReverse(f2.getList(), dest, number);
        else if (source == "f3")
            MoveFoundationToListReverse(f3.getList(), dest, number);
        else if (source == "f4")
            MoveFoundationToListReverse(f4.getList(), dest, number);
    }
    void MoveListToListReverse(list<card*>& source, list<card*>& dest, int& number) {
        dest.getTail()->toggleHide();
        list<card*> ::iterator it = source.end();

        for (int i = 1; i < number; i++)
            it--;

        it.moveNodesToAnotherListFromFoundations(dest, source, number);
    }
    void MoveFundationToWasteReverse(list<card*>& source, list<card*>& dest, int& number) {
        list<card*> ::iterator it = source.end();

        for (int i = 1; i < number; i++)
            it--;

        it.moveNodesToAnotherListFromFoundations(dest, source, number);
    }
    void MoveFoundationToListReverse(list<card*>& source, list<card*>& dest, int& number) {
        list<card*> ::iterator it = source.end();

        for (int i = 1; i < number; i++)
            it--;

        it.moveNodesToAnotherListFromFoundationsReverse(dest, source, number);
    }
    void forListsDestinationReverse(string source, string dest, int number) {
        if (dest == "c1")
            movetoListReverse(columnLists[0], source, number);
        else if (dest == "c2")
            movetoListReverse(columnLists[1], source, number);
        else if (dest == "c3")
            movetoListReverse(columnLists[2], source, number);
        else if (dest == "c4")
            movetoListReverse(columnLists[3], source, number);
        else if (dest == "c5")
            movetoListReverse(columnLists[4], source, number);
        else if (dest == "c6")
            movetoListReverse(columnLists[5], source, number);
        else if (dest == "c7")
            movetoListReverse(columnLists[6], source, number);
    }
    void forFoundationsDestinationReverse(string& source, string& dest, int& number) {
        if (dest == "f1")
            movetoFoundationReverse(f1, source);
        else if (dest == "f2")
            movetoFoundationReverse(f2, source);
        else if (dest == "f3")
            movetoFoundationReverse(f3, source);
        else if (dest == "f4")
            movetoFoundationReverse(f4, source);
    }

    void runUndoCommand() {
        if (commands.isEmpty())
            cout << "No Undo Possible!" << endl;
        else
        if (commands.top() == "s" && wastePile.size() > 0) {
            wastePile.getList().movelistToFoundation(stackPile.getList());
            commands.pop();
        }
        else if (commands.top() == "s" && wastePile.size() == 0) {
            for (int i = 23; i >= 0; i--)
                stackPile.getList().movelistToFoundation(wastePile.getList());
            commands.pop();
        }
        else {
            stringstream ss(commands.top());
            char action;
            string source, dest;
            int number;
            ss >> action >> dest >> source >> number;

            if (dest[0] == 'f')
                forFoundationsDestinationReverse(source, dest, number);
            else if (dest[0] == 'c')
                forListsDestinationReverse(source, dest, number);
            else
                moveToWasteReverse(wastePile.getList(), source, number);
            commands.pop();
        }
    }
    void runCommand() {
        if (command == "s" && stackPile.size() > 0) {
            stackPile.getList().movelistToFoundation(wastePile.getList());
            commands.push(command);
        }
        else if (command == "s" && stackPile.size() == 0) {
            for (int i = 23; i >= 0; i--) {
                wastePile.getList().movelistToFoundation(stackPile.getList());
                commands.push(command);
            }
        }
        else if (command == "z") {
            runUndoCommand();
        }
        else if (command == "quit")
            return;
        else {

            stringstream ss(command);
            char action;
            string source, dest;
            int number;

            ss >> action >> source >> dest >> number;

            if (action == 'm') {
                if (source == dest) {
                    cout << "Invalid Command! Source and Destination must be different." << endl;
                    return;
                }
                if (dest[0] == 'f')
                    forFoundationsDestination(source, dest, number);
                else if (dest[0] == 'c')
                    forListsDestination(source, dest, number);
                else
                    cout << "Invalid Destination!" << dest << endl;
            }
            else
                cout << action << " Invalid Command!" << endl;
        }
    }
    void startGame() {
        initialization();
        while (f1.size()<13 || f2.size()<13 || f3.size() < 13 || f4.size() < 13 ) {
            display();
            input();
            system("cls");
            runCommand();
            if (command == "quit")
                return;
        }
        cout << "Game Over!" << endl;
    }
};

/////////////////////////////////////// main function ///////////////////////////////////////
int main()
{
    solitaire game;
    game.startGame();

    return 0;
}