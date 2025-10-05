#include "collector/article_repository.h"
#include "collector/feed_reader.h"
#include <cstdlib>
#include <format>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

int main() {
  try {
    // Set up file logger for fetcher
    auto logger = spdlog::basic_logger_mt("fetcher", "fetcher.log");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);

    spdlog::info("Starting article fetcher");

    // Get database credentials from environment
    const char *db_user = std::getenv("DB_USER");
    const char *db_password = std::getenv("DB_PASSWORD");

    if (!db_user || !db_password) {
      spdlog::error(
          "DB_USER and DB_PASSWORD environment variables must be set");
      return 1;
    }

    // Connect to database
    std::string conn_str =
        std::format("host=localhost dbname=collector user={} password={}",
                    db_user, db_password);

    ArticleRepository repo(conn_str);
    spdlog::info("Connected to database");

    // Hardcoded feed URLs (just Hacker News for now)
    std::vector<std::string> feed_urls = {
        "https://feeds.arstechnica.com/arstechnica/index",
        "https://www.theverge.com/rss/index.xml",
        "https://techcrunch.com/feed/",
        "https://www.technologyreview.com/feed/"};

    int total_fetched = 0;
    int total_inserted = 0;
    int total_duplicates = 0;
    int total_errors = 0;

    // Process each feed
    for (const auto &feed_url : feed_urls) {
      try {
        FeedReader reader(feed_url);
        spdlog::info("Fetching feed: {}", feed_url);
        auto articles = reader.fetch();
        total_fetched += articles.size();

        spdlog::info("Found {} articles in feed", articles.size());

        // Try to insert each article
        for (auto &article : articles) {
          try {
            // Set source to the feed URL
            article.source = feed_url;
            article.processing_state = ProcessingState::unprocessed;

            int id = repo.insert_article(article);
            if (id == -1) {
              total_duplicates++;
              spdlog::debug("Duplicate article skipped: {}", article.url);
            } else {
              total_inserted++;
              spdlog::info("Inserted article ID {}: {}", id, article.title);
            }
          } catch (const std::exception &e) {
            total_errors++;
            spdlog::error("Failed to insert article '{}': {}", article.title,
                          e.what());
            // Continue with next article
          }
        }

      } catch (const std::exception &e) {
        total_errors++;
        spdlog::error("Failed to fetch feed '{}': {}", feed_url, e.what());
        // Continue with next feed
      }
    }

    // Summary
    spdlog::info("Fetcher complete: {} fetched, {} inserted, {} duplicates, {} "
                 "errors",
                 total_fetched, total_inserted, total_duplicates, total_errors);

    return total_errors > 0 ? 1 : 0;

  } catch (const std::exception &e) {
    spdlog::error("Fatal error: {}", e.what());
    return 1;
  }
}
