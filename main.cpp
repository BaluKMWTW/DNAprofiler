/*
DNA Profiling
Jan Komorowski - jankomorowski.01@gmail.com
Project description:
DNA profiler loads a database that contains strings with people's names and their dna sequences, loads a file with dna string and matches the dna with a person 
from database (if there's any)
How to use:
1.load the database using: load_db (small.txt or large.txt)
2.load the dna using: load_dna (1-20.txt)
3.to process the dna use: process
4. to find matches use: search
5. to display the results of any command type: display
6. to transcript the dna to mRNA type: transcript
*/

#include "ourvector.h"  
#include <sstream>
#include <fstream>
using namespace std;
bool aalpha(string);
void load_db(string);
void display();
void load_dna(string);
void str_to_ov(string&, ourvector<char>&);
void process();
void ov_to_str(string&, ourvector<char>&);
void search();
void transcription(ourvector<char>&);

class Person {       
  private:
  string name;
  ourvector<int> vals;
  public:
  Person() {
    name = "";
    vals = {};

  }

  Person(string name) {
    this->name = name;
    this->vals = {};
  }

  virtual ~Person(){
  }             

  void setname(string name) {
    this->name = name;
  }

  void push_val(int val) {
    this->vals.push_back(val);
  }
  void print_name() {
    cout << this->name;
  }
  void print_vals() {
    for(int i = 0; i < vals.size(); ++i) {
        cout<< vals[i];
        if(i < vals.size() - 1)
            cout<< " ";
    }
  }
  bool cmp_name(string name) {
    if(this->name == name)
        return true;
    return false;
  }
  ourvector<int> give_vals() {
    return vals;
  }
};

ourvector<string> combinations; //AGATC, TATC etc...
ourvector<Person> people; // each person(name, their sequence) is a signle object
ourvector<char> dna;
ourvector<ourvector<char>> sequences; // same as combinations, used for process
ourvector<int> counts;
bool process_flag = false; 
bool load_db_flag = false;
bool dna_flag = false;

int main() {    
    cout << "Welcome to the DNA Profiling Application.";
    bool close = true;
    while(close) {
        cout << endl << "Enter command or # to exit:";// << endl;
        string command;
        cin >> command;
        //#
        if(command == "#") {
            close = false;
        }
        //load_db
        if(command == "load_db") {
            cout << "Loading database..." << endl;
            if(load_db_flag) { //free the memory stored from revious load_db
                combinations.clear();
                sequences.clear();
                people.clear();
            }
            string filename;
            cin >> filename;
            load_db(filename);
            
        }
        //display
        if(command == "display") {
            display();
        }

        //load_dna
        if(command == "load_dna") {
            string filename;
            cin >> filename;
            cout << "Loading DNA..." << endl;
            load_dna(filename);

        }

        //process
        if(command == "process") {
            if(!load_db_flag) {
                cout << "No database loaded." << endl;
            }else if(!dna_flag) {
                cout << "No DNA loaded." << endl;
            }else { 
                cout << "Processing DNA..." << endl;
                process();
                
            }
           
        }

        //search
        if(command == "search") {
            //cout << "Searching database..." << endl;
            search();
        }
        //Creative Component: transcript
        if(command == "transcript") {
            transcription(dna);
            cout << "Transcripting DNA..." << endl;
        }
    }
    
    
    

    return 0;
}

bool aalpha(string x) {
    for(int i = 0; i < x.size(); i++) {
        if(!isalpha(x[i]))
            return false;
    }
    return true;
}
void load_db(string filename) {
    fstream fs(filename);
    
    if(!fs.is_open()) {
        cout << "Error: unable to open '"<< filename << "'" <<endl;
        return;
    }

    string trash; 
    getline(fs, trash, ','); //"name" 
    
    //Get the combinations
    string line1;
    getline(fs, line1);
    stringstream ss(line1);
    while(ss) {
        string tmp;
        if(!getline(ss, tmp, ','))
            break;
        ourvector<char> temp;
        str_to_ov(tmp, temp);
        combinations.push_back(tmp);
        sequences.push_back(temp);
    }

    //Get the personal data and store it

    while(!fs.eof()) {
        //Get the names and create Person object
        string name;
        getline(fs, name, ',');
        if(isalpha(name[0])) {
            Person p(name);
            
            //get the vals and assing them to the Person object
            string line;
            //ourvector<int> x;
            getline(fs, line);
            stringstream s(line);
            string tmp;
            while(getline(s, tmp, ',')) {
                int val = stoi(tmp);
                //cout << val << endl;
                p.push_val(val);
        }
        people.push_back(p);
        } 
    }
    load_db_flag = true;
}
void display() {
    //load_db 
    if(!load_db_flag) {
        cout << "No database loaded." << endl;

    }else {
        cout << "Database loaded:" << endl;
    }
    
    for(auto& e: people) {
        e.print_name();
        cout << " ";
        e.print_vals();
        cout << endl;
    }
    cout << endl;

    //load_dna
    if(!dna_flag) {
        cout << "No DNA loaded." << endl;
    }else {
        cout << "DNA loaded: " << endl;
        for(auto& e: dna) 
            cout << e;
        cout << endl;
    }
    cout << endl; 

    //process
    if(!process_flag)
        cout << "No DNA has been processed." << endl;
    if(process_flag) {
        int i = 0;
        cout << "DNA processed, STR counts: " << endl;
        for(auto& e: counts) {
            cout << combinations[i] << ": " << e << endl;
            i++;
        }
    }

}
void load_dna(string filename) {
    if(dna_flag) {
        dna.clear();
    }
    fstream fs(filename);
    if(!fs.is_open()) {
        cout << "Error: unable to open '"<< filename << "'" <<endl;
        return;
    }else {
        dna_flag = true;
    }
    //load_dna_flag = true;

    string tmp;
    getline(fs, tmp);
    str_to_ov(tmp, dna);

}
//transition from string to ourvector<char>
void str_to_ov(string& str, ourvector<char>& ov) {
    for(int i = 0; i < str.size(); i++) {
        ov.push_back(str[i]);
    }
}

