#pragma once

#include "DecisionStump.h"

#include <iostream>
#include <vector>
#include <cmath>

#define AB_POSLABEL DS_POSLABEL
#define AB_NEGLABEL DS_NEGLABEL

template<typename WEAKER,typename T>
class AdaBoost {
private:
	typedef unsigned int ui;

	//input basic element
	//the feature is 2D, each row is a attribute;
	//if the WEAKER need a sorted feature, the row data MUST be sorted!!
	const T * m_pFeature = nullptr;
	//if the WEAKER need a sorted feature ,then this index MUST store the sorted index of feature;
	//else just =nullptr is ok;
	const ui * m_pIndex = nullptr;
	//the counts of samples(cols);
	ui m_uiSampleCnts = 0;
	//the counts of feature's attribute;
	ui m_uiFeatureDims = 0;
	//the label of samples, its value should be AB_POSLABEL or AB_NEGLABEL
	std::vector<char> m_vLabel;
	//the weight of each sample, empty means use InitWeight() to init weight 
	std::vector<double> m_vWeight;

	//train para, stop condition
	//the max counts of weak classifier
	ui m_uiMaxWeakerCounts = 1;
	//if a weaker's train err > this value ,the adaboost will break and stop train
	double m_dMaxWeakerErr = 0.45;
	//if the sum train of adaboost < this value ,the adaboost will break and stop train
	double m_dMinSumErr = 0.005;

	//train output 
	//the set of a weak classifier
	std::vector<WEAKER> m_vWeakerSet;
	//each weaker's weight corresponding to the m_vWeakerSet
	std::vector<double> m_vWeakerWeight;

	//method 
	//init weight, NOTE: sum weight after this fun != 1
	bool InitWeight(std::vector<double> &vWeight);
	//after this fun, sum weight =1
	bool NormalizeWeight(std::vector<double> &vWeight);
	//if sum >0 return AB_POSLABEL, else return AB_NEGLABEL
	char Sign(double sum);

public:
	//member
	//the train result(contain the label of each sample)
	std::vector<char> m_vTrainRes;
	std::vector<double> m_vTrainResConfidence;

	//method
	AdaBoost() {};
	AdaBoost(const T * const pFeature, const ui * const pIndex, const ui uiSampleCnts, const ui uiFeatureDims, const std::vector<char> vLabel, const std::vector<double> vWeight = {});
	bool Initialize(const T * const pFeature, const ui * const pIndex, const ui uiSampleCnts, const ui uiFeatureDims, const std::vector<char> vLabel, const std::vector<double> vWeight = {});
	//this fun should be run before Train()
	bool SetTrainPara(const ui uiMaxWeakerCounts, const double dMaxWeakerErr, const double m_dMinSumErr);
	bool Train(void);
	bool Test(const std::vector<T> vSample, char &cLabel, double &dConfidence);
};

template<typename WEAKER, typename T>
inline bool AdaBoost<WEAKER, T>::InitWeight(std::vector<double>& vWeight)
{
	if (vWeight.empty()) {
		ui uiPosCounts = 0;
		ui uiNegCounts = 0;
		for (ui i = 0; i != m_uiSampleCnts; ++i) {
			if (m_vLabel[i] == AB_POSLABEL)
				uiPosCounts++;
			else
				uiNegCounts++;
			vWeight.push_back(1.0);
		}
		for (ui i = 0; i != m_uiSampleCnts; ++i) {
			if (m_vLabel[i] == AB_POSLABEL)
				vWeight[i] /= static_cast<double>(uiPosCounts);
			else
				vWeight[i] /= static_cast<double>(uiNegCounts);
		}
	}
	return true;
}

template<typename WEAKER, typename T>
inline bool AdaBoost<WEAKER, T>::NormalizeWeight(std::vector<double>& vWeight)
{
	double dSumWeight = 0.0;
	for (auto iter = vWeight.cbegin(); iter != vWeight.cend(); ++iter) {
		dSumWeight += *iter;
	}
	if (dSumWeight < 1e-6) {
		std::cout << "[AdaBoost Err] : NormalizeWeight Err : SumWeight < 1e-6" << std::endl;
		return false;
	}
	for (auto iter = vWeight.begin(); iter != vWeight.end(); ++iter) {
		*iter /= dSumWeight;
	}
	return true;
}

