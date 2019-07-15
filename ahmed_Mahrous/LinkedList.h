#pragma once
#include<iostream>
#include<vector>
#include <stdio.h>
#include <stdlib.h>
#include<string>
#include <sys/stat.h>
#include<direct.h>
#include <Windows.h>
#include <psapi.h>
#include <malloc.h> 
#include <tchar.h>
#include <comdef.h>
//#include <boost/filesystem.hpp>
#include<experimental/filesystem>

using namespace std;

struct pointer
{
	char * buffer;
	long long size;
	const char * fileName;
	//const char *type;
	const char * pathName;
	string name;
	string extension;
	string path;

	vector<pointer>  next;
	bool isFolder;
	pointer() {
		//this->next = NULL;
		this->isFolder = false;
	}
};

class dataStructure {
private:
	vector<pointer> root;
	unsigned int getMaxSize() {
		unsigned int max_size = root[0].path.size();
		for (int i = 1; i < root.size(); i++) {
			if (root[i].path.size()> max_size) {
				max_size = root[i].path.size();
			}
		}
		return max_size;
	}

	bool checkFolderIsExistKernal(vector<string> str, int sIndex, string folderName, vector<pointer> p) {

		for (int i = 0; i < p.size(); i++) {
			if (str.size() == sIndex && p[i].isFolder &&folderName == p[i].name) {
				return true;
			}
			if (sIndex < str.size() && str[sIndex] == p[i].name && p[i].isFolder) {
				return checkFolderIsExistKernal(str, sIndex + 1, folderName, p[i].next);

			}

		}
		return false;
	}
	vector<pointer> getFolder(vector<string> str, vector<string> strCopy, int sIndex, unsigned int max_size, vector<pointer> &p) {
		//vector<pointer> asd;
		int i;
		for (i = 0; i < p.size(); i++) {
			if (sIndex >= strCopy.size()) {
				break;
			}
			if (sIndex < str.size() && str[sIndex] == p[i].name && p[i].isFolder) {
				return getFolder(str, strCopy, sIndex + 1, max_size, p[i].next);
			}
		}
		return p;
	}

	vector<pointer> getFolderToStoreFile(vector<string> str, int sIndex, vector<pointer> &p) {
		//vector<pointer> asd;
		int i;
		for (i = 0; i < p.size(); i++) {

			if (sIndex < str.size() && str[sIndex] == root[i].name && root[i].isFolder) {
				return getFolderToStoreFile(str, sIndex + 1, root[i].next);

			}
		}

		return p;
	}

	void print(vector<string> str, vector<string> strCopy, int sIndex, unsigned int max_size, vector<pointer> p) {
		if (max_size < 20)
			max_size = 25;
		vector<pointer> asd = getFolder(str, strCopy, sIndex, max_size, root);
		for (int i = 0; i < asd.size(); i++) {
			cout << asd[i].path;
			print_space(max_size, asd[i].path.size());
			cout << "| " << asd[i].name;
			print_space(max_size, asd[i].name.size() + 2);
			cout << " | " << (double)asd[i].size / (1024 * 1024) << " MB";
			string ss = to_string((double)asd[i].size / (1024 * 1024));
			print_space(max_size, max_size - ss.size() - 4);
			cout << " | " << asd[i].extension;
			print_space(max_size, asd[i].extension.size() + 2);
			cout << endl;
		}
		cout << "============================================================================\n";

	}
	void print_space(int max_size, int lenght) {
		for (int i = 0; i < max_size - lenght; i++)
			cout << " ";
	}
	void showTableHeader(unsigned int max_size) {
		//cout << max_size << endl;
		cout << "File Path ";
		print_space(max_size, 10);
		cout << "| File Name ";
		print_space(max_size, 11);
		cout << "| Size ";
		print_space(max_size, 6);
		cout << "| Type ";
		print_space(max_size, 6);
		cout << endl;

	}

public:
	bool fInternally(vector<string> str, int sIndex, string name) {
		return fInternally(str, sIndex, name, root);
	}
	bool fInternally(vector<string> str, int sIndex, string name, vector<pointer> &poi) {
		bool found = false;
		for (int i = 0; i < str.size() && i<poi.size(); i++) {
			if (str[i] == poi[i].name && i == str.size() - 1 && i == poi.size() - 1) {
				found = true;
				break;
			}
			if (str[i] == poi[i].name) {
				found = fInternally(str, sIndex + 1, name, poi[i].next);
			}
		}
		return found;
	}
	bool checkFolderIsExist(vector<string> str, int sIndex, string folderName) {
		return checkFolderIsExistKernal(str, sIndex, folderName, root);
		//vector<pointer> p = root;

	}

