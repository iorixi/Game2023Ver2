#pragma once
#include	<functional>
#include	<thread>
#include    <d3d11.h>
#include    <string>
#include    <locale.h>
#include    <d3dcompiler.h>
#include <vector>

#include	<iostream>
#include	<filesystem>
#include    <assimp\Importer.hpp>
#include    <assimp\scene.h>
#include    <assimp\postprocess.h>
#include    <assimp/cimport.h>
#include	<thread>

// �w�莞�Ԍ�ɃX���b�h���N��
void Invoke(std::function<void()> Function, int Time);

// �t�@�C���p�X����t�@�C�����������擾����
std::string GetFileName(std::string filepath);
// �t�@�C���p�X����t�@�C�����������擾����
std::wstring GetFileNameWide(std::string filepath);
// �t�@�C���p�X����t�@�C���g���q���擾����
std::string GetFileExt(std::string filepath);
// �t�@�C���p�X����e�f�B���N�g�����擾����
std::wstring GetParentDirectoryWide(std::string filepath);