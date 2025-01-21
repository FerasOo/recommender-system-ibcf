#include <iostream>
#include <fstream>
#include "sstream"
#include "list"
#include "RecommenderSystem.h"

using namespace std;

int main() {

    RecommenderSystem rs;
    RecommenderSystem rs_rev;
    list<double> actualRating;
    list<double> predictedRating;
    const int TEST_SIZE = 0;
    int test_counter = TEST_SIZE;

    
    // open training data
    ifstream train_file("./data/train.csv");
    if (train_file.fail())
        throw runtime_error("Cannot open training file: ./data/train.csv");


    string line;
    getline(train_file, line); // skip the first line

    cout << "Processing training data..." << endl;
    // read file
    while(getline(train_file, line, '\n')){
        stringstream ss(line);
        string substr;

        getline( ss, substr, ',' );
        int id = stoi(substr);
        getline( ss, substr, ',' );
        int item_id = stoi(substr);
        getline( ss, substr, ',' );
        double rating = stod(substr);


        if (test_counter > 0){
            // add user to testList
            rs_rev.addtestInfo(item_id,id);
            actualRating.push_back(rating);
            test_counter--;
        }
        else {
            // add user to userMap
            rs.addUserInfo(id, item_id, rating);
            rs_rev.addUserInfo(item_id,id,rating);
        }
    }

    cout << "Processing test data..." << endl;
    // open test data
    ifstream test_file("./data/test.csv");
    if (test_file.fail())
        throw runtime_error("Cannot open test file: ./data/test.csv");

    string lline;
    getline(test_file, lline); // skip the first line

    // read file
    while(getline(test_file, lline, '\n')){

        stringstream ss(lline);
        string substr;

        getline( ss, substr, ',' );
        int id_useless = stoi(substr);
        getline( ss, substr, ',' );
        int id = stoi(substr);
        getline( ss, substr, ',' );
        int item_id = stoi(substr);


        rs_rev.addtestInfo(item_id,id);

    }

    rs.topUsers();
    cout <<"          " << endl;
    rs_rev.topItems();
    cout << endl;

    cout << "Generating predictions..." << endl;
    // open output file
    ofstream myfile;
    myfile.open("./data/predictions.csv");
    if (!myfile.is_open())
        throw runtime_error("Cannot open output file: ./data/predictions.csv");
    myfile << "ID" << ","<<"Predicted" << endl;

    predictedRating = rs_rev.make_predictions();
    auto it2 = predictedRating.begin();
    int i = 0;
    while(it2 != predictedRating.end()){
        //cout << "predicted rating: " << *it2 << endl;
        myfile << i << "," << *it2 <<endl;
        it2++;i++;
    }
    myfile.close();

    return 0;
}
