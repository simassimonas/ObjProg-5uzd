#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <limits>
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <deque>
#include <list>
#include <algorithm>
#include <iomanip>
#include <regex>

using std::cout; using std::endl; using std::cin; using std::string; using std::vector; using std::deque; using std::list;

class zodis{
public:
    string z;   //zodis
    int eil;    //eilute
    zodis(string z_, int eil_ = 0): z(z_), eil(eil_) {};
    bool operator< (const zodis& temp) const {
        return (z < temp.z);
    }
};

int main()
{
    std::multiset<zodis> visiZodziai;
    std::set<zodis> uniqueZodziai;
    std::set<int> eilutes;
    std::set<string> url;

    std::ifstream fin ("text.txt");
    string line;
    string temp;
    int eilute=0;
    string separator{" \t\r\n,\".!?^/&$;:"};
    string word;
    while (getline(fin, line)){
        size_t e,s=0;            // s = offset of next word, e = end of next word
        eilute++;
        std::istringstream iss(line);
        while(iss>>temp){
            if (std::regex_match (temp, std::regex("^(https?:\\/\\/)?([\\da-z\\.-]+)\\.([a-z\\.]{2,6})([\\/\\w \\.-]*)*\\/?$") )){
                url.insert(temp);
                //istrina linkus, kad nesimaisytu
                auto eraseFrom = line.find(temp);
                line.erase(eraseFrom, temp.length());
            }
        }
        do{
            s = line.find_first_not_of(separator,s);  // skip leading separators
            if (s==string::npos)                  // stop if no word left
                break;
            e=line.find_first_of(separator, s);   // find next separator
            string word(line.substr(s,e-s));      // construct the word
            std::transform(word.begin(), word.end(), word.begin(), ::tolower); //pavercia visas raides i mazasias
            visiZodziai.insert(zodis(word, eilute));
            uniqueZodziai.insert(zodis(word));
            s=e+1;                                // position after the separator
        }while (e!=string::npos);
    }

    int kiekEil = 0; //kiek eiluciu bus atspausdinta
    std::ofstream fout ("rez.txt");
    for(std::set<zodis>::iterator it = uniqueZodziai.begin(); it != uniqueZodziai.end(); it++) {
        int kiek = 0;
        eilutes.clear();
        auto begin = visiZodziai.lower_bound(*(it));
        auto end = visiZodziai.upper_bound(*(it));
        while(begin != end){
            kiek++;
            eilutes.insert(begin->eil);
            begin++;
        }
        if(kiek >= 2) {
            kiekEil++;
            fout << it->z << " pasikartoja " << kiek << " kartus siose eilutese: ";
            for (auto i: eilutes) fout << i << " ";
            fout << endl;
        }
    }

    std::ifstream ffin ("rez.txt");
    string lline;
    vector<string> e1;
    vector<string> e2;
    for(auto i=0; i<kiekEil/2; i++){
        (getline(ffin, lline));
        e1.push_back(lline);
    }

    for(auto i=kiekEil/2; i<kiekEil; i++){
        (getline(ffin, lline));
        e2.push_back(lline);
    }

    std::ofstream ffout ("rezGalutinis.txt");
    for(auto i=0; i<e1.size(); i++){
        ffout << std::left << std::setw(60) << e1[i] << std::setw(60) << std::left << e2[i] << endl;
    }

    for(auto i: url) ffout << i << endl;

    return 0;
}
