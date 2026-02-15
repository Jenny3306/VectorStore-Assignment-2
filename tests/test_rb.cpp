#include "doctest/doctest.h"
#include "src/VectorStore.h"

TEST_CASE("test_101")
{
    RedBlackTree<int, string> rbt;
    // http://www.rnlkwc.ac.in/pdf/study-material/comsc/Design.pdf

    // Insert 8
    rbt.insert(8, "8");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == " (8,B)  \n");
    }

    // Insert 18
    rbt.insert(18, "18");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "  (8,B)    \n NULL  (18,R)  \n");
    }

    // Insert 5
    rbt.insert(5, "5");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "  (8,B)    \n (5,R)  (18,R)  \n");
    }

    // Insert 15
    rbt.insert(15, "15");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "    (8,B)        \n  (5,B)    (18,B)    \n NULL  NULL  (15,R)  NULL  \n");
    }

    // Insert 17
    rbt.insert(17, "17");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "    (8,B)        \n  (5,B)    (17,B)    \n NULL  NULL  (15,R)  (18,R)  \n");
    }

    // Insert 25
    rbt.insert(25, "25");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "        (8,B)                \n    (5,B)        (17,R)        \n  NULL    NULL    (15,B)    (18,B)    \n NULL  NULL  NULL  NULL  NULL  NULL  NULL  (25,R)  \n");
    }

    // Insert 40
    rbt.insert(40, "40");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "        (8,B)                \n    (5,B)        (17,R)        \n  NULL    NULL    (15,B)    (25,B)    \n NULL  NULL  NULL  NULL  NULL  NULL  (18,R)  (40,R)  \n");
    }

    //  Insert 80
    rbt.insert(80, "80");
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "        (17,B)                \n    (8,R)        (25,R)        \n  (5,B)    (15,B)    (18,B)    (40,B)    \n NULL  NULL  NULL  NULL  NULL  NULL  NULL  (80,R)  \n");
    }
}

TEST_CASE("test_102")
{
    RedBlackTree<int, string> rbt;
    rbt.insert(8, "8");
    rbt.insert(18, "18");
    rbt.insert(5, "5");
    rbt.insert(15, "15");
    rbt.insert(17, "17");
    rbt.insert(25, "25");
    rbt.insert(40, "40");
    rbt.insert(80, "80");

    rbt.remove(5);
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "        (17,B)                \n    (8,B)        (25,R)        \n  NULL    (15,R)    (18,B)    (40,B)    \n NULL  NULL  NULL  NULL  NULL  NULL  NULL  (80,R)  \n");
    }

    rbt.remove(17);
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "        (15,B)                \n    (8,B)        (25,R)        \n  NULL    NULL    (18,B)    (40,B)    \n NULL  NULL  NULL  NULL  NULL  NULL  NULL  (80,R)  \n");
    }

    rbt.remove(18);
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "    (15,B)        \n  (8,B)    (40,R)    \n NULL  NULL  (25,B)  (80,B)  \n");
    }

    rbt.remove(40);
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "    (15,B)        \n  (8,B)    (25,B)    \n NULL  NULL  NULL  (80,R)  \n");
    }

    rbt.remove(15);
    {
        std::ostringstream oss;
        std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
        rbt.printTreeStructure();
        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        CHECK(output == "  (25,B)    \n (8,B)  (80,B)  \n");
    }
}

// helpers
template <class K, class T>
static void ins(RedBlackTree<K,T>& t, std::initializer_list<K> ks) {
    for (auto k : ks) t.insert(k, static_cast<T>(k));
}

// ---------- INSERT CASES ----------
TEST_CASE("insert_case2_recolor_only") {
    RedBlackTree<int,int> t;
    ins<int,int>(t, {7,3,18,10,22,8,11}); // triggers recolors
    CHECK(t.size() == 7);
    CHECK(t.contains(8));
    CHECK(t.contains(11));
    // duplicate key -> update data, shape unchanged
    t.insert(11, 111);
    CHECK(t.contains(11));
}

TEST_CASE("insert_case3_then_case4_LR_and_RL") {
    {   // Left-Right (30,10,20)
        RedBlackTree<int,int> t;
        ins<int,int>(t, {30,10,20});
        CHECK(t.size() == 3);
        CHECK(t.contains(20));
    }
    {   // Right-Left (10,30,20)
        RedBlackTree<int,int> t;
        ins<int,int>(t, {10,30,20});
        CHECK(t.size() == 3);
        CHECK(t.contains(20));
    }
}

