/****************************************************************
 * File: inverted_index.cpp
 * Description: Cpp file for InvertedIndex class.
 *
 * System: Linux
 * Status: Parallel Program, Version 1.0
 * Lauguage: C++
 * Author: Yueying Cui
 * Date: 01/20/2013
******************************************************************/
#include "inverted_index.h"

/*
 * Function:	parse_direcory
 * Description: Read all files' name in the directory.
 *
 * Parameter:	
 *		* directory: directory name saving .txt files
 * Return:	
 *		* -1: failed
 *		* 0: succeed
*/
int InvertedIndex::parse_directory(const std::string directory, std::queue<std::string> & files){
	DIR * dir = NULL; 
	struct dirent *entry = NULL;
	dir = opendir(directory.c_str());
	// check valid
	if(!dir){
		std::cout << "Open directory: " << directory << " failed." << std::endl;
		return -1;
	}
	else{
		entry = readdir(dir);
		// check valid
		if(!entry){
			std::cout << "Open entry: " << entry -> d_name << " failed." << std::endl;
			return -1;
		}
		else{
			while((entry = readdir(dir)) != NULL){
				// debug info
				//std::cout << "Scaning http://s3.amazonaws.com/shoefitr-recruit/review-indexer/" << entry -> d_name << std::endl;

				// filter . ..
				if(strcmp(entry -> d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0)
					continue;

				std::string file = directory + "/" + entry -> d_name;
				files.push(file);
			}
		}
	}
	return 0;
}

/*
 * Function:	parse_file
 * Description: Read all strings in the file.
 *		Save the distinct brand (i.e. 1st line) into brand_table map.
 *		Parse the distinct word per review (i.e. other lines).
 *
 * Parameter:	
 *		* file: file name
 * Return:	
 *		* -1: failed
 *		* 0: succeed
*/
int InvertedIndex::parse_file(const std::string file){
	std::string line;
	std::string word;
	std::string brand;
	std::vector<std::string> words_in_review;
	std::ifstream fin(file.c_str());
	// check valid	
	if(!fin){
		std::cout << "Open file: " << file << " failed." << std::endl;
		return -1;
	}
	// STEP 1: read 1st line (brand + model name)
	getline(fin, brand);

	// debug info
	//std::cout << "first line = " << brand << std::endl;
	std::cout << "Indexing http://s3.amazonaws.com/shoefitr-recruit/review-indexer/" << file << std::endl;

	// STEP 2: read words per line(review)	
	while(getline(fin, line)){	
		// clear vector
		words_in_review.clear();
		std::istringstream stream(line);
		while(stream >> word){
			std::string::iterator string_iter;
			string_iter = word.begin();
			while(string_iter != word.end()){
				// capital case -> lowercase
				if(*(string_iter) >= 'A' && *(string_iter) <= 'Z')
					*(string_iter) = *(string_iter) - 'A' + 'a';
				// remove punctuation
				if((*(string_iter) < 'A' || *(string_iter) > 'Z') && *(string_iter) < ('a' || *(string_iter) > 'z')){
					//string_iter = word.erase(string_iter);
					word.erase(string_iter);
					string_iter--;
				}
				string_iter++;
			}

			// debug info
			//std::cout << word << std::endl;
	
			// filter stopwords
			if(find(stopwords.begin(), stopwords.end(), word) != stopwords.end()){
				// debug info
				//std::cout << "  this is a stopword." << std::endl;
	
				continue;
			}

			// skip the same word in the current review
			if(find(words_in_review.begin(), words_in_review.end(), word) != words_in_review.end())
				continue;
			// insert word into vector			
			words_in_review.push_back(word);

			int r;
			r = this -> merge(brand, word);
		}	
	}
	return 0;
}

/*
 * Function:	InvertedIndex
 * Description: Parse stopwords into stopwords vector.
 *
 * Parameter:
 * Return:	
 *		* -1: failed
 *		* 0: succeed
*/
InvertedIndex::InvertedIndex(void){
	std::string word;
	std::ifstream fin("./doc/stopWords.txt");
	// check valid
	if(!fin){
		std::cout << "Open file: " << "./doc/stopWords.txt" << " failed." << std::endl;
		return;
	}
	while (fin >> word){
		std::string::iterator string_iter;
		string_iter = word.begin();
		while(string_iter != word.end()){
			// capital case -> lowercase
			if(*(string_iter) >= 'A' && *(string_iter) <= 'Z')
				*(string_iter) = *(string_iter) - 'A' + 'a';
			// remove punctuation
			if((*(string_iter) < 'A' || *(string_iter) > 'Z') && *(string_iter) < ('a' || *(string_iter) > 'z'))
				string_iter = word.erase(string_iter);
			string_iter++;
		}

		// debug info
		// std::cout << word << std::endl;

		stopwords.push_back(word);
	}	
	return;
}

/*
 * Function:	merge
 * Description: Construct Inverted Index unordered_map.
 *
 * Parameter:
 *		* brand: brand name 
 *		* word: word
 * Return:	
 *		* 0: succeed, case 1
 *		* 1: succeed, case 2
 *		* 2: succeed, case 3
*/
int InvertedIndex::merge(const std::string brand, std::string & word){
	std::unordered_map<std::string, std::list<struct Node> >::iterator unordered_map_iter;
	unordered_map_iter = dictionary.find(word);

	// debug info
	/*if(brand_table.empty() == 0){
		std::cout << "map is empty." << std::endl;
		std::cout << "brand_table.size() = " << brand_table.size() << std::endl;
		return -1;
	}*/

	// insert brand name into map
	int brand_id_current = -1;
	std::map<int, std::string>::iterator map_iter;
	map_iter = brand_table.begin();
	int flag = 0;
	while(map_iter != brand_table.end()){
		if((map_iter -> second).compare(brand) == 0){
			flag = 1;
			break;
		}
		map_iter++;
	}
	// FOUND brand
	if(flag == 1)		
		brand_id_current = map_iter -> first;	
	// NOT FOUND brand
	else{
		brand_id_current = brand_table.size();
		brand_table.insert(std::pair<int, std::string>(brand_id_current, brand));	
	}
	
	// NOT FOUND word: insert new word into unordered_map
	if(unordered_map_iter == dictionary.end()){
		// new struct Node
		struct Node new_node;
		new_node.brand_id = brand_id_current;
		new_node.frequence = 1;

		// debug info
		/*std::cout << "  case 0: " << std::endl;
		std::cout << "  brand_id = " << new_node.brand_id << std::endl;
		std::cout << "  frequence = " << new_node.frequence << std::endl;*/

		// insert new struct Node into  list
		std::list<struct Node> node_list;
		node_list.insert(node_list.end(), new_node);
		// insert node_list into unordered_map
		dictionary[word] = node_list;
		return 0;
	}
	// FOUND word
	else{
		std::list<struct Node>::iterator list_iter;
		list_iter = (unordered_map_iter -> second).begin();
		while(list_iter != (unordered_map_iter -> second).end()){
			// FOUND brand: increase frequence
			if((brand_table.find(list_iter -> brand_id) -> second).compare(brand) == 0){
				list_iter -> frequence = list_iter -> frequence + 1;

				// debug info
				/*std::cout << "  case 1: " << std::endl;
				std::cout << "  brand_id = " << list_iter -> brand_id << std::endl;
				std::cout << "  frequence = " << list_iter -> frequence << std::endl;*/
				return 1;
			}
			list_iter++;
		}
		// NOT FOUND brand: insert new Node into list
		// new struct Node
		struct Node new_node;
		new_node.brand_id = brand_id_current;
		new_node.frequence = 1;
		
		// debug info
		/*std::cout << "  case 2: " << std::endl;
		std::cout << "  brand_id = " << new_node.brand_id << std::endl;
		std::cout << "  frequence = " << new_node.frequence << std::endl;*/
			
		// insert new struct Node into list
		list_iter--;
		(unordered_map_iter -> second).insert(list_iter, new_node);
		return 2;
	}
}

/*
 * Function:	search
 * Description: Search keyword using inverted index.
 *
 * Parameter: 
 *		* word: word
 * Return:	
 *		* 0: failed
 *		* 1: succeed
*/
int InvertedIndex::search(std::string & word){	
	std::string::iterator string_iter;
	string_iter = word.begin();
	while(string_iter != word.end()){
		// capital case -> lowercase
		if(*(string_iter) >= 'A' && *(string_iter) <= 'Z'){
			*(string_iter) = *(string_iter) - 'A' + 'a';
		}
		string_iter++;
	}

	// search
	std::unordered_map<std::string, std::list<struct Node>>::iterator unordered_map_iter;
	unordered_map_iter = dictionary.find(word);
	// NOT FOUND word
	if(unordered_map_iter == dictionary.end()){
		std::cout << "Sorry, " << word << " is not in the dictionary." << std::endl;
		return 0;
	}
	// FOUND word
	else{
		// debug info
		/*std::cout << brand_table.find(0) -> second << std::endl;
		std::cout << brand_table.find(1) -> second << std::endl;*/

		std::list<struct Node>::iterator list_iter;
		std::list<struct Node> node_list = unordered_map_iter -> second;
		list_iter = node_list.begin();
		while(list_iter != node_list.end()){	
			std::cout << list_iter -> frequence << " review(s) from " <<  brand_table.find(list_iter -> brand_id) -> second << std::endl;
			list_iter++;
		}
		return 1;
		
	}
}