	void write(const char *path, int index, pointer chain) {
		FILE * pFile1;
		pFile1 = fopen(path, "wb");
		fwrite(chain.buffer, sizeof(char), chain.size, pFile1);
		//fwrite(chain.buffer,  chain.size,  1 ,pFile1);
		fclose(pFile1);
	}
	bool checkFileIsExist(vector<string> str, vector<string> strCopy, int sIndex, string fName) {
		bool lastfound = false; vector<pointer> foundFolder = root; unsigned int max_size = 25;
		if (str.size() != 0) {
			foundFolder = getFolder(str, strCopy, sIndex, max_size, root);
		}
		for (int i = 0; i < foundFolder.size(); i++) {
			//cout << deseFileName << "   " << fileNames[i] << " \n";
			if (fName == foundFolder[i].name) {
				//write(desePathFile, i, foundFolder[i]);
				//cout << "copy Work Success \n";

				return true;
			}

		}
		return false;
	}
	void copyFromRAMOut(vector<string> str, vector<string> strCopy, int sIndex) {
		cout << "Enter The Destination file Path :-  ";
		string desePath = "";
		cin >> desePath;
		//sourcePath = "C:\\Users\\Ahmed\\Desktop\\";
		//cout << sourcePath << endl;
		cout << "Enter The last Name Of File :-  ";


		string  fileName = "", fullName = "C:\\Users\\Ahmed\\Desktop\\";// = "C:\\Users\\Ahmed\\Desktop\\ali.mp4";
		cin >> fileName;
		cout << "Enter New Name : ";
		string nName;
		cin >> nName;
		desePath += nName;
		const char * desePathFile = desePath.c_str();
		const char * deseFileName = fileName.c_str();
		//root.find_file(deseFileName, current_folder, desePathFile);

		//find the current location in RAM
		bool lastfound = false; vector<pointer> foundFolder = root; unsigned int max_size = 25;
		if (str.size() != 0) {
			foundFolder = getFolder(str, strCopy, sIndex, max_size, root);
		}

		//create file to OPen It
		FILE * WriteFile;
		for (int i = 0; i < foundFolder.size(); i++) {
			//cout << deseFileName << "   " << fileNames[i] << " \n";
			if (deseFileName == foundFolder[i].name) {
				write(desePathFile, i, foundFolder[i]);
				cout << "copy Work Success \n";

				return;
			}

		}
		cout << "THis File IS Not Found \n";

	}





	void add_file(pointer p, vector<string> str, int sIndex, char * buf) {
		add_fileKernal(p, str, sIndex, root);
	}
	void add_fileKernal(pointer p, vector<string> str, int sIndex, vector<pointer> &poi) {
		int i;
		for (i = 0; i < poi.size(); i++) {

			if (sIndex<str.size() && str[sIndex] == poi[i].name && poi[i].isFolder) {
				add_fileKernal(p, str, sIndex + 1, poi[i].next);
				return;
			}

			//break;
		}
		//poi.push_back(p);
		if (i > 0)
			i--;
		poi.push_back(p);
	}