TEST_CASE("insert_case4_LL_and_RR") {
    {   // LL
        RedBlackTree<int,int> t;
        ins<int,int>(t, {30,20,10});
        CHECK(t.size() == 3);
        CHECK(t.contains(10));
    }
    {   // RR
        RedBlackTree<int,int> t;
        ins<int,int>(t, {10,20,30});
        CHECK(t.size() == 3);
        CHECK(t.contains(30));
    }
}

// ---------- BOUNDS ----------
TEST_CASE("bounds_empty_and_singleton") {
    {
        RedBlackTree<int,int> t;
        bool found=false;
        CHECK(t.lowerBound(5, found) == nullptr);
        CHECK(found == false);
        CHECK(t.upperBound(5, found) == nullptr);
        CHECK(found == false);
        CHECK(t.empty());
        CHECK(t.size() == 0);
        t.clear(); // no-op
        CHECK(t.size() == 0);
    }
    {
        RedBlackTree<int,int> t;
        t.insert(10, 10);
        bool found=false;

        auto lb = t.lowerBound(10, found);
        REQUIRE(lb != nullptr);
        CHECK(found);

        auto ub = t.upperBound(10, found);
        CHECK(ub == nullptr);
        CHECK(!found);            // still false because no upper bound exists

        lb = t.lowerBound(5, found);
        REQUIRE(lb != nullptr);
        CHECK(lb->key == 10);
        CHECK(found);             // bound exists (not exact) â†’ true

        ub = t.upperBound(5, found);
        REQUIRE(ub != nullptr);
        CHECK(ub->key == 10);
        CHECK(found);             // bound exists (not exact) â†’ true

    }
}

TEST_CASE("bounds_edges_and_interior") {
    RedBlackTree<int,int> t;
    ins<int,int>(t, {5,10,15,20,25});
    bool found=false;

    auto lb = t.lowerBound(1, found);
    REQUIRE(lb != nullptr);
    CHECK(lb->key == 5);
    CHECK(found);   // bound exists

    auto ub = t.upperBound(1, found);
    REQUIRE(ub != nullptr);
    CHECK(ub->key == 5);
    CHECK(found);   // bound exists

    lb = t.lowerBound(30, found);
    CHECK(lb == nullptr);

    ub = t.upperBound(30, found);
    CHECK(ub == nullptr);

    lb = t.lowerBound(12, found);
    REQUIRE(lb != nullptr);
    CHECK(lb->key == 15);
    CHECK(found);   // bound exists

    ub = t.upperBound(15, found);
    REQUIRE(ub != nullptr);
    CHECK(ub->key == 20);
    CHECK(found);   // bound exists
}

// ---------- DELETE FAST PATHS ----------
TEST_CASE("delete_red_leaf_fastpath") {
    RedBlackTree<int,int> t;
    ins<int,int>(t, {10,5,15});
    t.remove(5); // removing a red leaf: immediate
    CHECK_FALSE(t.contains(5));
    CHECK(t.size() == 2);
}

TEST_CASE("delete_black_with_red_child_fastpath") {
    RedBlackTree<int,int> t;
    ins<int,int>(t, {20,10,30,25});
    t.remove(30); // typical shape yields black-with-red-child case
    CHECK_FALSE(t.contains(30));
    t.remove(999); // no-op
    CHECK(t.size() >= 2);
}

// ---------- DOUBLE-BLACK CASES ----------
TEST_CASE("delete_double_black_red_sibling_then_rotate") {
    RedBlackTree<int,int> t;
    ins<int,int>(t, {10,5,15,1,7,12,18,0,2});
    t.remove(1);
    CHECK_FALSE(t.contains(1));
    CHECK(t.size() == 8);
}

TEST_CASE("delete_double_black_black_sib_black_kids_recolor_up") {
    RedBlackTree<int,int> t;
    ins<int,int>(t, {8,4,12,2,6,10,14});
    t.remove(2);
    CHECK_FALSE(t.contains(2));
    t.remove(6);
    CHECK_FALSE(t.contains(6));
}

