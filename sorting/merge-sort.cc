//! @brief       Insertion sort Advanced Analysis. Merge sort + inversion
//! @author      Mahesh H D
//! @date        Sun Jul  3 10:22:25 IST 2016

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

class mergeSort {
	public:
		mergeSort();
		~mergeSort();

		template <typename T>
		size_t       eval(std::vector<T> &inArr);

	private:
		template <typename T>
		void              _sort(std::vector<T> &inArr, size_t start, size_t end);

		template <typename T>
		void              _merge(std::vector<T> &inArr, size_t start, size_t mid, size_t end);

		inline void       _resetInversionCount() {_inversionCount = 0;}
		inline size_t     _getInversionCount() const {return _inversionCount;}

		size_t            _inversionCount;
};

mergeSort::mergeSort()
	: _inversionCount(0)
{
}

mergeSort::~mergeSort()
{
}

template <typename T>
size_t
mergeSort::eval(std::vector<T>  &inArr)
{
	if(inArr.size() <= 1) return 0;
	_resetInversionCount();
	_sort(inArr, 0, inArr.size()-1);
	return _getInversionCount();
}

template <typename T>
void
mergeSort::_sort(
	std::vector<T> &inArr,
	size_t start, size_t end)
{
	if (start < end) {
		size_t mid = (start + end) / 2;
		_sort(inArr, start, mid);
		_sort(inArr, mid+1, end);
		_merge(inArr, start, mid, end);
	}
}

template <typename T>
void
mergeSort::_merge(
	std::vector<T> &inArr,
	size_t start, size_t mid,
	size_t end)
{
	std::vector<T> resArr(end-mid);
	std::copy(inArr.begin()+mid+1, inArr.begin()+end+1, resArr.begin());

	size_t lindex = mid, rindex = resArr.size()-1;
	size_t count = end-start+1;
	while (count > 0) {
		if (inArr[lindex] > resArr[rindex]) {
			_inversionCount += rindex+1;
			inArr[start + count - 1] = inArr[lindex];
			if (lindex == start) {
				std::copy(resArr.begin(), resArr.begin()+rindex+1, inArr.begin()+start);
				break;
			}
			--lindex;
		} else {
			inArr[start + count - 1] = resArr[rindex];
			if (rindex == 0) break;
			--rindex;
		}
		--count;
	}
}

int
main()
{
	unsigned int T = 0;
	std::cin >> T;
	while (T--) {
		unsigned int sz = 0;
		std::cin >> sz;
		std::vector<int> inArr(sz);
		for (unsigned int index = 0; index < sz; ++index) {
			std::cin >> inArr[index];
		}
		mergeSort msObj;
		std::cout << msObj.eval<int>(inArr) << std::endl;
	}
	return 0;
}
