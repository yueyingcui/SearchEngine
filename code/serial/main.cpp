/****************************************************************
 * File: main.cpp
 * Description: Main file for Indexing Product Reviews.
 *		This file used Inverted Index data structure.
 *              This file used InvertedIndex class.
 *
 * System: Linux
 * Status: Serial Program, Version 1.0
 * Lauguage: C++
 * Author: Yueying Cui
 * Date: 01/20/2013
******************************************************************/
#include <iostream>
#include "inverted_index.h"

/*
 * Function:	main
 * Description: main function
 *
 * Parameter: 
 * Return:	
*/
int main(){
	// indexing
	std::string directory_name;
	// input directory_name
	/*std::cout << "Please input a directory name: ";
	std::cin >> directory_name;*/
	directory_name = "./download";
	InvertedIndex invidx;
	int r;
	r = invidx.parse_directory(directory_name);	
	std::cout << "Finished." << std::endl;
	if(r < 0){
		std::cout << "Index failed." << std::endl;
		return 0;
	}
	

	// search
	std::string searchword;
	std::string ans;
	while(1){
		std::cout << "Enter a keyword to search: ";
		std::cin >> searchword;

		// debug info
		//std::cout << "searchword = " << searchword << std::endl;

		invidx.search(searchword);

		std::cout << "Do you want to search another word (Y/N): ";
		std::cin >> ans;
		if(ans == "N")
			break;
	}

	return 1;
}