//see how manay uninterrupted occurences for each sequence
void processingCounts(ourvector<char> dna,
ourvector<ourvector<char>> sequences, ourvector<int> &numRepeats) { //gets right val for some bases in large.txt + 20.txt
  for (int i = 0; i < sequences.size(); i++) {
    int max = 0;
    int x = 0;
    int count = 0;
    string sequence;
    ov_to_str(sequence, sequences[i]);
    string dna_str;
    ov_to_str(dna_str, dna);

    while(x < dna_str.size()) {
        if(dna_str.substr(x,sequence.size()) == sequence) {
            count++;
            x += sequence.size();
            if(max < count)
                max = count;
        }else {
            count = 0;
            x++;
        }
    }
    //store the number of uninterrupted occurences
    numRepeats.push_back(max);
  }
}
// ourvector<int> process(ourvector<ourvector<char>> db, ourvector<char> dna) {
//         ourvector<int> counts;
//         if (dbStored && dnaStored) {
//             cout << "Processing DNA...";
//             ourvector<ourvector<char>> strands;
//             ourvector<char> temp;
//             ourvector<int> prevCounts;

//             for (int i = 0; i < numStrands; i++) { //strand index is same as count
//                 counts.push_back(0);
//                 prevCounts.push_back(0);
//             }

//             strands = moveStrands(db);

//             for (int j = 0; j < numStrands; j++) { //algo for checking recurring substrings
//                 for (int i = 0; i < dna.size(); i++) {
//                     if (checkvec(strands[j], dna, i) == 1) {
//                         counts[j]++;
//                         i += strands[j].size() - 1;
//                     } else if (counts[j] > prevCounts[j]) {
//                         prevCounts[j] = counts[j];
//                         counts[j] = 0;
//                     } else
//                         counts[j] = 0;

//                 }
//             }
//             dbProcessed = true;
//             return prevCounts;
//         } else if (!dbStored) { //checker to make sure that db and dna are loaded
//             cout << "No database loaded.";
//         } else if (!dnaStored) {
//             cout << "No DNA loaded.";
//         }
//         return counts;
//     }
void process() {
    if(!process_flag) //is it first time process is called
        process_flag = true;
    if(process_flag) 
        counts.clear(); //if not, empty counts

    processingCounts(dna, sequences, counts);

}

//transition from ourvector<char> to string
void ov_to_str(string& str, ourvector<char>& ov) {
    for(char c: ov) {
        str.push_back(c);
    }
}

//search for DNA matches
void search() {
    //check if other instructions were implemented prior to the search call
    if(!load_db_flag) {
        cout << "No database loaded." << endl;
        return;
    }
    if(dna.size() == 0) {
        cout << " No DNA loaded." << endl;
        return;
    }
    if(!process_flag) {
        cout << "No DNA processed." << endl;
        return;
    }
    cout << "Searching database..." << endl;
    bool found = false;
    for(auto& e: people) {
        // e.print_name();
        // cout<< " " ;
        // e.print_vals();
        // cout << endl;
        int match = 0;
        int i = 0;
        ourvector<int> v;
        v = e.give_vals();
        for(auto& k: v) {
            // cout << "people: "<< k << endl;
            // cout << "counts" << counts[i];
            if(k == counts[i]) {
                match++; 
                // cout << "match =" << match << endl;
                if(match == v.size()){
                    found = true;
                }
            }else {
                match = 0;
                // cout << "match = " << match << endl;
            }
            i++;
        }
        if(found) {
            cout <<  "Found in database! DNA matches: ";
            e.print_name();
            cout << endl;
            return;
        }
    }
    if(!found) {
        cout << "Not found in database. " << endl;
    }
   
}
    

    
    // //search
    // cout << "Searching database..." << endl;
    // int match = 0;
    // bool found = false;
    // //cout << "match = 0";
    // //look for the matches
    // for(auto& e: people) {
    //     ourvector<int> v = e.give_vals();
    //     int i = 0;
    //     //e.print_name();
    //     //cout << endl;
    //     for(auto& k: v) {
    //         //cout << "person: " << k << endl;
    //         //cout << "counts[i]" << counts[i] << endl;
    //         if(k == counts[i]) {
    //             //cout << k << " = " << "counts[" <<i << "]" << endl;
    //             match++;
    //             //cout << "match++" << endl;    
    //         }

    //             //cout << "match = " << match << endl;
    //         i++;
    //     }
    //     //all sequence counts match = person is found
    //      if(match == counts.size()) {
    //         found = true;
    //      }

void transcription(ourvector<char>& dna) {
    for(auto& e: dna) {
        if(e == 'A'){
            e = 'U';
        }else if(e == 'G') {
            e = 'C';
        }else if(e == 'T') {
            e = 'A';
        }else if(e == 'C') {
            e = 'G';
        }
    }
}      

   

