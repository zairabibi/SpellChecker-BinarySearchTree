#include <fstream>
#include <algorithm>
#include "SpellChecker.h"
#include "timer.h"

using namespace std;

Tree<string> dictionary;

//method to clean the word and return the cleaned word.
//this removes all the non-alphabet and non-digit characters from the word, with the exception of the apostrophe character
//it converts the word to lowercase then returns it
string cleanWord(string temp) {

	// Remove non-letters from the word, keeping only the apostrophe and numbers
	temp.erase(remove_if(temp.begin(), temp.end(), [](char character)
	{
		return !isalpha(character) && character != '\'' && !isdigit(character);
	}),
		temp.end()
		);

	//convert to lower case
	for (int i = 0; i < temp.size(); i++)
		temp[i] = tolower(temp[i]);

	return temp;
}

//method to write the misspelled words to the dictionary
void saveMisspelledWords()
{
	string word, cleanedWord;
	ofstream misspelledFile("misspelled.txt", ios::out);

	ifstream bookFile("book.txt", ios::in);

	while (bookFile >> word)
	{
		cleanedWord = cleanWord(word);
		if (cleanedWord.length() == 0)
			continue;

		if (!isalpha(cleanedWord[0]))
			continue;

		int result = dictionary.find(cleanedWord);

		if (result < 0)
			misspelledFile << cleanedWord << endl;
	}
}

int main()
{
	long long int correctWords = 0, misspelledWords = 0, skippedWords=0, comparesCorrect=0, comparesMisspelled=0;
	string word, cleanedWord;

	//loads words from the file and adds them to the tree
	ifstream dictionaryFile("dict.txt", ios::in);

	while (dictionaryFile >> word)
	{
		cleanedWord = cleanWord(word);

		if (cleanedWord.length() == 0)
			continue;

		if (!isalpha(cleanedWord[0]))
			continue;

		dictionary.insert(cleanedWord);
	}
	dictionaryFile.close();

	cout << "dictionary Size " << dictionary.getSize() << endl;

	//start the timer
	Timer time;
	time.Start();

	//open the book file and compare the cleaned words with the dictionary
	ifstream bookFile("book.txt", ios::in);

	while (bookFile >> word)
	{
		cleanedWord = cleanWord(word);
		if (cleanedWord.length() == 0)
			continue;

		//count the skipped words
		if (!isalpha(cleanedWord[0]))
		{
			skippedWords++;
			continue;
		}

		//find the word in the dictionary
		int result = dictionary.find(cleanedWord);

		//if the word exists, increment the correct words
		if (result > 0) 
		{
			correctWords++;
			comparesCorrect += result;
		}

		//else increment the misspelled words
		else {
			misspelledWords++;
			comparesMisspelled -= result;
		}
	}

	//stop the timer
	time.Stop();

	//print the numbers
	cout << "done checking and these are the results\n";
	cout << "Finished in time " << time.Time() << endl;
	cout << "There are " << correctWords << " words found in the dictionary.\n";
	cout << "\t" << comparesCorrect << " compares. Average " << comparesCorrect / correctWords << endl;
	cout << "There are " << misspelledWords << " words NOT found in the dictionary.\n";
	cout << "\t" << comparesMisspelled << " compares. Average " << comparesMisspelled / misspelledWords << endl;
	cout << "There are " << skippedWords << " words not checked.\n";

	//save the misspelled words in a file by calling the function
	saveMisspelledWords();
}