TEST_CASE("delete_double_black_near_red_then_far_red_paths") {
    RedBlackTree<int,int> t;
    ins<int,int>(t, {40,20,60,10,30,50,70,25});
    t.remove(10);  CHECK_FALSE(t.contains(10));
    t.remove(25);  CHECK_FALSE(t.contains(25));
}

// ---------- TWO-CHILD DELETE (predecessor swap) ----------
TEST_CASE("delete_two_children_swaps_with_predecessor") {
    RedBlackTree<int,int> t;
    ins<int,int>(t, {20,10,30,5,15,25,35,13});
    CHECK(t.contains(20));
    t.remove(20);
    CHECK_FALSE(t.contains(20));
    CHECK(t.size() == 7);
}

// ---------- CLEAR & REUSE ----------
TEST_CASE("clear_then_reuse") {
    RedBlackTree<int,int> t;
    ins<int,int>(t, {1,2,3,4,5});
    CHECK(t.size() == 5);
    t.clear();
    CHECK(t.size() == 0);
    CHECK(t.empty());
    ins<int,int>(t, {9,8,7});
    CHECK(t.size() == 3);
    CHECK(t.contains(8));
}

// ---------- DOUBLE SPECIALIZATION (still standard) ----------
TEST_CASE("double_keys_values_bounds_delete") {
    RedBlackTree<double,double> t;
    ins<double,double>(t, {-2.0, -1.5, 0.0, 2.71, 3.14});
    bool found=false;
    
    auto lb = t.lowerBound(-1.6, found);
    REQUIRE(lb != nullptr);
    CHECK(lb->key == -1.5);
    CHECK(found);   // bound exists (not exact)

    auto ub = t.upperBound(3.14, found);
    CHECK(ub == nullptr);

    t.remove(-2.0);
    CHECK_FALSE(t.contains(-2.0));
}

// ---- PRINT EMPTY TREE ----
TEST_CASE("rbt_print_empty_tree_outputs_NULL_line") {
    RedBlackTree<int,int> t;
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    t.printTreeStructure();
    std::cout.rdbuf(old);
    CHECK(oss.str() == "NULL\n");
}

// ---- FIND MISS EARLY RETURN ----
TEST_CASE("rbt_find_returns_null_when_absent") {
    RedBlackTree<int,int> t;
    t.insert(1,1);
    t.insert(2,2);
    auto n = t.find(42);
    CHECK(n == nullptr);
}

// ---- DELETE: ONLY ROOT (z is root, x==nullptr, p==nullptr) ----
TEST_CASE("rbt_delete_only_root_node") {
    RedBlackTree<int,int> t;
    t.insert(10,10);                 // root black
    CHECK(t.size() == 1);
    t.remove(10);                    // replaceParentChild with parent=null
    CHECK(t.size() == 0);
    CHECK(t.empty());
}

// ---- DELETE: ROOT WITH ONE RED CHILD (fast path recolor child & return) ----
TEST_CASE("rbt_delete_root_with_single_red_child") {
    RedBlackTree<int,int> t;
    t.insert(10,10);                 // root black
    t.insert(5,5);                   // child likely red
    // After deletion: child becomes new black root
    t.remove(10);
    CHECK(t.size() == 1);
    CHECK(t.contains(5));
    // also exercise clear() when non-empty again
    t.clear();
    CHECK(t.size() == 0);
}

TEST_CASE("rbt_explicit_recolor_helpers_are_callable") {
    RedBlackTree<int,int> t;
    t.insert(10,10);
    t.insert(5,5); // ensure we have at least two nodes
    auto n = t.find(5);
    REQUIRE(n != nullptr);
    n->recolorToRed();
    CHECK(n->color == RED);
    n->recolorToBlack();
    CHECK(n->color == BLACK);
}

TEST_CASE("rbt_rotateRight_covers_y_right_and_parent_right_link") {
    RedBlackTree<int,int> t;
    // Build RL triangle under a non-root parent: rotateRight(P=40) then rotateLeft(G=10)
    // Ensures y->right exists during rotateRight, and x is parent's right child.
    t.insert(10,10);
    t.insert(40,40);
    t.insert(20,20);
    t.insert(30,30);
    // Sanity: structure valid and all keys present
    CHECK(t.contains(10));
    CHECK(t.contains(20));
    CHECK(t.contains(30));
    CHECK(t.contains(40));
}

