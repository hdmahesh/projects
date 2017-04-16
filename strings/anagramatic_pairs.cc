//!
//! g++ <file-name> -std=c++11
//!
#include <cmath>
#include <cstdio>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
using namespace std;

class trieNode {
    public:
        trieNode() {
            _count = 1;
            for_each(_children, _children+MAX_CHILDREN, [] (trieNode *& node) {node = NULL;});
            for_each(_alphaCount, _alphaCount+MAX_CHILDREN, [] (unsigned &alphaCount) {alphaCount = 0;});
        };
        ~trieNode() {
            for_each(_children, _children+MAX_CHILDREN, [] (trieNode *& node) {if (node) delete node;});
        };
    
        void insert(const string &input, size_t startIndex = 0);
        void copyAlphaCount(const size_t &cIndex);
    
        bool search(const string &subStr, size_t startIndex = 0);
    
        trieNode *getChildAt(size_t index) {
            if (index >= MAX_CHILDREN) return NULL;
            return _children[index];
        };
        
        void pushValidChildren(queue<trieNode*> &Q);
    
        class compareNode {
            public:
                bool operator() (const trieNode* left, const trieNode* right) {
                    for(size_t index = 0; index < MAX_CHILDREN; ++index) {
                        if (left->_alphaCount[index] == right->_alphaCount[index]) {
                            continue;
                        }
                        return (left->_alphaCount[index] < right->_alphaCount[index]);
                    }
                    return true;
                }
    
        };
    
        bool operator==(const trieNode &other) const {
            for(size_t index = 0; index < MAX_CHILDREN; ++index) {
                if (_alphaCount[index] == other._alphaCount[index]) {
                    continue;
                } 
                // cout << "INSIDE == " << endl;
                // print();
                // other.print();
                
                return false;
            }
            return true;
        }
    
        size_t getCount() const {return _count;}
    
        void print() const {
            cout << "-----" << endl;
            cout << "count = " << _count << endl;
            for(size_t i = 0; i < MAX_CHILDREN; i++) {
                if (_alphaCount[i]) {
                    cout << (char)('a' + i) << " : " << _alphaCount[i] << endl; 
                }
            }
            cout << "-----" << endl;
        }
    
    private:
        const static unsigned MAX_CHILDREN = 26;
    
        unsigned       _count;
        trieNode      *_children[MAX_CHILDREN];
        unsigned       _alphaCount[MAX_CHILDREN];
};

void
trieNode::copyAlphaCount(const size_t &cIndex)
{
    copy(_alphaCount, _alphaCount+MAX_CHILDREN, _children[cIndex]->_alphaCount);
}

void
trieNode::insert(const string &input, size_t startIndex) {
    if (startIndex >= input.length()) return;
    
    char toAdd = input[startIndex];
    size_t cIndex = toAdd - 'a';
    if (_children[cIndex]) {
        // cout << "Insert found char : " << toAdd << endl;
        ++_children[cIndex]->_count;        
    } else {
        // cout << "Insert not found char : " << toAdd << endl;
        _children[cIndex] = new trieNode();
        copyAlphaCount(cIndex);
        ++_children[cIndex]->_alphaCount[cIndex];        
    }
    _children[cIndex]->insert(input, startIndex+1);
}

bool
trieNode::search(const string &subStr, size_t startIndex)
{
    if (startIndex >= subStr.length()) return true;
    
    char key = subStr[startIndex];
    size_t cIndex = key - 'a';
    
    if (_children[cIndex]) {
        // cout << "Found char : " << key << endl;
        return _children[cIndex]->search(subStr, startIndex + 1);
    }
    // cout << "Not found char : " << key << endl;
    return false;
}

void
trieNode::pushValidChildren(queue<trieNode*> &Q)
{
    for_each(_children, _children+MAX_CHILDREN, [&](trieNode* node) {if(node) Q.push(node);});
}



size_t
bfsTraversal(trieNode *root, bool printDbg = false) {
    if (root == NULL) return 0;
    
    queue<trieNode*> Q;
    Q.push(root);
    
    vector<trieNode*> nodeList;
    
    while (!Q.empty()) {
        trieNode *currNode = Q.front();
        Q.pop();
        nodeList.push_back(currNode);
        
        /* Add all the valid children of current node to the Q */
        currNode->pushValidChildren(Q);
    }
    
    sort(nodeList.begin(), nodeList.end(), trieNode::compareNode());
    
    double result = 0;
    size_t rSum = 0;
    vector<trieNode*>::iterator start = nodeList.begin();
    vector<trieNode*>::iterator end   = nodeList.end();
    while (start != end) {
        // cout << "NEW LIST>>>>>>" << endl;
        if (printDbg) (*start)->print();
        rSum = (*start)->getCount();
        vector<trieNode*>::iterator next = start+1;
        while (next != end && *(*start) == *(*next)) {
            if (printDbg) (*next)->print();
            rSum += (*next)->getCount();
            next++;
        }
        if(printDbg && rSum > 1) cout << "Rsum = " << rSum << endl;
        start = next;
        result += ((rSum * (rSum-1)) / 2.0);
    }
    return static_cast<size_t>(result);
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    unsigned T;
    cin >> T;
    while (T--) {
        string S;
        cin >> S;
        
        trieNode rootObj;
        for (size_t i = 0; i < S.length(); ++i) {
            rootObj.insert(S.substr(i, S.length()-i));
        }
        cout << bfsTraversal(&rootObj, true) << endl;
    }
    return 0;
}

