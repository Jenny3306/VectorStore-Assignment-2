#include "doctest/doctest.h"
#include "src/VectorStore.h"
#include <vector>
#include <cmath>
#include <iomanip>
#include "helper.h"
#include <algorithm>

#include "doctest/doctest.h"
#include "src/VectorStore.h"
#include <vector>
#include <cmath>
#include <iomanip>
#include "helper.h"
#include <algorithm>

TEST_CASE("test_301")
{
    std::vector<float> refVec = {0.0f};
    VectorStore store(1, simpleEmbedding, refVec);

    for (int i = 0; i <= 10; ++i)
    {
        store.addText(std::to_string(i));
    }
    {
        int *rawResult = store.rangeQueryFromRoot(2, 8);
        std::vector<int> result;
        if (rawResult)
        {
            result.assign(rawResult, rawResult + 7);
            delete[] rawResult;
        }
        // std::sort(result.begin(), result.end());
        const std::vector<int> expected = {3, 4, 5, 6, 7, 8, 9};
        for (int i = 0; i < 7; i++)
            CHECK(result[i] == expected[i]);
    }

    {
        int *rawResult = store.rangeQuery({5}, 2, "euclidean");
        std::vector<int> result;
        if (rawResult)
        {
            result.assign(rawResult, rawResult + 5);
            delete[] rawResult;
        }
        // std::sort(result.begin(), result.end());
        const std::vector<int> expected = {4, 5, 6, 7, 8};
        for (int i = 0; i < 5; i++)
            CHECK(result[i] == expected[i]);
    }

    {
        int *rawResult = store.boundingBoxQuery({5}, {11});
        std::vector<int> result;
        if (rawResult)
        {
            result.assign(rawResult, rawResult + 4);
            delete[] rawResult;
        }
        // std::sort(result.begin(), result.end());
        const std::vector<int> expected = {7, 8, 9, 10};
        for (int i = 0; i < 4; i++)
            CHECK(result[i] == expected[i]);
    }

    {
        CHECK(store.getMaxDistance() == 10);
        CHECK(store.getMinDistance() == 0);
    }

    {
        vector<VectorRecord *> list_VectorRecord = store.getAllVectorsSortedByDistance();
        VectorRecord center = store.computeCentroid(list_VectorRecord);

        CHECK((*center.vector)[0] == 5);
        delete center.vector;
    }
}

TEST_CASE("test_302")
{
    std::vector<float> refVec = {0.0f};
    VectorStore store(1, simpleEmbedding, refVec);

    for (int i = 0; i <= 10; ++i)
    {
        store.addText(std::to_string(i));
    }

    int id = store.findNearest({6.2}, "manhattan");
    CHECK(id == 7);

    std::ostringstream oss;
    std::streambuf *oldCout = std::cout.rdbuf(oss.rdbuf());
    int *ids = store.topKNearest({5}, 5, "manhattan");
    for (int i = 0; i < 4; i++)
        cout << ids[i];
    delete[] ids;
    std::cout.rdbuf(oldCout);
    std::string output = oss.str();
    CHECK(output == "Value m: 4\n6578");
}

// ========================================
// Test rangeQuery edge cases
// ========================================
// TEST_CASE("test_303_rangeQuery_edge") {
//     std::vector<float> refVec = {0.0f};
//     VectorStore store(1, simpleEmbedding, refVec);
    
//     store.addText("1");
//     store.addText("5");
//     store.addText("10");
    
//     // Range that includes nothing
//     int* result1 = store.rangeQueryFromRoot(100, 200);
//     delete[] result1;
    
//     // Range that includes everything
//     int* result2 = store.rangeQueryFromRoot(0, 100);
//     delete[] result2;
    
//     // Test rangeQuery with different metrics
//     int* result3 = store.rangeQuery({5.0f}, 100.0, "l2");
//     delete[] result3;
    
//     int* result4 = store.rangeQuery({5.0f}, 100.0, "l1");
//     delete[] result4;
    
//     int* result5 = store.rangeQuery({5.0f}, 100.0, "cosine");
//     delete[] result5;
// }

// // ========================================
// // Test boundingBoxQuery edge cases
// // ========================================
// TEST_CASE("test_304_boundingBox") {
//     std::vector<float> refVec = {0.0f, 0.0f};
//     VectorStore store(2, simpleEmbedding, refVec);
    
//     store.addText("1 1");
//     store.addText("5 5");
//     store.addText("10 10");
    
//     // Box that includes nothing
//     int* result1 = store.boundingBoxQuery({100, 100}, {200, 200});
//     delete[] result1;
    
//     // Box that includes everything
//     int* result2 = store.boundingBoxQuery({-100, -100}, {100, 100});
//     delete[] result2;
    
//     // Partial box
//     int* result3 = store.boundingBoxQuery({2, 2}, {6, 6});
//     delete[] result3;
// }

