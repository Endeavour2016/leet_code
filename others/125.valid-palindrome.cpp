/*
 * @lc app=leetcode id=125 lang=cpp
 *
 * [125] Valid Palindrome
 *
 * https://leetcode.com/problems/valid-palindrome/description/
 *
 * algorithms
 * Easy (30.87%)
 * Total Accepted:    346.1K
 * Total Submissions: 1.1M
 * Testcase Example:  '"A man, a plan, a canal: Panama"'
 *
 * Given a string, determine if it is a palindrome, considering only
 * alphanumeric characters and ignoring cases.
 * 
 * Note: For the purpose of this problem, we define empty string as valid
 * palindrome.
 * 
 * Example 1:
 * 
 * 
 * Input: "A man, a plan, a canal: Panama"
 * Output: true
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: "race a car"
 * Output: false
 * 
 * 
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
                if (!isSame(s[left], s[right]))
                    return false;
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

