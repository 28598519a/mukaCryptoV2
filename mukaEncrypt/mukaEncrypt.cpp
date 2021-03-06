#include "stdafx.h"
#include <iostream>
#include "Files.h"
#include "Encrypt.h"

using namespace std;
#define _O_U16TEXT     0x20000 // file mode is UTF16 no BOM (translated)

int wmain(){
	/*
	1. locale("")：調用構造函數創建一個local，其中的空字串具有特殊含義：使用客戶環境中缺省的locale。
	(例如在簡體中文系統上，會返回簡體中文的locale[字碼頁.936])
	2. locale::global(locale(""))：將“C++標準IO庫的全域locale”設為“客戶環境中缺省的locale”。
	注意它還會設置C標準庫的locale環境，造成與“setlocale(LC_ALL, "")”類似的效果。
	3. 當local設為NULL時(取客戶端環境)，須將wcout.imbue(locale("設為程式顯示語言的字碼頁"));
	(否則若客戶端的字碼頁不支援，程式顯示的字元時會顯示為'?')
	4. wcin.imbue(locale(""))：使wcin使用“客戶環境中缺省的locale”。
	*/
	locale::global(locale(""));
	wcin.imbue(locale(""));
	/*
	1. _setmode的模式為寬自元(_O_U8TEXT 或 _O_U16TEXT 或 _O_WTEXT)模式時，為Unicode輸出。
	(效果比指定字碼頁好很多，但也有缺點[參考第2點])
	2. _setmode後，被setmode的流就得使用該mode對應的方法
	(例如_fileno(stdout),_O_U16TEXT，若使用cout則會錯誤，應使用wcout)
	*/
	_setmode(_fileno(stdout), _O_U16TEXT);
	// 輸入密鑰
	aes_key key = { 0 };
	wstring folder;
	wcout << L"輸入目錄：\n";
	wcin >> folder;
	wcout << L"請輸入16位密鑰：\n";
	cin >> (char *)&key[0];

	// 尋找所有png圖片
	vector<wstring> pngfiles;
	auto all_files = path::walk(folder);
	for (auto filename : all_files)
	{
		if (path::splitext(filename)[1] == L".png")
		{
			pngfiles.push_back(filename);
		}
	}

	for (auto &filename : pngfiles)	filename = filename.substr(folder.size(), filename.size());
	wcout << L"正在加密中...\n";

	// 圖片加密操作
	EncryptPNG(pngfiles, key);

	_wsystem(L"pause");
	return 0;
}