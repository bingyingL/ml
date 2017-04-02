#pragma once

#include <algorithm>
#include <utility>
#include <memory>

//sort the feature and creat correspondent sorted index 
template<typename T>
bool SortFeature(T * const feature, unsigned int * const index, const unsigned int row, const unsigned int col) {

	if (feature == nullptr || index == nullptr) {
		std::cout << "[SortFeature Err] : feature Or index Is Empty" << std::endl;
		return false;
	}
	if (row == 0 || col == 0) {
		std::cout << "[SortFeature Err] : row Or col Is 0" << std::endl;
		return false;
	}

	std::unique_ptr<std::pair<T, unsigned int>[]> arow(new std::pair<T, unsigned int>[col]);
	for (unsigned int x = 0; x != row; ++x) {
		for (unsigned int y = 0; y != col; ++y) {
			arow[y] = std::make_pair(*(feature + x*col + y), y);
		}
		std::sort(arow.get(), arow.get() + col);
		for (unsigned int y = 0; y != col; ++y) {
			*(feature + x*col + y) = arow[y].first;
			*(index + x*col + y) = arow[y].second;
		}
	}
	return true;
}


//Transpose a matrix, use Transpose()
/* 后继 */
inline unsigned int getNext(unsigned int i, unsigned  int x, unsigned  int y) {
	return (i%y)*x + i / y;
}

/* 前驱 */
inline unsigned int getPre(unsigned int i, unsigned int x, unsigned int y) {
	return (i%x)*y + i / x;
}

/* 处理以下标i为起点的环 */
template<typename T>
void movedata(T *mtx, unsigned int i, unsigned  int x, unsigned  int y) {
	T temp = mtx[i];  // 暂存
	unsigned int cur = i;       // 当前下标
	unsigned int pre = getPre(cur, x, y);
	while (pre != i)
	{
		mtx[cur] = mtx[pre];
		cur = pre;
		pre = getPre(cur, x, y);
	}
	mtx[cur] = temp;
}

/* 转置，即循环处理所有环 */
template<typename T>
void Transpose(T *mtx, unsigned int x, unsigned  int y) {
	for (unsigned int i = 0; i < x*y; ++i) {
		unsigned int next = getNext(i, x, y);
		while (next > i) // 若存在后继小于i说明重复
			next = getNext(next, x, y);
		if (next == i)   // 处理当前环 
			movedata(mtx, i, x, y);
	}
}

/* 输出矩阵 */
template<typename T>
void Print(T *mtx, unsigned int x, unsigned int y) {
	for (unsigned int i = 0; i < x*y; ++i) {
		if ((i + 1) % y == 0)
			std::cout << mtx[i] << std::endl;
		else
			std::cout << mtx[i] << " ";
	}
}



template<typename T>
bool SubFeature(const T*const feature, const unsigned int x, const unsigned int y,
	T*const subfeature, std::vector<unsigned int> subx, std::vector<unsigned int> suby) {
	if (feature == nullptr || subfeature == nullptr) {
		std::cout << "[SubFeature Err] : feature Or subfeature Is Empty" << std::endl;
		return false;
	}
	if (x == 0 || y == 0) {
		std::cout << "[SubFeature Err] : x Or y Is 0" << std::endl;
		return false;
	}
	if (subx.empty() || suby.empty()) {
		std::cout << "[SubFeature Err] : subx Or suby Is Empty" << std::endl;
		return false;
	}
	for()

}