// // ========================================
// // Test topKNearest with insufficient candidates
// // ========================================
// TEST_CASE("test_305_topK_insufficient") {
//     std::vector<float> refVec = {0.0f};
//     VectorStore store(1, simpleEmbedding, refVec);

//     store.addText("1");
//     store.addText("2");

//     int* result = store.topKNearest({1.5f}, 2, "l2"); // Lựa chọn metric l2

//     // Đảm bảo chỉ trả về số kết quả có sẵn
//     CHECK(result != nullptr);
//     CHECK((result[0] == 1 || result[0] == 2));
//     CHECK((result[1] == 1 || result[1] == 2));

//     delete[] result;
// }

// // ========================================
// // Test removeAt various positions
// // ========================================
// TEST_CASE("test_306_removeAt_positions") {
//     std::vector<float> refVec = {0.0f};
//     VectorStore store(1, simpleEmbedding, refVec);
    
//     for (int i = 0; i <= 5; i++) {
//         store.addText(std::to_string(i));
//     }
    
//     // Remove from middle
//     CHECK(store.removeAt(2) == true);
//     CHECK(store.size() == 5);
    
//     // Remove from end
//     CHECK(store.removeAt(4) == true);
//     CHECK(store.size() == 4);
    
//     // Remove from beginning
//     CHECK(store.removeAt(0) == true);
//     CHECK(store.size() == 3);
// }

// // ========================================
// // Test distance metrics edge cases
// // ========================================
// TEST_CASE("test_307_distance_metrics") {
//     std::vector<float> refVec = {0.0f, 0.0f};
//     VectorStore store(2, simpleEmbedding, refVec);
    
//     std::vector<float> v1 = {0.0f, 0.0f};
//     std::vector<float> v2 = {0.0f, 0.0f};
    
//     // Distance between identical vectors
//     CHECK(store.l2Distance(v1, v2) == 0.0);
//     CHECK(store.l1Distance(v1, v2) == 0.0);
    
//     std::vector<float> v3 = {1.0f, 0.0f};
//     std::vector<float> v4 = {1.0f, 0.0f};
    
//     // Cosine similarity of identical vectors
//     double sim = store.cosineSimilarity(v3, v4);
//     CHECK(std::abs(sim - 1.0) < 1e-6);
// }

// TEST_CASE("test_309_edge_case_empty_vector") {
//     std::vector<float> refVec = {0.0f};
//     VectorStore store(1, simpleEmbedding, refVec);

//     // Adding an empty vector and checking the size
//     store.addText(""); // Should work and add an empty vector
//     CHECK(store.size() == 1);
//     CHECK(store.getAverageDistance() == 0.0);
// }

// TEST_CASE("test_310_range_query_no_results") {
//     std::vector<float> refVec = {0.0f};
//     VectorStore store(1, simpleEmbedding, refVec);

//     store.addText("1");
//     store.addText("5");
//     store.addText("10");

//     // Query range that should not return any results
//     int* result = store.rangeQuery({20.0f}, 5.0, "l2");

//     // Kiểm tra xem kết quả trả về có phải là nullptr không khi không có kết quả
//     CHECK(result == nullptr);  // Nếu không có kết quả, phải trả về nullptr
// }


// TEST_CASE("test_311_adding_large_vector") {
//     std::vector<float> refVec = {0.0f};
//     VectorStore store(1000, simpleEmbedding, refVec); // Larger dimension

//     // Adding a large vector
//     std::string longText = "a very long string for testing";
//     store.addText(longText); // Should handle large text properly
//     CHECK(store.size() == 1);
// }

// TEST_CASE("test_312_rebalancing_after_multiple_deletions") {
//     std::vector<float> refVec = {0.0f};
//     VectorStore store(1, simpleEmbedding, refVec);

//     store.addText("1"); // ID=1
//     store.addText("2"); // ID=2
//     store.addText("3"); // ID=3
//     store.addText("4"); // ID=4

//     store.removeAt(0); // Remove ID=1
//     store.removeAt(1); // Remove ID=2

//     CHECK(store.size() == 2);
//     CHECK(store.getAverageDistance() == 3.0); // Điều chỉnh giá trị kỳ vọng
// }

// TEST_CASE("test_313_remove_and_rebuild_root") {
//     std::vector<float> refVec = {0.0f};
//     VectorStore store(1, simpleEmbedding, refVec);

//     store.addText("1");  // ID=1
//     store.addText("2");  // ID=2
//     store.addText("3");  // ID=3

//     store.removeAt(0);  // Xóa phần tử với ID=1

//     // Sử dụng rebuildRootForTest() để cập nhật rootVector
//     VectorRecord* newRoot = store.getVector(2);  // Giả sử ID=3 là root mới (sau khi xóa ID=1)
    
//     store.rebuildRootForTest();  // Gọi phương thức thay thế root trong test case

//     // Kiểm tra lại gốc cây
//     CHECK(store.getRootVector()->id == 3);  // Sau khi rebuild, rootVector phải là ID=3
// }
