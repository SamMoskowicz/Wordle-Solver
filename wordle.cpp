#include <bits/stdc++.h>
#include "allWords.h"
#include "hiddenWords.h"
// char *hiddenWords = allWords;

int compare(char *word, char *hidden);
std::string decode(int mask);
int encode(std::string &s);
void makeWord(int i, char *word, char *res);
template <class T>
int compareArrays(T *arr1, T *arr2, int len);
bool matches(char *word, char *query, int comp);
void updateRemaining(int comp, char *query);
int compareWords(int i1, int i2, char *words);
void reverseRemaining(int lenBefore);
std::vector<int> dfs(int level, char *word, int turn, int mn, int mx);
bool isInOrder(char *words, int len);
std::vector<std::string> sort1();
std::vector<std::string> sort0(char *word);

int allWordsLen = sizeof(allWords) / 5;
int remainingLen = sizeof(hiddenWords) / 5;

struct Info
{
    bool cands[5][26];
    int count[26][2];
    Info();
    bool matches(char *word);
    void updateRemaining();
    void update(char *word, int comp);
    std::string estimateBest();
    void print();
};

std::pair<int, int> dfs(int level, std::string &word);

int main()
{
    srand(time(0));
    Info info;
    // std::cout << "How many tries do you have?\n"
    // int tries;
    // std::cin >> tries;
    // std::cout << "remaining len: " << remainingLen << std::endl;
    while (true)
    {
        // std::cout << "remaining len: " << remainingLen << std::endl;
        // std::cout << "info: {\n     cands: {\n";
        // for (int i = 0; i < 5; i++)
        // {
        //     std::cout << "          {";
        //     for (int j = 0; j < 26; j++)
        //     {
        //         std::cout << info.cands[i][j] << ", ";
        //     }
        //     std::cout << "}\n";
        // }
        // std::cout << "      }\n";
        // std::cout << "      count: {";
        // for (int i = 0; i < 26; i++)
        //     std::cout << info.count[i] << ", ";
        // std::cout << "}\n";
        // std::cout << "}\n";

        std::string curr;
        if (remainingLen == 0)
        {
            std::cout << "There is no word in the dictionary that mathces the given input\n";
            break;
        }
        if (remainingLen == 1)
        {
            for (int i = 0; i < 5; i++)
                curr += hiddenWords[i];
            std::cout << "the word is: " << curr << std::endl;
            break;
        }
        if (remainingLen <= 20)
        {
            std::cout << "The possible words are:\n";
            for (int i = 0; i < remainingLen; i++)
            {
                std::string currWord;
                for (int j = 0; j < 5; j++)
                    currWord += hiddenWords[i * 5 + j];
                std::cout << "     - " << currWord << std::endl;
            }
            std::cout << std::endl;
        }
        curr = info.estimateBest();
        if (remainingLen == 2)
        {
            int r = rand() % 2;
            curr.clear();
            for (int i = 0; i < 5; i++)
                curr += hiddenWords[r * 5 + i];
        }
        // std::cout << "remaining len: " << remainingLen << std::endl;
        std::cout << curr << " is the estimated best word\n";
        std::cout << "Enter the colors\n";
        std::string currInfo;
        std::cin >> currInfo;
        int comp = encode(currInfo);
        // std::cout << "comp: " << comp << std::endl;
        char currWord[5];
        for (int i = 0; i < 5; i++)
            currWord[i] = curr[i];
        // std::cout << "updating...\n";
        info.update(currWord, comp);
        // std::cout << "updated\n";
        // info.print();
    }
    // std::vector<int> best(244, INT_MAX);
    // std::string bestWord = "zzzzz";
    // char raise[5];
    // for (int i = 0; i < 5; i++)
    //     raise[i] = bestWord[i];
    // best = dfs(2, raise, 0, INT_MIN, INT_MAX);
    // std::cout << "zzzzz: ";
    // for (int j = 0; j < best.size(); j++)
    // {
    //     std::cout << best[j] << ", ";
    // }
    // std::cout << std::endl;
    // for (int i = 0; i < allWordsLen; i++)
    // {
    //     char currWord[5];
    //     makeWord(i, allWords, currWord);
    //     std::string currW;
    //     for (int j = 0; j < 5; j++)
    //         currW += currWord[j];
    //     std::cout << "curr word: " << currW << std::endl;
    //     std::vector<int> curr = dfs(2, currWord, 0, INT_MIN, INT_MAX);
    //     for (int j = 0; j < curr.size(); j++)
    //     {
    //         std::cout << curr[j] << ", ";
    //     }
    //     std::cout << std::endl;
    //     if (curr < best)
    //     {
    //         best = curr;
    //         for (int j = 0; j < 5; j++)
    //             bestWord[j] = currWord[j];
    //         std::cout << "best word: " << bestWord << std::endl;
    //     }
    //     std::cout << "curr best: " << bestWord << std::endl;
    //     for (int j = 0; j < best.size(); j++)
    //     {
    //         std::cout << best[j] << ", ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << "The best word is: " << bestWord << std::endl;
}

