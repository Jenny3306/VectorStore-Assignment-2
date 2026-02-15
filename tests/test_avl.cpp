#include "doctest/doctest.h"
#include "src/VectorStore.h"
#include <vector>

TEST_CASE("test_001")
{
    AVLTree<int, std::string> tree;

    tree.insert(10, "ten");
    tree.insert(20, "twenty");
    tree.insert(5, "five");

    std::ostringstream oss;
    std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
    tree.printTreeStructure();
    std::cout.rdbuf(oldCout);
    std::string output = oss.str();

    CHECK(output == "  ten    \n five  twenty  \n");
}

TEST_CASE("test_002")
{
    AVLTree<int, std::string> tree;

    tree.insert(1, "1");
    tree.insert(2, "2");
    tree.insert(3, "3");

    std::ostringstream oss;
    std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
    tree.printTreeStructure();
    std::cout.rdbuf(oldCout);
    std::string output = oss.str();

    CHECK(output == "  2    \n 1  3  \n");
}

// TODO
// ========================================
// Test AVL tree operations
// ========================================
TEST_CASE("test_003_avl_operations") {
    AVLTree<int, int> tree;
    
    // Test empty tree
    CHECK(tree.empty() == true);
    CHECK(tree.getSize() == 0);
    CHECK(tree.getHeight() == 0);
    
    // Test contains on empty tree
    CHECK(tree.contains(5) == false);
    
    // Insert and test
    tree.insert(5, 5);
    CHECK(tree.empty() == false);
    CHECK(tree.getSize() == 1);
    CHECK(tree.contains(5) == true);
    CHECK(tree.contains(10) == false);
    
    // Insert duplicate (should not increase size)
    tree.insert(5, 50);
    CHECK(tree.getSize() == 1);
    
    // Remove non-existent
    tree.remove(999);
    CHECK(tree.getSize() == 1);
    
    // Remove existing
    tree.remove(5);
    CHECK(tree.empty() == true);
    CHECK(tree.contains(5) == false);
    
    // Clear empty tree
    tree.clear();
    CHECK(tree.empty() == true);
}

// Add these to test_avl.cpp - FIXED to match your actual printer output

// ---- 0) Empty tree print ----
TEST_CASE("empty_tree_prints_NULL") {
    AVLTree<int, std::string> t;
    std::ostringstream oss; 
    auto* old = std::cout.rdbuf(oss.rdbuf());
    t.printTreeStructure();
    std::cout.rdbuf(old);
    CHECK(oss.str() == "NULL\n");
}

// ---- 1) contains() true/false paths ----
TEST_CASE("contains_true_false") {
    AVLTree<int, std::string> t;
    t.insert(10,"ten");
    t.insert(5,"five");
    CHECK(t.contains(10) == true);
    CHECK(t.contains(7)  == false);
}

// ---- 2) duplicate key is ignored ----
TEST_CASE("insert_duplicate_noop") {
    AVLTree<int, std::string> t;
    t.insert(10,"ten");
    t.insert(10,"TEN"); // ignored
    CHECK(t.getSize() == 1);
    CHECK(t.getHeight() == 1);
}

// ---- 3) LL rotation ----
TEST_CASE("rotation_LL") {
    AVLTree<int, std::string> t;
    t.insert(30,"thirty");
    t.insert(20,"twenty");
    t.insert(10,"ten");
    
    // After LL rotation, root should be 20
    auto* root = t.getRoot();
    CHECK(root != nullptr);
    CHECK(root->key == 20);
    CHECK(t.getHeight() == 2);
}

// ---- 4) RR rotation ----
TEST_CASE("rotation_RR") {
    AVLTree<int, std::string> t;
    t.insert(10,"ten");
    t.insert(20,"twenty");
    t.insert(30,"thirty");
    
    // After RR rotation, root should be 20
    auto* root = t.getRoot();
    CHECK(root != nullptr);
    CHECK(root->key == 20);
    CHECK(t.getHeight() == 2);
}

