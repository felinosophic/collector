#pragma once

#include <optional>
#include <string>
#include <vector>
namespace collector {
enum ProcessingState { unprocessed, embedded, classified };

enum RelevanceRating { very_unlikely, unlikely, likely, very_likely, unsure };

std::string to_string(ProcessingState state);
std::string to_string(RelevanceRating rating);
ProcessingState state_from_string(std::string_view str);
RelevanceRating rating_from_string(std::string_view str);

struct Article {
  std::optional<int> id;
  std::string title = "";
  std::string url = "";
  std::string summary = "";
  std::string content = "";
  std::string published_date = "";
  std::string source = "";
  std::optional<std::vector<float>> embedding;
  ProcessingState processing_state = ProcessingState::unprocessed;
  std::optional<RelevanceRating> relevance_rating;
  std::optional<std::string> relevance_reason;
  std::optional<int> duplicate_of_id;
};
} // namespace collector