int compare(char *word, char *hidden)
{
    int res = 0;
    int cnt[26] = {};
    for (int i = 0; i < 5; i++)
    {
        cnt[hidden[i] - 'a']++;
        cnt[word[i] - 'a']--;
    }
    for (int i = 0; i < 5; i++)
    {
        int currBit = 0;
        if (word[i] == hidden[i])
        {
            currBit = 2;
        }
        else if (cnt[word[i] - 'a']++ >= 0)
            currBit = 1;
        res = res * 3 + currBit;
    }
    return res;
}

template <class T>
int compareArrays(T *arr1, T *arr2, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (arr1[i] < arr2[i])
            return -1;
        if (arr1[i] > arr2[i])
            return 1;
    }
    return 0;
}

Info::Info()
{
    for (int i = 0; i < 26; i++)
    {
        count[i][0] = 0;
        count[i][1] = 5;
    }
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            cands[i][j] = true;
        }
    }
}

bool Info::matches(char *word)
{
    int cnt[26] = {};
    for (int i = 0; i < 5; i++)
    {
        if (!cands[i][word[i] - 'a'])
            return false;
        cnt[word[i] - 'a']++;
    }
    for (int i = 0; i < 26; i++)
        if (cnt[i] < count[i][0] || cnt[i] > count[i][1])
            return false;
    return true;
}

void Info::updateRemaining()
{
    int newLen = 0;
    for (int i = 0; i < remainingLen; i++)
    {
        char currWord[5];
        makeWord(i, hiddenWords, currWord);
        if (matches(currWord))
        {
            for (int j = 0; j < 5; j++)
                hiddenWords[newLen * 5 + j] = hiddenWords[i * 5 + j];
            newLen++;
        }
    }
    remainingLen = newLen;
}

void Info::update(char *word, int comp)
{
    int tmpCnt[26] = {};
    bool black[26] = {};
    // std::cout << "tmpCnt before: ";
    // for (int i = 0; i < 26; i++)
    //     std::cout << tmpCnt[i] << ", ";
    // std::cout << std::endl;
    for (int i = 4; i >= 0; i--)
    {
        int currBit = comp % 3;
        // std::cout << "currBit: " << currBit << std::endl;
        // std::string currChar;
        // currChar += word[i];
        // std::cout << "currChar: " << currChar << std::endl;
        comp /= 3;
        if (currBit > 0)
            tmpCnt[word[i] - 'a']++;
        if (currBit == 0)
            black[word[i] - 'a'] = true;
        if (currBit == 2)
        {
            for (int j = 0; j < 26; j++)
                if (j != word[i] - 'a')
                    cands[i][j] = false;
        }
        else
            cands[i][word[i] - 'a'] = false;
        // print();
    }
    int totalCnt = 0;
    // std::cout << "tmpCnt after: ";
    // for (int i = 0; i < 26; i++)
    //     std::cout << tmpCnt[i] << ", ";
    // std::cout << std::endl;
    for (int i = 0; i < 26; i++)
    {
        // std::cout << "i: " << i << std::endl;
        count[i][0] = std::max(count[i][0], tmpCnt[i]);
        if (black[i])
            count[i][1] = std::min(count[i][1], tmpCnt[i]);
        totalCnt += count[i][0];
        // print();
    }
    // std::cout << "total count: " << totalCnt << std::endl;
    for (int i = 0; i < 26; i++)
        count[i][1] = std::min(count[i][1], 5 - totalCnt + count[i][0]);
    // print();
    updateRemaining();
}

