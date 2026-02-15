// NOTE: Per assignment rules, only this single include is allowed here.
#include "VectorStore.h"

// =====================================
// Helper functions
// =====================================

// Helper function to print n spaces for tree visualization
void printNSpace(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << " ";
    }
}

/*
cây AVL được dùng làm chỉ mục chính sắp xếp theo khoảng cách Euclidean đến một reference vector, 
trong khi RBT đóng vai trò là chỉ mục phụ (norm index) sắp xếp theo chuẩn (norm) vector 
để tối ưu hóa quá trình lọc ứng viên khi tìm kiếm láng giềng gần nhất (top-k)
*/

// Độ phức tạp của các thao tác chèn/xóa/tìm kiếm trong AVL là O(log n)
// =====================================
// AVLTree<K, T> implementation
// =====================================
// AVL Tree có 6 thuộc tính: key, data, pLeft, pRight, balance, và 1 thuộc tính được khai báo thêm height
template <class K, class T>
void AVLTree<K, T>::printTreeStructure() const{
    // Print the tree structure level by level (BFS)
    // Calculate tree height for spacing
    int height = this->getHeight();
    if (this->root == NULL)
    {
        cout << "NULL\n";
        return;
    }
    // Queue for BFS, start from root
    queue<AVLNode *> q;
    q.push(root);
    AVLNode *temp;

    int count = 0;          // count nodes printed at current level
    int maxNode = 1;        // max nodes at current level
    int level = 0;          // current level
    int space = pow(2, height); // initial space based on height (khoảng cách in cho đẹp)
    printNSpace(space / 2);

    // vòng lặp BFS: Mỗi pần lấy 1 ptu ra để in và thêm con của nó vào hàng đợi
    while (!q.empty())
    {
        temp = q.front();
        q.pop();
        if (temp == NULL)
        {
            cout << " ";
            q.push(NULL);
            q.push(NULL);
        }
        else
        {
            cout << temp->data;
            q.push(temp->pLeft);
            q.push(temp->pRight);
        }
        printNSpace(space);
        count++;
        if (count == maxNode)
        {
            cout << endl;
            count = 0;
            maxNode *= 2;       // double max nodes for next level
            level++;
            space /= 2;         // halve space for next level (càng xuống dưới node càng gần nhau)
            printNSpace(space / 2);
        }
        // dừng để tranh in vô hạn
        if (level == height)
            return;
    }
    // O(n)
}

// TODO: Implement all AVLTree<K, T> methods here
template <class K, class T> 
AVLTree<K, T>::AVLTree() : root(nullptr) {}

template <class K, class T>
AVLTree<K, T>::~AVLTree() { clear(); }

template <class K, class T>
void AVLTree<K, T>::clearHelper(typename AVLTree<K,T>::AVLNode* node) {
    // đệ quy xoá tất cả các node trong các cây con tại node
    if (!node) return;
    clearHelper(node->pLeft);
    clearHelper(node->pRight);
    delete node;
    // O(n)
}

template <class K, class T>
void AVLTree<K, T>::clear() {
    // xoá toàn bộ cây AVL
    clearHelper(root);
    root = nullptr;
    // O(n)
}

// kiểm tra cây có rỗng không
template <class K, class T>
bool AVLTree<K, T>::empty() const { return root == nullptr; }
// O(1)

// =========================
// Height helpers
// =========================
template<class K, class T>
int AVLTree<K,T>::h(AVLNode* n) {
    // lấy chiều cao của node n
    return n ? n->height : 0;
}

template<class K, class T>
void AVLTree<K,T>::pull(AVLNode* n) {
    // cập nhật chiều cao của node n dựa trên chiều cao của con trái và con phải
    if (!n) return;
    int hl = h(n->pLeft), hr = h(n->pRight);
    n->height = (hl > hr ? hl : hr) + 1; // chiều cao node n = max(chiều cao con trái, chiều cao con phải) + 1
}

template<class K, class T>
int AVLTree<K,T>::dfsHeight(AVLNode* n) {
    // đệ quy tính chiều cao thực sự của cây con gốc tại node n
    if (!n) return 0;
    int hl = dfsHeight(n->pLeft), hr = dfsHeight(n->pRight);
    return (hl > hr ? hl : hr) + 1;
}

// trả về chiều cao của toàn bộ cây AVL
template<class K,class T>
int AVLTree<K,T>::getHeight() const { return dfsHeight(root); }

template<class K, class T>
int AVLTree<K,T>::__avlCount(AVLNode* n) {
    // đếm tổng số node tại node n (= tổng node cây con trái + tổng node cây con phải + node đó)
    if (!n) return 0; // nếu cây rỗng thì trả về 0
    return 1 + __avlCount(n->pLeft) + __avlCount(n->pRight);
}

template <class K, class T>
int AVLTree<K, T>::getSize() const {
    return __avlCount(root); // kích thước tại gốc (tổng số node của cây)
}

// =========================
// Rotations (return new root of the rotated subtree)
// =========================
// Thực hiện xoáy phải tại node cần cân bằng khi cây con trái cao hơn cây con phải quá 1 node
template <class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K, T>::rotateRight(AVLNode*& z) {
    // z là node cần được xoay phải
    AVLNode* y = z->pLeft;
    AVLNode* T3 = y ? y->pRight : nullptr;
    y->pRight = z;
    z->pLeft = T3;
    // update heights: child first, then parent
    pull(z);
    pull(y);
    return y; // node mới làm gốc
    // O(1)
}

// xoay trái tại node cần đụoc cân bằng khi cây con phải cao hơn cây con trái quá 1 node
template <class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K, T>::rotateLeft(AVLNode*& x) {
    AVLNode* y = x->pRight;
    AVLNode* T2 = y ? y->pLeft : nullptr;
    y->pLeft = x;
    x->pRight = T2;
    pull(x);
    pull(y);
    return y;
    // O(1)
}

// =========================
// contains (BST search)
// =========================
template <class K, class T>
bool AVLTree<K, T>::contains(const K& key) const {
    // kiểm tra xem khóa key có tồn tại trong cây AVL không theo quy tắc BST
    AVLNode* cur = root;
    while (cur) {
        if (key < cur->key) cur = cur->pLeft;
        else if (cur->key < key) cur = cur->pRight;
        else return true;
    }
    return false;
    // O(log n)
}

// =========================
// Insertion (BST + AVL rebalancing)
// =========================
template<class K, class T>
int AVLTree<K,T>::__avlBalance(AVLNode* n) {
    // tính hệ số cân bằng (balance factor) của node n
    return n ? h(n->pLeft) - h(n->pRight) : 0;
}

// ---- Insert (BST + rebalancing) ----
template <class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K, T>::insertRec(AVLNode* node, const K& key, const T& value) {
    // đệ quy chèn key, value vào cây AVL tại node
    using Node = typename AVLTree<K,T>::AVLNode;

    if (!node) return new Node(key, value);
    if (key < node->key)      node->pLeft  = insertRec(node->pLeft,  key, value);
    else if (node->key < key) node->pRight = insertRec(node->pRight, key, value);
    else return node; // duplicate key, do nothing

    // update cached height for this node
    pull(node);

    int bf = __avlBalance(node); // balance factor
    // LL Case
    if (bf > 1 && key < node->pLeft->key)
        return rotateRight(node);

    // RR Case
    if (bf < -1 && key > node->pRight->key)
        return rotateLeft(node);

    // LR Case
    if (bf > 1 && key > node->pLeft->key) {
        node->pLeft = rotateLeft(node->pLeft);
        return rotateRight(node);
    }

    // RL Case
    if (bf < -1 && key < node->pRight->key) {
        node->pRight = rotateRight(node->pRight);
        return rotateLeft(node);
    }

    return node;
}

template <class K, class T>
void AVLTree<K, T>::insert(const K& key, const T& value) {
    // chèn 1 node vào cây AVL
    root = insertRec(root, key, value);
    // O(log n)
}

