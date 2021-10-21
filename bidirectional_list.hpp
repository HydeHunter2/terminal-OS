#pragma once

#include <initializer_list>
#include <vector>

template<typename T>
class BidirectionalList {
  public:
    struct Node {
      public:
        friend class BidirectionalList<T>;
        Node() = delete;

        T value;

      private:
        Node* _prev{nullptr};
        Node* _next{nullptr};

        Node(T value, Node* prev, Node* next) : value(std::move(value)),
                                                _prev(prev),
                                                _next(next) {}
    };

    BidirectionalList() = default;
    ~BidirectionalList() {
        reset();
    }

    BidirectionalList(const std::initializer_list<T>& initList) {
        reset();
        for (const auto& value : initList) {
            pushBack(value);
        }
    }

    BidirectionalList& operator=(const BidirectionalList<T>& list) {
        if (this != &list) {
            reset();
            for (const auto node : list) {
                pushBack(node->value);
            }
        }
        return *this;
    }
    BidirectionalList& operator=(BidirectionalList<T>&& list) noexcept {
        if (this != &list) {
            reset();
            _front = std::move(list._front);
            _back = std::move(list._back);
            _size = list._size;
            list._front = nullptr;
            list._back = nullptr;
            list._size = 0;
        }
        return *this;
    }
    BidirectionalList(const BidirectionalList<T>& list) {
        *this = list;
    }
    BidirectionalList(BidirectionalList<T>&& list) noexcept {
        *this = std::move(list);
    }

    size_t size() const {
        return _size;
    }
    bool isEmpty() const {
        return (_size == 0);
    }
    std::vector<T> toVector() const {
        std::vector<T> out;
        out.reserve(_size);
        for (const auto node : *this) {
            out.push_back(node->value);
        }
        return out;
    }

    Node* front() {
        return _front;
    }
    Node* back() {
        return _back;
    }
    const Node* front() const {
        return _front;
    }
    const Node* back() const {
        return _back;
    }

    void pushFront(T value) {
        Node* newFrontNode = new Node(std::move(value), nullptr, _front);
        if (isEmpty()) {
            _back = newFrontNode;
            _front = newFrontNode;
        } else {
            _front = newFrontNode;
            syncNeighboringNodes(newFrontNode);
        }
        ++_size;
    }
    void pushBack(T value) {
        Node* newBackNode = new Node(std::move(value), _back, nullptr);
        if (isEmpty()) {
            _back = newBackNode;
            _front = newBackNode;
        } else {
            _back = newBackNode;
            syncNeighboringNodes(newBackNode);
        }
        ++_size;
    }
    void popFront() {
        erase(_front);
    }
    void popBack() {
        erase(_back);
    }

    void insertBefore(Node* element, T value) {
        Node* nodeToInsert = new Node(std::move(value), element->_prev, element);
        syncNeighboringNodes(nodeToInsert);
        ++_size;
    }
    void insertAfter(Node* element, T value) {
        Node* nodeToInsert = new Node(std::move(value), element, element->_next);
        syncNeighboringNodes(nodeToInsert);
        ++_size;
    }

    void moveToBegin(Node* element) {
        if (_size > 1) {
            extract(element);
            _front->_prev = element;
            element->_next = _front;
            _front = element;
        } 
    }

    void extract(Node* element) {
        if (element == _front && element == _back) {
            _front = nullptr;
            _back = nullptr;
        } else if (element == _front) {
            _front = element->_next;
            _front->_prev = nullptr;
        } else if (element == _back) {
            _back = element->_prev;
            _back->_next = nullptr;
        } else {
            element->_prev->_next = element->_next;
            element->_next->_prev = element->_prev;
        }
    }
    void erase(Node* element) {
        if (element == nullptr) {
            return;
        }

        extract(element);

        delete element;
        --_size;
    }

    void reset() {
        Node* it = _front;
        while (_size > 0) {
            Node* tmp = it->_next;
            delete it;
            it = tmp;
            --_size;
        }
        _front = nullptr;
        _back = nullptr;
    }

    template<typename K>
    Node* find(const K& value) const {
        return *std::find_if(begin(), end(), [value] (const Node* node) {
            return (node->value == value);
        });
    }

    Node* operator[](size_t i) {
        if (i < 0 || i >= _size) {
            return nullptr;
        }
        return *std::next(begin(), i);
    }
    const Node* operator[](size_t i) const {
        if (i < 0 || i >= _size) {
            return nullptr;
        }
        return *std::next(begin(), i);
    }

    bool operator==(const BidirectionalList<T>& list) const {
        if (_size != list._size) {
            return false;
        }

        return std::equal(begin(), end(),
                          list.begin(),
                          [] (const Node* n1, const Node* n2) {
                              return (n1->value == n2->value);
                          });
    }
    bool operator!=(const BidirectionalList<T>& list) const {
        return !(*this == list);
    }

    /// A wrapper over 'Node' so that foreach and standard functions can be used.
    /// (e.g. std::equal, std::next)
    class NodeIterator : public std::iterator<std::input_iterator_tag, Node> {
      public:
        explicit NodeIterator(Node* node) : _node(node) {}

        NodeIterator& operator++() {
            _node = _node->_next;
            return *this;
        }
        NodeIterator& operator--() {
            _node = _node->_prev;
            return *this;
        }
        Node* operator*() const {
            return _node;
        }
        bool operator==(const NodeIterator& nodeIter) const {
            return (_node == nodeIter._node);
        }
        bool operator!=(const NodeIterator& nodeIter) const {
            return (_node != nodeIter._node);
        }

      private:
        Node* _node{ nullptr };
    };
    NodeIterator begin() const {
        return NodeIterator(_front);
    }
    NodeIterator end() const {
        return NodeIterator(nullptr);
    }

  private:
    size_t _size{ 0 };
    Node* _front{ nullptr };
    Node* _back{ nullptr };

    /// makes 'node' synchronized with its neighbors (it means node is now in
    /// the middle between the neighbors).
    /// 'prev_' and 'next_' of the corresponding neighbors points to 'node'.
    void syncNeighboringNodes(Node* node) {
        if (node->_prev) {
            node->_prev->_next = node;
        } else {
            _front = node;
        }
        if (node->_next) {
            node->_next->_prev = node;
        } else {
            _back = node;
        }
    }
};