TEST_CASE("rbt_rotateLeft_covers_y_left_and_parent_left_link") {
    RedBlackTree<int,int> t;
    // Build LR triangle under a non-root parent: rotateLeft(P=10) then rotateRight(G=40)
    t.insert(40,40);
    t.insert(10,10);
    t.insert(20,20);
    t.insert(15,15);
    CHECK(t.contains(40));
    CHECK(t.contains(10));
    CHECK(t.contains(20));
    CHECK(t.contains(15));
}

TEST_CASE("rbt_delete_fixup_right_child_symmetry_paths") {
    RedBlackTree<int,int> t;
    // Balanced base
    for (int k : {50, 25, 75, 12, 37, 62, 87, 31, 43, 56, 68}) t.insert(k, k);
    // These removals tend to create a double-black on the right child side
    t.remove(68);   // prune a leaf on the right subtree
    t.remove(87);   // sibling/parent colors/rotations on the right branch
    t.remove(75);   // remove a black with one child on the right
    // Tree still valid and contains many nodes
    CHECK(t.contains(50));
    CHECK(t.contains(25));
    CHECK(t.contains(12));
    CHECK(t.contains(62));
    CHECK_FALSE(t.contains(68));
    CHECK_FALSE(t.contains(87));
    CHECK_FALSE(t.contains(75));
}

TEST_CASE("rbt_rotateRight_hits_y_right_parent_and_parent_left_link") {
    RedBlackTree<int,int> t;
    // Build left subtree under 50, then form LR at node 30: rotateLeft(30) then rotateRight(50).
    // During rotateRight(50): x = 50, y = 30 (after the prior rotate), and y->right exists.
    t.insert(50,50);
    t.insert(30,30);   // left child of 50
    t.insert(40,40);   // creates LR at 30
    t.insert(35,35);   // ensure y->right exists during rotateRight

    // Make the subtree non-root so x is a LEFT child during a later rotateRight.
    t.insert(70,70);   // 50 now has a parent (after rebalancing steps)

    // Insert that triggers another LR inside the left chain so a rotateRight occurs where
    // the rotating node is the LEFT child of its parent, and y->right is non-null.
    t.insert(34,34);   // small nudge to keep a right child under y
    t.insert(33,33);

    // Sanity: keys present
    CHECK(t.contains(33));
    CHECK(t.contains(34));
    CHECK(t.contains(35));
    CHECK(t.contains(40));
}

TEST_CASE("rbt_delete_fixup_right_branch_red_sibling") {
    RedBlackTree<int,int> t;
    // Balanced base:
    for (int k : {60, 30, 90, 20, 40, 80, 100, 35, 45, 85}) t.insert(k, k);

    // Manipulate to make x the right child and s (p->left) become RED at fix-up time.
    // These removals typically create the right-branch red-sibling scenario.
    t.remove(85);   // prune a red/black leaf on right subtree
    t.remove(100);  // force double-black propagation up on the right side
    t.remove(90);   // now p is 60; s = 30 is usually red â†’ rotateRight(p)

    CHECK(t.contains(60));
    CHECK(t.contains(30));
    CHECK_FALSE(t.contains(85));
    CHECK_FALSE(t.contains(100));
    CHECK_FALSE(t.contains(90));
}

TEST_CASE("rbt_delete_fixup_right_branch_near_red_then_parent_rotate") {
    RedBlackTree<int,int> t;
    // Construct a right-heavy setup that tends to yield: s black, s->near child red.
    for (int k : {50, 25, 75, 12, 37, 62, 87, 31, 43, 56, 68, 52}) t.insert(k, k);

    // Removals tuned to produce the near-red case on the right branch
    t.remove(52);  // adjust colors in right subtree
    t.remove(56);  // cause double-black at right child of parent (p), with s's near child red
    t.remove(62);  // walk the near-red â†’ rotateLeft(s) path

    CHECK(t.contains(50));
    CHECK(t.contains(25));
    CHECK_FALSE(t.contains(52));
    CHECK_FALSE(t.contains(56));
    CHECK_FALSE(t.contains(62));
}

// =====================
// FINAL EDGE COVERAGE
// =====================