// =========================
// inorderTraversal (O(n))
// =========================
template <class K, class T>
void AVLTree<K, T>::inorderTraversal(void (*action)(const T&)) const {
    // duyệt trung thứ tự (in-order) và thực hiện hàm action trên mỗi DATA của node
    struct Local {
        // dùng struct local để định nghĩa hàm trong hàm gián tiếp
        static void dfs(typename AVLTree<K,T>::AVLNode* n, void (*act)(const T&)) {
            if (!n) return;
            dfs(n->pLeft, act);
            act(n->data);
            dfs(n->pRight, act);
        }
    };
    Local::dfs(root, action);
}

// =========================
// remove helpers (O(log n))
// =========================
template <class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::minValueNode(AVLNode* node) {
    AVLNode* cur = node;
    while (cur && cur->pLeft) cur = cur->pLeft;
    return cur;
}

template <class K, class T>
typename AVLTree<K,T>::AVLNode* AVLTree<K,T>::removeRec(AVLNode* node, const K& key) {
    if (!node) return nullptr;

    if (key < node->key) {
        node->pLeft = removeRec(node->pLeft, key);
    } else if (node->key < key) {
        node->pRight = removeRec(node->pRight, key);
    } else {
        // node to delete found
        if (!node->pLeft || !node->pRight) {
            AVLNode* child = node->pLeft ? node->pLeft : node->pRight;
            delete node;
            return child; // may be nullptr
        } else {
            // two children: replace with inorder successor
            AVLNode* succ = minValueNode(node->pRight);
            node->key  = succ->key;
            node->data = succ->data;
            node->pRight = removeRec(node->pRight, succ->key);
        }
    }

    // update cached height
    pull(node);

    // rebalance (standard AVL delete cases)
    int bf = h(node->pLeft) - h(node->pRight);

    // Left heavy
    if (bf > 1) {
        int bfL = h(node->pLeft->pLeft) - h(node->pLeft->pRight);
        if (bfL >= 0) {
            return rotateRight(node);             // LL
        } else {
            node->pLeft = rotateLeft(node->pLeft);// LR
            return rotateRight(node);
        }
    }

    // Right heavy
    if (bf < -1) {
        int bfR = h(node->pRight->pLeft) - h(node->pRight->pRight);
        if (bfR <= 0) {
            return rotateLeft(node);              // RR
        } else {
            node->pRight = rotateRight(node->pRight); // RL
            return rotateLeft(node);
        }
    }

    return node;
}

template <class K, class T>
void AVLTree<K, T>::remove(const K& key) {
    // xóa 1 gtri khỏi AVL theo khoá key
    // Chọn node có khóa bé nhất của cây con phải nếu node có 2 con để thay thế
    root = removeRec(root, key);
}

// =====================================
// RedBlackTree<K, T> implementation
// =====================================

template <class K, class T>
void RedBlackTree<K, T>::printTreeStructure() const
{
    if (this->root == nullptr){
        cout << "NULL\n";
        return;
    }

    queue<RBTNode *> q;
    q.push(root);
    RBTNode *temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;

    // Calculate tree height for spacing
    int height = 0;
    queue<RBTNode *> heightQ;
    heightQ.push(root);
    while (!heightQ.empty()){
        int size = heightQ.size();
        height++;
        for (int i = 0; i < size; i++){
            RBTNode *node = heightQ.front();
            heightQ.pop();
            if (node->left)
                heightQ.push(node->left);
            if (node->right)
                heightQ.push(node->right);
        }
    }

    int space = pow(2, height);
    printNSpace(space / 2);

    while (!q.empty()){
        temp = q.front();
        q.pop();

        if (temp == nullptr){
            cout << "NULL";
            q.push(nullptr);
            q.push(nullptr);
        }
        else {
            // Print node data and color as tuple: (data, color)
            cout << "(" << temp->data << ",";
            if (temp->color == RED) {
                cout << "R)";
            }
            else {
                cout << "B)";
            }

            q.push(temp->left);
            q.push(temp->right);
        }

        printNSpace(space);
        count++;

        if (count == maxNode){
            cout << endl;
            count = 0;
            maxNode *= 2;
            level++;
            space /= 2;
            printNSpace(space / 2);
        }

        if (level == height){
            return;
        }
    }
}

// TODO: Implement all other RedBlackTree<K, T> methods here
// =====================================
// Core RedBlackTree<K, T> methods
// =====================================
// các phép search và thao tác cập nhật sau khi cân bằng có độ phức tạp O(log n)
// Đầu tiên là khởi nút RBTNode mới với key, data, right = left = parent = nullptr, color = RED (do là node mới chèn vào)
template <class K, class T>
RedBlackTree<K, T>::RBTNode::RBTNode(const K &k, const T &v)
    : key(k), data(v), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}

template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToRed() { this->color = RED; }

template <class K, class T>
void RedBlackTree<K, T>::RBTNode::recolorToBlack() { this->color = BLACK; }

template <class K, class T>
RedBlackTree<K, T>::RedBlackTree() : root(nullptr) {}

template <class K, class T>
RedBlackTree<K, T>::~RedBlackTree() { this->clear(); }
// giải phóng toàn bộ bộ nhớ đã cấp phát cho nút của cây RBT

template <class K, class T>
bool RedBlackTree<K, T>::empty() const {
    // kiểm tra cây RBT có rỗng không
    return this->root == nullptr;
}

template <class K, class T>
int RedBlackTree<K, T>::size() const {
    // trả về số lượng nút trong cây RBT bằng BFS
    if (!this->root) return 0;
    int n = 0;
    queue<RBTNode*> q;
    q.push(this->root);
    while (!q.empty()) {
        RBTNode* x = q.front(); q.pop();
        ++n;
        if (x->left)  q.push(x->left);
        if (x->right) q.push(x->right);
    }
    return n;
}

