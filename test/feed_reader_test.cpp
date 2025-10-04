#include "collector/feed_reader.h" 
#include <gtest/gtest.h>
#include <string>

TEST(FeedReaderIntegrationTest, CanFetchKnownRssFeed) {
  const std::string url = "https://news.ycombinator.com/rss";
  size_t n = 0;

  try {
    FeedReader feed(url);
    auto articles = feed.fetch();
    n = articles.size();
  } catch (const std::runtime_error &e) {
    FAIL() << "get_rss threw an exception: " << e.what();
  }

  ASSERT_TRUE(n > 0);
}
