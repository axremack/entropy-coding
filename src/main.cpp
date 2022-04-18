#include <vector>
#include <queue>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace std;

class Symbol {
  public:
    string name;
    double freq;
    string code;
    bool isLeaf;
    Symbol *left, *right;

    Symbol(): name(""),freq(0),code(""),isLeaf(true), left(NULL), right(NULL){}
    Symbol(string newName, double newFreq, string newCode="", bool newLeaf=true, Symbol* newLeft=NULL, Symbol* newRight=NULL):name(newName), freq(newFreq),code(newCode), isLeaf(newLeaf), left(newLeft), right(newRight){}

    // Ajout de code de feuille en mode récursif
    void AddCode() {  
      if(!isLeaf){
        left->code = code + "0";
        left->AddCode();
        right->code = code + "1";
        right->AddCode();
      }
    }

    // Ajout de code de feuille en mode récursif
    double MoyenneTailleCode() {  
      double totalFeuilles = 0;
      double sommeTailleCode = 0;
      ParcoursCode(totalFeuilles, sommeTailleCode);
      double moyenne = sommeTailleCode / totalFeuilles;
      return moyenne;
    }

    void ParcoursCode(double& totalFeuilles, double& sommeTailleCode) {  
      if(!isLeaf){
        left->ParcoursCode(totalFeuilles, sommeTailleCode);
        right->ParcoursCode(totalFeuilles, sommeTailleCode);
      } else {
        totalFeuilles++;
        sommeTailleCode = sommeTailleCode + code.length();
      }
    }

};

// Permet d'ordonner correctement la priority queue (par fréquence)
class SymbolPtrComp{
  public:
    bool operator() (Symbol* & lhs, Symbol* & rhs) const{
      return lhs->freq > rhs->freq;
    } 
}; 


Symbol* CreateHuffmanCode(vector<Symbol*>& alphabet)
{
  priority_queue<Symbol*, vector<Symbol*>, SymbolPtrComp> pqSymbol (alphabet.begin(), alphabet.end()); // du plus petit au plus grand
  Symbol* root;

  while (pqSymbol.size() > 1) {
    Symbol* last = pqSymbol.top();
    pqSymbol.pop();
    Symbol* penultimate = pqSymbol.top();
    pqSymbol.pop();

    root = new Symbol(last->name + penultimate->name, last->freq + penultimate->freq, "", false, last, penultimate);

    pqSymbol.push(root);
  } 
  
  return root;
}

int nodeNumMax = 0;
void genDOT(Symbol* node, ofstream& myfile) {
  string print;

  // DOT for current node
  print = "n" + to_string(nodeNumMax) + " [label=\"" + node->name + " / " + node->code + "\"]\n";

  int rootNumBackup = nodeNumMax;
  nodeNumMax = nodeNumMax + 1;
  
  if (!node->isLeaf) {
    print += "n" + to_string(rootNumBackup) + "->n" + to_string(nodeNumMax) + "\n";
    genDOT(node->left, myfile);
    nodeNumMax = nodeNumMax + 1;
    print += "n" + to_string(rootNumBackup) + "->n" + to_string(nodeNumMax) + "\n";
    genDOT(node->right, myfile);
  }
  myfile << print << endl;
}

void launchGenDOT(Symbol* root) {
  ofstream myfile;
  myfile.open ("graph.gv");
  myfile << "digraph G{\n";
  genDOT(root, myfile);
  myfile << "}\n";
  myfile.close();
  nodeNumMax = 0;
}


void CreateAlphabet(vector<Symbol*>& alphabet, bool Proba=true)
{
  if(Proba) {
    // Probability of french letters
    alphabet.push_back(new Symbol("a",8.11));
    alphabet.push_back(new Symbol("b",0.81));
    alphabet.push_back(new Symbol("c",3.38));
    alphabet.push_back(new Symbol("d",4.28));
    alphabet.push_back(new Symbol("e",17.69));
    alphabet.push_back(new Symbol("f",1.13));
    alphabet.push_back(new Symbol("g",1.19));
    alphabet.push_back(new Symbol("h",0.74));
    alphabet.push_back(new Symbol("i",7.24));
    alphabet.push_back(new Symbol("j",0.18));
    alphabet.push_back(new Symbol("k",0.02));
    alphabet.push_back(new Symbol("l",5.99));
    alphabet.push_back(new Symbol("m",2.29));
    alphabet.push_back(new Symbol("n",7.68));
    alphabet.push_back(new Symbol("o",5.2));
    alphabet.push_back(new Symbol("p",2.92));
    alphabet.push_back(new Symbol("q",0.83));
    alphabet.push_back(new Symbol("r",6.43));
    alphabet.push_back(new Symbol("s",8.87));
    alphabet.push_back(new Symbol("t",7.44));
    alphabet.push_back(new Symbol("u",5.23));
    alphabet.push_back(new Symbol("v",1.28));
    alphabet.push_back(new Symbol("w",0.06));
    alphabet.push_back(new Symbol("x",0.53));
    alphabet.push_back(new Symbol("y",0.26));
    alphabet.push_back(new Symbol("z",0.12));
  }
  else {
    // Ouverture du fichier et test d'ouverture
    fstream file;
    file.open("text.txt");

    if (!file.is_open()) {
      cerr << "Could not open the file" << endl;
    }
    
    // Lecture caractère par caractère
    char c;
    map<char, int> freq_text;
    int char_total = 0;
    while (file.get(c)) {
      if (freq_text.find(c) != freq_text.end()) { // si la lettre a déja été entrée
        freq_text.at(c)++;
      } else { // si c'est la première occurence de la lettre
        freq_text.insert(pair<char, int>(c, 1));
      }
      char_total++;
    }

    for(map<char,int>::iterator it = freq_text.begin(); it != freq_text.end(); ++it) {
      float freq = it->second / char_total;
      alphabet.push_back(new Symbol(string(1, it->first),freq));
    }

    file.close();
  }
} 


void DeleteMemory(vector<Symbol*>& alphabet, Symbol* root)
{
  // To Do: Code Here
  // Clear the memory	
}


int main()
{
  vector<Symbol*> alphabet;

  // Compute the frequencies of the symbol
  CreateAlphabet(alphabet);

  // Build the Huffman code tree 
  Symbol* root = CreateHuffmanCode(alphabet);
  root->AddCode();
  launchGenDOT(root);
  
  double moyenneTailleCode = root->MoyenneTailleCode();
  cout << "La taille moyenne des codes est de : " << moyenneTailleCode << endl;

  // Same as before but with a file
  vector<Symbol*> alphabet_text;
  CreateAlphabet(alphabet_text, false);
  Symbol* root_text = CreateHuffmanCode(alphabet_text);
  root_text->AddCode();
  launchGenDOT(root_text);

  double moyenneTailleCodeText = root_text->MoyenneTailleCode();
  cout << "La taille moyenne des codes est de : " << moyenneTailleCodeText << endl;

  // Clear the memory
  DeleteMemory(alphabet,root);

  return 0;
}

