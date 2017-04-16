#include <iostream>
#include <list>

class trieNode {
	public:
		trieNode() : _count(1) {
			for (unsigned short index = 0; index < MAX_CHILDREN; ++index) {
				_children[index] = NULL;
			}
		}
		~trieNode() {
			for (unsigned short index = 0; index < MAX_CHILDREN; ++index) {
				delete _children[index];
			}
		}

		bool        insert(const std::string& inStr);
		bool        search(const std::string& inStr);
		// trieNode*   getChildAt(unsigned short index);
		int         getWordsList(std::list<std::string>& outList);

	private:
		static const unsigned short MAX_CHILDREN = 26;

		bool        _insert(const std::string& inStr, size_t slen, size_t sIndex);
		void        _doDFSAndGetWordList(std::string& word, std::list<std::string> &outList);

		unsigned int    _count;
		trieNode       *_children[MAX_CHILDREN];
};

bool
trieNode::insert(
	const std::string& inStr)
{
	return _insert(inStr, inStr.length(), 0);
}

bool
trieNode::_insert(
	const std::string& inStr,
	size_t slen,
	size_t sIndex)
{
	if (sIndex >= slen) return true;
	unsigned short cIndex = inStr[sIndex] - 'a';
	if (_children[cIndex]) {
		return _children[cIndex]->_insert(inStr, slen, sIndex+1);
	}
	trieNode* nodeToUse = this;
	for (;sIndex < slen; ++sIndex) {
		unsigned short ccIndex = inStr[sIndex] - 'a';
		nodeToUse->_children[ccIndex] = new trieNode();
		nodeToUse->_count++;
		nodeToUse = nodeToUse->_children[ccIndex];
	}
	return false;
}

bool
trieNode::search(const std::string& inStr)
{
	bool toReturn  = true;
	trieNode* nodeToUse = this;
	for (size_t sIndex = 0; sIndex < inStr.length(); ++sIndex) {
		unsigned short cIndex = inStr[sIndex] - 'a';
		if (nodeToUse->_children[cIndex]) {
			nodeToUse = nodeToUse->_children[cIndex];
			continue;
		}
		toReturn = false;
		break;
	}
	return toReturn;
}

int
trieNode::getWordsList(
	std::list<std::string>& outList)
{
	for (unsigned short index = 0; index < MAX_CHILDREN; ++index) {
		if (_children[index]) {
			std::string word(1, 'a'+index);
			_children[index]->_doDFSAndGetWordList(word, outList);
		}
	}
	return outList.size();
}

void
trieNode::_doDFSAndGetWordList(
	std::string& word,
	std::list<std::string>& outList)
{
	if (_count == 1) {
		outList.push_back(word);
		return;
	}
	for (unsigned short index = 0; index < MAX_CHILDREN; ++index) {
		if (_children[index]) {
			word.append(1, 'a'+index);
			_children[index]->_doDFSAndGetWordList(word, outList);
		}
	}
}

int
main()
{
	unsigned int T = 0;
	std::cin >> T;
	while (T--) {
		unsigned int numStrings = 0;
		std::cin >> numStrings;
		trieNode tn;
		while (numStrings--) {
			std::string str;
			std::cin >> str;
			std::cout << tn.insert(str) << std::endl;
		}
		std::list<std::string> wordList;
		tn.getWordsList(wordList);
		for (std::list<std::string>::const_iterator it = wordList.begin(); it != wordList.end(); ++it) {
			std::cout << *it << std::endl;
		}
	}
	return 0;
}
