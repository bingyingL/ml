#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#define DS_POSLABEL ( 1)
#define DS_NEGLABEL (-1)
#define DS_NEG_POS ( -1)
#define DS_POS_NEG ( 1)

template<typename T>
class DecisionStump {
private:
	typedef unsigned int ui;
	//input basic element
	//the feature is 2D, each row is a attribute, and each col is a sample.each row MUST BE SORTED!!;
	const T * m_pSortedFeature = nullptr;				
	//the index is 2D, the same size as feature each row correspond to the same row in feature, each row store the sorted index of feature;
	const ui * m_pSortedIndex = nullptr;				
	//the counts of samples(cols);
	ui m_uiSampleCnts = 0;
	//the counts of feature's attribute;
	ui m_uiFeatureDims = 0;
	//the label of samples, its value should be DS_POSLABEL or DS_NEGLABEL
	std::vector<char> m_vLabel;
	//the weight of each sample
	std::vector<double> m_vWeight;
	
	//output after train
	//the optimal dim of feature that selected 
	ui m_uiDim = 0;
	//the split value 
	T m_tSplit = 0;
	//<= or >, the dir
	char m_cDir = 0;
	//the train err
	double m_dErr = 0.0;

public:
	//the train result(contain the label of each sample)
	std::vector<char> m_vTrainRes;

	DecisionStump() {};
	DecisionStump(const T*const pSortedFeature, const ui *const pSortedIndex, const ui uiSampleCnts, const  ui uiFeatureDims, const std::vector<char> vLabel, const std::vector<double> vWeight);
	bool Initialize(const T*const pSortedFeature, const ui *const pSortedIndex, const ui uiSampleCnts, const  ui uiFeatureDims, const std::vector<char> vLabel, const std::vector<double> vWeight);
	//train 
	bool Train(void);
	bool Test(const std::vector<T> &vSample, char &cLabel, double &dConfidence);
	char GetDir(void) { return m_cDir; };
	T GetSplit(void) { return m_tSplit; };
	ui GetDim(void) { return m_uiDim; };
	double GetTrainErr(void) { return m_dErr; };
	std::vector<char> GetTrainRes(void) { return m_vTrainRes; };
};

template<typename T>
inline DecisionStump<T>::DecisionStump(const T * const pSortedFeature, const ui * const pSortedIndex, const ui uiSampleCnts, const ui uiFeatureDims, const std::vector<char> vLabel, const std::vector<double> vWeight)
{
	Initialize(pSortedFeature, pSortedIndex, uiSampleCnts, uiFeatureDims, vLabel, vWeight);
}

template<typename T>
inline bool DecisionStump<T>::Initialize(const T * const pSortedFeature, const ui * const pSortedIndex, const ui uiSampleCnts, const ui uiFeatureDims, const std::vector<char> vLabel, const std::vector<double> vWeight)
{
	//check para 
	if (pSortedFeature == nullptr || pSortedIndex == nullptr) {
		std::cout << "[DecisionStump Err] : Feature Or Index Empty" << std::endl;
		return false;
	}
	if (uiSampleCnts != vLabel.size() || uiSampleCnts != vWeight.size()) {
		std::cout << "[DecisionStump Err] : Creat DecisionStump Class False" << std::endl;
		return false;
	}
	if (uiSampleCnts == 0 || uiFeatureDims == 0) {
		std::cout << "[DecisionStump Err] : SampleCnts Or FeatureDims Is 0" << std::endl;
		return false;
	}
	for (auto iter = vLabel.cbegin(); iter != vLabel.cend(); ++iter) {
		if (*iter != DS_POSLABEL && *iter != DS_NEGLABEL) {
			std::cout << "[DecisionStump Err] : Label Value Err" << std::endl;
			return false;
		}
	}
	for (auto iter = vWeight.cbegin(); iter != vWeight.cend(); ++iter) {
		if (*iter < 0.0) {
			std::cout << "[DecisionStump Err] : Weight Value Err" << std::endl;
			return false;
		}
	}

	m_pSortedFeature = pSortedFeature;
	m_pSortedIndex = pSortedIndex;
	m_uiSampleCnts = uiSampleCnts;
	m_uiFeatureDims = uiFeatureDims;
	m_vLabel = vLabel;
	m_vWeight = vWeight;
	return true;
}

