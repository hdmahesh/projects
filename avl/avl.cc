#include "avl.h"

namespace avl {
}

int
main()
{
	using namespace avl;
	avlTree<std::string>  ats;
	std::string toAdd;
	toAdd.assign("0"); ats.add(toAdd);
	toAdd.assign("1"); ats.add(toAdd);
	toAdd.assign("3"); ats.add(toAdd);
	toAdd.assign("2"); ats.add(toAdd);
	ats.print();
	return 0;
}
