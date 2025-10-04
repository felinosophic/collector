#include "collector/feed_reader.h"
#include <iostream>

int main() {
  FeedReader feed("https://news.ycombinator.com/rss");
  auto articles = feed.fetch();

  for(const auto& article: articles) {
    std::cout << article.title << "\n";
  }

}
