#include <gtest/gtest.h>
#include "collector/embeder.h"
#include <filesystem>
#include <numeric>
#include <cmath> 
#include <string>
#include <vector>

TEST(EmbedderTest, BasicEmbedding) {
    const std::string model_path = "model/bge-m3-Q8_0.gguf";
    ASSERT_TRUE(std::filesystem::exists(model_path)) << "Model file not found: " << model_path;

  collector::Embedder embedder(model_path);

    const std::string text = "The quick brown fox jumps over the lazy dog.";
    auto emb = embedder.embed(text);

    ASSERT_FALSE(emb.empty());
    EXPECT_EQ(emb.size(), embedder.get_embedding_dim()) << "Embedding dimension mismatch";
    EXPECT_NEAR(std::sqrt(std::inner_product(emb.begin(), emb.end(), emb.begin(), 0.0f)), 1.0f, 1e-2)
        << "Embedding not normalized to unit length";
}

