#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;
typedef int dt;

long long mod = 1000000007LL;

long long power(long long a, long long b)
{
    if(b <= 0) return 1;
    long long tmp = power(a, b/2) % mod;
    if(b % 2 == 0)
    {
        return (tmp * tmp) % mod;
    }
    else
    {
        return (((a * tmp) % mod) * tmp) % mod;
    }
}

struct NchooseK {
    int range;
    vector<long long> fact, ifact;
    NchooseK() 
    {
        range = (int) 1e6 + 10;
        fact.resize(range+1);
        ifact.resize(range+1);
        calcFacts();
    }
    void calcFacts()
    {
        fact[0] = 1;
        for(long long i=1; i<= range; ++i)
        {
            fact[i] = (1LL*fact[i-1]*i)%mod;
        }
        ifact[range] = power(fact[range], mod-2);
        for(long long i=range-1; i>=0; --i)
        {
            ifact[i] = (1LL*ifact[i+1]*(i+1))%mod;
        }
    }
    long long choose(long long n, long long k)
    {
        if(k<0 || k>n || n<0) return 0;
        return ((1LL*fact[n])%mod * 1LL*ifact[n-k])%mod;
    }
};

long long fact(long long n) {
  static vector<long long> memo(1, 1);
  if(n < 0) return 1;
  if(memo.size() > n) return memo[n];
  int lsize = memo.size();
  memo.resize(n+1);
  for(int i = lsize; i < memo.size(); i++) {
    memo[i] = (memo[i-1] * i) % mod;
  }
  return memo[n];
}

long long facts(long long n) {
  static vector<long long> memo(1, 1);
  if(n < 0) return 1;
  if(memo.size() > n) return memo[n];
  int lsize = memo.size();
  memo.resize(n+1);
  for(int i = lsize; i < memo.size(); i++) {
    memo[i] = (memo[i-1] + fact(i)) % mod;
  }
  return memo[n];
}

//Segment tree code
struct dp
{
    dt val;
    dp(): val(0){};
    dp(dt i): val(i){};
    void update(const dt& u)
    {
        val = u;
    }
};

dp combine(dp l, dp r)
{
    return dp(l.val + r.val);
}

struct segment
{
    vector<dp> data;
    int s;
    
    segment(int sz, const vector<dp>& base = vector<dp>()): s(sz)
    {
        data.resize(2*s);
        for(int i=0; i < base.size(); i++)
        {
            data[s+i] = base[i];
        }
        for(int i=s-1; i>0; i--)
        {
            data[i] = combine(data[2*i], data[2*i+1]);
        }
    }

    void update(int spot, const dt& update)
    {
        data[spot + s].update(update);
        for(int i=(spot + s)/2; i>0; i/=2)
            data[i] = combine(data[2*i], data[2*i+1]);
    }
    dp query(int si, int ei)
    {
        dp res;
        for(si += s, ei += s; si < ei; si /= 2, ei /= 2)
        {
            if(si % 2 == 1)
                res = combine(res, data[si++]);
            if(ei % 2 == 1)
                res = combine(res, data[--ei]);
        }
        return res;
    }
};

//Trie code
struct trie
{
    string word = "";
    unordered_map<char, shared_ptr<trie>> children;
};

void insert(const string& s, const uint32_t& i, trie* t)
{
    if(i == s.size())
    {
        t->word = s;
        return;
    }

    auto& next = t->children[s[i]];
    if(!next) next.reset(new trie);
    
    insert(s, i+1, next.get());
}

int main()
{
    int k, n;
    cin >> n >> k;

    //Build trie
    trie t;
    vector<string> words(n);
    for(string& s : words)
    {
        cin >> s;
        insert(s, 0, &t);
    }

    //Read target word
    string curr;
    cin >> curr;

    vector<string> used;
    trie* node = &t;
    for(char c : curr)
    {
        if(node->word.size())
        {
            used.push_back(node->word);
            node = &t;
        }

        node = node->children[c].get();
    }
    used.push_back(node->word);

    //Get given words in sorted order
    sort(words.begin(), words.end());

    //Map words to index in seg tree
    unordered_map<string, int> wordMap; 
    int i=0; 
    for(string s : words)
        wordMap[s] = i++;

    //Build segment tree representing what words are unused
    vector<dp> segData(n, dp(1));
    segment segTree(n, segData);
    long long tlc = 0;

    NchooseK nk;
    //Process all words in target string in order
    long long left = k;
    for(string s : used)
    {
        //Mark the word as used
        auto here = wordMap[s];
        segTree.update(here, 0);
        left--;

        int wordsBefore = 0; 
        if(here != 0) 
          wordsBefore = segTree.query(0, here).val;
        int wordsAfter = 0;
        if(here != n - 1) 
          wordsAfter = segTree.query(here, n).val;
        //total diff = sum of facts times wordsBefore size
        // formula is wordsBefore * sum(nchoosek) on 0..min(k, wordsleftTotal))
        // + 1 for no words at all
        // cout << s << ": " << wordsBefore << " " << wordsAfter << " " << left << endl;
        tlc += wordsBefore * nk.choose(wordsBefore + wordsAfter, left);
        tlc %= mod;
        // cout << tlc << endl;
    }
    cout << (tlc + 1) % mod << endl;
    return 0;
}