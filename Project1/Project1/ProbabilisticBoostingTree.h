#pragma once
#include "AdaBoost.h"

#include <vector>
#include <iostream>
#include <set>


template<typename STRONGER ,typename T>
class Node {
private:
	typedef unsigned int ui;
	//input basic element
	const T * m_pFeature = nullptr;
	const ui * m_pIndex = nullptr;
	ui m_uiSampleCnts = 0;
	ui m_uiFeatureDims = 0;
	std::vector<char> m_vLabel;
	
	//some cal and get these var
	// ==0 is a split, !=0 is a leaf
	char m_cNodeAttribute;

	std::set<char> m_sLabelSet;
	std::vector<double> m_vLabelEprDist;

	STRONGER m_aStronger;
	Node * m_LeftChild;
	Node * m_RightChild;


private:
	



};


template <typename STRONGER, typename T>
class ProbabilisticBoostingTree {
private:
	typedef unsigned int ui;

	//basic element
	const T * m_pFeature = nullptr;
	const ui * m_pIndex = nullptr;
	ui m_uiSampleCnts = 0;
	ui m_uiFeatureDims = 0;
	std::vector<char> m_vLabel;
	std::vector<double> m_vWeight;

	//


public:
	// Root node of the tree
	Node<STRONGER, T>* m_pRootNode;
	//stop condition
	unsigned int m_uiMaxTreeDepth;
	unsigned int m_uiMinLeafNumber;
	double m_dMaxEprDistribution;

	// All nodes in the Probability boosting tree will be stored in this vector according to training sequence,
	// PBT users should use this vector to recover the tree structure. 
	std::vector<Node<STRONGER, T>*> m_nodes;
};



