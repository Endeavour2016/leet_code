/**
 * BM74. Restore IP Addresses
 *
 * Backtrack over four IP segments. Choose one to three digits per level and
 * reject leading zeroes or values greater than 255.
 */

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

class Solution {
 public:
  std::vector<std::string> restoreIpAddresses(const std::string& text) {
    answers_.clear();
    segments_.assign(4, 0);
    search(text, 0, 0);
    return answers_;
  }

 private:
  void search(const std::string& text, int segment_id, std::size_t start) {
    if (segment_id == 4) {
      if (start == text.size()) {
        answers_.push_back(std::to_string(segments_[0]) + "." + std::to_string(segments_[1]) + "." +
                           std::to_string(segments_[2]) + "." + std::to_string(segments_[3]));
      }
      return;
    }

    const std::size_t remaining_chars = text.size() - start;
    const std::size_t remaining_segments = 4 - segment_id;
    if (remaining_chars < remaining_segments || remaining_chars > remaining_segments * 3) {
      return;
    }

    if (text[start] == '0') {
      segments_[segment_id] = 0;
      search(text, segment_id + 1, start + 1);
      return;
    }

    int value = 0;
    for (std::size_t end = start; end < text.size() && end < start + 3; ++end) {
      value = value * 10 + (text[end] - '0');
      if (value > 255) {
        break;
      }
      segments_[segment_id] = value;
      search(text, segment_id + 1, end + 1);
    }
  }

  std::vector<std::string> answers_;
  std::vector<int> segments_;
};

int main() {
  Solution solution;
  auto actual = solution.restoreIpAddresses("25525511135");
  std::sort(actual.begin(), actual.end());
  const std::vector<std::string> expected = {"255.255.11.135", "255.255.111.35"};
  assert(actual == expected);
  assert(solution.restoreIpAddresses("0000") == std::vector<std::string>{"0.0.0.0"});
  assert(solution.restoreIpAddresses("1234567890123").empty());
  return 0;
}
