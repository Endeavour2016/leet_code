#include <string>
using namespace std;

/**
 * @brief: 125. 验证回文串
 * Given a string, determine if it is a palindrome, considering only
 * alphanumeric characters and ignoring cases.
 * 如果在将所有大写字符转换为小写字符、并移除所有非字母数字字符之后，短语正着读和反着读都一样
 * 则可以认为该短语是一个回文串
 */
class Solution {
public:
    bool isPalindrome(string s) {
        if (s.empty())
            return true;
        int left = 0, right = s.size()-1;
        while (left < right) {
            if (!isLetterOrDigit(s[left]))
                left++;
            else if (!isLetterOrDigit(s[right]))
                right--;
            else {
                if (!isSame(s[left], s[right])) {
                    return false;
                }
                left++;
                right--;
            }
        }
        return true;
    }

    bool isLetterOrDigit(char ch) {
        bool is_letter = (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
        bool is_digit = (ch >= '0' && ch <= '9');
        return (is_letter || is_digit);
    }
    // 判断ch1和ch2是否相同
    bool isSame(char ch1, char ch2) {
        // 如果有数字的话，直接进行比较
        if ((ch1 >= '0' && ch1 <= '9') || (ch2 >= '0' && ch2 <= '9'))
            return ch1 == ch2;
        else 
            return (abs(ch1 - ch2) == 32) || (ch1 == ch2);
    }
};

// 官方解法
bool isPalindrome2(string s) {
    string sgood;
    for (char ch: s) {
        if (isalnum(ch)) {
            sgood += tolower(ch);
        }
    }
    int n = sgood.size();
    int left = 0, right = n - 1;
    while (left < right) {
        if (sgood[left] != sgood[right]) {
            return false;
        }
        ++left;
        --right;
    }
    return true;
}


