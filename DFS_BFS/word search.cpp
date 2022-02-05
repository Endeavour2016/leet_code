#include "../head_file.h"
using namespace std;

// 2019-3-16 Satur
// 方法：深度优先搜索
class Solution {
public:
  bool exist(vector<vector<char>>& board, string word) {
    int rows = board.size();
    int cols = board[0].size();
    vector<vector<bool>> isVisited(rows, vector<bool>(cols, false));
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
    bool dfs(const vector<vector<char>> &board, string &word, 
        int index, int r, int c, vector<vector<bool>> &isVisited) {
        // 终止条件(根据dfs参数顺序判断终止条件)
        if (index == word.size())
            return true;
        
        if (r < 0 || c < 0 || r >= board.size() || c >= board[0].size())
            return false; // 边界条件
        
        if (isVisited[r][c] || board[r][c] != word[index])
            return false;
      
        isVisited[r][c] = true;           

        // 递归的从搜索上下左右四个邻接节点
        bool res = dfs(board, word, index + 1, r + 1, c, isVisited) || 
            dfs(board, word, index + 1, r - 1, c, isVisited) ||
            dfs(board, word, index + 1, r, c - 1, isVisited) ||
            dfs(board, word, index + 1, r, c + 1, isVisited);
        isVisited[r][c] = false;
        return res;
    }
};

int main() {
  return 0;
}