	void executeDIR(vector<string> str, vector<string> strCopy, int sIndex) {
		if (root.empty()) {
			cout << "not item to show it \n";
			return;

		}
		unsigned int max_size = getMaxSize();
		if (max_size < 20)
			max_size = 25;
		showTableHeader(max_size);
		//cout << "-----------------------------------------------------------------------------\n";
		printDash(max_size);
		printDash(max_size);
		printDash(max_size); printDash(max_size);
		cout << endl;
		print(str, strCopy, sIndex, max_size, root);
	}
	void printDash(unsigned int max_size) {
		for (int i = 0; i < max_size; i++)
			cout << "-";
		cout << "+";
	}
	void mkDIR(vector<string> str, int sIndex, string folderName) {
		mkDIRKernal(str, sIndex, folderName, root);
	}
	void mkDIRKernal(vector<string> str, int sIndex, string folderName, vector<pointer> &poi) {
		int i; bool f = false;
		for (i = 0; i < poi.size(); i++) {
			if (sIndex<str.size() && str[sIndex] == poi[i].name && poi[i].isFolder) {
				mkDIRKernal(str, sIndex + 1, folderName, poi[i].next);
				return;
			}
			f = true;
		}
		pointer p;
		p.extension = "Folder";
		p.isFolder = true;
		p.name = folderName;
		p.path = "root//";
		/*for (int j = 0; j < strCopy.size(); j++) {
		p.path += strCopy[j] + "//";
		}*/
		p.path += folderName;
		p.size = 0;

		if (i > 0)
			i--;
		poi.push_back(p);
	}
	bool foundFolderKernal(string folderName, vector<string> str, vector<string> strCopy, int sIndex, vector<pointer> &poi, bool &lastFound) {
		int i;
		for (i = 0; i < poi.size(); i++) {


			if (sIndex < str.size() && str[sIndex] == poi[i].name && poi[i].isFolder) {
				lastFound = true;
				return foundFolderKernal(folderName, str, strCopy, sIndex + 1, poi[i].next, lastFound);

			}
			else {
				lastFound = false;
			}/*
			 if (folderName == poi[i].name) {
			 return true;
			 }*/
		}
		if (i == 0 && sIndex <= strCopy.size() - 1) {

			return false;
		}
		if (lastFound  && i<poi.size() && folderName == poi[i].name)
			return true;
		else if (lastFound)
			return true;
		return false;
	}
	bool foundFolderFor(string folderName, vector<string> str, vector<string> strCopy, int sIndex) {
		bool found = false;
		return foundFolderKernalFor(folderName, str, strCopy, sIndex, root, found);
	}
	bool foundFolderKernalFor(string folderName, vector<string> str, vector<string> strCopy, int sIndex, vector<pointer> &poi, bool &lastFound) {
		int i;
		for (i = 0; i < poi.size(); i++) {

			if (sIndex >= strCopy.size() && !poi[i].isFolder) {
				break;
			}
			if (sIndex < str.size() && str[sIndex] == poi[i].name && poi[i].isFolder) {

				if (folderName == poi[i].name) {
					lastFound = true;
					goto label;

				}
				return foundFolderKernalFor(folderName, str, strCopy, sIndex + 1, poi[i].next, lastFound);

			}
			else if (sIndex < str.size() && poi[i].isFolder &&folderName == poi[i].name) {
				lastFound = true;
			}
			else {
				lastFound = false;
			}
			if (folderName == poi[i].name) {
				return true;
			}

		}
	label:
		/*
		if (i == 0 && sIndex <= strCopy.size() - 1) {

		return false;
		}*/
		if (lastFound  && i<poi.size() && folderName == poi[i].name)
			return true;
		else if (lastFound)
			return true;
		for (int i = 0; i < root.size(); i++) {
			if (root[i].name == folderName)
				return true;
		}

		return false;
	}
	bool foundFolder(string folderName, vector<string> str, vector<string> strCopy, int sIndex) {
		bool found = false;
		return foundFolderKernal(folderName, str, strCopy, sIndex, root, found);
	}
	void copyCurrentFolderUser(vector<string> str, vector<string> strCopy, int sIndex, string path) {
		copyCurrentFolder(str, strCopy, sIndex, path, root);
	}
	void copyCurrentFolder(vector<string> str, vector<string> strCopy, int sIndex, string path, vector<pointer> p) {
		string full_name, fNAme = "";
		unsigned int max_size = 0;
		/*
		for (int i = 0; i < sIndex && !str.empty(); i++) {
		fNAme += str[i];
		}*/
		vector<pointer> current = getFolder(str, strCopy, sIndex, max_size, p);
		if (!fNAme.empty()) {
			full_name = path + "\\" + fNAme;
		}
		else {
			full_name = path;
		}
		mkdir(full_name.c_str());
		for (int i = 0; i < current.size(); i++) {
			if (current[i].isFolder) {
				copyCurrentFolder(str, strCopy, sIndex + 1, full_name + "\\" + current[i].name, current[i].next);
				continue;
			}
			FILE * pFile1;
			full_name += "\\" + current[i].name;
			const char * asd = full_name.c_str();
			pFile1 = fopen(asd, "wb");
			fwrite(current[i].buffer, sizeof(char), current[i].size, pFile1);
			fclose(pFile1);
			string c = full_name;
			reverse(c.begin(), c.end());
			size_t found = c.find("\\");
			string type = (c.substr(found + 1, c.size() - 1));
			reverse(type.begin(), type.end());
			full_name = type;
		}
	}
	void copyFileByFile(string path, pointer p) {
		if (p.isFolder) {
			return;
		}
		FILE * pFile1;
		const char * asd = path.c_str();
		pFile1 = fopen(asd, "wb");
		fwrite(p.buffer, sizeof(char), p.size, pFile1);
		fclose(pFile1);



	}
	void copyFolderFromOut() {

	}
	vector<pointer> asd(string fNAme, vector<string> str, int sIndex) {
		return asd_call(fNAme, str, sIndex, root);
	}
	vector<pointer> getRoot() {
		return root;
	}
	vector<pointer> asd_call(string fNAme, vector<string> str, int sIndex, vector<pointer> poi) {
		int i;
		for (i = 0; i < poi.size(); i++) {
			if (sIndex < str.size() && str[sIndex] == poi[i].name && fNAme == poi[i].name) {
				return poi;
			}
			if (sIndex < str.size() && str[sIndex] == poi[i].name &&  poi[i].isFolder) {
				return asd_call(fNAme, str, sIndex + 1, poi[i].next);
			}
		}


		return root;
	}
	bool rename(string lName, string nName, vector<string> str, int sIndex) {
		return renameEnternally(lName, nName, str, sIndex, root);
	}
	bool renameEnternally(string lName, string nName, vector<string> str, int sIndex, vector<pointer> &poi) {
		int i; bool test = false;
		for (i = 0; i < poi.size(); i++) {
			if (sIndex < str.size() && poi[i].isFolder && str[sIndex] == poi[i].name &&sIndex == str.size() - 1 && poi[i].name == lName) {
				poi[i].name = nName;
			}
			if (sIndex < str.size() && poi[i].isFolder && str[sIndex] == poi[i].name) {
				return renameEnternally(lName, nName, str, sIndex + 1, poi[i].next);
			}
			if (poi[i].name == lName && sIndex == str.size()) {
				poi[i].name = nName;
				if (poi[i].isFolder) {
					poi[i].path = "root\\";
					for (int i = 0; i < str.size(); i++) {
						poi[i].path += str[i];
					}
				}
				test = true;
			}
		}
		return test;
	}