TEST_CASE("rotateRight: cover y->right->parent=x and parent-left relink") {
    RedBlackTree<int,int> t;
    // Build a non-root LL at a node that itself is a LEFT child of its parent,
    // and ensure y (x->left) has a RIGHT child when we rotateRight(x).
    // Sequence explained:
    // 50(B), 30(R), 70(B), 20(R), 25(R), 10(R)
    // Inserting 10 makes an LL at G=30 (left child of 50). y=20 has right child 25.
    // Fix-up does rotateRight(G=30) -> hits:
    //   if (y->right) y->right->parent = x;
    //   else-branch of parent link: x == x->parent->left  -> x->parent->left = y;
    t.insert(50,50);
    t.insert(30,30);
    t.insert(70,70);
    t.insert(20,20);
    t.insert(25,25);
    t.insert(10,10);

    // sanity
    CHECK(t.contains(10));
    CHECK(t.contains(25));
    CHECK(t.contains(20));
    CHECK(t.contains(30));
}

TEST_CASE("delete fix-up (right branch): red sibling -> rotateRight(p)") {
    RedBlackTree<int,int> t;
    // Construct so that when we delete on the RIGHT, x is right child and s = p->left is RED.
    // This sequence reliably produces the right-branch red-sibling path on typical RBTs.
    for (int k : {60, 30, 90, 20, 40, 80, 100, 10, 25, 35, 45, 85, 95}) t.insert(k, k);

    // Remove in this order to force double-black on the right side with a RED left sibling:
    t.remove(95);   // prune leaf in right subtree
    t.remove(100);  // push blackness up on right
    t.remove(90);   // at this point p=60, s=30 is red -> rotateRight(p) (right-branch red-sibling case)

    CHECK(t.contains(60));
    CHECK(t.contains(30));
    CHECK_FALSE(t.contains(95));
    CHECK_FALSE(t.contains(100));
    CHECK_FALSE(t.contains(90));
}

TEST_CASE("delete fix-up (right branch): near-red child -> rotateLeft(s)") {
    RedBlackTree<int,int> t;
    // We want: x is RIGHT child; sibling s is BLACK; s's NEAR child is RED (so rotateLeft(s)).
    for (int k : {50, 25, 75, 12, 37, 62, 87, 31, 43, 56, 68, 52, 58}) t.insert(k, k);

    // These deletions tilt colors so s becomes black with near red child (left),
    // triggering rotateLeft(s) in the right-branch near-red subcase.
    t.remove(58);   // adjust local colors
    t.remove(52);   // x becomes right child under the same parent
    t.remove(56);   // near-red at sibling -> rotateLeft(s)

    CHECK(t.contains(50));
    CHECK(t.contains(25));
    CHECK_FALSE(t.contains(58));
    CHECK_FALSE(t.contains(52));
    CHECK_FALSE(t.contains(56));
}

// --- Force left-branch red-sibling case: s = p->right is RED, x = left child ---
// Expect path: s->color=BLACK; p->color=RED; rotateLeft(p); s = p ? p->right : nullptr;
TEST_CASE("delete_fixup_left_branch_red_sibling_rotateLeft_p") {
    RedBlackTree<int,int> t;
    // base 3-node tree
    t.insert(20, 20);
    t.insert(10, 10);
    t.insert(30, 30);

    // Force: z=10 (leaf) is BLACK so deleting it creates double-black at x=null (left child).
    // Sibling s=30 stays RED to trigger the 'red sibling' subcase.
    auto *z = t.find(10);
    REQUIRE(z != nullptr);
    z->recolorToBlack();              // ensure deletedColor == BLACK

    auto *s = t.find(30);
    REQUIRE(s != nullptr);
    s->recolorToRed();                // ensure sibling is RED

    t.remove(10);                     // executes the left-branch red-sibling block
    CHECK(t.contains(20));
    CHECK(t.contains(30));
    CHECK_FALSE(t.contains(10));
}