template <class K, class T>
void RedBlackTree<K, T>::clear() {
    // xóa toàn bộ cây RBT bằng BFS, trả root về nullptr
    if (!this->root) return;
    queue<RBTNode*> q;
    q.push(this->root);
    while (!q.empty()) {
        RBTNode* x = q.front(); q.pop();
        if (x->left)  q.push(x->left);
        if (x->right) q.push(x->right);
        delete x;
    }
    this->root = nullptr;
    // O(n)
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode* RedBlackTree<K, T>::find(const K &key) const {
    // tìm và trả về con trỏ tới nút đầu tiên có chứa key trong cây RBT
    RBTNode* cur = this->root;
    while (cur) {
        if (key < cur->key)      cur = cur->left;
        else if (cur->key < key) cur = cur->right;
        else                     return cur; // found
    }
    return nullptr;
    // O(log n)
}

template <class K, class T>
bool RedBlackTree<K, T>::contains(const K &key) const {
    // kiểm tra sự tồn tại của key trong cây RBT không
    return this->find(key) != nullptr;
}

// =====================================
// Rotations (protected helpers): rotateLeft đối xứng vs rotateRight
// =====================================
template <class K, class T>
void RedBlackTree<K, T>::rotateLeft(RBTNode* x) {
    if (!x) return;
    RBTNode* y = x->right;
    if (!y) return; // nothing to rotate

    // move y's left subtree to be x's right subtree
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }

    // link y up to x's parent (may update root)
    y->parent = x->parent;
    if (!x->parent) {
        this->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    // put x on y's left
    y->left = x;
    x->parent = y;
}

template <class K, class T>
void RedBlackTree<K, T>::rotateRight(RBTNode* x) {
    if (!x) return;
    RBTNode* y = x->left;
    if (!y) return; // nothing to rotate

    // move y's right subtree to be x's left subtree
    x->left = y->right;
    if (y->right) {
        y->right->parent = x;
    }

    // link y up to x's parent (may update root)
    y->parent = x->parent;
    if (!x->parent) {
        this->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    // put x on y's right
    y->right = x;
    x->parent = y;
}

// =====================================
// Insert + fix-up (cases 1Ã¢â‚¬â€œ4)  Ã¢â‚¬â€ with duplicate key = NO-OP
// =====================================
template <class K, class T>
void RedBlackTree<K, T>::insert(const K& key, const T& value) {
    // chèn cặp (key, value) vào cây RBT với quy tắc không cho phép trùng key
    // Nếu key đã tồn tại thì không làm gì cả
    using Node = RBTNode;

    // --- Empty tree => new black root (Case 1) ---
    if (!this->root) {
        this->root = new Node(key, value);
        this->root->color = BLACK;
        return;
    }

    // --- Standard BST insert ---
    Node* p = nullptr;
    Node* cur = this->root;
    while (cur) {
        p = cur;
        if (key < cur->key) {
            cur = cur->left;
        } else if (cur->key < key) {
            cur = cur->right;
        } else {
            // Spec: duplicate key -> NO ACTION (do not update data)
            return;
        }
    }

    Node* n = new Node(key, value);
    n->parent = p;
    if (key < p->key) p->left = n; else p->right = n;

    // --- Fix-up loop while parent is RED ---
    auto isLeftChild = [](Node* x) -> bool {
        return x->parent && x == x->parent->left;
    };
    auto siblingOf = [](Node* x) -> Node* {
        if (!x || !x->parent) return nullptr;
        return (x == x->parent->left) ? x->parent->right : x->parent->left;
    };

    while (n != this->root && n->parent && n->parent->color == RED) {
        Node* P = n->parent;
        Node* G = P ? P->parent : nullptr;
        if (!G) break; // Case 1 will set root black later

        Node* U = siblingOf(P);

        // Case 2: Parent and Uncle are RED => recolor P + U and G, and climb up
        if (U && U->color == RED) {
            P->color = BLACK;
            U->color = BLACK;
            G->color = RED;
            n = G; // tiếp tục sửa chữa từ G
            continue;
        }

        // Cases 3 & 4: Uncle is BLACK (or null) => rotations
        if (isLeftChild(P)) {
            if (!isLeftChild(n)) {
                // Case 3 (Left-Right): rotate left at parent to straighten
                rotateLeft(P);
                n = P;
                P = n->parent;
                G = P ? P->parent : nullptr;
            }
            // Case 4 (Left-Left): rotate right at grandparent, then recolor
            rotateRight(G);
            P->color = BLACK;
            if (G) G->color = RED;
        } else {
            if (isLeftChild(n)) {
                // Case 3 (Right-Left): rotate right at parent to straighten (tạo thành đường thẳng trc rồi ms sửa chữa tiếp)
                rotateRight(P);
                n = P;
                P = n->parent;
                G = P ? P->parent : nullptr;
            }
            // Case 4 (Right-Right): rotate left at grandparent; recolor
            rotateLeft(G);
            P->color = BLACK;
            if (G) G->color = RED;
        }
        break; // Case 4 terminates the fix-up
    }

    // Case 1: root is always black
    this->root->color = BLACK;
}

// =====================================
// Bounds: lowerBound / upperBound
// =====================================
template <class K, class T>
typename RedBlackTree<K, T>::RBTNode*
RedBlackTree<K, T>::lowerBound(const K& key, bool &found) const {
    // tìm nút đầu tiên có khóa >= key
    found = false;
    RBTNode* cur = this->root;
    RBTNode* ans = nullptr;  // first node with key >= target
    while (cur) {
        if (key < cur->key) {               // go left, record candidate
            ans = cur;
            cur = cur->left;
        } else if (cur->key < key) {        // go right
            cur = cur->right;
        } else {                            // exact match
            found = true;                   // bound exists (and equal)
            return cur;
        }
    }
    found = (ans != nullptr);               // spec: found=true iff a bound exists
    return ans;                             // nullptr if none
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode*
RedBlackTree<K, T>::upperBound(const K& key, bool &found) const {
    // tìm nút đầu tiên có khóa > key
    found = false;
    RBTNode* cur = this->root;
    RBTNode* ans = nullptr;   // first node with key > target
    while (cur) {
        if (key < cur->key) {               // cur is a candidate (> key)
            ans = cur;
            cur = cur->left;
        } else {                            // cur->key <= key
            cur = cur->right;
        }
    }
    found = (ans != nullptr);               // spec: found=true iff a bound exists
    return ans;                             // nullptr if none
}

// =====================================
// Protected node-returning bounds (spec requires these)
// =====================================
template <class K, class T>
typename RedBlackTree<K, T>::RBTNode*
RedBlackTree<K, T>::lowerBoundNode(const K& key) const {
    // tìm con trỏ trỏ tới node đầu tiên chứa khóa >= key
    RBTNode* cur = this->root;
    RBTNode* ans = nullptr;                 // first node with key >= target
    while (cur) {
        if (key < cur->key) { ans = cur; cur = cur->left; }
        else if (cur->key < key) { cur = cur->right; }
        else { return cur; }                // exact match
    }
    return ans;                             // may be nullptr
}

template <class K, class T>
typename RedBlackTree<K, T>::RBTNode*
RedBlackTree<K, T>::upperBoundNode(const K& key) const {
    // tìm node đầu tiên chứa khóa > key
    RBTNode* cur = this->root;
    RBTNode* ans = nullptr;                 // first node with key > target
    while (cur) {
        if (key < cur->key) { ans = cur; cur = cur->left; }
        else { cur = cur->right; }          // cur->key <= key
    }
    return ans;                             // may be nullptr
}

template <typename K, typename T>
typename RedBlackTree<K,T>::RBTNode* 
RedBlackTree<K,T>::successor(RBTNode* x) const {
    if (!x) return nullptr;

    // Case 1: right subtree exists → successor = leftmost of right subtree (nhỏ nhất ở cây con phải)
    if (x->right) {
        RBTNode* cur = x->right;
        while (cur->left) cur = cur->left;
        return cur;
    }

    // Case 2: không có cây con phải => đi ngược lên trên tìm node cha của x mà x là con trái của nó
    RBTNode* cur = x;
    RBTNode* p = x->parent;
    while (p && cur == p->right) {
        cur = p;
        p = p->parent;
    }
    return p;
}

// =====================================
// Remove + delete fix-up (double-black)
// =====================================
template <class K, class T>
void RedBlackTree<K, T>::remove(const K& key) {
    using Node = RBTNode;

    auto getColor = [](Node* x) { 
        return x ? x->color : BLACK; // nếu node = nullptr (hoặc node lá) => black
    };
    /*
    auto isLeftChild = [](Node* x) -> bool {
        return x && x->parent && x == x->parent->left;
    };
    */
    // hàm thay thế node con cũ bằng node con mới trong cây
    auto replaceParentChild = [this](Node* parent, Node* oldChild, Node* newChild) {
        if (!parent) {
            this->root = newChild;
        } else if (oldChild == parent->left) {
            parent->left = newChild;
        } else {
            parent->right = newChild;
        }
        if (newChild) newChild->parent = parent;
    };
    auto maximum = [](Node* x) {
        while (x && x->right) x = x->right;
        return x;
    };

    // 1) Find the node to delete
    Node* z = this->find(key);
    if (!z) return;

    // 2) If two children, tìm node lớn nhất ở cây con trái (predecessor) \
    rồi swap with predecessor and delete that instead
    if (z->left && z->right) {
        Node* y = maximum(z->left); // predecessor
        // swap (keys and data)
        swap(z->key,  y->key);
        swap(z->data, y->data);
        z = y; // now delete z which has at most 1 child 
        // (nghĩa là vẫn xóa node đó nhưng ở vị trí khác chỉ có tối đa 1 con => dễ xóa hơn là khi có 2 con)
    }

    // 3) Now z has at most one child
    Node* x = (z->left) ? z->left : z->right; // may be nullptr
    Node* p = z->parent;
    Color deletedColor = z->color;

    // link x in place of z
    replaceParentChild(p, z, x);
    delete z;

    // If we removed a RED or lifted a RED child, easy cases\
    không ảnh hưởng đến tính chất RBT (black-height) => done
    if (deletedColor == RED) return;

    // nếu xóa node black nhưng x là RED => recolor to BLACK and done
    if (x && x->color == RED) { x->color = BLACK; return; }

    // 4) Double-black fix-up: xóa black nhưng x cx là black (hoặc nullptr)\
    => đường đi qua x thiếu 1 black => cần sửa chữa: double-black (tưởng tượng x mang 2 lớp black)
    // Cách sửa: đẩy blackness lên trên, xoay cây để cân bằng lại và recolor
    while ((x != this->root) && (getColor(x) == BLACK)) {
        if (p && x == (p ? p->left : nullptr)) {
            // symmetric: x is left child of p
            Node* s = p ? p->right : nullptr; // sibling của x
            if (getColor(s) == RED) {
                // Case 1: red sibling => rotate to make sibling black
                s->color = BLACK;
                p->color = RED;
                this->rotateLeft(p);
                s = p ? p->right : nullptr;
            }

            // Now sibling is black, và 2 con của s cũng black
            if (getColor(s ? s->left : nullptr) == BLACK &&
                getColor(s ? s->right : nullptr) == BLACK) {
                // Case: black sibling with black children => push blackness up
                if (s) s->color = RED;
                x = p; // đẩy double-black lên trên
                p = x ? x->parent : nullptr;
            } else {
                if (getColor(s ? s->right : nullptr) == BLACK) {
                    // Case: sibling's near child is red, far child black => rotate at sibling
                    if (s && s->left) s->left->color = BLACK;
                    if (s) s->color = RED;
                    this->rotateRight(s);
                    s = p ? p->right : nullptr;
                }
                // Case: sibling's far child is red => rotate at parent and recolor
                if (s) s->color = getColor(p);
                if (p) p->color = BLACK;
                if (s && s->right) s->right->color = BLACK;
                this->rotateLeft(p);
                x = this->root;
                break;
            }
        } else {
            // symmetric: x is right child of p
            Node* s = p ? p->left : nullptr; // sibling
            if (getColor(s) == RED) {
                s->color = BLACK;
                p->color = RED;
                this->rotateRight(p);
                s = p ? p->left : nullptr;
            }

            if (getColor(s ? s->left : nullptr) == BLACK &&
                getColor(s ? s->right : nullptr) == BLACK) {
                if (s) s->color = RED;
                x = p;
                p = x ? x->parent : nullptr;
            } else {
                if (getColor(s ? s->left : nullptr) == BLACK) {
                    if (s && s->right) s->right->color = BLACK;
                    if (s) s->color = RED;
                    this->rotateLeft(s);
                    s = p ? p->left : nullptr;
                }
                if (s) s->color = getColor(p);
                if (p) p->color = BLACK;
                if (s && s->left) s->left->color = BLACK;
                this->rotateRight(p);
                x = this->root;
                break;
            }
        }
    }

    if (x) x->color = BLACK;
    if (this->root) this->root->color = BLACK;
}

// =====================================
// VectorRecord implementation
// =====================================
// Overload operator << for VectorRecord - prints only the id
std::ostream &operator<<(std::ostream &os, const VectorRecord &record){
    os << record.id;
    return os;
}

// TODO: Implement all VectorStore methods here
VectorStore::VectorStore(int dimension, std::vector<float> *(*embeddingFunction)(const std::string&), const std::vector<float>& referenceVector) {
    this->dimension = dimension;
    this->embeddingFunction = embeddingFunction;
    this->referenceVector = new vector<float>(referenceVector);
    this->vectorStore = new AVLTree<double, VectorRecord>();
    this->normIndex = new RedBlackTree<double, VectorRecord>();
    this->rootVector = nullptr;
    this->count = 0;
    this->averageDistance = 0.0;
}

VectorStore::~VectorStore() {
    // giải phóng tòn bộ bộ nhớ đã cấp phát cho AVL tree, các bản ghi và dữ liệu liên quan
    // Free allocated vectors
    for (auto vec : allocatedVectors) {
        delete vec;
    }
    allocatedVectors.clear();

    delete referenceVector;
    delete vectorStore;
    delete normIndex;
    // O(n)
}

int VectorStore::size() {
    return count;
}

bool VectorStore::empty() {
    return count == 0;
}

void VectorStore::clear() {
    // xóa toàn bộ vector và siêu dữ liệu đi kèm trong AVL và RBT, chỉ giữ lại reference vector
    vectorStore->clear();
    normIndex->clear();

    // Clean up all allocated vectors
    for (auto vec : allocatedVectors) {
        delete vec;
    }
    allocatedVectors.clear();

    count = 0;
    averageDistance = 0.0;
    rootVector = nullptr;
    // O(n)
}

std::vector<float>* VectorStore::preprocessing(std::string rawText) {
    // tiền xử lý văn bản thành vecttor số thực nhe=iều chiều
    // gọi hàm embedding để so sánh, và chuẩn hóa chiều vector (như trong BTL1)
    vector<float>* vec = embeddingFunction(rawText);
    if ((int)vec->size() > dimension) {
        vec->resize(dimension); // resize: cắt bớt nếu vector quá dài
    } else {
        while ((int)vec->size() < dimension) {
            vec->push_back(0.0f); // post-padding: thêm 0.0f vào cuối cho đủ dimension => zero-padding
        }
    }
    return vec;
    // O(dimension)
}

void VectorStore::rebuildRootIfNeeded() {
    // Rebuild the tree with a new root vector closest to the average distance
    // tái cấu trúc cây AVL để chọn root vector tối ưu do ban đầu root AVL có thể không tối ưu
    if (vectorStore->empty()) return;
    
    VectorRecord* newRoot = findVectorNearestToDistance(averageDistance);
    if (!newRoot) {
        cerr << "Error: Cannot find nearest vector!" << endl;
        return;
    }
    
    // SAO CHÉP ID trước khi clear do con trỏ newRoot sẽ bị hủy \
    sau khi clear trong rebuildTreeWithNewRoot
    int newRootId = newRoot->id;
    
    // Rebuild với ID
    VectorRecord* freshPointer = findRecordById(newRootId); // tìm lại pointer mới trong cây có id == newRootId sau khi rebuild
    // Update the root vector with the newly calculated root vector
    if (freshPointer) {
        rebuildTreeWithNewRoot(freshPointer);
    }
}

void VectorStore::rebuildTreeWithNewRoot(VectorRecord* newRoot) {
    // tái cấu trúc cây AVL và RBT với newRoot làm root mới, đảm bảo root mới được chèn đầu tiên
    if (!newRoot){
        cerr << "Error: newRoot is null!" << endl;
        return;
    } 

    // ===== BẢO VỆ newRoot BẰNG CÁCH SAO CHÉP =====
    VectorRecord newRootCopy = *newRoot;  // Deep copy trước khi clear
    // nghĩa là copy toàn bộ dữ liệu bên trong VectorRecord (bao gồm cả vector con trỏ)

    // Collect all records except the new root
    vector<VectorRecord> records;
    vectorStore->inorderTraversalWithCallback([&](VectorRecord& record) {
        if (record.id != newRootCopy.id) {
            records.push_back(record);
        }
    });

    // Clear existing trees (bây giờ an toàn vì đã có bản sao)
    vectorStore->clear();
    normIndex->clear();

    // Insert the new root first (dùng bản sao)
    vectorStore->insert(newRootCopy.distanceFromReference, newRootCopy);
    normIndex->insert(newRootCopy.norm, newRootCopy);

    // Reinsert other records
    for (const auto& record : records) {
        vectorStore->insert(record.distanceFromReference, record);
        normIndex->insert(record.norm, record);
    }

    // Update root vector (tìm lại pointer mới trong cây)
    // vì bản sao newRootCopy không còn liên kết với cây mới => phải tìm lại
    rootVector = findRecordById(newRootCopy.id);
}

void VectorStore::addText(std::string rawText) {
    vector<float>* vec = preprocessing(rawText); // tiền xử lý văn bản thành vector số thực
    allocatedVectors.push_back(vec);

    // Tính norm và distance từ reference
    // NHỚ: DO NOT MODIFY referenceVector
    // distance = L2(vec, referenceVector): là khoảng cách Euclidean giữa reference vector và vector chuyển đổi từ rawText
    double distance = l2Distance(*vec, *referenceVector);
    double norm = 0.0;
    for (float val : *vec) {
        norm += val * val;
    }
    norm = sqrt(norm); // chuẩn L2 (Euclidean) của vector

    // ===== CHECK DUPLICATE KEY (DISTANCE) =====
    if (vectorStore->contains(distance)) {
        // Key already exists -> chỉ giữ lại cái đầu tiên
        // Clean up the allocated vector since we won't use it
        allocatedVectors.pop_back();
        delete vec;
        return;  // ko thêm nếu distance đã tồn tại
    }

    // Find current maximum ID
    int maxId = 0;
    if (!vectorStore->empty()) {
        vectorStore->inorderTraversalWithCallback([&](const VectorRecord& record) {
            if (record.id > maxId) {
                maxId = record.id;
            }
        });
    }
    int newId = maxId + 1; // khi thêm 1 vector mới thì id của vector = maxid + 1 để tránh trùng id
    // tạo VectorRecord mới để lưu vector mới này vào cây
    VectorRecord record(newId, rawText, vec, distance, norm);
    
    // Update average distance
    averageDistance = (averageDistance * count + distance) / (count + 1);
    count++;

    // ===== CASE 1: First element (count = 1) - simple insert =====
    if (count == 1) {
        vectorStore->insert(distance, record);
        normIndex->insert(norm, record);
        rootVector = findRecordById(newId);
        return;
    }

    // ===== CASE 2: Multiple elements - rebuild with new root closest to average =====
    
    // Step 1: Collect all existing records + new record
    vector<VectorRecord> allRecords;
    vectorStore->inorderTraversalWithCallback([&](const VectorRecord& r) {
        allRecords.push_back(r);
    });
    allRecords.push_back(record);  // Add new record
    
    // Step 2: Find record closest to average distance
    int closestIdx = 0;
    double minDiff = abs(allRecords[0].distanceFromReference - averageDistance);
    for (size_t i = 1; i < allRecords.size(); ++i) {
        double diff = abs(allRecords[i].distanceFromReference - averageDistance);
        if (diff < minDiff) {
            minDiff = diff;
            closestIdx = i;
        }
    }
    int newRootId = allRecords[closestIdx].id;
    
    // Step 3: Sort by distance (array is ALREADY sorted except for new element)
    // Use merge sort for consistency
    if (!allRecords.empty()) {
        mergeSortByDistance(allRecords, 0, allRecords.size() - 1);
    }
    
    // Step 4: Clear and rebuild both trees
    vectorStore->clear();
    normIndex->clear();
    
    // Build balanced AVL tree
    buildBalancedAVL(allRecords, 0, allRecords.size() - 1);
    
    // Sort by norm and build RBT
    vector<VectorRecord> sortedByNorm = allRecords;
    if (!sortedByNorm.empty()) {
        mergeSortByNorm(sortedByNorm, 0, sortedByNorm.size() - 1);
    }
    buildBalancedRBT(sortedByNorm, 0, sortedByNorm.size() - 1);
    
    // Step 5: Update rootVector pointer
    rootVector = findRecordById(newRootId);
}

VectorRecord* VectorStore::findRecordById(int id) {
    // Tìm và trả về con trỏ tới VectorRecord có id tương ứng
    VectorRecord* result = nullptr;
    vectorStore->inorderTraversalWithCallback([&](VectorRecord& record) {
        if (record.id == id) {
            result = &record;
        }
    });
    return result;
}

VectorRecord* VectorStore::getVector(int index) {
    // Get vector at position 'index' during in-order AVL traversal (tính từ 0)
    if (index < 0 || index >= count) {
        throw out_of_range("Index is invalid!");
    }
    
    VectorRecord* result = nullptr;
    int currentIndex = 0;
    
    // DO THIS FRESH EACH TIME
    vectorStore->inorderTraversalWithCallback([&](VectorRecord& record) {
        if (currentIndex == index) {
            result = &record;  // Pointer valid during this traversal
        }
        currentIndex++;
    });

    if (!result) {
        throw out_of_range("Index is invalid!");
    }
    
    return result;  // O(n) - acceptable
}

std::string VectorStore::getRawText(int index) {
    if (index < 0 || index >= count) {
        throw out_of_range("Index is invalid!");
    }

    VectorRecord* record = getVector(index);
    return record->rawText;
    // O(n)
}

int VectorStore::getId(int index) {
    if (index < 0 || index >= count) {
        throw out_of_range("Index is invalid!");
    }
    VectorRecord* record = getVector(index);
    return record->id;
    // O(n)
}

bool VectorStore::removeAt(int index) {
    // Xóa vector và metadata tại vị trí index trong duyệt cây AVL theo thứ tự in-order, 
    // giải phóng bộ nhớ và xóa khỏi cây AVL. 
    // Nếu vector bị xóa là root vector, tìm vector mới gần nhất với khoảng cách trung bình 
    // cập nhật để làm root mới. 
    // Đồng thời, vector đó cũng phải được xoá trong red-black tree để đảm bảo tính thống nhất
    if (index < 0 || index >= count) {
        throw out_of_range("Index is invalid!");
    }
    
    // Find record to remove
    VectorRecord* recordToRemove = nullptr;
    int currentIndex = 0;
    vectorStore->inorderTraversalWithCallback([&](VectorRecord& record) {
        if (currentIndex == index) {
            recordToRemove = &record;
        }
        currentIndex++;
    });
    
    if (!recordToRemove) {
        throw out_of_range("Index is invalid!");
    }
    
    int idToRemove = recordToRemove->id;
    vector<float>* vecToDelete = recordToRemove->vector;
    
    // Collect all records EXCEPT the one to remove
    vector<VectorRecord> allRecords;
    vectorStore->inorderTraversalWithCallback([&](const VectorRecord& r) {
        if (r.id != idToRemove) {
            allRecords.push_back(r);
        }
    });
    
    // Clear trees
    vectorStore->clear();
    normIndex->clear();
    count--;
    
    // Delete vector from allocated list (giải phóng bộ nhớ đã cấp phát cho vector đó)
    for (size_t i = 0; i < allocatedVectors.size(); i++) {
        if (allocatedVectors[i] == vecToDelete) {
            delete allocatedVectors[i];
            allocatedVectors.erase(allocatedVectors.begin() + i);
            break;
        }
    }
    
    // Handle empty case
    if (count == 0) {
        averageDistance = 0.0;
        rootVector = nullptr;
        return true;
    }
    
    // Recalculate average
    double totalDistance = 0.0;
    for (const auto& r : allRecords) {
        totalDistance += r.distanceFromReference;
    }
    averageDistance = totalDistance / count;
    
    // Find closest to average
    int closestIdx = 0;
    double minDiff = abs(allRecords[0].distanceFromReference - averageDistance);
    for (size_t i = 1; i < allRecords.size(); ++i) {
        double diff = abs(allRecords[i].distanceFromReference - averageDistance);
        if (diff < minDiff) {
            minDiff = diff;
            closestIdx = i;
        }
    }
    int newRootId = allRecords[closestIdx].id;
    
    // Sort and rebuild
    mergeSortByDistance(allRecords, 0, allRecords.size() - 1);
    buildBalancedAVL(allRecords, 0, allRecords.size() - 1);
    
    vector<VectorRecord> sortedByNorm = allRecords;
    mergeSortByNorm(sortedByNorm, 0, sortedByNorm.size() - 1);
    buildBalancedRBT(sortedByNorm, 0, sortedByNorm.size() - 1);
    
    rootVector = findRecordById(newRootId);
    
    return true;
}

void VectorStore::setReferenceVector(const std::vector<float>& newReference) {
    // Cập nhật reference vector (đổi sang reference vecor mới)
    // và tái tính toán khoảng cách từ reference mới đến tất cả vector trong cây
    // cập nhật lại average distance, chọn lại root vector mới
    // tái cấu trúc lại cả hai cây AVL và RBT
    delete referenceVector;
    referenceVector = new vector<float>(newReference);
    
    if (vectorStore->empty()) {
        averageDistance = 0.0;
        rootVector = nullptr;
        return;
    }
    
    // Step 1: Collect all records and recompute distances
    vector<VectorRecord> allRecords;
    vectorStore->inorderTraversalWithCallback([&](const VectorRecord& record) {
        VectorRecord newRecord = record;
        double newDistance = l2Distance(*record.vector, *referenceVector);
        newRecord.distanceFromReference = newDistance;
        allRecords.push_back(newRecord);
    });
    
    // Step 2: Compute average distance
    double totalDist = 0.0;
    for (const auto& rec : allRecords) {
        totalDist += rec.distanceFromReference;
    }
    averageDistance = totalDist / allRecords.size();
    
    // Find vector closest to average distance - LƯU ID THAY VÌ INDEX
    int rootId = allRecords[0].id;  
    double minDiff = abs(allRecords[0].distanceFromReference - averageDistance);
    for (size_t i = 1; i < allRecords.size(); ++i) {
        double diff = abs(allRecords[i].distanceFromReference - averageDistance);
        if (diff < minDiff) {
            minDiff = diff;
            rootId = allRecords[i].id;  // <-- LƯU ID của vector gần nhất để xíu nx tìm lại làm root vector
        }
    }
    
    // Step 3: Sort by distance in ascending order (using MERGE SORT)
    if (!allRecords.empty()) {
        mergeSortByDistance(allRecords, 0, allRecords.size() - 1); // tham số arr, left, right
    }
    
    // Step 4: Rebuild both trees
    vectorStore->clear();
    normIndex->clear();
    
    buildBalancedAVL(allRecords, 0, allRecords.size() - 1);
    
    vector<VectorRecord> sortedByNorm = allRecords;
    if (!sortedByNorm.empty()) {
        mergeSortByNorm(sortedByNorm, 0, sortedByNorm.size() - 1);
    }
    buildBalancedRBT(sortedByNorm, 0, sortedByNorm.size() - 1);
    
    // Update rootVector pointer by finding it using ID
    rootVector = findRecordById(rootId);  // DÙNG rootId
}

std::vector<float>* VectorStore::getReferenceVector() const {
    return referenceVector;
}

VectorRecord* VectorStore::getRootVector() const {
    return rootVector;
}

double VectorStore::getAverageDistance() const {
    return averageDistance;
}

void VectorStore::setEmbeddingFunction(std::vector<float>* (*newEmbeddingFunction)(const std::string&)) {
    // cập nhật hàm embedding mới
    embeddingFunction = newEmbeddingFunction;
}

void VectorStore::forEach(void (*action)(std::vector<float> &, int, std::string &)) {
    // duyệt tất cả bản ghi vector trong cây AVL theo thứ tự inorder vs khoảng cách từ reference vector tăng dần
    // và thực hiện hàm action với tham số là vector, id và rawText trên mỗi bản ghi
    vectorStore->inorderTraversalWithCallback([&](VectorRecord& record) {
        if(!record.vector) return; // Skip if vector is null
        action(*record.vector, record.id, record.rawText);
    });
}

std::vector<int> VectorStore::getAllIdsSortedByDistance() const {
    // trả về danh sách các ID của tất cả vector trên cây AVL đã được 
    // sắp xếp theo khoảng cách từ reference vector tăng dần
    vector<int> result;
    vectorStore->inorderTraversalWithCallback([&](const VectorRecord& record) {
        result.push_back(record.id);
    });
    return result;
}

std::vector<VectorRecord*> VectorStore::getAllVectorsSortedByDistance() const {
    // trả về danh sách các con trỏ tới tất cả các VectorRecord trên cây AVL đã được
    // sắp xếp theo khoảng cách từ reference vector tăng dần
    vector<VectorRecord*> result;
    vectorStore->inorderTraversalWithCallback([&](const VectorRecord& record) {
        result.push_back(const_cast<VectorRecord*>(&record));
    });
    return result;
}

double VectorStore::cosineSimilarity(const std::vector<float>& v1, const std::vector<float>& v2) {
    double dotProduct = 0.0;
    double norm1 = 0.0;
    double norm2 = 0.0;
    for (size_t i = 0; i < v1.size(); ++i) {
        dotProduct += v1[i] * v2[i];
        norm1 += v1[i] * v1[i];
        norm2 += v2[i] * v2[i];
    }

    // Handle zero vectors
    if (norm1 < 1e-10 || norm2 < 1e-10) {
        return 0.0;  // Orthogonal / no similarity
    }

    return dotProduct / (sqrt(norm1) * sqrt(norm2));
    // O(dimension)
}

double VectorStore::l1Distance(const std::vector<float>& v1, const std::vector<float>& v2) {
    double distance = 0.0;
    for (size_t i = 0; i < v1.size(); ++i) {
        distance += abs(v1[i] - v2[i]);
    }
    return distance;
}

double VectorStore::l2Distance(const std::vector<float>& v1, const std::vector<float>& v2) {
    double distance = 0.0;
    for (size_t i = 0; i < v1.size(); ++i) {
        distance += pow(v1[i] - v2[i], 2);
    }
    return sqrt(distance);
}

double VectorStore::distanceByMetric(const std::vector<float>& a, const std::vector<float>& b, const std::string& metric) const {
    if (metric == "cosine") {
        return 1.0 - cosineSimilarity(a, b); // chuyển sang khoảng cách: [0,1,2] là các giá trị hợp lệ
    } else if (metric == "manhattan" || metric == "l1") {
        return l1Distance(a, b);
    } else if (metric == "euclidean" || metric == "l2") {
        return l2Distance(a, b);
    }
    throw invalid_metric();
}

double VectorStore::estimateD_Linear(const std::vector<float>& query, int k, double averageDistance, const std::vector<float>& reference, double c0_bias, double c1_slope) {
    // Ước lượng bán kính D sử dụng mô hình tuyến tính với các tham số:
    // k: số lượng nearest neighbors cần tìm
    // averageDistance: khoảng cách trung bình từ tất cả vectors đến reference vector
    // reference: reference vector
    // c0_bias và c1_slope: hệ số tinh chỉnh đơn giản (gtri mặc định 1e-9 và 0.05)
    // công thức: D = |d_r - averageDistance| + c1_slope * averageDistance * k + c0_bias
    // trong đó d_r là khoảng cách Euclidean từ query đến reference vector
    double d_r = l2Distance(query, reference);
    double D = abs(d_r - averageDistance) + c1_slope * averageDistance * k + c0_bias;
    return D;
}

int VectorStore::findNearest(const std::vector<float>& query, std::string metric) {
    // Tìm VectorRecord gần nhất với query theo metric đã cho => TRẢ VỀ ID CỦA VECTOR ĐÓ
    const double EPS = 1e-9;
    // Validation
    if (metric != "cosine" && metric != "manhattan" && metric != "euclidean" && 
        metric != "l1" && metric != "l2") {
        throw invalid_metric();
    }

    // Lấy tất cả vectors đã được sắp xếp theo distance từ reference trong store
    vector<VectorRecord*> allVectors = getAllVectorsSortedByDistance();
    if (allVectors.empty()) {
        return -1;
    }
    
    double bestDist = numeric_limits<double>::max(); // khởi tạo biến bestDist lưu khoảng cách tốt nhất (gần nhất vs khoảng cách trung bình từ reference vector)
    // với gtri ban đầu là vô cực để so sánh vs những khoảng cách nhỏ hơn thì lưu vào
    int bestId = -1; // khởi tạo biến bestId lưu ID của vector gần nhất
    // ban đầu là -1 (không tìm thấy)
    
    // Duyệt qua tất cả vectors bằng linear search
    for (VectorRecord* record : allVectors) {
        // tính khoảng cách từ query đến từng vector theo metric đã cho
        double dist = distanceByMetric(*record->vector, query, metric);
        // So sánh với EPS để tránh floating-point error
        if (dist + EPS < bestDist) {
            bestDist = dist;
            bestId = record->id;
        }
    }
    return bestId;
    // O(n * dimension)
}

int* VectorStore::topKNearest(const std::vector<float>& query, int k, std::string metric) {
    // Tìm top-k nearest neighbors với query theo metric đã cho
    // trả về mảng động các ID của k vector gần nhất (nếu không đủ k thì phần còn lại = 0)
    // sử dụng norm để lọc nhanh các candidates nằm ngoài bán kính ước lượng D
    // sau đó tính khoảng cách thực và sắp xếp để lấy k gần nhất
    const double EPS = 1e-9;
    
    // ===== VALIDATION =====
    if (k <= 0 || k > count) {
        throw invalid_k_value();
    }
    if (metric != "cosine" && metric != "manhattan" && metric != "euclidean" && 
        metric != "l1" && metric != "l2") {
        throw invalid_metric();
    }
       
    // ===== STEP 1: Tính chuẩn Euclidean của query vector =====
    double query_norm = l2Distance(query, vector<float>(query.size(), 0.0f));
    
    // ===== STEP 2: Ước lượng bán kính D =====    
    double D = estimateD_Linear(query, k, averageDistance, *referenceVector, 1e-9, 0.05);
    
    // ===== STEP 3: Tìm khoảng [lowerBoundNode(nq-D)->key, upperBoundNode(nq+D)->key] =====
    // sử dụng Red-Black Tree (normIndex) để tìm nhanh các node trong khoảng này
    // tạo tập candidates kích thước m
    double lowerBound = query_norm - D;
    double upperBound = query_norm + D;
    auto lowerNode = normIndex->lowerBoundNode(lowerBound);
    auto upperNode = normIndex->upperBoundNode(upperBound);
    
    // Nếu không tìm thấy bounds
    if (!lowerNode && !upperNode) {
        int* result = new int[k];
        for (int i = 0; i < k; ++i) result[i] = 0;
        return result;
    }
    
    // Xác định range [minKey, maxKey]
    double minKey = lowerNode ? lowerNode->key : 0.0;
    double maxKey = upperNode ? upperNode->key : numeric_limits<double>::max();
    
    // ===== STEP 4: Lấy tất cả node trong khoảng [minKey, maxKey] =====
    vector<VectorRecord> candidates;
    
    auto currentNode = lowerNode;
    while (currentNode && currentNode->key <= maxKey) {
        candidates.push_back(currentNode->data);
        currentNode = normIndex->successor(currentNode);
    }
    
    int m = candidates.size();
    cout << "Value m: " << m;
    
    // Nếu không có candidates
    if (m == 0) {
        int* result = new int[k];
        for (int i = 0; i < k; ++i) result[i] = 0;
        return result;
    }
    
    // ===== STEP 5: Tính khoảng cách thực và sort =====
    // Tính distance từ query tới từng candidate
    for (int i = 0; i < (int)candidates.size(); ++i) {
        double actualDist = distanceByMetric(*candidates[i].vector, query, metric);
        candidates[i].distanceFromReference = actualDist;
    }
    
    // BUBBLE SORT theo distance (tăng dần), nếu bằng thì theo ID (tăng dần) với EPS
    if (!candidates.empty()) {
        for (size_t i = 0; i < candidates.size() - 1; ++i) {
            for (size_t j = 0; j < candidates.size() - i - 1; ++j) {
                bool shouldSwap = false;
                
                double diff = candidates[j].distanceFromReference - candidates[j+1].distanceFromReference;
                
                if (fabs(diff) > EPS) {
                    // Khác nhau rõ ràng -> so sánh distance và swap để đưa distance nhỏ lên trước và sắp xếp tăng dần
                    shouldSwap = (candidates[j].distanceFromReference > candidates[j+1].distanceFromReference);
                } else {
                    // Bằng nhau (trong EPS) -> so sánh ID và sắp xếp tăng dần
                    shouldSwap = (candidates[j].id > candidates[j+1].id);
                }
                
                if (shouldSwap) {
                    swap(candidates[j], candidates[j+1]);
                }
            }
        }
    }
    
    // ===== STEP 6: Trả về kết quả =====
    // Nếu m < k thì trả về m giá trị, còn lại = 0
    int actualK = min(k, (int)candidates.size());
    int* result = new int[k];
    for (int i = 0; i < actualK; ++i) {
        result[i] = candidates[i].id;
    }
    for (int i = actualK; i < k; ++i) {
        result[i] = 0;
    }   
    return result;
    // O(m * dimension + m^2) worst-case due to bubble sort
}

int* VectorStore::rangeQueryFromRoot(double minDist, double maxDist) const {
    // Tìm tất cả vector có khoảng cách từ reference vector trong khoảng [minDist, maxDist]
    vector<int> result;
    if (vectorStore->empty()) {
        return nullptr;  // Return nullptr thay vì empty array
    }
    
    // ===== Collect results using DFS =====
    // ===== Dùng AVL Tree (vectorStore) để truy vấn theo distance =====
    // AVL tree được sắp xếp theo distanceFromReference
    // Nên có thể range query hiệu quả
    struct AVLRangeCollector {
        double minDist, maxDist;
        vector<int>& result;
        // Constructor lưu tham chiếu tới khoảng [minDist, maxDist] và result vector 
        AVLRangeCollector(double minD, double maxD, vector<int>& r)
            : minDist(minD), maxDist(maxD), result(r) {}
        
        void dfs(AVLTree<double, VectorRecord>::AVLNode* node) {
            if (!node) return;
            
            // if key < minDist, toàn bộ cây con trái cũng < minDist
            // only search right
            if (node->key < minDist) {
                dfs(node->pRight);
                return;
            }
            
            // tương tự: if key > maxDist, only search left
            if (node->key > maxDist) {
                dfs(node->pLeft);
                return;
            }
            
            // còn nếu nằm trong khoảng: 
            // In-order: left → current → right
            dfs(node->pLeft);
            if (node->key >= minDist && node->key <= maxDist) {
                result.push_back(node->data.id);
            } 
            dfs(node->pRight);
        }
    };
    
    AVLRangeCollector collector(minDist, maxDist, result);
    collector.dfs(vectorStore->getRoot()); // Bắt đầu DFS từ root của AVL tree
    
    // Return nullptr nếu không tìm thấy kết quả
    if (result.empty()) {
        return nullptr;
    }
  
    // Allocate (cấp phát) mảng động và copy kết quả
    int size = result.size();
    int* rawResult = new int[size];
    copy(result.begin(), result.end(), rawResult);
    return rawResult;
    // O(max(logn, k)) với k là số phần tử trong khoảng [minDist, maxDist]
}

int* VectorStore::rangeQuery(const std::vector<float>& query, double radius, std::string metric) const {
    // Tìm tất cả vector trên AVL có khoảng cách từ query nhỏ hơn radius theo metric đã cho
    const double EPS = 1e-9;
    if (metric != "cosine" && metric != "manhattan" && metric != "euclidean" && metric != "l1" && metric != "l2") { 
        throw invalid_metric(); 
    }

    // struct helper lưu ID và distance
    struct IDWithDistance {
        int id;
        double distance;
        IDWithDistance(int i, double d) : id(i), distance(d) {}
    };
    vector<IDWithDistance> candidates;
    
    // ===== BƯỚC 1: Thu thập candidates =====
    // Duyệt toàn bộ vector trong vectorStore (AVL tree) và tính distance tới query
    // Lấy các vector có distance(metric) < radius
    vectorStore->inorderTraversalWithCallback([&](const VectorRecord& record) {
        double distance = distanceByMetric(*record.vector, query, metric);
        if (distance < radius) {  // chứ không phải <=
            candidates.push_back(IDWithDistance(record.id, distance));
        }
    });
    if (candidates.empty()) {
        return nullptr;
    }
    
    // ===== BƯỚC 2: Sắp xếp candidates =====
    // Sort theo distance tăng dần, nếu bằng thì theo ID tăng dần
    for (size_t i = 0; i < candidates.size() - 1; ++i) {
        for (size_t j = 0; j < candidates.size() - i - 1; ++j) {
            bool shouldSwap = false;
            double diff = candidates[j].distance - candidates[j+1].distance;
            // nếu khác nhau rõ ràng => so sánh distance
            if (fabs(diff) > EPS) {
                shouldSwap = (candidates[j].distance > candidates[j+1].distance);
            } else {
                // bằng nhau => so sánh ID
                shouldSwap = (candidates[j].id > candidates[j+1].id);
            }
            
            if (shouldSwap) {
                swap(candidates[j], candidates[j+1]);
            }
        }
    }
    
    // ===== BƯỚC 3: Trả về kết quả =====
    int size = candidates.size();
    int* result = new int[size];
    for (int i = 0; i < size; ++i) {
        result[i] = candidates[i].id;
    }
    return result; // trả về mảng động các ID đã sắp xếp
    // O(n * dimension + m^2) worst-case due to bubble sort
}

int* VectorStore::boundingBoxQuery(const std::vector<float>& minBound, const std::vector<float>& maxBound) const {
    // Tìm tất cả vector nằm trong bounding box được xác định bởi minBound và maxBound vs khỏang mở (minBound, maxBound)
    // minBound và maxBound có cùng kích thước với vector trong store và \
    minBound vs maxBound là 2 vector chứ không phải 2 gtri 
    vector<int> result;
    // Duyệt toàn bộ vector trong vectorStore (AVL tree)
    vectorStore->inorderTraversalWithCallback([&](const VectorRecord& record) {
        // Kiểm tra vector có nằm trong bounding box không
        if (record.vector->size() != minBound.size()) return;
        bool inside = true;
        for (size_t i = 0; i < record.vector->size(); ++i) {
            // Khoảng mở: không bao gồm 2 đầu mút
            if ((*record.vector)[i] <= minBound[i] || (*record.vector)[i] >= maxBound[i]) {
                inside = false;
                break;
            }
        }
        if (inside) {
            result.push_back(record.id);
        }
    });
    if (result.empty()) {
        return nullptr;
    }
    
    int size = result.size();
    int* rawResult = new int[size];
    copy(result.begin(), result.end(), rawResult);
    return rawResult;
}

// =====================================================
// MERGE SORT IMPLEMENTATION FOR DISTANCE
// =====================================================
void VectorStore::mergeSortedArrays(std::vector<VectorRecord>& arr, int left, int mid, int right) {
    // BƯỚC 1: Tạo 2 mảng tạm leftArr và rightArr
    vector<VectorRecord> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<VectorRecord> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);
    
    // BƯỚC 2: Merge hai mảng đã sắp xếp (dựa trên distanceFromReference)
    int i = 0, j = 0, k = left;
    while (i < (int)leftArr.size() && j < (int)rightArr.size()) {
        if (leftArr[i].distanceFromReference <= rightArr[j].distanceFromReference) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    // BƯỚC 3: Nếu right Arr hết => Copy phần tử còn lại của leftArr
    while (i < (int)leftArr.size()) {
        arr[k++] = leftArr[i++];
    }
    // BƯỚC 4: Ngược lại, copy phần tử còn lại của rightArr
    while (j < (int)rightArr.size()) {
        arr[k++] = rightArr[j++];
    }
}

void VectorStore::mergeSortByDistance(std::vector<VectorRecord>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        // Đệ quy sắp xếp nửa trái
        mergeSortByDistance(arr, left, mid);
        // Đệ quy sắp xếp nửa phải
        mergeSortByDistance(arr, mid + 1, right);
        // Merge hai nửa đã sắp xếp
        mergeSortedArrays(arr, left, mid, right);
    }
}

// =====================================================
// MERGE SORT IMPLEMENTATION FOR NORM
// =====================================================
void VectorStore::mergeSortedArraysByNorm(std::vector<VectorRecord>& arr, int left, int mid, int right) {
    vector<VectorRecord> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<VectorRecord> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);
    
    int i = 0, j = 0, k = left;
    while (i < (int)leftArr.size() && j < (int)rightArr.size()) {
        if (leftArr[i].norm <= rightArr[j].norm) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }
    
    while (i < (int)leftArr.size()) {
        arr[k++] = leftArr[i++];
    }
    while (j < (int)rightArr.size()) {
        arr[k++] = rightArr[j++];
    }
}

void VectorStore::mergeSortByNorm(std::vector<VectorRecord>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortByNorm(arr, left, mid);
        mergeSortByNorm(arr, mid + 1, right);
        mergeSortedArraysByNorm(arr, left, mid, right);
    }
}

