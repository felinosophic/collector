#pragma once
#include <string>
#include <vector>

// Forward declarations
struct llama_model;
struct llama_context;

namespace collector {

class Embedder {
public:
    explicit Embedder(const std::string& model_path);
    ~Embedder();
    
    // Non-copyable
    Embedder(const Embedder&) = delete;
    Embedder& operator=(const Embedder&) = delete;
    
    // Generate embedding for text
    std::vector<float> embed(const std::string& text);
    
    // Get embedding dimension
    int get_embedding_dim() const;
    
private:
    llama_model* model_;
    llama_context* ctx_;
    int embedding_dim_;
};

} // namespace collector