std::string Info::estimateBest()
{
    int best[243];
    for (int i = 0; i < 243; i++)
        best[i] = remainingLen;
    int bestCount = 0;
    char res[5];
    char currWord[5];
    makeWord(0, hiddenWords, res);
    // std::string printedStr;
    // for (int i = 0; i < 5; i++)
    //     printedStr += res[i];
    // // std::cout << "res: " << res << std::endl;
    for (int i = 0; i < allWordsLen; i++)
    {
        makeWord(i, allWords, currWord);
        // printedStr.clear();
        // for (int j = 0; j < 5; j++)
        //     printedStr += currWord[j];
        // std::cout << "currWord: " << printedStr << std::endl;
        int cnt[243] = {};
        for (int j = 0; j < remainingLen; j++)
        {
            char currHidden[5];
            makeWord(j, hiddenWords, currHidden);
            int k = 0;
            for (; k < 5; k++)
                if (currHidden[k] != currWord[k])
                    break;
            if (k == 5)
                continue;
            int comp = compare(currWord, currHidden);
            cnt[comp]++;
        }
        std::sort(cnt, cnt + 243, std::greater<int>());

        int compared = compareArrays(cnt, best, 243);
        if (compared == -1)
        {
            for (int k = 0; k < 243; k++)
                best[k] = cnt[k];
            bestCount = 1;
            for (int k = 0; k < 5; k++)
                res[k] = currWord[k];
        }
        else if (compared == 0)
        {
            bestCount++;
            int r = rand() % bestCount;
            if (r == 0)
            {
                for (int k = 0; k < 5; k++)
                    res[k] = currWord[k];
            }
        }
    }

    std::string endRes;
    for (int i = 0; i < 5; i++)
        endRes += res[i];
    return endRes;
}

void Info::print()
{
    std::cout << "Info: {\n     cands: {\n";
    for (int i = 0; i < 5; i++)
    {
        std::cout << "          {";
        for (int j = 0; j < 26; j++)
        {
            std::cout << cands[i][j] << ", ";
        }
        std::cout << "}\n";
    }
    std::cout << "      }\n";
    std::cout << "      count: {";
    for (int i = 0; i < 26; i++)
        std::cout << "{" << count[i][0] << ", " << count[i][1] << "}, ";
    std::cout << "}\n";
    std::cout << "}\n";
}

std::string decode(int mask)
{
    std::string res;
    char c[] = {'B', 'Y', 'G'};
    for (int i = 0; i < 5; i++)
    {
        res += c[mask % 3];
        mask /= 3;
    }
    reverse(res.begin(), res.end());
    return res;
}

int encode(std::string &s)
{
    int m[26];
    m['B' - 'A'] = 0;
    m['Y' - 'A'] = 1;
    m['G' - 'A'] = 2;
    int res = 0;
    for (char c : s)
        res = res * 3 + m[c - 'A'];
    return res;
}

void makeWord(int i, char *words, char *res)
{
    for (int j = 0; j < 5; j++)
        res[j] = words[5 * i + j];
}

bool matches(char *word, char *query, int comp)
{
    bool black[25] = {};
    int cnt[26] = {};
    for (int i = 4; i >= 0; i--)
    {
        int currBit = comp % 3;
        comp /= 3;
        if (currBit == 2 && word[i] != query[i])
            return false;
        if (currBit < 2 && word[i] == query[i])
            return false;
        if (currBit > 0)
            cnt[query[i] - 'a']++;
        else
            black[query[i] - 'a'] = true;
        cnt[word[i] - 'a']--;
    }
    for (int i = 0; i < 26; i++)
    {
        if (cnt[i] > 0)
            return false;
        if (cnt[i] < 0 && black[i])
            return false;
    }
    return true;
}

void updateRemaining(int comp, char *query)
{
    int newLen = 0;
    for (int i = 0; i < remainingLen; i++)
    {
        char currWord[5];
        makeWord(i, hiddenWords, currWord);
        if (matches(currWord, query, comp))
        {
            for (int j = 0; j < 5; j++)
                std::swap(hiddenWords[newLen * 5 + j], hiddenWords[i * 5 + j]);
            newLen++;
        }
    }
    remainingLen = newLen;
}

int compareWords(int i1, int i2, char *words)
{
    for (int i = 0; i < 5; i++)
    {
        if (words[i1 * 5 + i] < words[i2 * 5 + i])
            return -1;
        if (words[i1 * 5 + i] > words[i2 * 5 + i])
            return 1;
    }
    return 0;
}

void reverseRemaining(int lenBefore)
{
    int rl = remainingLen - 1;
    for (int i = lenBefore - 1; i >= 0; i--)
    {
        int compared = compareWords(i, rl, hiddenWords);
        if (compared < 1)
        {
            for (int j = 0; j < 5; j++)
            {
                std::swap(hiddenWords[5 * i + j], hiddenWords[5 * rl + j]);
            }
            rl--;
        }
    }
    remainingLen = lenBefore;
}

bool isInOrder(char *arr, int len)
{
    for (int i = 1; i < len; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (arr[i * 5 + j] > arr[(i - 1) * 5 + j])
                break;
            if (arr[i * 5 + j] < arr[(i - 1) * 5 + j])
                return false;
        }
    }
    return true;
}