// Helper function to build balanced AVL tree from sorted array
void VectorStore::buildBalancedAVL(const std::vector<VectorRecord>& records, int start, int end) {
    if (start > end) return;
    int mid = start + (end - start) / 2;
    
    // Insert middle element first (this will be the root of this subtree)
    vectorStore->insert(records[mid].distanceFromReference, records[mid]);
    
    // Recursively build left and right subtrees
    buildBalancedAVL(records, start, mid - 1);
    buildBalancedAVL(records, mid + 1, end);
}

void VectorStore::buildBalancedRBT(const std::vector<VectorRecord>& records, int start, int end) {
    if (start > end) return;
    int mid = start + (end - start) / 2;
    
    // Insert middle element first (this will be the root of this subtree)
    normIndex->insert(records[mid].norm, records[mid]);
    
    // Recursively build left and right subtrees
    buildBalancedRBT(records, start, mid - 1);
    buildBalancedRBT(records, mid + 1, end);
}

double VectorStore::getMaxDistance() const {
    // trả về khoảng cách lớn nhất từ reference vector đến tất cả vector trong store
    if (vectorStore->empty()) return 0.0;
    
    double maxDist = 0.0;
    vectorStore->inorderTraversalWithCallback([&](const VectorRecord& record) {
        if (record.distanceFromReference > maxDist) {
            maxDist = record.distanceFromReference;
        }
    });
    
    return maxDist;
}

