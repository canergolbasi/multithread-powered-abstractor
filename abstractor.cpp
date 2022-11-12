#include <iostream>
#include <string>
#include<fstream>
#include <vector>
#include <pthread.h>
#include <filesystem>
#include <unistd.h>
#include <queue>
#include<set>
#include<iomanip> // setprecison, 4 precision
#include<algorithm>

//running
// g++ abstracgtor.cpp -abstractor.out -lpthread
// ./abstractor.out /home/ubuntu/Masaüstü/src/input_1.txt /home/ubuntu/Masaüstü/src/output_1.txt
using namespace std;

vector<string> letters={"A", "B", "C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};  //DEVAMINI TAMAMLA
int t,a,n;
pthread_mutex_t mutex, mutex2, mutex3, mutex4;  //Mutex

string inputwords;
string inputs[]={};
vector<string> vec;

vector<string> qwords;  // taking some queries inputs to these vectors, for making operations more easily
vector<string> abstractfiles;
queue<string> qabstracts;

set<string> querywords;

struct arg_struct{
    string letter;  // use this struct for sending some information to thread
    string txtname;
    string path;
};

struct result{  // use this struct for store resutls properly
    string file;
    float score;
    string summary;
};

vector<result> globalresults;


string input_file_path;
string output_file_path;


void readvalues(){


// read from txt
ifstream infile;
infile.open(input_file_path);

infile>>t>>a>>n;


int i=0;
while(infile>>inputwords){

    qwords.push_back(inputwords);
    i++;
}

for(int i=0;i<qwords.size();i++){
    querywords.insert(qwords[i]);
}

infile.close();

for(int j=qwords.size()-a;j<qwords.size();j++){
    abstractfiles.push_back(qwords.at(j));

}

    qwords.erase(qwords.end()-a , qwords.end());

}


void *abstract(void *arguments){

    if(qabstracts.empty()){   // if it is empty exit
        pthread_exit(0);
    }

    struct arg_struct *args=(struct arg_struct *)arguments;  // taking some arguments from pthread creat
    string letter1=args->letter;
    string path=args->path;


    pthread_mutex_lock(&mutex);

    while(!qabstracts.empty()){

    string filename1=qabstracts.front();
    qabstracts.pop();

    path+=filename1;  //adding parent path the file name

    string part1="";
    part1=part1+"Thread "+letter1+" is calculating "+filename1+"\n";  // create the string which is write top of output result txt


  ofstream file_out;
  file_out.open(output_file_path,ios_base::app);  // adding string to txt, it only append txt. do not create a new txt
  file_out<<part1;

  file_out.close();

    pthread_mutex_unlock(&mutex);


    //
    ifstream infile;
    infile.open(path);
    path=args->path;

    //create some local variables
    vector<vector<string>> text;
    string inputw1;
    vector<string> sentence;

    int sentencenumber=0;

    while(infile>>inputw1){

        if(inputw1=="."){  // if find . we udnerstand it is end of that sentence.
            sentence.push_back(inputw1);
            text.push_back(sentence);  // adding sentence to text

            sentence.clear();
            continue;
        }
        sentence.push_back(inputw1);

    }

    //SIMPLE TEXT SIMILARITY ALGORITM
    set<string> wordset2;

    for(int i=0;i<text.size();i++){
        for(int j=0;j<text[i].size();j++){

            wordset2.insert(text[i][j]);
        }
    }

    vector<string> v1;  // querwords to vector/
    for(auto it=qwords.begin();it != qwords.end(); ++it){
        string ss1=*it;
        v1.push_back(ss1);
    }

    vector<string> v2; // pass wordset2 elemtns to a v2 vector. For making operations easily.
    for(auto it=wordset2.begin();it != wordset2.end(); ++it){
        string ss1=*it;
        v2.push_back(ss1);
    }

    vector<string> v3;  // intersection v1 - v2 vector

    for(int i=0;i<v1.size();i++){
        for(int j=0;j<v2.size();j++){
            if(v1[i]==v2[j]){
                v3.push_back(v1[i]);
            }
        }
    }

    set<string> uniontwoset;

    for(int i=0;i<v1.size();i++){
        uniontwoset.insert(v1[i]);
    }

    for(int j=0;j<v2.size();j++){
            uniontwoset.insert(v2[j]);
    }

    vector<string> v4;
    for(auto it=uniontwoset.begin();it != uniontwoset.end(); ++it){
        string ss1=*it;
        v4.push_back(ss1);
    }

    // Jaccard, find it by dividing two vector size
    float a=v3.size();
    float b=v4.size();
    float jaccard=a/b;

    string sum_string="";

    for(int i=0;i<text.size();i++){
        for(int j=0;j<text[i].size();j++){
            for(int k=0;k<v3.size();k++){
                if(text[i][j]==v3[k]){   // if the word of text vector<vector<string>> same with v3 vector, take all of that sentence
                    // cumlesini stringe ekle
                    for(int l=0;l<text[i].size();l++){  //text[i].size()
                        sum_string=sum_string+text[i][l];
                        sum_string+=" ";
                    }
                    k=v3.size();
                    j=text[i].size();

                }
            }

        }


    }

    result newresult;
    newresult.summary=sum_string;   // using struct for store results
    newresult.file=filename1;
    newresult.score=jaccard;

    pthread_mutex_lock(&mutex2);
    globalresults.push_back(newresult);   // push results a global results
    pthread_mutex_unlock(&mutex2);


    } // end of while
    pthread_exit(0);
}


bool compareResult(result i1,result i2){  // use this function for sorting the results due to their jaccard score
    return (i1.score>=i2.score);
}

int main(int argc, char** argv) {

    input_file_path=argv[1];
    output_file_path=argv[2];

    //create a new txt
    fstream file;
    file.open(output_file_path,ios::out);
    file.close();

readvalues();

for(int i=0;i<abstractfiles.size();i++){
    qabstracts.push(abstractfiles[i]);   // create a filename queue, threads read from here the filenames
}


input_file_path.pop_back();
    char ss='/';  // use it for compare
    char lastelement=input_file_path.back(); // delete last element, becasue may be  it /

    //editing path
    for(int i=0;i<2;i++){
    while(true){
        if(lastelement==ss){  // if the last element is /
            break;
        }
        input_file_path.pop_back();
        lastelement=input_file_path.back();

    }
    input_file_path.pop_back();
    lastelement=input_file_path.back();
    }

    input_file_path+="/abstracts/";




 pthread_t tidlist[t];

 //create thread and pass argument
 for(int i=0;i<t;i++){

     struct arg_struct* arg1=new arg_struct();
     arg1->letter=letters[i];   //send letters A, B ... to threads

     arg1->path=input_file_path;  // sending path line ../abstracts/

     if(pthread_create(tidlist+i,NULL,&abstract, (void *)arg1)!=0){
         perror("failed to create thread");
         return 1;
     }

    }


 for(int m=0;m<t;m++){
     if(pthread_join(tidlist[m],NULL)!=0){
         return 2;
     }
 }

//sorting results due to their jaccaard score
sort(globalresults.begin(),globalresults.end(),compareResult);

//append result to txt
ofstream file1;
file1.open(output_file_path,ios_base::app);
file1<<"###"<<endl;


for(int i=0;i<n;i++){
    file1<<"Result "<<i+1<<":"<<endl;
    file1<<"File: "<<globalresults[i].file<<endl;
    file1<<"Score: ";
    file1<<fixed<<setprecision(4)<<globalresults[i].score<<endl;
    file1<<"Summary: "<<globalresults[i].summary<<endl;
    file1<<"###"<<endl;

}


file1.close();


  pthread_mutex_destroy(&mutex);
  return 0;
}


