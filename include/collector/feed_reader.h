#pragma once
#include "article.h"
#include <vector>
namespace collector {
class FeedReader {
public:
  FeedReader(std::string_view feed_url);
  std::vector<Article> fetch() const;

private:
  std::string feed_url_;
};
} // namespace collector