// ---- 5) LR rotation ----
TEST_CASE("rotation_LR") {
    AVLTree<int, std::string> t;
    t.insert(30,"thirty");
    t.insert(10,"ten");
    t.insert(20,"twenty");
    
    // After LR rotation, root should be 20
    auto* root = t.getRoot();
    CHECK(root != nullptr);
    CHECK(root->key == 20);
    CHECK(t.getHeight() == 2);
}

// ---- 6) RL rotation ----
TEST_CASE("rotation_RL") {
    AVLTree<int, std::string> t;
    t.insert(10,"ten");
    t.insert(30,"thirty");
    t.insert(20,"twenty");
    
    // After RL rotation, root should be 20
    auto* root = t.getRoot();
    CHECK(root != nullptr);
    CHECK(root->key == 20);
    CHECK(t.getHeight() == 2);
}

// ---- 7) clear() ----
TEST_CASE("clear_then_empty") {
    AVLTree<int, std::string> t;
    t.insert(2,"2"); 
    t.insert(1,"1"); 
    t.insert(3,"3");
    
    CHECK(t.empty() == false);
    CHECK(t.getSize() == 3);
    
    t.clear();
    
    CHECK(t.empty() == true);
    CHECK(t.getHeight() == 0);
    CHECK(t.getSize() == 0);
}

// ---- getRoot on empty tree ----
TEST_CASE("getRoot_empty_is_null") {
    AVLTree<int, std::string> t;
    auto r = t.getRoot();
    CHECK(r == nullptr);
}

// ---- getRoot after rotation ----
TEST_CASE("getRoot_after_rotation") {
    AVLTree<int, std::string> t;
    t.insert(1, "1");
    t.insert(2, "2");
    t.insert(3, "3");

    auto* r = t.getRoot();
    REQUIRE(r != nullptr);
    CHECK(r->key == 2);
    CHECK(r->data == std::string("2"));
}

// ---- getSize ----
TEST_CASE("getSize_multiple_inserts") {
    AVLTree<int, std::string> t;
    t.insert(10,"ten");
    t.insert(20,"twenty");
    t.insert(5,"five");
    t.insert(15,"fifteen");
    CHECK(t.getSize() == 4);
}

// ---- getHeight ----
TEST_CASE("getHeight_multiple_inserts") {
    AVLTree<int, std::string> t;
    t.insert(10,"ten");
    t.insert(20,"twenty");
    t.insert(5,"five");
    t.insert(15,"fifteen");
    CHECK(t.getHeight() == 3);
}

// ---- contains on empty tree ----
TEST_CASE("contains_empty_tree") {
    AVLTree<int, std::string> t;
    CHECK(t.contains(42) == false);
}

// ---- inorderTraversal helpers ----
namespace {
    static std::vector<std::string> __visited;
    static void __collect(const std::string& s) {
        __visited.push_back(s);
    }
}

// ---- inorderTraversal ----
TEST_CASE("inorderTraversal_sorted_order") {
    AVLTree<int, std::string> t;
    t.insert(20,"twenty");
    t.insert(10,"ten");
    t.insert(30,"thirty");
    t.insert(25,"twentyfive");
    t.insert(40,"forty");

    __visited.clear();
    t.inorderTraversal(&__collect);

    REQUIRE(__visited.size() == 5);
    CHECK(__visited[0] == "ten");
    CHECK(__visited[1] == "twenty");
    CHECK(__visited[2] == "twentyfive");
    CHECK(__visited[3] == "thirty");
    CHECK(__visited[4] == "forty");
}

// ---- inorderTraversal on empty tree ----
TEST_CASE("inorderTraversal_empty") {
    AVLTree<int, std::string> t;
    __visited.clear();
    t.inorderTraversal(&__collect);
    CHECK(__visited.empty());
}

