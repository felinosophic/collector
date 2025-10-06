#include "collector/embeder.h"
#include "llama.h"
#include <stdexcept>

namespace collector {
Embedder::Embedder(const std::string& model_path) {
  llama_backend_init();
  llama_model_params model_params = llama_model_default_params();
  model_ = llama_model_load_from_file(model_path.c_str(), model_params);

  if (!model_) {
    throw std::runtime_error("Model could not be loaded");
  }

  llama_context_params ctx_params = llama_context_default_params();
  ctx_params.n_ctx = 512;
  ctx_params.n_batch = ctx_params.n_ctx;
  ctx_params.embeddings = true;
  ctx_params.pooling_type = LLAMA_POOLING_TYPE_MEAN;
  ctx_ = llama_init_from_model(model_, ctx_params);
  if(!ctx_) {
    llama_model_free(model_);
    throw std::runtime_error("Context could not be created");
  } 
  embedding_dim_ = llama_model_n_embd(model_);
}
Embedder::~Embedder() {
  if(ctx_) {
    llama_free(ctx_);
  }
  if(model_) {
    llama_model_free(model_);
  }
  llama_backend_free();
}
} // namespace collector
