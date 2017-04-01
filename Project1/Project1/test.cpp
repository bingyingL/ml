#include "DecisionStump.h"
#include "AdaBoost.h"
#include "Common.h"

#include <array>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <memory>


using namespace std;



int main(void) {

	//int a[2][3] = { {1,2,3},{4,5,6} };
	////int *b = &a[0][0];
	//array<array<int, 2>, 3> c;
	////c = b;
	//int(*b)[2][3] = &a;
	//int e[6] = {1,2,3,4,5,6};
	//int x = 2, y = 3;
	//b = (int(*)[2][3])e;
	//for (int x = 0; x != 2; ++x) {
	//	for (int y = 0; y != 3; ++y)
	//		cout << (*b)[x][y] << " ";
	//	cout << endl;
	//}
	//int tmp1 = 0;
	//const int tmp2 = 10;
	//tmp1 = tmp2;
	//int * tmp4 = nullptr;
	//vector<int> test(100000,0);
	//vector<int> testb;
	//for (int i = 0; i != 100000; ++i) {
	//	test[i] = rand();
	//}
	//testb = test;


	//unique_ptr<double[]> ftr(new double[10 * 20]);
	//unique_ptr<unsigned int[]> ind(new unsigned int[10 * 20]);
	//double(*pftr)[10][20] = (double(*)[10][20])ftr.get();
	//unsigned int(*pind)[10][20] = (unsigned int(*)[10][20])ind.get();
	//for (int x = 0; x != 10; ++x) {
	//	for (int y = 0; y != 20; ++y) {
	//		(*pftr)[x][y] = rand()%20;
	//		cout << (*pftr)[x][y] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	//SortFeature(ftr.get(), ind.get(), 10, 20);
	//for (int x = 0; x != 10; ++x) {
	//	for (int y = 0; y != 20; ++y) {
	//		cout << (*pftr)[x][y] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	//for (int x = 0; x != 10; ++x) {
	//	for (int y = 0; y != 20; ++y) {
	//		cout << (*pind)[x][y] << " ";
	//	}
	//	cout << endl;
	//}


	////double feature[4][13] = { 
	////{0,1,2,3,4,5,6,7,8,9,10,11,12},
	////{ 0,1,2,3,4,5,6,7,8,9,10,11,12 },
	////{ 0,1,2,3,4,5,6,7,8,9,10,11,12 },
	////{0,1,2,3,4,5,6,7,8,9,10,11,12} };
	//unique_ptr<double[]> featurememory(new double[10000 * 2000]);
	//double (*feature)[10000][2000];
	//feature = (double(*)[10000][2000])featurememory.get();
	//for (int x = 0; x != 10000; ++x) {
	//	for (int y = 0; y != 2000; ++y) {
	//		(*feature)[x][y] = y;
	//	}
	//}
	//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	////unsigned int index[4][13] = { 
	////{0,1,2,3,4,5,6,7,8,9,10,11,12},
	////{ 0,9,10,11,1,2,3,4,5,6,7,8,12 },
	////{ 3,12,0,9,10,11,1,2,4,5,6,7,8 },
	////{ 2,3,4,5,6,8,7,12,0,1,9,10,11 } };
	//unique_ptr<double[]> indexmemory(new double[10000 * 2000]);
	//unsigned int (*index)[10000][2000];
	//index = (unsigned int(*)[10000][2000])indexmemory.get();
	//for (int x = 0; x != 10000; ++x) {
	//	vector<int> tmp(2000, 0);
	//	int sum = 0;
	//	for (int i = 0; i != 2000; ++i) tmp[i] = i;
	//	for (int y = 0; y != 2000; ++y) {
	//		auto a = rand() % tmp.size();
	//		(*index)[x][y] = tmp[a];
	//		tmp.erase(tmp.begin() + a);
	//		sum += (*index)[x][y];
	//	}
	//}

	unsigned int samplecounts = 20;
	unsigned int featuredim = 100;
	unique_ptr<double[]> ftr(new double[featuredim * samplecounts]);
	unique_ptr<double[]> ftr_copy(new double[featuredim * samplecounts]);
	unique_ptr<unsigned int[]> ind(new unsigned int[featuredim * samplecounts]);
	vector<char> label;
	for (int i = 0; i != featuredim*samplecounts; ++i) {
		if (rand() % 10 < 8) {
			if (i > 1) 
				ftr[i] = ftr[i - 1];
			else
				ftr[i] = rand() % (featuredim*samplecounts / 1);
		}
		else
			ftr[i] = rand() % (featuredim*samplecounts / 1);

		ftr_copy[i] = ftr[i];
	}
	for (int i = 0; i != samplecounts; ++i) {
		if (rand() % 2 == 1)
			label.push_back(1);
		else
			label.push_back(-1);
	}
	SortFeature(ftr.get(), ind.get(), featuredim, samplecounts);

	AdaBoost<DecisionStump<double>, double> bbb;
	bbb.Initialize(ftr.get(), ind.get(), samplecounts, featuredim, label);
	bbb.SetTrainPara(300, 0.49, 0.01);
	bbb.Train();
	Print(ftr.get(), featuredim, samplecounts);
	for (int x = 0; x != featuredim - 1; ++x) {
		for (int y = 0; y != samplecounts - 1; ++y) {
			if (*(ftr.get() + x*samplecounts + y) == *(ftr.get() + x*samplecounts + y + 1))
				*(ftr.get() + x*samplecounts + y) = 0;
			else
				*(ftr.get() + x*samplecounts + y) = 1;
		}
	}
	cout << " " << endl;
	Print(ftr.get(), featuredim, samplecounts);


	////test res and test fun
	//vector<vector<double>> s;
	//for (int y = 0; y != samplecounts; ++y) {
	//	vector < double > asample;
	//	for (int x = 0; x != featuredim; ++x) {
	//		asample.push_back(*(ftr_copy.get() + x*samplecounts + y));
	//	}
	//	s.push_back(asample);
	//}
	//for (int i = 0; i != samplecounts; ++i) {
	//	char label;
	//	double conf;
	//	bbb.Test(s[i], label, conf);
	//	cout << (int)label << "  " << (int)bbb.m_vTrainRes[i] << "     " << conf << "   " << bbb.m_vTrainResConfidence[i] << endl;
	//}


	////test time 
	//s.clear();
	//for (int i = 0; i != 1000; i++) {
	//	s.push_back(vector<double>{});
	//	for (int j = 0; j != 20000; ++j) {
	//		s[i].push_back(j);
	//	}
	//}


	////test transpose array
	//Print(ftr.get(), featuredim, samplecounts);
	//cout << endl;
	//Transpose(ftr.get(), featuredim, samplecounts);
	//cout << endl;
	//Print(ftr.get(), samplecounts, featuredim);
	//cout << endl;

	system("pause");
}