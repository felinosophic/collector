#include "collector/article.h"
#include <stdexcept>

namespace collector {
std::string to_string(ProcessingState state) {
  switch (state) {
  case ProcessingState::unprocessed:
    return "unprocessed";
  case ProcessingState::embedded:
    return "embedded";
  case ProcessingState::classified:
    return "classified";
  }
  throw std::runtime_error("Unknown ProcessingState");
}

std::string to_string(RelevanceRating rating) {
  switch (rating) {
  case RelevanceRating::very_unlikely:
    return "very_unlikely";
  case RelevanceRating::unlikely:
    return "unlikely";
  case RelevanceRating::likely:
    return "likely";
  case RelevanceRating::very_likely:
    return "very_likely";
  case RelevanceRating::unsure:
    return "unsure";
  }
  throw std::runtime_error("Unknown RelevanceRating");
}

ProcessingState state_from_string(std::string_view str) {
  if (str == "unprocessed")
    return ProcessingState::unprocessed;
  if (str == "embedded")
    return ProcessingState::embedded;
  if (str == "classified")
    return ProcessingState::classified;
  throw std::runtime_error("Unknown processing state: " + std::string(str));
}

RelevanceRating rating_from_string(std::string_view str) {
  if (str == "very_unlikely")
    return RelevanceRating::very_unlikely;
  if (str == "unlikely")
    return RelevanceRating::unlikely;
  if (str == "likely")
    return RelevanceRating::likely;
  if (str == "very_likely")
    return RelevanceRating::very_likely;
  if (str == "unsure")
    return RelevanceRating::unsure;
  throw std::runtime_error("Unknown relevance rating: " + std::string(str));
}
} // namespace collector
