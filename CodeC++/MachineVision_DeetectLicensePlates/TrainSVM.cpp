// TrainSVM.cpp : implementation file
//


#include "TrainSVM.h"
#include "feature.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

// TrainSVM dialog

IMPLEMENT_DYNAMIC(TrainSVM, CDialogEx)

TrainSVM::TrainSVM(CWnd* pParent /*=NULL*/)
	: CDialogEx(TrainSVM::IDD, pParent)
{
	
	
}

TrainSVM::~TrainSVM()
{
}

void TrainSVM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, data_folder);
	DDX_Control(pDX, IDC_EDIT2, output_file);
	DDX_Control(pDX, IDC_BUTTON1, browse1);
	DDX_Control(pDX, IDC_BUTTON2, browse2);
	DDX_Control(pDX, IDC_COMBO1, svm_type);
	DDX_Control(pDX, IDC_COMBO2, svm_kernel);
	DDX_Control(pDX, IDC_EDIT3, degree);
	DDX_Control(pDX, IDC_EDIT4, gamma);
	DDX_Control(pDX, IDC_EDIT5, coef0);
	DDX_Control(pDX, IDC_EDIT6, c);
	DDX_Control(pDX, IDC_EDIT7, p);
	DDX_Control(pDX, IDC_EDIT8, nu);
	DDX_Control(pDX, IDC_COMBO3, type);
	DDX_Control(pDX, IDC_EDIT9, max_iter);
	DDX_Control(pDX, IDC_EDIT10, epsilon);
	DDX_Control(pDX, IDC_BUTTON3, start);
	DDX_Control(pDX, IDC_BUTTON4, cancel);
	DDX_Control(pDX, IDC_EDIT11, num_class);
	DDX_Control(pDX, IDC_EDIT12, num_sample);
	DDX_Control(pDX, IDC_EDIT13, num_feature);
	DDX_Control(pDX, IDC_STATUS, status);
}


BEGIN_MESSAGE_MAP(TrainSVM, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &TrainSVM::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &TrainSVM::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &TrainSVM::OnBnClickedButton2)
	ON_CBN_EDITCHANGE(IDC_COMBO1, &TrainSVM::OnCbnEditchangeCombo1)
	ON_CBN_SELENDOK(IDC_COMBO1, &TrainSVM::OnCbnSelendokCombo1)
	ON_CBN_SELENDOK(IDC_COMBO2, &TrainSVM::OnCbnSelendokCombo2)
	ON_CBN_SELENDOK(IDC_COMBO3, &TrainSVM::OnCbnSelendokCombo3)
	ON_BN_CLICKED(IDC_BUTTON4, &TrainSVM::OnBnClickedButton4)
END_MESSAGE_MAP()


// TrainSVM message handlers
//BOOL TrainSVM::OnInitDialog()
//{
	/*if ( ::IsWindow(this->m_hWnd))
	{
		epsilon.SetWindowTextW(_T("1e-6"));
		epsilon.EnableWindow(false);
		num_feature.SetWindowTextW(_T("32"));
		num_feature.EnableWindow(false);
	}
	*/
	//return true;
//}//

void TrainSVM::OnBnClickedButton1()
{
	//Browse folder data
	CFolderDialog dlg(_T("Sellect Folder"), _T("C:/Windows"), this);
	if(dlg.DoModal() == IDOK)
	{
		data_folder.SetWindowTextW(dlg.GetFolderPath());
	}
}


