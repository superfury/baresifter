#pragma once

#include <cstdint>

// A raw set of bytes representing an instruction (potentially).
struct instruction_bytes {
  // x86 instructions are at most 15 bytes long.
  uint8_t raw[15];

  template <typename... T>
  constexpr instruction_bytes(T... v)
  : raw {(uint8_t)v...}
  {}
};

class prefix_group_lut {
public:
    int8_t data[256];

    prefix_group_lut(size_t detect_prefixes_); //Prototype!
};

class search_engine {
  instruction_bytes current_;
  size_t increment_at_ = 0;

  const size_t max_prefixes_; //How many prefixes to use at once.
  const size_t used_prefixes_; //What prefixes to scan through.
  prefix_group_lut group_lut_; //What group lut to use!

public:

  // Find the next candidate for an interesting instruction. Returns false, if
  // the search is done.
  bool find_next_candidate();

  // Reset the incrementing position after an interesting instruction was found.
  void start_over(size_t length);

  // Clear any bytes after the given position.
  void clear_after(size_t pos);

  // Return the current instruction candidate.
  instruction_bytes const &get_candidate() const
  {
    return current_;
  }

  search_engine(size_t max_prefixes = 0, size_t used_prefixes = 0xFF, size_t detect_prefixes = 0xFF, instruction_bytes const &start = {})
      : current_(start), max_prefixes_(max_prefixes), used_prefixes_(used_prefixes), group_lut_(detect_prefixes)
  {}
};