// --- Force right-branch red-sibling case: s = p->left is RED, x = right child ---
// Expect path: s->color=BLACK; p->color=RED; rotateRight(p); s = p ? p->left : nullptr;
TEST_CASE("delete_fixup_right_branch_red_sibling_rotateRight_p") {
    RedBlackTree<int,int> t;
    // base 3-node tree
    t.insert(20, 20);
    t.insert(10, 10);
    t.insert(30, 30);

    // Force: z=30 (leaf) is BLACK so deleting it creates double-black at x=null (right child).
    // Sibling s=10 stays RED to trigger the 'red sibling' subcase.
    auto *z = t.find(30);
    REQUIRE(z != nullptr);
    z->recolorToBlack();              // ensure deletedColor == BLACK

    auto *s = t.find(10);
    REQUIRE(s != nullptr);
    s->recolorToRed();                // ensure sibling is RED

    t.remove(30);                     // executes the right-branch red-sibling block
    CHECK(t.contains(20));
    CHECK(t.contains(10));
    CHECK_FALSE(t.contains(30));
}

// ==========================
// Coverage for *protected* helpers: lowerBoundNode / upperBoundNode
// via a tiny test-only subclass exposing wrappers
// ==========================
template <class K, class T>
struct RBExpose : public RedBlackTree<K,T> {
    using Base = RedBlackTree<K,T>;
    using Node = typename Base::RBTNode; // RBTNode is protected; subclass can name it
    Node* lbNode(const K& k) const { return this->lowerBoundNode(k); }
    Node* ubNode(const K& k) const { return this->upperBoundNode(k); }
};

TEST_CASE("lowerBoundNode/upperBoundNode: exact-match, below-min, above-max, in-between") {
    RBExpose<int,int> t;

    // Build a small tree:   10, 20, 30, 40, 50
    for (int k : {30, 10, 50, 20, 40}) t.insert(k, k);

    // --- exact match paths (return cur inside loop) ---
    {
        auto n = t.lbNode(30);
        REQUIRE(n != nullptr);
        CHECK(n->key == 30);

        n = t.ubNode(30);
        REQUIRE(n != nullptr);
        CHECK(n->key == 40);
    }

    // --- below minimum (ans set on first step; return ans at end) ---
    {
        auto n = t.lbNode(5);   // first >= 5 is 10
        REQUIRE(n != nullptr);
        CHECK(n->key == 10);

        n = t.ubNode(5);        // first > 5 is also 10
        REQUIRE(n != nullptr);
        CHECK(n->key == 10);
    }

    // --- between existing keys (choose candidate via left step; return ans) ---
    {
        auto n = t.lbNode(21);  // first >= 21 is 30
        REQUIRE(n != nullptr);
        CHECK(n->key == 30);

        n = t.ubNode(20);       // first > 20 is 30
        REQUIRE(n != nullptr);
        CHECK(n->key == 30);
    }

    // --- above maximum (no candidate; returns nullptr) ---
    {
        auto n = t.lbNode(999);
        CHECK(n == nullptr);

        n = t.ubNode(999);
        CHECK(n == nullptr);
    }
}

TEST_CASE("test_103_rbt_operations") {
    RedBlackTree<int, int> rbt;
    
    // Test empty tree
    CHECK(rbt.empty() == true);
    CHECK(rbt.size() == 0);
    
    // Test contains and find on empty
    CHECK(rbt.contains(5) == false);
    CHECK(rbt.find(5) == nullptr);
    
    // Insert and test
    rbt.insert(5, 5);
    CHECK(rbt.empty() == false);
    CHECK(rbt.size() == 1);
    CHECK(rbt.contains(5) == true);
    CHECK(rbt.find(5) != nullptr);
    
    // Insert duplicate
    rbt.insert(5, 50);
    CHECK(rbt.size() == 1);
    
    // Test bounds
    bool found;
    auto lb = rbt.lowerBound(5, found);
    CHECK(found == true);
    CHECK(lb != nullptr);
    
    auto ub = rbt.upperBound(5, found);
    CHECK(found == false);
    
    // Insert more for bounds testing
    rbt.insert(3, 3);
    rbt.insert(7, 7);
    
    lb = rbt.lowerBound(4, found);
    CHECK(found == true);
    CHECK(lb->key == 5);
    
    ub = rbt.upperBound(5, found);
    CHECK(found == true);
    CHECK(ub->key == 7);
    
    // Remove non-existent
    rbt.remove(999);
    CHECK(rbt.size() == 3);
    
    // Remove existing
    rbt.remove(5);
    CHECK(rbt.size() == 2);
    CHECK(rbt.contains(5) == false);
    
    // Clear
    rbt.clear();
    CHECK(rbt.empty() == true);
}