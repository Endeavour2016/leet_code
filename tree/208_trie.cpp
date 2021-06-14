/*
 * @Author: zlm 
 * @Date: 2021-06-13 21:51:46 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-06-13 22:13:42
 */


#include "../head_file.h"
using namespace std;

/**
 * Trie树，即字典树，又称单词查找树或键树，是一种树形结构，是一种哈希树的变种。
 * 典型应用是用于统计和排序大量的字符串（但不仅限于字符串），所以经常被搜索引擎系统用于文本词频统计。
 * 它的优点是：最大限度地减少无谓的字符串比较，查询效率比哈希表高。
 * Trie的核心思想是空间换时间。利用字符串的公共前缀来降低查询时间的开销以达到提高效率的目的。
 * 它有3个基本性质：
 * 1.根节点不包含字符，除根节点外每一个节点都只包含一个字符。
 * 2.从根节点到某一节点，路径上经过的字符连接起来，为该节点对应的字符串。
 * 3.每个节点的所有子节点包含的字符都不相同。
 */

/**
 * @problem: 208. Implement Trie (Prefix Tree)
 * @descr: 字典树的简单实现
 * @method: 每个节点有26个分支，分别表示字母 a~z, a对应的分支为next_[0], 
 * 则其余字母x的分支next_[x-'a'].
 */ 
class TrieNode {
public:
  TrieNode(bool flag = false) : is_word_(flag), next_{nullptr} {}

public:
  TrieNode* next_[26];  // 每个节点有26个分支，字母种类为26
  bool is_word_ = false;  // 判断root到当前节点是否形成一个单词
};

class Trie {
private: 
  TrieNode *root;

public:
  /** Initialize your data structure here. */
  Trie() : root(new TrieNode()) {}
  
  /** Inserts a word into the trie. */
  void insert(string word) {
    TrieNode* p = root;
    // 依次确定word中每个字母的位置
    for (int i = 0; i < word.size(); i++) {
      // str[i]对应的分支不存在，则创建相应的节点
      if (p->next_[word[i] - 'a'] == nullptr)   // ->和[]优先级相同，这里相当于(p->next_)[i]
        p->next_[word[i] - 'a'] = new TrieNode();
      p = p->next_[word[i] - 'a'];
    }
    
    // 当前word中的字母全部插入后，从root 到 p 节点的路径上已经形成了一个单词
    p->is_word_ = true;
  }
  
  // 判断一个单词时否在该字典树中
  bool search(string word) {
    // 从root节点开始,根据word中的字母确定分支，逐步向下查找
    // 直到找到单词的最后一个字母。
    TrieNode* p = root;
    int len = word.size();
    // p == nullptr时，表明当前字母不存在分支
    // p 移动到当前字母对应分支下的子节点
    for (int i = 0; i < len && p != nullptr; i++) { 
        p = p->next_[word[i] - 'a'];
    }
    
    return p != nullptr && p->is_word_;
  }
  
  // Returns if there is any word in the trie that starts with the given prefix
  bool startsWith(string prefix) {
    // 本题跟search类似，但是只要匹配前缀即可，不需要是一个完整的单词
    // 所以不用考虑is_word_。
    TrieNode *p = root;
    int len = prefix.size();
    for (int i = 0; i < len && p != nullptr; i++) { 
      p = p->next_[prefix[i] - 'a']; 
    }
    return p != nullptr;
  }
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie obj = new Trie();
 * obj.insert(word);
 * bool param_2 = obj.search(word);
 * bool param_3 = obj.startsWith(prefix);
 */

int main() {
  system ("pause");
  return 0;
}