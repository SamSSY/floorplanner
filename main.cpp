#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>
using namespace std;

class Macro{
    public:
        Macro(){
            name = "";
            width = 0;
            height = 0;    
        }
        ~Macro(){}
        string name;
        int width;
        int height;
};

class Terminal{
    public:
        string name;
        int x;
        int y;
};

class Net{
    public:
        Net(){
            degree = 0;
        }
        ~Net(){}
        int degree; 
        vector< string > nodes;    
};

int* parseBlock(string& line, string& blockName){
    int* arr = new int[2];
    int index1 = line.find_first_of(" ");
    int index2 = line.find_first_not_of(" ", index1 + 1);
    int index3 = line.find_first_of(" ", index2 + 1);
    int index4 = line.find_first_not_of(" ", index3 + 1);
    
    blockName = line.substr(0, index1);
    string width = line.substr(index2, index3 - index2);
    string height = line.substr(index4);
    
    arr[0] = atoi(width.c_str());
    arr[1] = atoi(height.c_str());   
    return arr;
}

int* parseTerminal(string& line, string& terminalName){
    int* arr = new int[2];
    int index1 = line.find_first_of(" ");
    int index2 = line.find_first_not_of(" ", index1 + 1);
    int index3 = line.find_first_of(" ", index2 + 1);
    int index4 = line.find_first_not_of(" ", index3 + 1);
    int index5 = line.find_first_of("\t", index4 + 1);
    int index6 = line.find_first_not_of("\t", index5 + 1);    
    
    terminalName = line.substr(0, index1);
    string x = line.substr(index4, index5 - index4);
    string y = line.substr(index6);
    
    arr[0] = atoi(x.c_str());
    arr[1] = atoi(y.c_str());   
    return arr;
}

bool isTerminal(string& line){
    int index1 = line.find_first_of(" ");
    int index2 = line.find_first_not_of(" ", index1 + 1);
    int index3 = line.find_first_of(" ", index2 + 1);
    string str = line.substr(index2, index3 - index2);
    return ( str.compare("terminal") == 0); 
}

void printAllMacros( vector< Macro >& macros){
    for( int i = 0; i < macros.size(); ++i){
        cout<< i << " " << macros[i].name << " " << macros[i].width << " " << macros[i].height << endl;
    }  
}

void printAllTerminals( vector< Terminal >& terminals){
    for( int i = 0; i < terminals.size(); ++i){
        cout<< i << " " << terminals[i].name << " " << terminals[i].x << " " << terminals[i].y << endl;
    }  
}

void printAllNets( vector< Net >& nets){
    for(int i = 0; i < nets.size(); ++i){
        cout << i << " " << nets[i].degree << endl;
        for(int j = 0; j < nets[i].nodes.size(); ++j){
            cout<< nets[i].nodes[j] << endl;
        }
        cout<< endl;
    }
}

int main(int argc, char *argv[]){
    
    float alpha = stof(argv[1]);
    string inputBlock = argv[2];
    string inputNet = argv[3];
    string outputFileName = argv[4];
    
    ifstream inputBlockFile( inputBlock.c_str() );
    ifstream inputNetFile( inputNet.c_str() );
    ifstream outputFile( outputFileName.c_str() );

    int outline[2];
    int numBlocks = 0;
    int numTerminals = 0;
    
    vector< Macro > macros;
    vector< Terminal > terminals;    
    int blockLineIndex = 0;
    
    while(!inputBlockFile.eof()){
        string line; 
        getline(inputBlockFile, line);
        ++blockLineIndex;
        
        // avoid blank line
        if( line.length() > 1 ){
            
            //cout<< blockLineIndex<< line << endl;       
            // for Outline/ NumBlocks/ NumTerminals
            if (blockLineIndex <= 3){
                string itemName;
                int* number = parseBlock(line, itemName);
                if(itemName.compare("Outline:") == 0){
                    outline[0] = number[0];
                    outline[1] = number[1];
                }
                else if(itemName.compare("NumBlocks:") == 0){
                    numBlocks = number[0];
                }
                else if(itemName.compare("NumTerminals:") == 0){
                    numTerminals = number[0];
                }
            }
            else if (blockLineIndex > 3){
                if( !isTerminal(line) ){
                    string blockName;
                    int* widthHeight = parseBlock(line, blockName);
                    Macro temp;
                    temp.name = blockName;
                    temp.width = widthHeight[0];
                    temp.height = widthHeight[1];
                    macros.push_back(temp);       
                }
                else{
                    string terminalName;
                    int* xy = parseTerminal(line, terminalName);
                    Terminal temp;
                    temp.name = terminalName;
                    temp.x = xy[0];
                    temp.y = xy[1];
                    terminals.push_back(temp);
                }
            }                        
        }
    } 
    inputBlockFile.close();
    
    //
    // print all macros to test 
    // printAllMacros(macros);    
    //
    // print all terminals to test
    // printAllTerminals(terminals);
    
    int numNets = 0;
    vector< Net > nets;
    int netLineIndex = 0;
    
    while(!inputNetFile.eof()){
        string line; 
        getline(inputNetFile, line);
        ++netLineIndex;
        if(line.length() > 1){
            if(netLineIndex == 1){
                int index1 = line.find_first_of(" ");
                int index2 = line.find_first_not_of(" ", index1 + 1);
                numNets = atoi( line.substr(index2).c_str() );
            }
            else{
                int index1 = line.find_first_of(" ");
                if(line.substr(0, index1).compare("NetDegree:") == 0){
                    int index2 = line.find_first_not_of(" ", index1 + 1);
                    int netDegree = atoi( line.substr(index2).c_str() );
                    Net temp;
                    temp.degree = netDegree;
                    nets.push_back(temp);
                }
                else{
                    nets.back().nodes.push_back( line.substr(0, index1) );
                }                
            }
        }        
    }
    
    // print all nets to test 
    // printAllNets(nets);

}
