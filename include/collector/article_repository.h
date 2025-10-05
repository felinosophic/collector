#pragma once
#include "collector/article.h"
#include <pqxx/pqxx>
#include <vector>

class ArticleRepository {
public:
  ArticleRepository(std::string_view connection_string)
      : conn(std::string(connection_string)) {
    if (!conn.is_open()) {
      throw std::runtime_error("Can't connect to database!");
    }
  };
  int insert_article(const Article &article);
  std::vector<Article> get_articles_by_state(ProcessingState state);
  int update_state(int id, ProcessingState new_state);
  int update_embedding(int id, const std::vector<float>& embedding);
  bool article_exists(std::string_view url);

private:
  pqxx::connection conn;
  pqxx::result run_sql(const std::string& sql, const pqxx::params& params);
};
