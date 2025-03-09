#include "ladder.h"

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << endl;
    cerr << "Word 1: " << word1 << endl;
    cerr << "Word 2: " << word2 << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    if (abs(static_cast<int>(str1.size()) - static_cast<int>(str2.size())) > d) {
        return false;
    }
    
    int m = str1.size();
    int n = str2.size();
    
    if (m == 0) return n <= d;
    if (n == 0) return m <= d;
    
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));
    
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + min({dp[i - 1][j],
                                    dp[i][j - 1],
                                    dp[i - 1][j - 1]
                                   });
            }
        }
    }
    
    return dp[m][n] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are the same!");
        return {};
    }
    
    if (word_list.find(end_word) == word_list.end()) {
        error(begin_word, end_word, "End word is not in the dictionary!");
        return {};
    }
    
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    
    set<string> visited;
    visited.insert(begin_word);
    
    const int MAX_LADDER_LENGTH = 20;
    
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();
        
        if (ladder.size() > MAX_LADDER_LENGTH) {
            continue;
        }
        
        string last_word = ladder.back();
        
        for (size_t i = 0; i < last_word.length(); i++) {
            string new_word = last_word;
            for (char c = 'a'; c <= 'z'; c++) {
                new_word[i] = c;
                if (word_list.find(new_word) != word_list.end() && visited.find(new_word) == visited.end()) {
                    visited.insert(new_word);
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(new_word);
                    
                    if (new_word == end_word) {
                        return new_ladder;
                    }
                    
                    ladder_queue.push(new_ladder);
                }
            }
        }
        
        for (size_t i = 0; i <= last_word.length(); i++) {
            string new_word = last_word;
            new_word.insert(i, 1, ' ');
            for (char c = 'a'; c <= 'z'; c++) {
                new_word[i] = c;
                if (word_list.find(new_word) != word_list.end() && visited.find(new_word) == visited.end()) {
                    visited.insert(new_word);
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(new_word);
                    
                    if (new_word == end_word) {
                        return new_ladder;
                    }
                    
                    ladder_queue.push(new_ladder);
                }
            }
        }
        
        for (size_t i = 0; i < last_word.length(); i++) {
            string new_word = last_word;
            new_word.erase(i, 1);
            if (word_list.find(new_word) != word_list.end() && visited.find(new_word) == visited.end()) {
                visited.insert(new_word);
                vector<string> new_ladder = ladder;
                new_ladder.push_back(new_word);
                
                if (new_word == end_word) {
                    return new_ladder;
                }
                
                ladder_queue.push(new_ladder);
            }
        }
    }
    
    error(begin_word, end_word, "No word ladder exists between these words!");
    return {};
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file) {
        cerr << "Error: Could not open file " << file_name << endl;
        return;
    }
    
    string word;
    while (file >> word) {
        for (char& c : word) {
            c = tolower(c);
        }
        word_list.insert(word);
    }
    
    file.close();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i < ladder.size() - 1) {
            cout << " ";
        }
    }
    cout << " " << endl;
}

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    
    #define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}
    
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}