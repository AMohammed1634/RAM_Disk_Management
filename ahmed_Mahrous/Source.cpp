#include <iostream>
#include <fstream>
#include <filesystem>
#include"LinkedList.h"
#include<string>
#include <limits>
#include <stdexcept>
vector<string> strCopy;
using namespace std;
long double curent_size;
dataStructure root;
long double total_size;
vector<string> str;




void copy_file() {
	//input the file name and the destination of file 
	pointer  p;
	cout << "Enter The source file Path :-  ";
	string sourcePath = "";
	cin >> sourcePath;

	p.path = sourcePath;
	//sourcePath = "C:\\Users\\Ahmed\\Desktop\\";
	//cout << sourcePath << endl;
	cout << "Enter The Name Of File :-  ";

	string  fileName = "", fullName = "C:\\Users\\Ahmed\\Desktop\\";// = "C:\\Users\\Ahmed\\Desktop\\ali.mp4";
	cin >> fileName;
	size_t found = fileName.find(".");
	string type;

	type = (fileName.substr(found, fileName.size() - 1));


	//extension.push_back(type);
	p.extension = type;
	p.name = fileName;
	sourcePath += fileName;
	//fileNames.push_back(fileName);

	const char * sourcePathFile = sourcePath.c_str();

	///input the file name and the destination of file 

	FILE * pFile;
	long long lSize;


	size_t result;

	//vector<vector<char>> files;
	pFile = fopen(sourcePathFile, "rb"); //E:\\cou\\JAVA\\java101\\5.mp4
	if (pFile == NULL) {
		cout << "Thair Are An Error On the Path You Entered \n";
		return;
	}

	//  file size:
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	p.size = lSize;
	rewind(pFile);
	if (root.calcAlocatedSpace() + p.size > total_size) {
		cout << "Not enough space in the partiona \n";
		curent_size -= p.size;
		return;
	}
	// allocate memory to contain the whole file:
	p.buffer = new char[lSize];
	char * buf = p.buffer;
	if (p.buffer == NULL) {
		cout << "Memory error :: Error To reading a buffer \n";
		return;
	}
	// copy the file into the buffer:
	result = fread(p.buffer, 1, lSize, pFile);
	buf = p.buffer;

	if (result != lSize) {
		cout << "Reading error :: Error To reading a buffer \n";
		return;
	}

	FILE * pFile1;

	fullName.append(fileName);
	const char *path = fullName.c_str();
	//concatenate th two pointer of char
	p.fileName = fileName.c_str();
	cout << p.size << "  " << p.fileName << endl;
	p.pathName = fullName.c_str();
	bool f = root.checkFileIsExist(str, strCopy, 0, fileName);
	if (f) {
		cout << "this Folder Is Exist \n ";
		return;
	}
	root.add_file(p, strCopy, 0, buf);
	//write(path, chain.size() - 1);
	/*
	pFile1 = fopen(path, "wb");
	fwrite(p.buffer, sizeof(char), lSize, pFile1); //E:\cou\JAVA\java101\
	fclose(pFile1);
	*/
	//p.name = "dwe";
	fclose(pFile);
	///free(p.buffer);
}

