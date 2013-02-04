/****************************************************************
 * File: inverted_index.h
 * Description: Head file for InvertedIndex class.
 *
 * System: Linux
 * Status: Parallel Program, Version 1.0
 * Lauguage: C++
 * Author: Yueying Cui
 * Date: 01/20/2013
******************************************************************/
#ifndef INVIDX_H
#define INVIDX_H

#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <string.h>
#include <dirent.h>
#include <pthread.h>

struct Node{
	int brand_id; // brand_id start from 0
	int frequence;
};

class InvertedIndex{
	private:
		// inverted index dictionary
		std::unordered_map<std::string, std::list<struct Node> > dictionary;
		// brand_table[brand_id, brand_name]
		std::map<int, std::string> brand_table;
		// stopwords[word]
		std::vector<std::string> stopwords;
	public:
		/* Constructor. */
		InvertedIndex();
		int parse_directory(const std::string directory, std::queue<std::string> & files);
		int parse_file(const std::string file);
		int merge(const std::string brand, std::string & word);
		int search(std::string & word);
		/* Destructor. */
		~InvertedIndex(void){};
};

#endif // INVIDX_H
