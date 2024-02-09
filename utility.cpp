#include    "utility.h"
#include  <filesystem>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

namespace fs = std::filesystem;

// �w�莞�Ԍ�ɃX���b�h���N��
void Invoke(std::function<void()> Function, int Time)
{
	std::thread([=]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(Time));
			//			Sleep(Time);
			Function();
		}).detach();
}

// �t�@�C���p�X����t�@�C�����������擾����
std::string GetFileName(std::string filepath) {
	// u8�G���R�[�h�ł����������R���X�g���N�g
	fs::path p = fs::u8path(filepath.c_str());

	// �p�X����A�t�@�C�������������f�B���N�g���A�t�@�C�����A
	// �g���q���������t�@�C�����A�g���q�𒊏o
//    std::cout << "�f�B���N�g���� : " << p.parent_path() << std::endl;
//    std::cout << "�t�@�C����  : " << p.filename() << std::endl;
//    std::cout << "�t�@�C�����i�g���q�Ȃ��j : " << p.stem() << std::endl;
//    std::cout << "�g���q : " << p.extension() << std::endl;

	return p.filename().string();
}

// �t�@�C���p�X����t�@�C�����������擾����
std::wstring GetFileNameWide(std::string filepath) {
	// u8�G���R�[�h�ł����������R���X�g���N�g
//    fs::path p = fs::u8path(filepath.c_str());
	fs::path p = fs::path(filepath.c_str());

	// �V�X�e���v�h�c�d�����Ŏ擾
	const std::wstring wsfilename = p.filename().wstring();

	// ���P�[������{�ɂ���
	setlocale(LC_ALL, "Japanese");
	std::wcout << wsfilename << std::endl;

	return wsfilename;
}

// �t�@�C���p�X����t�@�C���g���q���擾����
std::string GetFileExt(std::string filepath) {
	// UTF-8
//    fs::path p = fs::u8path(filepath.c_str());
	fs::path p = fs::path(filepath.c_str());

	return p.extension().string();
}

// �t�@�C���p�X(UTF16)����t�@�C���g���q���擾����
std::wstring GetFileExtWide(std::string filepath) {
	//u8�G���R�[�h�ł����������R���X�g���N�g
	fs::path p = fs::u8path(filepath.c_str());

	return p.extension().wstring();
}

// �t�@�C���p�X����e�f�B���N�g�����擾����
std::wstring GetParentDirectoryWide(std::string filepath) {
	// u8�G���R�[�h�ł����������R���X�g���N�g
	fs::path p = fs::u8path(filepath.c_str());

	// �V�X�e���v�h�c�d����(UTF-16)�Ŏ擾
	std::wstring wsparentdirectory;

	// �e�f�B���N�g���������Ă��邩�H
	if (p.has_parent_path()) {
		wsparentdirectory = p.parent_path().wstring() + L"\\";
	}
	else {
		wsparentdirectory = L"\\";
	}

	return wsparentdirectory;
}