template <typename T> class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node *prev;
        Node *next;
    };
    Node *head;
    typedef Node* NODE;
public:
    DoublyLinkedList(){
        first = nullptr;
    }
    void insertFront(T element){
        NODE ele = new Node;
        Node 
        if(ele == nullptr){
            std::cout << "ERROR: Doubly Linked List Memory not allocated" << std::endl;
            exit(-1);
        }
        ele->data = element;
        if(first == nullptr){
            first = ele;
            ele->prev = ele;
            ele->next = ele;
        } else{
            ele->next = first;
            ele->prev = first->prev;
            first->prev = ele;
            first = ele;
        }
    }
    void insertRear(T element){
        NODE ele = new Node;
        if(ele == nullptr){
            std::cout << "ERROR: Doubly Linked List Memory not allocated" << std::endl;
            exit(-1);
        }
        ele->data = element;
        if(first == nullptr){
            first = ele;
            ele->prev = nullptr;
            ele->next = nullptr;
        } else{
            NODE temp = first->prev;
            ele->next = first;
            first->prev = ele;
            ele->prev = temp;
            temp->next = ele;
        }
    }
    void insertAt(int index, T element){
        NODE ele = new Node;
        if(ele == nullptr){
            std::cout << "ERROR: Doubly Linked List Memory not allocated" << std::endl;
            exit(-1);
        }
        ele->data = element;
        if(first == nullptr){
            first = ele;
            ele->prev = nullptr;
            ele->next = nullptr;
        } else{
            NODE temp = first;
            for(int i = 0; i < index; i++){
                temp = temp->next;
            }
            ele->next = temp;
            ele->prev = temp->prev;
            temp->prev->next = ele;
            temp->prev = ele;
        }
    }
    void deleteFront(){
        if(first == nullptr){
            std::cout << "ERROR: Doubly Linked List is empty" << std::endl;
            exit(-1);
        } else{
            NODE temp = first;
            first = first->next;
            first->prev = temp->prev;
            temp->prev->next = first;
            delete temp;
        }
    }
    void deleteRear(){
        if(first == nullptr){
            std::cout << "ERROR: Doubly Linked List is empty" << std::endl;
            exit(-1);
        } else{
            NODE temp = first->prev;
            first->prev = temp->prev;
            temp->prev->next = first;
            delete temp;
        }
    }
    void deleteAt(int index){
        if(first == nullptr){
            std::cout << "ERROR: Doubly Linked List is empty" << std::endl;
            exit(-1);
        }else if(getSize() <= index){
            std::cout << "ERROR: Index out of bounds" << std::endl;
            exit(-1);
        } else{
            NODE temp = first;
            for(int i = 0; i < index; i++){
                temp = temp->next;
            }
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            delete temp;
        }
    }
    T getElementAt(int index){
        if(first == nullptr){
            std::cout << "ERROR: Doubly Linked List is empty" << std::endl;
            exit(-1);
        } else{
            NODE temp = first;
            for(int i = 0; i < index; i++){
                temp = temp->next;
            }
            return temp->data;
        }
    }   
    int getSize(){
        if(first == nullptr){
            return 0;
        } else{
            NODE temp = first;
            int count = 0;
            while(temp->next != first){
                count++;
                temp = temp->next;
            }
            return count;
        }
    }
};