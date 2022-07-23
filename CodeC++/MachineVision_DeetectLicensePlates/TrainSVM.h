#pragma once
#include <opencv2/ml/ml.hpp>
#include <opencv2/core/core.hpp>
#include "dirent.h"

using namespace std;
// TrainSVM dialog

class TrainSVM : public CDialogEx
{
	DECLARE_DYNAMIC(TrainSVM)

public:
	TrainSVM(CWnd* pParent = NULL);   // standard constructor
	virtual ~TrainSVM();

// Dialog Data
	enum { IDD = IDD_TRAINING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
//protected:
	//BOOL OnInitDialog();
public:
	CEdit data_folder;
	CEdit output_file;
	CButton browse1;
	CButton browse2;
	CComboBox svm_type;
	CComboBox svm_kernel;
	CEdit degree;
	CEdit gamma;
	CEdit coef0;
	CEdit c;
	CEdit p;
	CEdit nu;
	CComboBox type;
	CEdit max_iter;
	CEdit epsilon;
	CButton start;
	CButton cancel;
	CEdit num_class;
	CEdit num_sample;
	CEdit num_feature;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnCbnEditchangeCombo1();
	afx_msg void OnCbnSelendokCombo1();
	afx_msg void OnCbnSelendokCombo2();
	afx_msg void OnCbnSelendokCombo3();
private:
	void training_status(bool sst);
	cv::SVM svm;
	cv::SVMParams params;
	bool set_params();
	vector<string> list_file(string);
	vector<string> list_folder(string);
	bool do_training();
	
public:
	CStatic status;
};
