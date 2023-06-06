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
    };
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
    };
    void insertAt(int index, T element);
    void deleteFront();
    void deleteRear();
    void deleteAt(int index);
    T getElementAt(int index);
    void getSize();
};