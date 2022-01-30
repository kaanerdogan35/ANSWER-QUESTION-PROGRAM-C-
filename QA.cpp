#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<list>
#include<iterator>
#include <algorithm>
#include<ctime>
using namespace std;

int BMH(string text, string pattern)
{
	int p = pattern.size();
	int t = text.size();
	if (p > t)
		return -1;

	int table[128];

	for (int i = 0; i < 128; i++)
		table[i] = p;

	for (int j = 0; j < p - 1; j++)
	{
		table[int(pattern[j])] = p - j - 1;
	}


	int k = p - 1;

	while (k < t)
	{
		int j = p - 1;
		int i = k;

		while ((j >= 0) && (text[i] == pattern[j]))
		{
			j--;
			i--;
		}
		if (j == -1)
			return i + 1;

		k += table[int(text[k])];
	}

	return -1;


}
bool checkBMH(int a) {
	if (a < 0) {
		return false;
	}
	else return true;
}
const list<string> createlist(const string& sentence, const char& divedechar) {
	string word{ "" };
	string questionsword{ "what where who how many how much " };
	list<string> listcreater;

	for (auto karakter : sentence) {
		if (karakter != divedechar) {
			word += karakter;
		}
		else if (karakter == divedechar && word != "") {
			for_each(word.begin(), word.end(), [](char& c) {
				c = ::tolower(c);
				});

			if (word != "") {
				listcreater.push_back(word);
				word = "";
			}
		}

	}
	if (word != "") {
		int index = BMH(word, "?");
		if (checkBMH(index)) {
			word.erase(index);
		}

		if (word != "") {
			listcreater.push_back(word);
			word = "";
		}
	}
	return listcreater;
}

void readtext(string filename, list<string>* stringvector) {
	string questionsword{ "what where who how many how much the he his of from on a an does do as at to" };
	fstream file;
	file.open(filename);
	string temp;
	string tempa;
	if (file.is_open()) {
		file >> temp;
		int i = 0;
		do {
			for_each(temp.begin(), temp.end(), [](char& c) {
				c = ::tolower(c); });
			if (checkBMH(BMH(temp, "."))) {
				temp.erase(temp.end());
				tempa.append(temp);
				stringvector->push_back(tempa);
				tempa.clear();

			}
			else {
				int index;
				while ((index = temp.find_first_of("!?;-*+<()/=")) != string::npos)
				{
					int it = index + 1;
					temp.replace(index, it, "");

				}
				if (!checkBMH(BMH(questionsword, temp))) {
					temp.push_back(' ');
					tempa.append(temp);
				}
			}

		} while (file >> temp);
		file.close();
	}
}
const bool checkit(string sentence, list<string> questionss, string* result) {
	int iterator = 0, index = 0;
	string a;
	for (auto it1 = questionss.begin(); it1 != questionss.end(); ++it1) {
		int checkindex = BMH(sentence, *it1);
		if (checkBMH(checkindex)) {
			a = "" + *it1;
			result->append(a);
			index++;
		}

		iterator++;

	}
	if (((double)index / (double)(iterator) > 0.80)) {
		return true;
	}
	else { return false; }

}
const string checker(list<string > text, list<string> questionsss) {
	string lastresult;
	for (auto it = text.begin(); it != text.end(); it++) {
		string result;
		if (checkit(*it, questionsss, &result)) {
			return *it;
		}

	}
	return "not answer";

}

void readquestion(string filename, list<list<string>>* questionsd) {
	fstream file;
	file.open(filename);
	if (file.is_open()) {

		string cc;
		do {
			cc.clear();
			getline(file, cc);
			if (!cc.empty()) {
				questionsd->push_back(list<string>(createlist(cc, ' ')));
			}
		} while (!cc.empty());
	}
	file.close();
}

void skipquestion(list<string> text, list<list<string>> questions) {
	string questionsword{ "what where who how many how much the he his of from on a an does do as " };
	list<string>  a;
	for (auto qit = questions.begin(); qit != questions.end(); ++qit) {
		for (auto it = qit->begin(); it != qit->end(); ++it) {
			cout << *it << " ";
			if (checkBMH(BMH(questionsword, *it))) {
				it->clear();
			}

		}
		string result = checker(text, *qit);
		cout << "?" << endl;
		int ind;

		list<string> ress = createlist(result, ' ');
		for (auto it = qit->begin(); it != qit->end(); ++it) {

			int index = BMH(result, *it);
			//cout << *it<<"1"<<endl;
			if (checkBMH(index)) {
				result.shrink_to_fit();
				result.replace(index, it->length(), "");
			}
		}
		while ((ind = result.find_first_of(".,")) != string::npos)
		{
			result.replace(ind, ind + 1, "");

		}
		for (auto a : ress) {
			while ((ind = a.find_first_of(".,")) != string::npos)
			{
				a.replace(ind, ind + 1, "");
			}
			if (checkBMH(BMH(result, a))) {
				cout << a;
			}
		}

		cout << endl;

	}

}


int main()
{
	clock_t baslangic = clock(), bitis;
	list<list<string>> questions;
	readquestion("questions.txt", &questions);
	list<string> text;
	readtext("the_truman_show_script.txt", &text);
	skipquestion(text, questions);
	bitis = clock();
	cout << (float)(bitis - baslangic) / CLOCKS_PER_SEC;

}