template<typename WEAKER, typename T>
inline char AdaBoost<WEAKER, T>::Sign(double sum)
{
	return sum > 0 ? AB_POSLABEL : AB_NEGLABEL;
}

template<typename WEAKER, typename T>
inline AdaBoost<WEAKER, T>::AdaBoost(const T * const pFeature, const ui * const pIndex, const ui uiSampleCnts, const ui uiFeatureDims, const std::vector<char> vLabel, const std::vector<double> vWeight)
{
	Initialize(pFeature, pIndex, uiSampleCnts, uiFeatureDims, vLabel, vWeight);
}

template<typename WEAKER, typename T>
inline bool AdaBoost<WEAKER, T>::Initialize(const T * const pFeature, const ui * const pIndex, const ui uiSampleCnts, const ui uiFeatureDims, const std::vector<char> vLabel, const std::vector<double> vWeight)
{
	//check para 
	if (uiSampleCnts != vLabel.size()) {
		std::cout << "[AdaBoost Err] : SampleCnts != Label.size()" << std::endl;
		return false;
	}
	if (!vWeight.empty() && vWeight.size() != uiSampleCnts) {
		std::cout << "[AdaBoost Err] : SampleCnts != Weight.empty()" << std::endl;
		return false;
	}
	for (auto iter = m_vLabel.cbegin(); iter != m_vLabel.cend(); ++iter) {
		if (*iter != AB_POSLABEL && *iter != AB_NEGLABEL) {
			std::cout << "[AdaBoost Err] : Label Value Err" << std::endl;
			return false;
		}
	}
	for (auto iter = m_vWeight.cbegin(); iter != m_vWeight.cend(); ++iter) {
		if (*iter < 0.0) {
			std::cout << "[AdaBoost Err] : Weight Value Err" << std::endl;
			return false;
		}
	}

	m_pFeature = pFeature;
	m_pIndex = pIndex;
	m_uiSampleCnts = uiSampleCnts;
	m_uiFeatureDims = uiFeatureDims;
	m_vLabel = vLabel;
	m_vWeight = vWeight;
	return true;
}

template<typename WEAKER, typename T>
inline bool AdaBoost<WEAKER, T>::SetTrainPara(const ui uiMaxWeakerCounts, const double dMaxWeakerErr, const double dMinSumErr)
{
	m_uiMaxWeakerCounts = uiMaxWeakerCounts;
	if (0.0 < dMaxWeakerErr && dMaxWeakerErr < 0.5)
		m_dMaxWeakerErr = dMaxWeakerErr;
	else {
		std::cout << "[AdaBoost Err] : MaxWeakerErr Should 0< ? <0.5" << std::endl;
		return false;
	}
	if (0.0 < dMinSumErr && dMinSumErr < 1.0)
		m_dMinSumErr = dMinSumErr;
	else {
		std::cout << "[AdaBoost Err] : MinWeakerErr Should 0< ? <1.0" << std::endl;
		return false;
	}

	return true;
}

