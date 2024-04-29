/*
 * @Author: zlm 
 * @Date: 2019-03-16
 * @Last Modified by: zlm
 * @Last Modified time: 2024-04-29
 */

#include "../head_file.h"
using namespace std;


/**
 * @problem: 79. word search
 * @descr: 给定一个 m x n 二维字符网格 board 和一个字符串单词 word 。如果 word 存在于网格中，返回 true ；
 * 否则，返回 false；单词必须按照字母顺序，通过相邻的单元格内的字母构成，其中“相邻”单元格是那些水平相邻或垂直相邻的单元格。
 * 同一个单元格内的字母不允许被重复使用。
 * @method: DFS+剪枝
 * 分析：本题等效为在字符数组board中找到一条连续的“路径”，使得该路径上的字符串跟word匹配。
 *   1.涉及到路径的问题可以考虑用DFS
 *   2.搜索路径的过程中可以考虑剪枝：遇到重复的节点或者当前路径不可能匹配，应该立即返回，从而避免不要的搜索分支（递归终止条件）
 * 参考：https://leetcode.cn/problems/word-search/solutions/2361646/79-dan-ci-sou-suo-hui-su-qing-xi-tu-jie-5yui2/
 */

class Solution {
public:
  bool exist(vector<vector<char>>& board, string word) {
    int rows = board.size();
    int cols = board[0].size();
    vector<vector<bool>> isVisited(rows, vector<bool>(cols, false));
    // 遍历每一个字符，判断从某个字符作为起点能否找到匹配单词的“路径”
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (dfs(board, word, 0, i, j, isVisited)) {
          return true;
        }
      }
    }
    return false;
}
    
private:
    /**
     * @param: index 当前匹配的目标字符在word中的下标
     *         r, c 当前字符在board中的行和列下标
     *         isVisited 记录每个字符是否已经被访问过
     */
    bool dfs(const vector<vector<char>> &board, string &word, 
        int index, int r, int c, vector<vector<bool>> &isVisited) {
        // 终止条件-已经匹配到word
        if (index == word.size()) {
            return true;
        }

        // 终止条件-当前元素下标无效
        if (r < 0 || c < 0 || r >= board.size() || c >= board[0].size()) {
            return false;
        }

        if (isVisited[r][c] || board[r][c] != word[index]) {
            return false;
        }

        // 访问当前字符     
        isVisited[r][c] = true;           

        // 递归的搜索上下左右四个邻接节点
        // 这里表达式用||连接，表示只要前面有条件为true（找到了可行路径），则直接返回true，
        // || 后面的条件就不用判断了，即不用做后面的DFS
        bool res = dfs(board, word, index + 1, r + 1, c, isVisited) || 
                       dfs(board, word, index + 1, r - 1, c, isVisited) ||
                       dfs(board, word, index + 1, r, c - 1, isVisited) ||
                       dfs(board, word, index + 1, r, c + 1, isVisited);

        // 当前字符的访问状态复原（实际是为了回溯，保证下次for循环从其它的字符作为起点开始搜索路径时，
        // 能正常访问到当前字符）
        isVisited[r][c] = false;
        return res;
    }
};

int main() {
  return 0;
}