template<typename T>
inline bool DecisionStump<T>::Train(void)
{
	if (m_pSortedFeature == nullptr || m_pSortedIndex == nullptr) {
		std::cout << "[DecisionStump Err] : Feature Or Index Empty" << std::endl;
		return false;
	}
	//train
	
	//get the pos and neg sum weight
	double dPosWeight = 0.0;
	double dNegWeight = 0.0;
	for (ui i = 0; i != m_uiSampleCnts; ++i) {
		m_vLabel[i] == DS_POSLABEL ? (dPosWeight += m_vWeight[i]) : (dNegWeight += m_vWeight[i]);
	}
	//get the feature dim and splite
	T feature;
	ui index;
	char label;
	double weight;

	double neg_pos_err = dNegWeight;
	double pos_neg_err = dPosWeight;

	double err = std::min(neg_pos_err, neg_pos_err);
	char dir = neg_pos_err < pos_neg_err ? DS_NEG_POS : DS_POS_NEG;
	T split = *m_pSortedFeature-1;
	ui dim = 0;

	//get the min err, dim, split, dir
	for (ui x = 0; x != m_uiFeatureDims; ++x) {

		neg_pos_err = dNegWeight;
		pos_neg_err = dPosWeight;
		double errtmp = std::min(neg_pos_err, pos_neg_err);
		char dirtmp = neg_pos_err < pos_neg_err ? DS_NEG_POS : DS_POS_NEG;
		T splittmp = *(m_pSortedFeature+x*m_uiSampleCnts)-1;
		for (ui y = 0; y != m_uiSampleCnts; ++y) {
			feature = *(m_pSortedFeature + x*m_uiSampleCnts + y);
			index= *(m_pSortedIndex + x*m_uiSampleCnts + y);
			label = m_vLabel[index];
			weight = m_vWeight[index];
			
			//if ---_++++ 
			if (label == DS_NEGLABEL) {
				neg_pos_err -= weight;
				pos_neg_err += weight;
			}
			//else +++_---
			else {
				neg_pos_err += weight;
				pos_neg_err -= weight;
			}
			if (neg_pos_err < pos_neg_err) {
				if (neg_pos_err <= errtmp) {
					errtmp = neg_pos_err;
					splittmp = feature;
					dirtmp = DS_NEG_POS;
				}
			}
			else {
				if (pos_neg_err <= errtmp) {
					errtmp = pos_neg_err;
					dirtmp = DS_POS_NEG;
					splittmp = feature;
				}
			}
		}
		if (errtmp <= err) {
			err = errtmp;
			split = splittmp;
			dir = dirtmp;
			dim = x;
		}
	}

	//output
	m_uiDim = dim;
	m_tSplit = split;
	m_cDir = dir;
	m_dErr = err;
	//the train res in feature set
	m_vTrainRes.resize(m_uiSampleCnts);
	for (ui i = 0; i != m_uiSampleCnts; ++i) {
		feature = *(m_pSortedFeature + m_uiDim*m_uiSampleCnts + i);
		index = *(m_pSortedIndex + m_uiDim*m_uiSampleCnts + i);
		if (m_cDir == DS_NEG_POS) {
			feature <= m_tSplit ? (m_vTrainRes[index] = DS_NEGLABEL) : (m_vTrainRes[index] = DS_POSLABEL);
		}
		else {
			feature > m_tSplit ? (m_vTrainRes[index] = DS_NEGLABEL) : (m_vTrainRes[index] = DS_POSLABEL);
		}
	}
	return true;
}

template<typename T>
inline bool DecisionStump<T>::Test(const std::vector<T> &vSample, char &cLabel, double &dConfidence)
{
	if (vSample.size() != m_uiFeatureDims)
		return false;
	if (m_cDir == DS_NEG_POS) {
		vSample[m_uiDim] <= m_tSplit ? (cLabel = DS_NEGLABEL) : (cLabel = DS_POSLABEL);
	}
	else {
		vSample[m_uiDim] > m_tSplit ? (cLabel = DS_NEGLABEL) : (cLabel = DS_POSLABEL);
	}
	dConfidence = 1;
	return true;
}


