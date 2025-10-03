#include <cpr/cpr.h>
#include <fmt/core.h>
#include <fmtlog/fmtlog.h>
#include <pugixml.hpp>
#include <iostream>

int main() {
  fmtlog::setLogLevel(fmtlog::INF);

  logi("Getting Data from RSS");
  auto r = cpr::Get(
      cpr::Url{"https://news.ycombinator.com/rss"});

  logi("Converting rss feed to xml document");
  pugi::xml_document doc;
  auto result = doc.load_string(r.text.c_str());

  if(!result)  {
    loge("Something went really wrong");
    return 1;
  }

  pugi::xpath_node_set titles = doc.select_nodes("/rss/channel/item/title");

  for(pugi::xpath_node title_node: titles) {
    std::cout << "found one!" << "\n";
    pugi::xml_node title = title_node.node();
    std::cout << title.text().get() << "\n";

  }


  return 0;
}
