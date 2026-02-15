#ifndef VECTORSTORE_H
#define VECTORSTORE_H

#pragma once
// NOTE: Per assignment rules, only this single include is allowed here.
#include "main.h"

// ------------------------------
// AVL balance enum
// ------------------------------
enum BalanceValue
{
    LH = -1, // Left Higher
    EH = 0,  // Equal Height
    RH = 1   // Right Higher
};

// ------------------------------
// Generic AVL Tree (template)
// ------------------------------
template <class K, class T>
class AVLTree
{
    friend class VectorStore; // Allow VectorStore to access protected/private members

public:
    class AVLNode
    {
    public:
        K key; // khóa dùng để so sánh và sắp xếp các nút trong cây
        T data; // gtri dữ liệu dc lưu tại node
        AVLNode *pLeft; // con trỏ tới node con trái
        AVLNode *pRight; // con trỏ tới node con phải
        BalanceValue balance; // hệ số cân bằng của node, biểu diễn bằng enum

        int height;  // cached height of this node (1 for leaf)

        //khởi tạo 1 node mới với khóa và gtri dữ liệu, con trỏ pLeft, pRight = nullptr và balance là EH
        AVLNode(const K& key, const T& value)
            : key(key), data(value), pLeft(nullptr), pRight(nullptr), balance(EH), height(1) {}

        friend class VectorStore; // Allow VectorStore to access AVLNode members
    };

protected:
    AVLNode *root; // con trỏ tới node gốc được khởi tạo là nullptr khi tạo cây rỗng

    AVLNode *rotateRight(AVLNode *&node);
    AVLNode *rotateLeft(AVLNode *&node);
    void clearHelper(AVLNode *node);
    
    AVLNode* insertRec(AVLNode* node, const K& key, const T& value);

    AVLNode* removeRec(AVLNode* node, const K& key);
    AVLNode* minValueNode(AVLNode* node);

    template<typename Func>
    void inorderTraversalWithCallbackHelper(AVLNode* node, Func& action) {
        if (!node) return;
        inorderTraversalWithCallbackHelper(node->pLeft, action);
        action(node->data);
        inorderTraversalWithCallbackHelper(node->pRight, action);
    }

    // Const helper
    template<typename Func>
    void inorderTraversalWithCallbackHelperConst(const AVLNode* node, Func& action) const {
        if (!node) return;
        inorderTraversalWithCallbackHelperConst(node->pLeft, action);
        action(node->data);  // const T&
        inorderTraversalWithCallbackHelperConst(node->pRight, action);
    }

public:
    AVLTree(); // khởi tạo 1 cây AVL rỗng với root = nullptr: O(1)
    ~AVLTree(); // giải phóng toàn bộ bộ nhớ đã cấp phát cho cây AVL (tất cả các node): O(n)

    void insert(const K &key, const T &value);
    void remove(const K &key);
    bool contains(const K &key) const;

    int getHeight() const;
    int getSize() const;
    bool empty() const;
    void clear();

    void printTreeStructure() const;

    void inorderTraversal(void (*action)(const T &)) const; // Const version
    template<typename Func>
    void inorderTraversalWithCallback(Func action) {
        inorderTraversalWithCallbackHelper(root, action);
    }

    // Const version (for read-only operations)
    template<typename Func>
    void inorderTraversalWithCallback(Func action) const {
        inorderTraversalWithCallbackHelperConst(root, action);
    }

    AVLNode *getRoot() const { return root; }

    static int h(AVLNode* n);
    static void pull(AVLNode* n);
    static int dfsHeight(AVLNode* n);
    static int __avlCount(AVLNode* n);
    static int __avlBalance(AVLNode* n);
};

enum Color
{
    RED,
    BLACK
};

// RedBlackTree class
template <class K, class T>
class RedBlackTree
{
    friend class VectorStore; // Allow VectorStore to access protected/private members

public:
    // RBTNode class

    class RBTNode
    {
    public:
        K key;
        T data;
        Color color;
        RBTNode *parent;
        RBTNode *left;
        RBTNode *right;

        // Constructor
        RBTNode(const K &key, const T &value);

        void recolorToRed();
        void recolorToBlack();

        friend class VectorStore; // Allow VectorStore to access RBTNode members
    };

private:
    RBTNode *root; // pointer to the root of the Red-Black Tree, khởi tạo nullptr khi cây rỗng

protected:
    void rotateLeft(RBTNode *node);
    void rotateRight(RBTNode *node);

    RBTNode *lowerBoundNode(const K &key) const;
    RBTNode *upperBoundNode(const K &key) const;

    template<typename Func>
    void inorderTraversalWithCallbackHelper(RBTNode* node, Func& action) {
        if (!node) return;
        inorderTraversalWithCallbackHelper(node->left, action);
        action(node->data);
        inorderTraversalWithCallbackHelper(node->right, action);
    }

    // Const helper
    template<typename Func>
    void inorderTraversalWithCallbackHelperConst(const RBTNode* node, Func& action) const {
        if (!node) return;
        inorderTraversalWithCallbackHelperConst(node->left, action);
        action(node->data);  // const T&
        inorderTraversalWithCallbackHelperConst(node->right, action);
    }

public:
    RedBlackTree();
    ~RedBlackTree();

    bool empty() const;
    int size() const;
    void clear();
    void insert(const K &key, const T &value);
    void remove(const K &key);
    RBTNode *find(const K &key) const;
    bool contains(const K &key) const;