std::vector<std::string> sort1()
{
    std::vector<std::string> res;
    std::vector<std::pair<int, std::string>> m;
    for (int i = 0; i < allWordsLen; i++)
    {
        int cnt[243] = {};
        char currWord[5];
        makeWord(i, allWords, currWord);
        int currMax = 0;
        for (int j = 0; j < remainingLen; j++)
        {
            char currHidden[5];
            makeWord(j, hiddenWords, currHidden);
            int comp = compare(currWord, currHidden);
            cnt[comp]++;
            currMax = std::max(currMax, cnt[comp]);
        }
        std::string str;
        for (int j = 0; j < 5; j++)
            str += currWord[j];
        m.push_back({currMax, str});
    }
    std::sort(m.begin(), m.end());
    for (std::pair<int, std::string> &r : m)
        res.push_back(r.second);
    return res;
}

std::vector<std::string> sort0(char *word)
{
    std::vector<std::pair<int, std::string>> m;
    std::vector<std::string> res;
    for (int i = 0; i < remainingLen; i++)
    {
        char currHidden[5];
        makeWord(i, hiddenWords, currHidden);
        int comp = compare(word, currHidden);
        int currCnt = 0;
        for (int j = 0; j < allWordsLen; j++)
        {
            char currWord[5];
            makeWord(j, allWords, currWord);
            if (matches(currWord, word, comp))
                currCnt++;
        }
        std::string str;
        for (int j = 0; j < 5; j++)
            str += currHidden[j];
        m.push_back({currCnt, str});
    }
    std::sort(m.begin(), m.end(), [](std::pair<int, std::string> &a, std::pair<int, std::string> &b)
              { return a > b; });
    for (std::pair<int, std::string> &r : m)
        res.push_back(r.second);
    return res;
}

std::vector<int> dfs(int level, char *word, int turn, int mn, int mx)
{
    if (remainingLen == 0)
    {
        std::vector<int> res(244);
        res[243] = -level;
        return res;
    }
    std::vector<int> res(244, turn ? INT_MAX : INT_MIN);
    if (turn == 0)
    {
        if (level == 1)
        {
            for (int i = 0; i < 243; i++)
                res[i] = 0;
        }
        res[243] = -1;
        std::vector<std::string> words = sort0(word);
        for (std::string w : words)
        {
            char currHidden[5];
            // makeWord(i, hiddenWords, currHidden);
            for (int i = 0; i < 5; i++)
                currHidden[i] = w[i];
            // std::cout << "curr hidden word: ";
            // for (int j = 0; j < 5; j++)
            //     std::cout << currHidden[j];
            // std::cout << std::endl;
            // std::cout << "curr word: ";
            // for (int j = 0; j < 5; j++)
            //     std::cout << word[j];
            // std::cout << std::endl;
            if (compareArrays(currHidden, word, 5) == 0)
                continue;
            int currComp = compare(word, currHidden);
            // std::cout << "curr comp: " << currComp << std::endl;
            // std::cout << "decoded: " << decode(currComp) << std::endl;
            if (level == 1)
            {
                res[currComp]++;
            }
            else
            {
                int lenBefore = remainingLen;
                // std::cout << "remaining before: ";
                // for (int j = 0; j < remainingLen; j++)
                // {
                //     for (int k = 0; k < 5; k++)
                //     {
                //         std::cout << hiddenWords[j * 5 + k];
                //     }
                //     std::cout << ", ";
                // }
                // std::cout << std::endl;
                // if (!isInOrder(hiddenWords, remainingLen))
                // {
                //     std::cout << "hidden words is not in order before updating\n";
                // }
                updateRemaining(currComp, word);
                std::vector<int> next = dfs(level - 1, word, 1, mn, mx);
                reverseRemaining(lenBefore);
                // std::cout << "remaining after: ";
                // for (int j = 0; j < remainingLen; j++)
                // {
                //     for (int k = 0; k < 5; k++)
                //     {
                //         std::cout << hiddenWords[j * 5 + k];
                //     }
                //     std::cout << ", ";
                // }
                // std::cout << std::endl;
                // if (!isInOrder(hiddenWords, remainingLen))
                // {
                //     std::cout << "hidden words is not in order after reversing\n";
                // }
                if (next > res)
                    res = next;
                if (next[0] > mx)
                    return res;
                mn = std::max(mn, next[0]);
            }
        }
        if (level == 1)
            sort(res.begin(), res.end() - 1, std::greater<int>());
    }
    else
    {
        std::vector<std::string> words = sort1();
        for (std::string w : words)
        {
            char currWord[5];
            // makeWord(i, allWords, currWord);
            for (int i = 0; i < 5; i++)
                currWord[i] = w[i];
            std::vector<int> next = dfs(level, currWord, 0, mn, mx);
            if (next < res)
                res = next;
            if (next[0] < mn)
                return res;
            mx = std::min(mx, next[0]);
        }
    }
    return res;
}