// ---- remove leaf node ----
TEST_CASE("remove_leaf") {
    AVLTree<int, std::string> t;
    t.insert(20,"20");
    t.insert(10,"10");
    t.insert(30,"30");
    t.insert(5,"5");

    CHECK(t.contains(5));
    int size_before = t.getSize();

    t.remove(5);

    CHECK_FALSE(t.contains(5));
    CHECK(t.getSize() == size_before - 1);
}

// ---- remove node with one child ----
TEST_CASE("remove_one_child") {
    AVLTree<int, std::string> t;
    t.insert(20,"20");
    t.insert(10,"10");
    t.insert(30,"30");
    t.insert(25,"25");

    CHECK(t.contains(30));
    t.remove(30);

    CHECK_FALSE(t.contains(30));
    CHECK(t.contains(25));
}

// ---- remove node with two children ----
TEST_CASE("remove_two_children") {
    AVLTree<int, std::string> t;
    t.insert(20,"20");
    t.insert(10,"10");
    t.insert(30,"30");
    t.insert(25,"25");
    t.insert(40,"40");

    CHECK(t.contains(30));
    t.remove(30);

    CHECK_FALSE(t.contains(30));
    CHECK(t.contains(25));
    CHECK(t.contains(40));
}

// ---- remove triggers LL rotation ----
TEST_CASE("remove_LL_rotation") {
    AVLTree<int, std::string> t;
    t.insert(30,"30");
    t.insert(20,"20");
    t.insert(40,"40");
    t.insert(10,"10");

    t.remove(40);

    // After rebalancing, root should be 20
    auto* root = t.getRoot();
    CHECK(root != nullptr);
    CHECK(root->key == 20);
}

// ---- remove triggers RR rotation ----
TEST_CASE("remove_RR_rotation") {
    AVLTree<int, std::string> t;
    t.insert(10,"10");
    t.insert(5,"5");
    t.insert(20,"20");
    t.insert(30,"30");

    t.remove(5);

    // After rebalancing, root should be 20
    auto* root = t.getRoot();
    CHECK(root != nullptr);
    CHECK(root->key == 20);
}

// ---- remove non-existent key ----
TEST_CASE("remove_nonexistent") {
    AVLTree<int, std::string> t;
    t.insert(2,"2");
    t.insert(1,"1");
    t.insert(3,"3");

    int size_before = t.getSize();
    t.remove(999);

    CHECK(t.getSize() == size_before);
}

// ---- remove triggers LR rotation ----
TEST_CASE("remove_LR_rotation") {
    AVLTree<int, std::string> t;
    t.insert(20,"20");
    t.insert(10,"10");
    t.insert(30,"30");
    t.insert(15,"15");

    t.remove(30);

    // After LR rotation, root should be 15
    auto* root = t.getRoot();
    CHECK(root != nullptr);
    CHECK(root->key == 15);
    CHECK(t.contains(10));
    CHECK(t.contains(15));
    CHECK(t.contains(20));
}

// ---- remove triggers RL rotation ----
TEST_CASE("remove_RL_rotation") {
    AVLTree<int, std::string> t;
    t.insert(20,"20");
    t.insert(10,"10");
    t.insert(30,"30");
    t.insert(25,"25");

    t.remove(10);

    // After RL rotation, root should be 25
    auto* root = t.getRoot();
    CHECK(root != nullptr);
    CHECK(root->key == 25);
    CHECK(t.contains(20));
    CHECK(t.contains(25));
    CHECK(t.contains(30));
}

// ---- Test NULL branch in printer ----
TEST_CASE("printer_null_child") {
    AVLTree<int, std::string> t;
    t.insert(2, "2");
    t.insert(1, "1");

    std::ostringstream oss; 
    auto* old = std::cout.rdbuf(oss.rdbuf());
    t.printTreeStructure();
    std::cout.rdbuf(old);

    // Just verify something was printed
    CHECK(oss.str().length() > 0);
    CHECK(t.contains(2));
    CHECK(t.contains(1));
}