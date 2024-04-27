/*
 * @Author: zlm 
 * @Date: 2024-03-31
 * @Last Modified by: zlm
 * @Last Modified time: 2024-03-31
 */

#include <iostream>
#include <vector>
#include <queue>
#include "../data_structure.h"

using namespace std;

/**
 * @problem: 1268.搜索推荐系统
 * @descr: 给你一个产品数组 products 和一个字符串 searchWord ，products  数组中每个产品都是一个字符串。
 * 请你设计一个推荐系统，在依次输入单词 searchWord 的每一个字母后，
 * 推荐 products 数组中前缀与searchWord相同的最多三个产品。
 * 如果前缀相同的可推荐产品超过三个，请按字典序返回最小的三个。
 * 请你以二维列表的形式，返回在输入 searchWord 每个字母后相应的推荐产品的列表。
 * @method: 
 * 时间复杂度O(), 空间复杂度 O()
 **/

// 例子
// 输入：products = ["mobile","mouse","moneypot","monitor","mousepad"], searchWord = "mouse"
// 输出：[
//  输入m后，从products数组中找出前缀为m的字符串   ["mobile","moneypot","monitor"],
//  输入mo后，从products数组中找出前缀为mo的字符串 ["mobile","moneypot","monitor"],
//  前缀为mou的字符串：  ["mouse","mousepad"],
//  前缀为mous的字符串： ["mouse","mousepad"],
//  前缀为mouse的字符串：["mouse","mousepad"]
// ]

// 参考官方题解
// 方法一：字典树
// 由于我们需要将 searchWord 的前缀与 products 中的字符串进行匹配，
// 因此我们可以使用字典树（Trie）来存储 products 中的所有字符串。
// 这样以来，当我们依次输入 searchWord 中的每个字母时，我们可以从字典树的根节点开始向下查找，
// 判断是否存在以当前的输入为前缀的字符串，并找出字典序最小的三个（若存在）字符串。

// 对于字典树中的每个节点，我们需要额外地存储一些数据来帮助我们快速得到答案。
// 设字典树中的某个节点为 node，从字典树的根到该节点对应的字符串为 prefix，
// 那么我们可以在 node 中使用数组或优先队列，存放字典序最小的三个以 prefix 为前缀的字符串。
// 具体的做法是：当我们在字典树中插入字符串 product 并遍历到节点 node 时，我们将 product 存储在 node 中，
// 若此时 node 中的字符串超过三个，就丢弃字典序最大的那个字符串。
// 这样在所有的字符串都被插入到字典树中后，字典树中的节点 node 就存放了当输入为 prefix 时应该返回的那些字符串。


struct TrieNode {
  unordered_map<char, TrieNode*> child;
  // 从root到当前节点对应的字符串记为prefix
  // 则words存放的是所有以prefix为前缀的字符串
  // 这里默认采用less<string>进行比较，内部为大顶堆结构
  priority_queue<string> words;
};

/*
      root
    /       \  
  {ch1}   {ch2}
*/


class Solution {
 private:
  void addWord(TrieNode* root, const string& word) {
    TrieNode* cur = root;
    for (const char& ch: word) {
      if (!cur->child.count(ch)) {
        cur->child[ch] = new TrieNode();
      }
      cur = cur->child[ch];
      cur->words.push(word);         // 记录当前node对应的字符串
      if (cur->words.size() > 3) {  // 保证堆里面保留字典序最小的三个字符串
          cur->words.pop();
      }
    }
  }

 public:
  vector<vector<string>> suggestedProducts(vector<string>& products, string searchWord) {
    // 1、构造字典树
    TrieNode* root = new TrieNode();
    for (const string& word: products) {
      addWord(root, word);
    }
    
    vector<vector<string>> ans;
    TrieNode* cur = root;  // 记录已经遍历到的node
    // 记录当前的前缀能否匹配到字符串，比如输入为 'm', 's', 'u'
    // 假设当输入为's'时，前缀'ms'不存在，显然前缀'msu'也不可能存在，后续所有字符对应的前缀也不可能存在
    // 此时设置flag为true，表示此时已经前缀已经无法匹配，直接返回对应的空结果即可
    bool flag = false;
    for (const char& ch: searchWord) {
      if (flag || !cur->child.count(ch)) {
        ans.emplace_back();
        flag = true;
      } else {
        cur = cur->child[ch];
        vector<string> selects;

        // 当前node对应的字符串集合均包含前缀prefix（从root到当前node路径对应的字符串）
        while (!cur->words.empty()) {
          selects.push_back(cur->words.top());
          cur->words.pop();
        }
        reverse(selects.begin(), selects.end());
        ans.push_back(move(selects));
      }
    }
    
    return ans;
  }
};

int main() {
  return 0;
}