	long double del(vector<string> str, int sIndex, string Name) {
		return delKernally(str, sIndex, Name, root);
	}
	long double delKernally(vector<string> str, int sIndex, string Name, vector<pointer> &poi) {
		long double ss = -1;
		for (int i = 0; i < poi.size(); i++) {
			if (sIndex < str.size() && poi[i].isFolder && str[sIndex] == poi[i].name &&sIndex == str.size() - 1 && poi[i].name == Name) {
				ss = poi[i].size;
				delete &poi[i];
				//pointer *temp = &poi[i];
				poi.erase(poi.begin() + i);
				//delete (temp);
				cout << "File Deleted \n";
				return ss;
			}
			else if (sIndex < str.size() && poi[i].isFolder && str[sIndex] == poi[i].name) {
				return delKernally(str, sIndex + 1, Name, poi[i].next);
			}
			else if (poi[i].name == Name && sIndex == str.size()) {
				ss = poi[i].size;
				poi.erase(poi.begin() + i);
				cout << "File Deleted \n";
				return ss;
			}
		}
	}
	long double calcAlocatedSpace() {
		long long size = 0;
		return calcAlocatedSpaceKernally(root, size);
	}
	long long calcAlocatedSpaceKernally(vector<pointer> poi, long long size) {

		for (int i = 0; i < poi.size(); i++) {
			size += poi[i].size;
			if (poi[i].isFolder) {
				size += calcAlocatedSpaceKernally(poi[i].next, 0);
			}
		}
		return size;
	}
	void cut(vector<string> str, int sIndex) {

	}