    RBTNode *lowerBound(const K &key, bool &found) const;
    RBTNode *upperBound(const K &key, bool &found) const;
    RBTNode* successor(RBTNode* node) const;

    void printTreeStructure() const;

    template<typename Func>
    void inorderTraversalWithCallback(Func action) {
        inorderTraversalWithCallbackHelper(root, action);
    }

    // Const version (for read-only operations)
    template<typename Func>
    void inorderTraversalWithCallback(Func action) const {
        inorderTraversalWithCallbackHelperConst(root, action);
    }
};

// ------------------------------
// VectorRecord
// ------------------------------
class VectorRecord
{
public:
    int id;
    std::string rawText;
    int rawLength;
    std::vector<float> *vector;
    double distanceFromReference;
    double norm;

    VectorRecord()
        : id(-1), rawLength(0), vector(nullptr), distanceFromReference(0.0), norm(0.0) {}

    VectorRecord(int _id,
                 const std::string &_rawText,
                 std::vector<float> *_vec,
                 double _dist,
                 double _norm = 0)
        : id(_id),
          rawText(_rawText),
          rawLength(static_cast<int>(_rawText.size())),
          vector(_vec),
          distanceFromReference(_dist),
          norm(_norm) {}

    // Overload operator << to print only the id
    friend std::ostream &operator<<(std::ostream &os, const VectorRecord &record);
};

// ------------------------------
// VectorStore
// ------------------------------
class VectorStore
{
private:
    AVLTree<double, VectorRecord> *vectorStore; // AVL tree storing VectorRecord với distanceFromReference làm khóa \
    (dùng để sắp xếp các bản ghi theo khoảng cách từ vector tham chiếu)
    RedBlackTree<double, VectorRecord> *normIndex; // RBT storing VectorRecord với norm làm khóa \
    giúp hỗ trợ tìm kiếm theo norm hiệu quả

    std::vector<float> *referenceVector; // điểm tham chiếu người dùng cung cấp, \
    không nhất thiết phải nằm trong store
    VectorRecord *rootVector; // phải nằm trong sotre và là vector gần nhất với khoảng cách trung bình từ referenceVector

    int dimension;
    int count;
    double averageDistance; // khoảng cách trung bình từ tất cả vector trong store đến referenceVector
    // dùng làm tiêu chí tìm root vector

    std::vector<float> *(*embeddingFunction)(const std::string &);
    // con trỏ hàm ánh xạ từ chuỗi văn bản thô sang vector số thực nhiều chiều

    vector<vector<float>*> allocatedVectors;
    VectorRecord* findRecordById(int id);

    double distanceByMetric(const std::vector<float> &a,
                            const std::vector<float> &b,
                            const std::string &metric) const;

    void rebuildRootIfNeeded();
    void rebuildTreeWithNewRoot(VectorRecord *newRoot);

    VectorRecord *findVectorNearestToDistance(double targetDistance) const;

    void buildBalancedAVL(const std::vector<VectorRecord>& records, int start, int end);
    void buildBalancedRBT(const std::vector<VectorRecord>& records, int start, int end);

    static void mergeSortedArrays(std::vector<VectorRecord>& arr, int left, int mid, int right);
    static void mergeSortByDistance(std::vector<VectorRecord>& arr, int left, int right);
    static void mergeSortedArraysByNorm(std::vector<VectorRecord>& arr, int left, int mid, int right);
    static void mergeSortByNorm(std::vector<VectorRecord>& arr, int left, int right);

public:
    VectorStore(int dimension,
                std::vector<float> *(*embeddingFunction)(const std::string &),
                const std::vector<float> &referenceVector);
    ~VectorStore();

    void test_vectorStore() { vectorStore->printTreeStructure(); }
    void test_normIndex() { normIndex->printTreeStructure(); }

    int size();
    bool empty();
    void clear();

    std::vector<float> *preprocessing(std::string rawText);
    void addText(std::string rawText);

    VectorRecord *getVector(int index);
    std::string getRawText(int index);
    int getId(int index);

    bool removeAt(int index);

    void setReferenceVector(const std::vector<float> &newReference);
    std::vector<float> *getReferenceVector() const;
    VectorRecord *getRootVector() const;
    double getAverageDistance() const;
    void setEmbeddingFunction(std::vector<float> *(*newEmbeddingFunction)(const std::string &));

    void forEach(void (*action)(std::vector<float> &, int, std::string &));
    std::vector<int> getAllIdsSortedByDistance() const;
    std::vector<VectorRecord *> getAllVectorsSortedByDistance() const;

    static double cosineSimilarity(const std::vector<float> &v1, const std::vector<float> &v2);
    static double l1Distance(const std::vector<float> &v1, const std::vector<float> &v2);
    static double l2Distance(const std::vector<float> &v1, const std::vector<float> &v2);

    double estimateD_Linear(const std::vector<float> &query, int k, double averageDistance, const std::vector<float> &reference, double c0_bias = 1e-9, double c1_slope = 0.05);

    int findNearest(const std::vector<float> &query, std::string metric = "cosine");
    int *topKNearest(const std::vector<float> &query, int k, std::string metric = "cosine");

    int *rangeQueryFromRoot(double minDist, double maxDist) const;
    int *rangeQuery(const std::vector<float> &query, double radius, std::string metric = "cosine") const;
    int *boundingBoxQuery(const std::vector<float> &minBound, const std::vector<float> &maxBound) const;

    double getMaxDistance() const;
    double getMinDistance() const;
    VectorRecord computeCentroid(const std::vector<VectorRecord *> &records) const;
};

#endif // VECTORSTORE_H
