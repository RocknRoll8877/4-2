    #include <iostream>
    #include <string>
    #include <exception>

    template <class T>
    class Ring;

    class Student {
        int id;
        std::string name;
        double rate;
    public:
        Student() 
        : id(rand() % 42), name("Noname"), rate(0.0) { }
        Student(const int id, const std::string student_name, const double student_rate = 0.0)
        : id(id), name(student_name), rate(student_rate)
        {
            if (rate < 0.0)
                rate = 0.0;
            else if (rate > 10.0)
                rate = 10.0;
        }
        ~Student() { }
        bool operator< (const Student& stud) { return id < stud.id; }
        bool operator== (const Student& stud) { return id == stud.id && name == stud.name; }
        friend std::ostream& operator<< (std::ostream& out,  const Student& student) 
            { out << student.id << ' ' << student.name << ' '<< student.rate << '\n'; return out;}
    };



    class Exception 
    {
    public:
        Exception(const std::string errorMessage = "Something wrong") : m_ErrorMessage(errorMessage) { }
        friend std::ostream& operator<< (std::ostream& out, const Exception& ex) {out << ex.m_ErrorMessage << '\n'; return out;}
    private:
        std::string m_ErrorMessage;
    };

    template <class T>
    struct Node {
            T value;
            Node* left, *right;
            Node() : value(), left(nullptr), right(nullptr) { }
        };

    template <class T>
    class Iterator
    {
    public:
        friend class Ring<T>;
        Iterator(Node<T>* node = nullptr) : node(node) { }
        ~Iterator() { }
        Iterator(const Iterator& it) { node = it.node; }
        inline Iterator& operator=(const Iterator& it) { node = it.node; return *this;}
        inline T& operator*() { if (node == nullptr) throw Exception("Empty iterator"); return node->value; }
        inline T* operator->() { if (node == nullptr) throw Exception("Empty iterator"); return &node->value; }
        inline Iterator& operator++() { if (node == nullptr) throw Exception("Empty iterator"); node = node->right; return *this;}
        inline Iterator& operator--() { if (node == nullptr) throw Exception("Empty iterator"); node = node->left; return *this;}
        inline Iterator operator++(int unused) { if (node == nullptr) throw Exception("Empty iterator"); Iterator res = *this; node = node->right; return res;}
        inline Iterator operator--(int unused) { if (node == nullptr) throw Exception("Empty iterator"); Iterator res = *this; node = node->left; return res;}
        inline bool operator== (const Iterator& it) { return node == it.node; }
        inline bool operator!= (const Iterator& it) { return node != it.node; }
    private:
        Node<T>* node;
    };

    template<class T>
    class Ring
    {
    public:
        typedef ::Iterator<T> Iterator;
    public:
        Ring();
        ~Ring();
        void push_front(T value, bool unique_mode = false);
        void push_back(T value, bool unique_mode = false);
        Iterator insert(Iterator, T value, bool unique_mode = false);
        void pop_front();
        void pop_back();
        Iterator erase(Iterator);
        Iterator find(T value) const; 
        void show() const;
        Iterator begin() { return Iterator(head); }
        Iterator null() { return Iterator(); }
    private:
        Node<T> *head;
    };

    
    template<class T>
    Ring<T>::Ring() 
        : head(nullptr)
    {
    }

    template<class T>
    Ring<T>::~Ring() 
    {
        Node<T>* it = head;
        head->left->right = nullptr;
        while (it != nullptr) {
            Node<T>* temp = it;
            it = it->right;
            delete temp;
        }
    }

    template<class T>
    void Ring<T>::push_front(T value, bool unique_mode) 
    {
        if (unique_mode) {
            Iterator it = find(value);
            if (it != null()) {
                throw Exception("duplicate value find");
            }   
        }
    
        Node<T>* newNode = new Node<T>();
        newNode->value = value;
        newNode->right = head;
        if (head != nullptr) {
            newNode->left = head->left;
            head->left->right = newNode;
            head->left = newNode;  
        }
        else {
            newNode->left = newNode;
            newNode->right = newNode;
        }
        head = newNode;
    }

    template<class T>
    void Ring<T>::push_back(T value, bool unique_mode) 
    {
        if (unique_mode) {
            Iterator it = find(value);
            if (it != null()) {
                throw Exception("duplicate value find");
            }   
        }  
        Node<T>* newNode = new Node<T>();
        newNode->value = value;
        newNode->right = head;
        if (head != nullptr) {
            newNode->left = head->left;
            head->left->right = newNode;
            head->left = newNode;   
        }
        else {
            newNode->right = newNode;
            newNode->left = newNode;
            head = newNode;
        }
    }

    template<class T>
    typename Ring<T>::Iterator Ring<T>::insert(Ring<T>::Iterator it, T value, bool unique_mode) 
    {
        if (unique_mode) {
            Iterator it = find(value);
            if (it != null()) {
                throw Exception("duplicate value find");
            }   
        } 
        Node<T>* newNode = new Node<T>();
        newNode->left = it.node;
        newNode->right = it.node->right;
        it.node->right = newNode;
        it = Ring<T>::Iterator(newNode);
        return it;
    }

    template<class T>
    void Ring<T>::pop_front() 
    {
        if (head == nullptr)
            throw Exception("Ring is empty");
        Node<T>* temp = head;
        if (head->right != head->left) {
            head->right->left = head->left;
            head->left->right = head->right;
            head = head->right;
        }
        else {
            head = nullptr;
        }
        delete temp;
    }

    template<class T>
    void Ring<T>::pop_back() 
    {
        if (head == nullptr)
            throw Exception("Ring is empty");
        Node<T>* temp = head->left;
        if (head->right != head->left) {
            temp->right->left = temp->left;
            temp->left->right = temp->right;
        }
        else {
            head = nullptr;
        }
        delete temp;
    }

    template<class T>
    typename Ring<T>::Iterator Ring<T>::erase(Ring<T>::Iterator it) 
    {
        if (head == nullptr)
            throw Exception("Ring is empty");
        if (it == null())
            throw Exception("Invalid iterator");
        Node<T>* temp = it.node;
        if (it.node->right != it.node->left) {
            it.node->right->left = it.node->left;
            it.node->left->right = it.node->right;
            ++it;
        }
        else {
            head = nullptr;
            it = Iterator();
        }
        delete temp;
        return it;
    }

    template<class T>
    typename Ring<T>::Iterator Ring<T>::find(T value) const
    {
        Node<T>* it = head;
        do
        {
            if (it->value == value)
                return Iterator(it);
            it = it->right;
        } while (it != head);
        return Iterator();
    }

    template<class T>
    void Ring<T>::show() const
    {
        Node<T>* it = head;
        do
        {
            std::cout << ' ' << it->value;
            it = it->right;
        } while (it != head);
        std::cout << '\n';
    }

    template<class T>
    void bubbleSort(Iterator<T> begin) {
        if (begin == Iterator<T>())
            throw Exception("Invalid iterator");
        Iterator<T> it = begin;
        Iterator<T> end = --begin;
        ++begin;
        while (begin != end) {
            it = begin;
            while (it != end) {
                Iterator<T> prev = it++;
                if (*it < *prev) {
                    T temp = *prev;
                    *prev = *it;
                    *it = temp;
                }
            }
            --end;
        }
    }

    void unexpected_handler() 
    {
        std::cerr << "Unexpected exception\n";
        std::terminate();
    }

    void terminate_handler()
    {
        std::cerr << "terminate called\n";
        abort();
    }
    int main() {
        srand(time(NULL));
        std::set_terminate(terminate_handler);
        std::set_unexpected(unexpected_handler);
        Ring<Student> ring;
        try {
            ring.pop_front();
        } catch(Exception ex) {
            std::cerr << ex;
        }
        std::cout << "Inserting\n";
        try {
            ring.push_front(Student(10, "a"));
            ring.show();
            ring.push_front(Student(0 ,"b"));
            ring.show();
            ring.push_back(Student(11, "c"));
            ring.show();
            ring.push_back(Student(5, "d"));
            ring.show();
            ring.push_back(Student(2, "e"));
            ring.show();
            ring.push_front(Student(1, "f"));
        } catch (Exception ex) {
            std::cerr << ex;
        }
    
        std::cout << "Before sort\n";
        ring.show();
        std::cout << "After sort\n";
        try {
            bubbleSort(ring.begin());
        } catch (Exception ex) {
            std::cerr << ex;
        }
        ring.show();
        std::cout << '\n';
        std::cout << "Popping\n";
        try {
            ring.pop_back();
            ring.show();
            ring.pop_front();
            ring.show();
            ring.erase(ring.find(Student(2, "c")));
            ring.show();
        } catch (Exception ex) {
            std::cerr << ex;
        }
        
        std::cout << '\n';

        std::cout << "Setting same value\n";
        try {
            ring.push_front(Student(5, "d"), true);
        } catch (Exception ex) {
            std::cerr << ex;
        } 
        ring.show();
        return 0;
    }
