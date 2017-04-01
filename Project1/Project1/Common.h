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
/* ��� */
inline unsigned int getNext(unsigned int i, unsigned  int x, unsigned  int y) {
	return (i%y)*x + i / y;
}

/* ǰ�� */
inline unsigned int getPre(unsigned int i, unsigned int x, unsigned int y) {
	return (i%x)*y + i / x;
}

/* �������±�iΪ���Ļ� */
template<typename T>
void movedata(T *mtx, unsigned int i, unsigned  int x, unsigned  int y) {
	T temp = mtx[i];  // �ݴ�
	unsigned int cur = i;       // ��ǰ�±�
	unsigned int pre = getPre(cur, x, y);
	while (pre != i)
	{
		mtx[cur] = mtx[pre];
		cur = pre;
		pre = getPre(cur, x, y);
	}
	mtx[cur] = temp;
}

/* ת�ã���ѭ���������л� */
template<typename T>
void Transpose(T *mtx, unsigned int x, unsigned  int y) {
	for (unsigned int i = 0; i < x*y; ++i) {
		unsigned int next = getNext(i, x, y);
		while (next > i) // �����ں��С��i˵���ظ�
			next = getNext(next, x, y);
		if (next == i)   // ����ǰ�� 
			movedata(mtx, i, x, y);
	}
}

/* ������� */
template<typename T>
void Print(T *mtx, unsigned int x, unsigned int y) {
	for (unsigned int i = 0; i < x*y; ++i) {
		if ((i + 1) % y == 0)
			std::cout << mtx[i] << std::endl;
		else
			std::cout << mtx[i] << " ";
	}
}