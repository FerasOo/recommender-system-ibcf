#ifndef PROJECT_RECOMMENDERSYSTEM_H
#define PROJECT_RECOMMENDERSYSTEM_H
#include <map>
#include <list>
#include <vector>

using namespace std;
class RecommenderSystem {
public:
    map<int,map<int,double> > userMap;
    list<pair<int,int> > testList;

public:
    RecommenderSystem();

    void addUserInfo(int,int,double);

    void addtestInfo(int,int);

    void printUser(int);

    void printAllUsers();

    void printTestList();

    void topUsers();

    void topItems();

    template<class type>
    int findmin_index(type[], int size);

    unsigned long size();

    double IBCF(int user_id, int item_id);

    list<double> make_predictions();

    double rmse(const list<double> &predictions, const list<double> &true_values);

    double cosine_similarity(const vector<double> &vec1, const vector<double> &vec2);

    void sortArray(int (&rate)[10], int (&id)[10]);

    double item_similarity(map<int, double> v1, map<int, double> v2);

    bool hasData() const {
        return !userMap.empty();
    }
};


#endif //PROJECT_RECOMMENDERSYSTEM_H