double VectorStore::getMinDistance() const {
    if (vectorStore->empty()) return 0.0;
    
    // Do AVL dc sắp xếp theo distanceFromReference
    // => The minimum distance is always at the leftmost node of AVL tree
    // (since tree is sorted by distance from reference vector)
    AVLTree<double, VectorRecord>::AVLNode* current = vectorStore->root;
    while (current && current->pLeft) {
        current = current->pLeft;
    }

    if (current) {
        return current->data.distanceFromReference;
    }
    
    return 0.0;
}

VectorRecord VectorStore::computeCentroid(const std::vector<VectorRecord*>& records) const {
    // tìm centroid (tâm) của tập các vector records theo công thức trung bình từng chiều
    if (records.empty()) return VectorRecord();  // Return empty if no records

    // khởi tạo vector centroid với kích thước bằng vector đầu tiên và giá trị 0.0f
    vector<float> centroid(records[0]->vector->size(), 0.0f);
    for (const auto& record : records) {
        for (size_t i = 0; i < centroid.size(); ++i) {
            centroid[i] += (*record->vector)[i]; // cộng dồn từng chiều
        }
    }
    for (auto& val : centroid) {
        val /= records.size();
    }

    return VectorRecord(-1, "Centroid", new vector<float>(centroid), 0.0, 0.0);
    // ID = -1, rawText = "Centroid", distanceFromReference và norm = 0.0
    // O(n * dimension)
}

