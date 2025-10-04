#include "collector/feed_reader.h"
#include <cpr/cpr.h>
#include <fmt/core.h>
#include <pugixml.hpp>
#include <stdexcept>
namespace {

std::string get_rss(const std::string &url) {
  auto r = cpr::Get(cpr::Url{url});
  if (r.status_code == 0 || r.status_code >= 400) {
    std::string error = fmt::format("Feed Url {} could not be reached", url);
    throw std::runtime_error(error);
  }
  return r.text;
}

pugi::xml_document get_xml_doc(const std::string &rss) {
  pugi::xml_document doc;
  const auto result = doc.load_string(rss.c_str());
  if (!result) {
    std::string error =
        fmt::format("XML document couldn't be parsed: {} (at offset {})",
                    result.description(), result.offset);
    throw std::runtime_error(error);
  }
  return doc;
}

Article convert_node_to_article(const pugi::xpath_node item,
                                const std::string &source) {
  auto item_node = item.node();
  Article article;
  article.title = item_node.child("title").text().get();
  article.url = item_node.child("link").text().get();
  article.published_date = item_node.child("pubDate").text().get();
  article.summary = item_node.child("description").text().get();
  article.source = item_node.child("description").text().get();

  return article;
}

} // namespace

FeedReader::FeedReader(std::string_view url) { feed_url_ = url; }
std::vector<Article> FeedReader::fetch() const {
  const auto rss = get_rss(feed_url_);
  const auto doc = get_xml_doc(rss);

  pugi::xpath_node_set items = doc.select_nodes("/rss/channel/item");
  std::vector<Article> articles;
  articles.reserve(items.size());

  for (pugi::xpath_node item : items) {
    articles.emplace_back(convert_node_to_article(item, feed_url_));
  }
  return articles;
}
