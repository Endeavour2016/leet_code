/**
 * @file 10_posting_compression.cpp
 * @brief 实现文档编号的差分压缩和变长整数编解码。
 * @details 先计算相邻编号的差值，再按每字节 7 位数据编码；解码时检查截断和溢出。
 * @note 文档编号必须严格递增，首项可以为 0；不实现磁盘文件格式。
 * @see notes/engineering/10_posting_compression.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_posting_compression
 */

#include <cstddef>
#include <cstdint>
#include <random>
#include <set>
#include <vector>

#include "../common/check.h"
#include "../common/posting.h"

typedef std::vector<std::uint8_t> Bytes;

// 区分输入被截断、数值溢出、编号无序和读取位置无效，便于调用方处理。
enum class CodecStatus {
  kOk,
  kTruncated,
  kOverflow,
  kNotIncreasing,
  kInvalidOffset
};

// 每字节低 7 位保存数据，最高位为 1 表示后面还有字节。
void encode_varint(std::uint32_t value, Bytes& bytes) {
  while (value >= 128) {
    bytes.push_back(static_cast<std::uint8_t>((value & 127) | 128));
    value >>= 7;
  }
  bytes.push_back(static_cast<std::uint8_t>(value));
}

// 用局部位置试读；成功后才更新 pos 和 output，失败不消耗输入。
CodecStatus decode_varint(const Bytes& bytes,
                          std::size_t& pos,
                          std::uint32_t& output) {
  if (pos > bytes.size()) {
    return CodecStatus::kInvalidOffset;
  }
  std::size_t cursor = pos;
  std::uint32_t value = 0;
  for (unsigned i = 0; i < 5; ++i) {
    if (cursor == bytes.size()) {
      return CodecStatus::kTruncated;
    }
    const std::uint8_t byte = bytes[cursor++];
    // uint32_t 最多占 5 字节，第五字节只能使用低 4 位。
    if (i == 4 && (byte & 0xf0)) {
      return CodecStatus::kOverflow;
    }
    value |= std::uint32_t(byte & 127) << (7 * i);
    if (!(byte & 128)) {
      pos = cursor;
      output = value;
      return CodecStatus::kOk;
    }
  }
  return CodecStatus::kOverflow;
}

// 严格递增的编号转换为差值；首项以前一个编号为 0 计算。
CodecStatus compress(const Posting& docs, Bytes& output) {
  Bytes bytes;
  std::uint32_t previous = 0;
  for (std::size_t i = 0; i < docs.size(); ++i) {
    if (i && docs[i] <= previous) {
      return CodecStatus::kNotIncreasing;
    }
    encode_varint(docs[i] - previous, bytes);
    previous = docs[i];
  }
  output.swap(bytes);
  return CodecStatus::kOk;
}

// 差值合法不代表累加结果不会溢出；完整解码成功后才替换输出。
CodecStatus decompress(const Bytes& bytes, Posting& output) {
  Posting docs;
  std::size_t pos = 0;
  std::uint32_t previous = 0;
  while (pos < bytes.size()) {
    std::uint32_t delta = 0;
    const auto status = decode_varint(bytes, pos, delta);
    if (status != CodecStatus::kOk) {
      return status;
    }
    if (!docs.empty() && delta == 0) {
      return CodecStatus::kNotIncreasing;
    }
    if (delta > UINT32_MAX - previous) {
      return CodecStatus::kOverflow;
    }
    previous += delta;
    docs.push_back(previous);
  }
  output.swap(docs);
  return CodecStatus::kOk;
}

// 以下为验证代码；复习时先阅读上方实现，main 检查往返及失败时输出不变。
int main() {
  Bytes bytes;
  Posting restored;
  for (auto docs :
       {Posting(), Posting({0}), Posting({1, 127, 128, 16384, UINT32_MAX})}) {
    CHECK(compress(docs, bytes) == CodecStatus::kOk);
    CHECK(decompress(bytes, restored) == CodecStatus::kOk);
    CHECK(restored == docs);
  }
  std::mt19937 rng(42);
  std::set<std::uint32_t> unique;
  for (int i = 0; i < 10000; ++i) {
    unique.insert(rng());
  }
  Posting docs(unique.begin(), unique.end());
  CHECK(compress(docs, bytes) == CodecStatus::kOk);
  CHECK(decompress(bytes, restored) == CodecStatus::kOk);
  CHECK(restored == docs);

  restored = {42};
  CHECK(decompress({0x80}, restored) == CodecStatus::kTruncated);
  CHECK(decompress({1, 0x80}, restored) == CodecStatus::kTruncated);
  CHECK(decompress({0xff, 0xff, 0xff, 0xff, 0x10}, restored) ==
        CodecStatus::kOverflow);
  CHECK(decompress({0x80, 0x80, 0x80, 0x80, 0x80, 0}, restored) ==
        CodecStatus::kOverflow);
  CHECK(decompress({0xff, 0xff, 0xff, 0xff, 0x0f, 1}, restored) ==
        CodecStatus::kOverflow);
  CHECK(decompress({1, 0}, restored) == CodecStatus::kNotIncreasing);
  CHECK(restored == Posting({42}));
  bytes = {42};
  CHECK(compress({2, 1}, bytes) == CodecStatus::kNotIncreasing);
  CHECK(compress({1, 1}, bytes) == CodecStatus::kNotIncreasing);
  CHECK(bytes == Bytes({42}));

  std::size_t pos = 0;
  std::uint32_t value = 42;
  CHECK(decode_varint({0x80}, pos, value) == CodecStatus::kTruncated);
  CHECK(pos == 0 && value == 42);
  CHECK(decode_varint({0xff, 0xff, 0xff, 0xff, 0x10}, pos, value) ==
        CodecStatus::kOverflow);
  CHECK(pos == 0 && value == 42);
  pos = 2;
  CHECK(decode_varint({0}, pos, value) == CodecStatus::kInvalidOffset);
  CHECK(pos == 2 && value == 42);
  CHECK(decode_varint({0, 0}, pos, value) == CodecStatus::kTruncated);
  CHECK(pos == 2 && value == 42);
}