void TrainSVM::OnBnClickedButton2()
{
	// Browse output file
	static CString Filter=_T("All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("*.txt"), NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,Filter,NULL); 
	dlg.m_ofn.lpstrTitle = _T("Save File");
	if(dlg.DoModal() == IDOK)
	{
		output_file.SetWindowTextW(dlg.GetPathName());
	}
}

void TrainSVM::OnBnClickedButton3()
{
	//Start training
	training_status(false);
	status.SetWindowTextW(_T("Training ..."));
	if(do_training())
		MessageBoxA(NULL, "Training completed!", "Message", 0);
	else
		MessageBoxA(NULL, "Training error, some parameters are not valid", "Message", 0);
	training_status(true);
	status.SetWindowTextW(_T("Done!"));
}

bool TrainSVM::do_training()
{
	if(!set_params()) return false;
	CString soutput_file;
	CString sdata_folder;
	CString snum_class;
	CString snum_sample;
	CString snum_feature;
	num_class.GetWindowTextW(snum_class);
	num_feature.GetWindowTextW(snum_feature);
	num_sample.GetWindowTextW(snum_sample);
	output_file.GetWindowTextW(soutput_file);
	data_folder.GetWindowTextW(sdata_folder);

	string input;
	wstring ss_folder = (wstring)sdata_folder;
	input.assign(ss_folder.begin(), ss_folder.end());

	int number_of_sample = _wtoi(snum_sample);
	int number_of_class = _wtoi(snum_class);

	vector<string> folders = list_folder(input);
	if(folders.size() <= 0) 
		return false;
	if(number_of_class != folders.size() || number_of_sample <=0 || number_of_class <=0)
		return false;
	Mat src;
	Mat data = Mat(number_of_sample * number_of_class, number_of_feature, CV_32FC1);
	Mat label = Mat(number_of_sample * number_of_class, 1, CV_32FC1);
	int index = 0;
	for(size_t i = 0; i < folders.size(); ++i)
	{
		vector<string> files = list_file(folders.at(i));
		if(files.size() <= 0 || files.size() != number_of_sample)
			return false;
		string folder_path = folders.at(i);
		string label_folder = folder_path.substr(folder_path.length() - 1);
		for(size_t j = 0; j < files.size(); ++j)
		{
			src = imread(files.at(j));
			if(src.empty()) return false;
			vector<float> feature = calculate_feature(src);
			for(size_t t = 0; t < feature.size(); ++t) 
				data.at<float>(index, t) = feature.at(t);
			label.at<float>(index, 0) = i;
			index++;
		}
	}

	svm.train_auto(data, label, Mat(), Mat(), params);
	
	wstring ss_output = (wstring)soutput_file;
	string output;
	output.assign(ss_output.begin(), ss_output.end());
	svm.save(output.c_str());

	return true;
}



void TrainSVM::OnCbnEditchangeCombo1()
{
	
}

void TrainSVM::training_status(bool sst)
{
	this->data_folder.EnableWindow(sst);
	this->output_file.EnableWindow(sst);
	this->browse1.EnableWindow(sst);
	this->browse2.EnableWindow(sst);
	this->num_class.EnableWindow(sst);
	this->num_feature.EnableWindow(sst);
	this->num_sample.EnableWindow(sst);
	this->svm_type.EnableWindow(sst);
	this->svm_kernel.EnableWindow(sst);
	this->degree.EnableWindow(sst);
	this->coef0.EnableWindow(sst);
	this->gamma.EnableWindow(sst);
	this->c.EnableWindow(sst);
	this->nu.EnableWindow(sst);
	this->p.EnableWindow(sst);
	this->type.EnableWindow(sst);
	this->max_iter.EnableWindow(sst);
	this->epsilon.EnableWindow(sst);
	this->start.EnableWindow(sst);
	this->cancel.EnableWindow(sst);
}
void TrainSVM::OnCbnSelendokCombo1()
{
	// Change SVM type
	int t = svm_type.GetCurSel();
	switch(t)
	{
	case 0:
		c.EnableWindow(true);
		nu.SetWindowTextW(_T(""));
		nu.EnableWindow(false);
		p.SetWindowTextW(_T(""));
		p.EnableWindow(false);
		break;
	case 1:
		nu.EnableWindow(true);
		c.EnableWindow(false);
		c.SetWindowTextW(_T(""));
		p.SetWindowTextW(_T(""));
		p.EnableWindow(false);
		break;
	case 2:
		nu.EnableWindow(true);
		c.EnableWindow(false);
		c.SetWindowTextW(_T(""));
		p.SetWindowTextW(_T(""));
		p.EnableWindow(false);
		break;
	case 3:
		c.EnableWindow(true);
		p.EnableWindow(true);
		nu.SetWindowTextW(_T(""));
		nu.EnableWindow(false);
		break;
	case 4:
		c.EnableWindow(true);
		nu.EnableWindow(true);
		p.SetWindowTextW(_T(""));
		p.EnableWindow(true);
		break;
	}
}


void TrainSVM::OnCbnSelendokCombo2()
{
	// Change kernel
	int t = svm_kernel.GetCurSel();
	switch(t)
	{
	case 0:
		degree.SetWindowTextW(_T(""));
		degree.EnableWindow(false);
		gamma.SetWindowTextW(_T(""));
		gamma.EnableWindow(false);
		coef0.SetWindowTextW(_T(""));
		coef0.EnableWindow(false);
		break;
	case 1:
		degree.EnableWindow(true);
		gamma.EnableWindow(true);
		coef0.EnableWindow(true);
		break;
	case 2:
		gamma.EnableWindow(true);
		degree.SetWindowTextW(_T(""));
		degree.EnableWindow(false);
		coef0.SetWindowTextW(_T(""));
		coef0.EnableWindow(false);
		break;
	case 3:
		gamma.EnableWindow(true);
		coef0.EnableWindow(true);
		degree.SetWindowTextW(_T(""));
		degree.EnableWindow(false);
		break;
	}
}


void TrainSVM::OnCbnSelendokCombo3()
{
	// TODO: Add your control notification handler code here
}

bool TrainSVM::set_params()
{
	
	CString sdegree;
	CString scoef0;
	CString sgamma;
	CString sc;
	CString sp;
	CString snu;
	CString smax_iter;
	CString sepsilon;
	
	degree.GetWindowTextW(sdegree);
	coef0.GetWindowTextW(scoef0);
	gamma.GetWindowTextW(sgamma);
	c.GetWindowTextW(sc);
	p.GetWindowTextW(sp);
	nu.GetWindowTextW(snu);
	max_iter.GetWindowTextW(smax_iter);
	epsilon.GetWindowTextW(sepsilon);

	params.svm_type =  svm_type.GetCurSel() + 100; // enum { C_SVC=100, NU_SVC=101,...}
	params.kernel_type = svm_kernel.GetCurSel();  // enum { LINEAR=0, POLY=1, ...}
	if(degree.IsWindowEnabled())
		params.degree = _wtof(sdegree);               // -wtof() convert cstring to double
	if(coef0.IsWindowEnabled())
		params.coef0 = _wtof(scoef0);
	if(gamma.IsWindowEnabled())
		params.gamma = _wtof(sgamma);
	if(c.IsWindowEnabled())
		params.C = _wtof(sc);
	if(nu.IsWindowEnabled())
		params.nu = _wtof(snu);
	if(p.IsWindowEnabled())
		params.p = _wtof(sp);
	
	//int t = type.GetCurSel();
	//if(t == 0) t = 1;
	//else t = 2;
	int t = CV_TERMCRIT_ITER;
	int miter = _wtoi(smax_iter);
	//double eps = _wtof(sepsilon);
	double eps = 1e-6;
	cv::TermCriteria termcrit = cv::TermCriteria(t, miter, eps);
	params.term_crit = termcrit;
	return true;	

}

vector<string> TrainSVM::list_folder(string path)
{
	vector<string> folders;
	DIR *dir = opendir(path.c_str());
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
		 if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) 
		 {
			 string folder_path = path + "/" + string(entry->d_name);
			 folders.push_back(folder_path);
		 }
    }
    closedir(dir);
	return folders;

}
vector<string> TrainSVM::list_file(string folder_path)
{
	vector<string> files;
	DIR *dir = opendir(folder_path.c_str());
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
		 if ((strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0)) 
		 {
			 string file_path = folder_path + "/" + string(entry->d_name);
			 files.push_back(file_path);
		 }
    }
    closedir(dir);
	return files;
}

void TrainSVM::OnBnClickedButton4()
{
	// Cancel
	this->OnCancel();
}
