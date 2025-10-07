#include "collector/embeder.h"
#include <llama.h>
#include <stdexcept>
#include <format>
#include <algorithm> 
#include <common.h>

namespace collector {
Embedder::Embedder(const std::string& model_path) {
  llama_backend_init();
  llama_model_params model_params = llama_model_default_params();
  model_ = llama_model_load_from_file(model_path.c_str(), model_params);

  if (!model_) {
    throw std::runtime_error("Model could not be loaded");
  }

  llama_context_params ctx_params = llama_context_default_params();
  ctx_params.n_ctx = llama_model_n_ctx_train(model_);
  ctx_params.n_batch = ctx_params.n_ctx;
  ctx_params.n_ubatch = ctx_params.n_batch;
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

std::vector<float> Embedder::embed(const std::string& text) {
  const int n_max = llama_model_n_ctx_train(model_);
  std::vector<llama_token> tokens = common_tokenize(ctx_, text, true, false);
  const int n_toks = std::min((int)tokens.size(), n_max);

  if(n_toks == 0) {
    throw std::runtime_error("no sequence embedding returned");
  }

  llama_batch batch = llama_batch_init(n_toks, 0, 1);
  for (int i=0; i < n_toks; ++i) {
    common_batch_add(batch, tokens[i], i, {0}, true);
  }
  batch.n_tokens = n_toks;

  llama_memory_clear(llama_get_memory(ctx_), true);
  int rc = llama_decode(ctx_, batch);
  if(rc < 0) {
    throw std::runtime_error("llama decode failed!");
  }

  const float* emb_seq = llama_get_embeddings_seq(ctx_, 0);
  if(emb_seq == nullptr) {
    throw std::runtime_error("no sequence embedding returned");
  }

  std::vector<float> embedding(embedding_dim_); 
  const int euclidean_embedding_norm = 2;
  common_embd_normalize(emb_seq, embedding.data(), embedding_dim_, euclidean_embedding_norm);
  llama_batch_free(batch);

  return embedding;
}
int Embedder::get_embedding_dim() const {
  return embedding_dim_;
}
} // namespace collector