VectorRecord* VectorStore::findVectorNearestToDistance(double targetDistance) const {
    // tìm vector có distanceFromReference gần nhất với targetDistance
    if (vectorStore->empty()) {
        return nullptr;
    }
    
    const VectorRecord* nearestRecord = nullptr; // con trỏ đến vector gần nhất (nullptr), kiểu dữ liệu const (do lambda nhận const VectorRecord&)
    double minDiff = numeric_limits<double>::max(); // chênh lệch nhỏ nhất (ban đầu là vô cực)
    
    // duyệt toàn bộ cây để tìm và lưu pointer trực tiếp
    vectorStore->inorderTraversalWithCallback([&](const VectorRecord& record) {
        double diff = abs(record.distanceFromReference - targetDistance);
        if (diff < minDiff) {
            minDiff = diff;
            nearestRecord = &record;  // Lưu pointer trực tiếp
        }
    });
    // mà hàm lại cần trả về VectorRecord* (không phải const)
    return const_cast<VectorRecord*>(nearestRecord); // ép kiểu const_cast bỏ đi
}

// Explicit template instantiation for the type used by VectorStore
template class AVLTree<double, VectorRecord>;
template class AVLTree<double, double>;
template class AVLTree<int, double>;
template class AVLTree<int, int>;
template class AVLTree<double, string>;
template class AVLTree<int, string>;

template class RedBlackTree<double, VectorRecord>;
template class RedBlackTree<double, double>;
template class RedBlackTree<int, double>;
template class RedBlackTree<int, int>;
template class RedBlackTree<double, string>;
template class RedBlackTree<int, string>;