int main()
{

	int i = 0; string order;
	string help = "del  : to delete Item \ndir  : to show the item \ncopy : to copy File From OUT \nfind : to copy file from RAM to OUT \n";
	help += "rm   : to rename item \n";
	help += "aloc : to show the alocated space \n";
	help += "val  : to show the free space \n";
	help += "mkdir: to make a directory \n";
	help += "cd   : to change directore \n";
	help += "cd.. : to change directory back \n";
	help += "loadf: To Load A Folder To RAM \n";
	help += "copyz: To Copy  Folder From RAM To DISK \n";
	help += "cut  : To Cut  Folder Or File From RAM To DISK \n";
	//cout << help;

	cout << "---------------------------------------------------------------------\n";
	cout << "Enter The TOtale siize of partion in Mega Byte  :- ";

	cin >> total_size;
	while (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cout << "Bad entry.  \nEnter The TOtale siize of partion in Mega Byte  :- ";
		std::cin >> total_size;
	}
	total_size = total_size * 1024 * 1024;
	cout << "E:\\cou\\JAVA\\java101\\\n";
	//lable:
	for (;; i++) {
	ll:
		cout << "root/";

		for (int i = 0; i < strCopy.size(); i++) {
			cout << strCopy[i] << "/";
		}

		cout << " _> ";//E:\cou\JAVA\java101\

		cin >> order;
		if (order == "ttt") {
			for (int i = 0; i < str.size(); i++)
				cout << str[i] << endl;
			cout << endl;
			for (int i = 0; i < strCopy.size(); i++)
				cout << strCopy[i] << endl;
		}
		else if (order == "cut") {
			string path;
			cin >> path;
			string name = "rootRAMDisk";
			cin >> name;
			root.cut(str, strCopy, name, path);
		}
		else if (order == "fin") {
			string s, path = ""; cin >> s;
			bool f = root.find(s, path);
			if (f) {
				cout << path << endl;
			}
			else {
				cout << "File Does Not Exists \n";

			}
		}
		else if (order == "loadf") {
			string s;
			cin >> s;
			root.loadFolder(strCopy, str, 0, s, total_size); // D:\\d1

		}
		else if (order == "del") {
			string name; cin >> name;
			long double ss = root.del(strCopy, 0, name);
			if (ss = -1)
				cout << "No File Or Directory Has This Name\n";
			cout << "JOB DONE \n";
			curent_size -= ss;
		}
		else if (order == "rm") {
			string lastName, newName;
			cin >> lastName >> newName;;
			bool test = root.rename(lastName, newName, strCopy, 0);
			if (!test)
				cout << "No File Or Directory Has This Name\n";
			cout << "JOB DONE \n";
		}
		else if (order == "aloc") {
			//cout << "the alocated space is = " << (double)(curent_size / (1024 * 1024)) <<" MB"<< endl;
			cout << "the alocated space is = " << (double)(root.calcAlocatedSpace() / (1024 * 1024)) << " MB" << endl;
		}
		else if (order == "val") {
			cout << "the free space is = " << (double)(total_size - root.calcAlocatedSpace()) / (1024 * 1024) << " MB" << endl;
		}
		else if (order == "copyz") {
			string path;
			cin >> path;
			string name = "rootRAMDisk";
			cin >> name;
			root.copyCurrentFolderUser(strCopy, str, 0, path + "\\" + name);
		}
		else if (order == "find") {
			root.copyFromRAMOut(str, strCopy, 0);
		}
		else if (order == "cd..") {
			if (!strCopy.empty()) {
				strCopy.erase(strCopy.begin() + strCopy.size() - 1);
			}
			//str.erase(str.begin() + str.size() - 1);
			if (!str.empty()) {
				///str.erase(str.begin() + str.size() - 1);
			}
			for (int i = 0; i < strCopy.size(); i++) {
				cout << strCopy[i] << "  ";
			}
			cout << endl;
		}
		else if (order == "cd") {
			string folder;
			cin >> folder;
			//vector<string> asd; asd.push_back(folder);
			///str.push_back(folder);
			strCopy.push_back(folder);
			//strCopy.push_back(folder);
			///bool result = root.foundFolderFor(folder, str,strCopy, 0) ;  //checkFolderIsExist
			bool result = root.check(strCopy, folder);
			//bool result = root.checkFolderIsExist(str, 0, folder);
			if (result) {
				//str.push_back(folder);
			}
			else {
				cout << "FOLDER DOES NOT EXIST \n";
				if (!strCopy.empty()) {
					//str.erase(str.begin() + str.size() - 1);
					strCopy.erase(strCopy.begin() + strCopy.size() - 1);
				}
				if (!str.empty()) {
					//str.erase(str.begin() + str.size() - 1);
					///str.erase(str.begin() + str.size() - 1);
				}

				//strCopy.erase(str.begin() + str.size() - 1);
			}
		}
		else if (order == "mkdir") {
			string asd;
			cin >> asd;
			//check the folder is exist 
			bool f = root.checkFolderIsExist(str, 0, asd);
			if (f) {
				cout << "this Folder Is Exixt change the Name \n";
				continue;
			}
			root.mkDIR(strCopy, 0, asd);
			str.push_back(asd);
			//strCopy.push_back(asd);
		}
		else if (order == "copy") {
			// check the file is exist in the location 
			//bool found = root.checkFileIsExist(str, 0);
			// end check 
			copy_file();

			cout << "Job Is Done \n";
		}
		else if (order == "exit") {
			return 0;
		}
		else if (order == "dir") {
			//print_dir();
			/*
			string c = current_folder+"\\";
			reverse(c.begin(), c.end());
			size_t found = c.find("\\");
			string type = (c.substr(found+1, c.size() - 1));
			reverse(type.begin(), type.end());*/
			root.executeDIR(strCopy, str, 0);

		}
		else if (order == "help") {
			cout << help;


		}
		else {
			cout << "\" " << order << " \" is not recognized as an internal or external command,\n"
				<< "operable program or batch file.\n";
		}
	}
	system("puse");
	return 0;
}