	void loadFolder(vector<string> &str, vector<string> &strCopy, int sIndex, string path, long long total_size) {
		string c = path;
		//reverse(c.begin(), c.end());
		size_t found = c.find("\\");
		string type = (c.substr(found + 1, c.size() - 1));
		//reverse(type.begin(), type.end());
		cout << type << endl;

		///
		const char * p = path.c_str();
		namespace bf = std::experimental::filesystem;
		size_t size = 0;
		for (bf::recursive_directory_iterator it(path);
			it != bf::recursive_directory_iterator();
			++it)
		{
			if (!bf::is_directory(*it))
				size += bf::file_size(*it);
		}
		cout << (double)size / (1024 * 1024) << " MB\n";
		if (size > total_size - calcAlocatedSpace()) {
			cout << "no enagh space \n";
			return;
		}
		/*bool exist = checkFolerExist(type, root);*/
		bool exist = checkFolderIsExist(strCopy, 0, type);
		if (exist) {
			cout << "This Folder Is Exist \n";
			return;
		}
		string narrow_string(path);
		wstring wide_string = wstring(narrow_string.begin(), narrow_string.end());
		const wchar_t* result = wide_string.c_str();
		WIN32_FIND_DATA fdFile;
		HANDLE hFind = NULL;

		wchar_t sPath[2048];

		//Specify a file mask. *.* = We want everything! 
		wsprintf(sPath, L"%s\\*.*", result);

		if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
		{
			wprintf(L"Path not found: [%s]\n", result);
			return;
		}
		loadFolder(str, strCopy, sIndex, type, root);

		ListDirectoryContents(result, str, strCopy, sIndex);
	}
	bool checkFolerExist(string name, vector<pointer> poi) {
		bool found = false;
		for (int i = 0; i<poi.size(); i++) {
			if (poi[i].name == name) {
				return true;
			}
			if (poi[i].isFolder) {
				return checkFolerExist(name, poi[i].next);
			}
		}
		return found;
	}
	bool ListDirectoryContents(const wchar_t *sDir, vector<string> &str, vector<string> &strCopy, int sIndex)
	{
		WIN32_FIND_DATA fdFile;
		HANDLE hFind = NULL;

		wchar_t sPath[2048];

		//Specify a file mask. *.* = We want everything! 
		wsprintf(sPath, L"%s\\*.*", sDir);

		if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
		{
			wprintf(L"Path not found: [%s]\n", sDir);
			return false;
		}

		do
		{
			//Find first file will always return "."
			//    and ".." as the first two directories. 
			if (wcscmp(fdFile.cFileName, L".") != 0
				&& wcscmp(fdFile.cFileName, L"..") != 0)
			{
				//Build up our file path using the passed in 
				//  [sDir] and the file/foldername we just found: 
				wsprintf(sPath, L"%s\\%s", sDir, fdFile.cFileName);

				//Is the entity a File or Folder? 
				if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
				{
					wprintf(L"Directory: %s\n", sPath);
					std::wstring ws(fdFile.cFileName);
					std::string s(ws.begin(), ws.end());
					////
					//
					//const WCHAR* wc = sPath;
					//_bstr_t b(wc);
					//const char* c = b;
					//string _string = c;
					//
					//const WCHAR* wcc = sDir;
					//_bstr_t bc(wcc);
					//const char* cc = bc;
					//string __string = cc;
					////
					//if (!strCopy.empty() && !str.empty() &&_string.length() < __string.length()) {
					//	strCopy.erase(strCopy.begin() + strCopy.size() - 1);
					//	str.erase(str.begin() + str.size() - 1);
					//}
					loadFolder(str, strCopy, sIndex, s, root);
					ListDirectoryContents(sPath, str, strCopy, sIndex); //Recursion, I love it! 
				}
				else {  // this is a file 
					wprintf(L"File: %s\n", sPath);
					std::wstring ws(fdFile.cFileName);
					std::string s(ws.begin(), ws.end());

					std::wstring wspath(sPath);
					std::string sspath(wspath.begin(), wspath.end());
					loadFile(str, sIndex, root, sspath, s);
				}
			}
		} while (FindNextFile(hFind, &fdFile)); //Find the next file. 

		FindClose(hFind); //Always, Always, clean things up! 

		return true;
	}
	void loadFolder(vector<string> &str, vector<string> &strCopy, int sIndex, string folderName, vector<pointer> &poi) {
		int i; bool f = false;
		for (i = 0; i < poi.size(); i++) {
			if (sIndex<str.size() && str[sIndex] == poi[i].name && poi[i].isFolder) {
				loadFolder(str, strCopy, sIndex + 1, folderName, poi[i].next);
				return;
			}
			f = true;
		}
		pointer p;
		p.extension = "Folder";
		p.isFolder = true;
		p.name = folderName;
		p.path = "root//";
		/*for (int j = 0; j < strCopy.size(); j++) {
		p.path += strCopy[j] + "//";
		}*/
		p.path += folderName;
		p.size = 0;

		if (i > 0)
			i--;
		poi.push_back(p);
		str.push_back(folderName);
		strCopy.push_back(folderName);
		cout << "done \n";
	}
	void loadFile(vector<string> str, int sIndex, vector<pointer> &poi, string path, string name) {
		pointer  p;

		p.path = path;
		//sourcePath = "C:\\Users\\Ahmed\\Desktop\\";
		//cout << sourcePath << endl;
		//cout << path << "  " << name<<endl;


		size_t found = name.find(".");
		string type;

		type = (name.substr(found, name.size() - 1));



		//extension.push_back(type);
		p.extension = type;
		//cout << p.extension << type<<name<<endl;
		p.name = name;
		//path += name;
		//fileNames.push_back(fileName);

		const char * sourcePathFile = path.c_str();

		///input the file name and the destination of file 

		FILE * pFile;
		long long lSize;


		size_t result;

		//vector<vector<char>> files;
		pFile = fopen(sourcePathFile, "rb"); //E:\\cou\\JAVA\\java101\\5.mp4
		if (pFile == NULL) {
			//fputs("File error", stderr);

			//exit(1);
			cout << "Thair Are An Error On the Path You Entered \n";
			return;
		}

		//  file size:
		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		p.size = lSize;
		rewind(pFile);

		//

		//p.size = p.size / (1024);
		// allocate memory to contain the whole file:
		p.buffer = new char[lSize];
		char * buf = p.buffer;
		if (p.buffer == NULL) {
			//fputs("Memory error", stderr);
			//exit(1);
			cout << "Memory error :: Error To reading a buffer \n";
			return;
		}

		// copy the file into the buffer:
		result = fread(p.buffer, 1, lSize, pFile);
		//vector<char> file;
		buf = p.buffer;

		if (result != lSize) {
			//fputs("Reading error", stderr);
			//exit(1);
			cout << "Reading error :: Error To reading a buffer \n";
			return;
		}

		FILE * pFile1;

		//path.append(name);
		//cout << fullName << endl;
		const char *aaaa = path.c_str();
		//cout << path << endl;
		//concatenate th two pointer of char
		p.fileName = name.c_str();
		///cout << "   6596846  " << p.fileName << endl;
		p.pathName = path.c_str();

		int i;
		for (i = 0; i < poi.size(); i++) {

			if (sIndex<str.size() && str[sIndex] == poi[i].name && poi[i].isFolder) {
				add_fileKernal(p, str, sIndex + 1, poi[i].next);
				return;
			}

			//break;
		}
		//poi.push_back(p);
		if (i > 0)
			i--;
		poi.push_back(p);
	}
	bool check(vector<string> str, string name) {
		return checkInternally(str, 0, name, root);
	}
	bool checkInternally(vector<string> str, int sIndex, string name, vector<pointer> p) {
		int i;
		for (i = 0; i < p.size(); i++) {
			if (sIndex == str.size() - 1 && name == p[i].name&& p[i].isFolder)
				return true;
			if (sIndex < str.size() && str[sIndex] == p[i].name && p[i].isFolder && !p[i].next.empty()) {
				return checkInternally(str, sIndex + 1, name, p[i].next);
			}
			else if (sIndex < str.size() && name == p[i].name) {
				return true;
			}
			if (str.empty() && name == p[i].name && p[i].isFolder) {
				return true;
			}
		}

		return false;
	}
	bool find(string name, string str) {
		str = "root\\";
		return findInternally(name, root, str);

	}
	bool findInternally(string name, vector<pointer> p, string &path) {
		string str = ""; bool f = false;
		for (int i = 0; i < p.size(); i++) {
			if (p[i].isFolder && name == p[i].name) {
				path += name + "\\";
				return true;
			}
			if (p[i].isFolder) {
				return  findInternally(name, p[i].next, path + "\\"); ;
			}
			if (name == p[i].name) {
				path += name + "\\";
				return true;
			}
			else {
				path += p[i].name + "\\";

			}
		}
		if (f) {
			return true;
		}
		else {

			return false;
		}
	}
	void cut(vector<string> str, vector<string> strCopy, string name, string destination) {
		cutInternally(str, strCopy, 0, name, destination, root);
		del(strCopy, 0, name);
	}
	void cutInternally(vector<string> str, vector<string> strCopy, int sIndex, string name, string path, vector<pointer> &p) {
		string full_name, fNAme = "";
		unsigned int max_size = 0;
		/*
		for (int i = 0; i < sIndex && !str.empty(); i++) {
		fNAme += str[i];
		}*/
		vector<pointer> current = getFolder(str, strCopy, sIndex, max_size, p);
		if (!fNAme.empty()) {
			full_name = path + "\\" + fNAme;
		}
		else {
			full_name = path;
		}
		mkdir(full_name.c_str());
		bool f = false; int index;
		for (int i = 0; i < current.size(); i++) {
			if (current[i].name != name)
				continue;
			f = true;
			if (current[i].isFolder) {
				copyCurrentFolder(str, strCopy, sIndex + 1, full_name + "\\" + current[i].name, current[i].next);
				continue;
			}
			FILE * pFile1;
			full_name += "\\" + current[i].name;
			const char * asd = full_name.c_str();
			pFile1 = fopen(asd, "wb");
			fwrite(current[i].buffer, sizeof(char), current[i].size, pFile1);
			fclose(pFile1);
			string c = full_name;
			reverse(c.begin(), c.end());
			size_t found = c.find("\\");
			string type = (c.substr(found + 1, c.size() - 1));
			reverse(type.begin(), type.end());
			full_name = type;
			index = i;
		}
		if (!f) {
			cout << "No File Or Diewctory Has This Name \n";
		}
		/*if (f) {
		p.erase(p.begin() + index);
		cout << "File Deleted \n";
		}*/
	}
};
