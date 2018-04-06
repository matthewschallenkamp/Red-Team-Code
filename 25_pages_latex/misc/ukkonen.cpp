using namespace std;

template <class T> string toStr(const T &x){ stringstream s; s << x; return s.str(); }
template <class T> int toInt(const T &x){ stringstream s; s << x; int r; s >> r; return r; }

#define For(i, a, b) for (int i=(a); i<(b); ++i)
#define foreach(x, v) for (typeof (v).begin() x = (v).begin(); x != (v).end(); ++x)
#define D(x) cout << #x " = " << (x) << endl


/*
 Ukkonen's algorithm for linear time construction of suffix trees.

 Usage:

 string s = "mississipi";
 SuffixTree tree(s + "$");

 If after that you need to create a new tree for a different string, you can recycle this tree's memory
 by calling assign:

 s = "banana";
 tree.assign(s + "$");

 IT IS ASSUMED THAT THE LAST CHARACTER IN THE STRING DOES NOT APPEAR IN ANY OTHER PLACE.


 The tree consists of 2 classes: SuffixTree and Node.
 You will basically only care about the Node class.
 Each node has a map<char, Node *> of outgoing edges, called sons.

 node->sons['a'] would be a pointer to the node that the edge starting with letter 'a' reaches, leaving from this node. (There can be at most one edge starting with letter 'a'). Each edge is labeled with a substring of the original string. To save space, we only save two integers, edgeStart and edgeEnd. So, the edge incoming to node "n" is labeled with the substring s[n->edgeStart, n->edgeEnd) (Note the last index is not included). It is guaranteed that n == n->parent->sons[s[n->edgeStart]] (except for the root).

 Every node represents a substring of s, and every leaf represents a suffix of s.

 The entry point to the tree is tree.root, which represents the empty string.
*/

struct Node;
struct SuffixTree;

struct Node {
 Node * parent, * suffixLink;
 int edgeStart, edgeEnd, parentDepth;
 // The edge that reaches this node contains the substring s[edgeStart, edgeEnd)
 map<char, Node *> sons;

 // Returns true if there is a path starting at root having length position + 1 that ends
 // in the edge that reaches this node.
 inline bool inEdge(int position){ return parentDepth <= position && position < depth(); }

 inline int edgeLength(){ return edgeEnd - edgeStart; }

 inline int depth(){ return parentDepth + edgeLength();}

 void link(Node * son, int start, int end, const string &s){
   // Links the current node with the son. The edge will have substring s[start, end)
   sons[s[start]] = son;
   son->parent = this;
   son->parentDepth = this->depth();
   son->edgeStart = start;
   son->edgeEnd = end;
 }

 inline bool isLeaf(){ return sons.size() == 0; }

 Node(){
   parent = suffixLink = NULL;
   edgeStart = edgeEnd = parentDepth = 0;
   sons = map<char, Node *>();  }};

struct SuffixTree {
 vector<Node> nodes;
 Node * root, * needSuffix;
 int currentNode;
 string s;
 int length;

 SuffixTree(){}

 SuffixTree(const string &str) {
   assign(s);
 }

 void assign(const string &str){
   s = str;
   length = str.length();
   initialize();
   build();
 }

private:
 void initialize(){
   if (nodes.size() < 2*length + 10){
     nodes.resize(2*length + 10);
   }
   currentNode = 0;
 }

 Node * newNode(){
   // Returns a pointer to a fresh node. Default values are:
   // parent == NULL, suffixLink == NULL, edgeStart == 0, edgeEnd == 0, parentDepth == 0, sons = <Empty map>
   nodes[currentNode] = Node();
   return &nodes[currentNode++];
 }


 Node * walkDown(Node * c, int j, int i) {
   int k = j + c->depth();
   if (i - j + 1 > 0){
     while (!c->inEdge(i - j)){
       c = c->sons[s[k]];
       k += c->edgeLength();
    }   }
   return c;
 }

 void addSuffixLink(Node * current){
   if (needSuffix != NULL){
     needSuffix->suffixLink = current;
   }
   needSuffix = NULL;
 }

 void build() {
   root = newNode();
   Node * c = newNode();
   needSuffix = NULL;
   root->link(c, 0, length, s);

   // Indicates if at the beginning of the phase we need to follow the suffix link of the current node and then walk down the tree using the skip and count trick.
   bool needWalk = true;

   for (int i=0, j=1; i<length-1; ++i){
     char nc = s[i+1];
     while (j <= i + 1){
       if (needWalk){
         if (c->suffixLink == NULL && c->parent != NULL) c = c->parent;
         c = (c->suffixLink == NULL ? root : c->suffixLink);
         c = walkDown(c, j, i);
       }

       needWalk = true;
       // Here c == the highest node below s[j...i] and we will add char s[i+1]
       int m = i - j + 1; // Length of the string s[j..i].
       if (m == c->depth()){
         // String s[j...i] ends exactly at node c (explicit node).
         addSuffixLink(c);
         if (c->sons.count(nc) > 0){
           c = c->sons[nc];
           needWalk = false;
           break;
         }else{
           Node * leaf = newNode();
           c->link(leaf, i+1, length, s);
         }
       }else{
         // String s[j...i] ends at some place in the edge that reaches node c.
         int where = c->edgeStart + m - c->parentDepth;
         // The next character in the path after string s[j...i] is s[where]
         if (s[where] == nc){ //Either rule 3 or rule 1
           addSuffixLink(c);
           if (!c->isLeaf() || j != c->edgeStart - c->parentDepth){
             // Rule 3
             needWalk = false;
             break;
           }
         }else{
           Node * split = newNode();
           c->parent->link(split, c->edgeStart, where, s);
           split->link(c, where, c->edgeEnd, s);
           split->link(newNode(), i+1, length, s);

           addSuffixLink(split);

           if (split->depth() == 1){
             //The suffix link is the root because we remove the only character and end with an empty string.
             split->suffixLink = root;
           }else{
             needSuffix = split;
           }
           c = split;
        }   } j++; }   }   }   };

//Example usage
int dfs(Node * c){
 if (c->isLeaf()){
   return 1;
 }
 int leaves = 0;
 for (map<char, Node *>::iterator i = c->sons.begin(); i != c->sons.end(); ++i){
   leaves += dfs(i->second);
 }

 if (leaves > 1) ans += c->edgeLength();
 return leaves;
}

int main(){
 string s;
 SuffixTree t;
 while (cin >> s && s[0] != '*'){
   t.assign(s + "$");
   ans = 0;
   dfs(t.root);
   cout << ans << endl;
 }
 return 0;
}