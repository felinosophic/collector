#include "collector/article_repository.h"
#include "collector/article.h"
#include <exception>
#include <format>
#include <sstream>
#include <stdexcept>

namespace {
std::string to_pgvector_string(const std::vector<float>& vec) {
  if (vec.empty()) {
    return "[]";
  }
  std::stringstream ss;
  ss << "[";
  for (size_t i = 0; i < vec.size(); ++i) {
    ss << vec[i];
    if (i < vec.size() - 1) {
      ss << ",";
    }
  }
  ss << "]";
  return ss.str();
}
} // namespace

namespace collector {
pqxx::result ArticleRepository::run_sql(const std::string& sql,
                                        const pqxx::params& params) {
  try {
    pqxx::work tx(conn);
    pqxx::result result = tx.exec(sql, params);
    tx.commit();
    return result;
  } catch (std::exception const& e) {
    throw std::runtime_error(std::format("DB statement failed: {}", e.what()));
  }
}

bool ArticleRepository::article_exists(std::string_view url) {
  const std::string sql = "SELECT id FROM articles WHERE url = $1";
  const pqxx::params params{url};
  auto result = run_sql(sql, params);
  return !result.empty();
}

std::vector<Article>
ArticleRepository::get_articles_by_state(ProcessingState state) {
  const std::string sql =
      "SELECT id, title, url, summary, content, published_date, source, "
      "processing_state, relevance_rating, relevance_reason, duplicate_of_id "
      "FROM articles WHERE processing_state = $1";
  const pqxx::params params{to_string(state)};

  auto result = run_sql(sql, params);
  std::vector<Article> articles;
  for (const auto& row : result) {
    Article article;
    article.id = row["id"].as<int>();
    article.title = row["title"].as<std::string>();
    article.url = row["url"].as<std::string>();
    article.summary = row["summary"].as<std::string>();
    article.content = row["content"].as<std::string>();
    article.published_date = row["published_date"].as<std::string>();
    article.source = row["url"].as<std::string>();
    article.processing_state =
        state_from_string(row["processing_state"].as<std::string>());

    // Handle optional fields
    if (!row["relevance_rating"].is_null()) {
      article.relevance_rating =
          rating_from_string(row["relevance_rating"].as<std::string>());
    }
    if (!row["relevance_reason"].is_null()) {
      article.relevance_reason = row["relevance_reason"].as<std::string>();
    }
    if (!row["duplicate_of_id"].is_null()) {
      article.duplicate_of_id = row["duplicate_of_id"].as<int>();
    }

    articles.push_back(article);
  }
  return articles;
}

int ArticleRepository::insert_article(const Article& article) {
  if (article_exists(article.url)) {
    return -1;
  }
  const std::string sql = "INSERT INTO articles "
                          "(title, url, summary, content, published_date, "
                          "url, processing_state) "
                          "VALUES ($1, $2, $3, $4, $5, $6, $7) "
                          "RETURNING id";
  pqxx::params params{article.title,
                      article.url,
                      article.summary,
                      article.content,
                      article.published_date,
                      article.source,
                      to_string(article.processing_state)};

  auto result = run_sql(sql, params);
  if (result.empty()) {
    throw std::runtime_error("INSERT failed");
  }

  return result[0][0].as<int>();
}

int ArticleRepository::update_state(int id, ProcessingState new_state) {
  const std::string sql = "UPDATE articles "
                          "SET processing_state  = $1 "
                          "WHERE id = $2";

  const pqxx::params params{to_string(new_state), id};

  auto result = run_sql(sql, params);

  return result.affected_rows();
}

int ArticleRepository::update_embedding(int id,
                                        const std::vector<float>& embedding) {

  const std::string sql = "UPDATE articles "
                          "SET embedding  = $1 "
                          "WHERE id = $2";

  const pqxx::params params{to_pgvector_string(embedding), id};

  auto result = run_sql(sql, params);

  return result.affected_rows();
}
} // namespace collector