template<typename WEAKER, typename T>
inline bool AdaBoost<WEAKER, T>::Train(void)
{
	//check para 
	if (m_pFeature == nullptr) {
		std::cout << "[AdaBoost Err] : Feature Empty" << std::endl;
		return false;
	}

	//output 
	m_vWeakerSet.clear();
	m_vWeakerWeight.clear();

	if (false == InitWeight(m_vWeight))
		return false;
	if (false == NormalizeWeight(m_vWeight))
		return false;

	std::vector<double> vTrainResSum(m_uiSampleCnts, 0);
	std::vector<double> vTrainResPos(m_uiSampleCnts, 0);
	std::vector<double> vTrainResNeg(m_uiSampleCnts, 0);
	for (ui uiWeakerNum = 0; uiWeakerNum != m_uiMaxWeakerCounts; ++uiWeakerNum) {
		//init a weaker classifier
		WEAKER aWeaker;
		if (false == aWeaker.Initialize(m_pFeature, m_pIndex, m_uiSampleCnts, m_uiFeatureDims, m_vLabel, m_vWeight)) {
			std::cout << "[AdaBoost Err] : aWeaker Initialize Err" << std::endl;
			continue;
		}

		//train a weaker
		if (false == aWeaker.Train()) {
			std::cout << "[AdaBoost Err] : aWeaker Train Err" << std::endl;
			continue;
		}

		//get this weaker's weight
		double alpha = 0.5*log((1.0 - aWeaker.GetTrainErr()) / (aWeaker.GetTrainErr()+1e-6));

		//update data weight 
		for (ui i = 0; i != m_uiSampleCnts; ++i) {
			if (aWeaker.m_vTrainRes[i] != m_vLabel[i])
				m_vWeight[i] *= exp(alpha);
			else
				m_vWeight[i] *= exp(-alpha);
		}
		if (false == NormalizeWeight(m_vWeight)) {
			break;		
		}


		//cal the sum err and get the tmp label train res
		//reference:Probabilistic Boosting-Tree: Learning Discriminative Models for Classification,Recognition, and Clustering Recognition, and Clustering
		ui errcounts = 0;
		for (ui i = 0; i != m_uiSampleCnts; ++i) {
			vTrainResPos[i] += 0.5*AB_POSLABEL*static_cast<double>(aWeaker.m_vTrainRes[i])*alpha;
			vTrainResNeg[i] += 0.5*AB_NEGLABEL * static_cast<double>(aWeaker.m_vTrainRes[i])*alpha;
			vTrainResSum[i] += static_cast<double>(aWeaker.m_vTrainRes[i])*alpha;
			if (Sign(vTrainResSum[i]) != m_vLabel[i])
				++errcounts;
		}
		double errrate = static_cast<double>(errcounts) / static_cast<double>(m_uiSampleCnts);

		//push weaker para
		std::cout << "Dim:"  << aWeaker.GetDim()      << " Split:"  << aWeaker.GetSplit() << " Dir:" << (int)aWeaker.GetDir() 
			      << " Err:" << aWeaker.GetTrainErr() << " Sumerr=" << errrate            << std::endl;
		aWeaker.MinClassifier();
		m_vWeakerSet.push_back(aWeaker);
		m_vWeakerWeight.push_back(alpha);

		//stop condition
		if (errrate < m_dMinSumErr) {
			std::cout << "[AdaBoost Stop] : SumErr < MinSumErr" << std::endl;
			break;
		}
		if (aWeaker.GetTrainErr() > m_dMaxWeakerErr) {
			std::cout << "[AdaBoost Stop] : aWeakerErr > MaxWeakerErr" << std::endl;
			break;
		}

	}

	//cal the train res and the confidendce 
	if (!m_vWeakerSet.empty()) {
		m_vTrainRes.resize(m_uiSampleCnts);
		m_vTrainResConfidence.resize(m_uiSampleCnts);
		for (ui i = 0; i != m_uiSampleCnts; ++i) {
			m_vTrainRes[i] = Sign(vTrainResSum[i]);
			if (m_vTrainRes[i] == AB_POSLABEL)
				m_vTrainResConfidence[i] = exp(vTrainResPos[i]) / (exp(vTrainResPos[i]) + exp(vTrainResNeg[i]) + 1e-6);
			else
				m_vTrainResConfidence[i] = exp(vTrainResNeg[i]) / (exp(vTrainResPos[i]) + exp(vTrainResNeg[i]) + 1e-6);
		}
	}
	else {
		return false;
	}

	return true;
}

template<typename WEAKER, typename T>
inline bool AdaBoost<WEAKER, T>::Test(const std::vector<T> vSample, char & cLabel, double & dConfidence)
{
	if (vSample.size() != m_uiFeatureDims)
		return false;
	if (m_vWeakerSet.empty() || m_vWeakerWeight.empty())
		return false;

	double dTrainResPos = 0.0;
	double dTrainResNeg = 0.0;
	double dTrainResSum = 0.0;
	for (ui i = 0; i != m_vWeakerSet.size(); ++i) {
		if (false == m_vWeakerSet[i].Test(vSample, cLabel, dConfidence))
			continue;
		double tmp = 0.5*AB_POSLABEL*static_cast<double>(cLabel)*m_vWeakerWeight[i];
		dTrainResPos += tmp;
		dTrainResNeg += -tmp;
		dTrainResSum += static_cast<double>(cLabel)*m_vWeakerWeight[i];
	}
	cLabel = Sign(dTrainResSum);
	dConfidence = (cLabel > 0 ? exp(dTrainResPos) : exp(dTrainResNeg)) / (exp(dTrainResPos) + exp(dTrainResNeg) + 1e-6);
}
