#include "RecommenderSystem.h"
#include <iostream>
#include <cmath>
#include <set>



RecommenderSystem::RecommenderSystem() {

}

/**
 * add user info to userMap.
 */
void RecommenderSystem::addUserInfo(int id, int item_id, double rating) {
    auto iterator = userMap.find(id);
    if (iterator == userMap.end()){
        // user not in map
        map<int,double> info;
        info.insert(make_pair(item_id, rating));

        // add new user to map
        userMap.insert(make_pair(id, info));
    }
    else{
        // user already in map
        // update the rating map for user
        iterator->second.insert(make_pair(item_id, rating));
    }
}

/**
 * add user info to testList.
 *
 */


void RecommenderSystem::addtestInfo(int id, int item_id) {
    testList.push_back(make_pair(id, item_id));
}

/**
 *
 * find the index of the min value
 *
 */

template<class type>
int RecommenderSystem::findmin_index(type array[], int size)
{
    type min = array[0];
    int index = 0;
    for(int i = 0; i < size; i++){
        if (array[i] < min){
            min = array[i];
            index = i;

        }
    }
    return index;
}

/**
 *
 * calculate the root mean square error
 *
 */

double RecommenderSystem::rmse( const list<double>  &predictions, const list<double> &true_values) {
    // Make sure that the two input vectors are of the same size
    if (predictions.size() != true_values.size()) {
        throw invalid_argument("Input vectors must have the same size");
    }

    // Calculate the sum of squared errors
    double sum_sq_errors = 0.0;
    auto it1 = predictions.begin();
    auto it2 = true_values.begin();
    while(it1 != predictions.end()){
        sum_sq_errors += pow(*it1 - *it2, 2);
        it1++;it2++;
    }

    // Return the root mean squared error
    return sqrt(sum_sq_errors / predictions.size());
}


/**
 *
 * calculate the cosine similarity between two vector
 *
 */

double RecommenderSystem::cosine_similarity(const vector<double> &vec1, const vector<double> &vec2) {
    // Make sure that the two input vectors are of the same size
    if (vec1.size() != vec2.size()) {
        throw invalid_argument("Input vectors must have the same size");
    }
    double dot_product = 0.0;
    double magnitude1 = 0.0;
    double magnitude2 = 0.0;
    for (size_t i = 0; i < vec1.size(); i++) {
        dot_product += vec1[i] * vec2[i];
        magnitude1 += pow(vec1[i], 2);
        magnitude2 += pow(vec2[i], 2);
    }

    magnitude1 = sqrt(magnitude1);
    magnitude2 = sqrt(magnitude2);

    // Return the cosine similarity
    return dot_product / (magnitude1 * magnitude2);
}


/**
 * calculate the cosine similarly between two items
 * @param item1,item2 a map containing all rated items
 * @returns a value between [-1,1]
 */

double RecommenderSystem::item_similarity(map<int, double> v1, map<int, double> v2) {

    auto it1 = v1.begin();
    auto it2 = v2.begin();
    vector<double> item1_vec;
    vector<double> item2_vec;

    double mean1 = 0;
    double mean2 = 0;
    for(auto const &i:v1){
        mean1 += i.second;
    }
    mean1 /= (double)v1.size();
    for(auto const &i:v2){
        mean2 += i.second;
    }
    mean2 /= (double)v2.size();
    while (it1 != v1.end() && it2 != v2.end()) {
        if (it1->first < it2->first) {
            // user1 rated item, but user2 didn't
            item1_vec.push_back(it1->second-mean1);
            item2_vec.push_back(0);
            it1++;
        } else if (it1->first > it2->first) {
            // user2 rated item, but user 1 didn't
            item1_vec.push_back(0);
            item2_vec.push_back(it2->second-mean2);
            it2++;
        } else {
            // both users rated the same item
            item1_vec.push_back(it1->second-mean1);
            item2_vec.push_back(it2->second-mean2);
            it1++;
            it2++;
        }
    }
    // it2 reached end, now add the remaining values in it1
    while (it1 != v1.end()) {
        item1_vec.push_back(it1->second-mean1);
        item2_vec.push_back(0);
        it1++;
    }
    // it1 reached end, now add the remaining values in it2
    while (it2 != v2.end()) {
        item1_vec.push_back(0);
        item2_vec.push_back(it2->second-mean2);
        it2++;
    }
    return cosine_similarity(item1_vec,item2_vec);
}
/**
 *
 * sort the array descending order
 *
 */
void RecommenderSystem::sortArray(int(& rate)[10], int(& id)[10]) {
    int temp;
    // n was undefined in the original code, should be 10
    const int n = 10;
    
    for(int i = 0; i < n - 1; i++) {
        for(int j = i + 1; j < n; j++) {
            if(rate[i] < rate[j]) {
                // Swap rates
                temp = rate[i];
                rate[i] = rate[j];
                rate[j] = temp;
                
                // Swap corresponding IDs
                temp = id[i];
                id[i] = id[j];
                id[j] = temp;
            }
        }
    }
}


/**
 * predict item_id rating to user_id
 * @returns a float value between (0,5]
 */
double RecommenderSystem::IBCF(int item_id, int user_id) {
    int const num_of_neighbors = 20;
    double array_sim[num_of_neighbors] = {0};
    double array_rating[num_of_neighbors] = {0};
    double array_ave[num_of_neighbors] = {0};
    double sum =0.0;
    double ave ;
    double k = 0; // sum of similarity
    double rating = 0; // sum of similarity * (rating - item average rating)
    map<int, double> item_ratings = userMap.at(item_id);
    for (auto const &i: userMap) {  // loops over all items in the map
        if (i.first != item_id && i.second.count(user_id) > 0){
            // if its different item, and item is rated by user, we calculate similarity
            double sim = item_similarity(item_ratings,i.second);
            sum = 0.0;
            for (auto const &y: i.second) {
                sum += y.second;
            }
            ave = sum / i.second.size();
            int min_index = findmin_index(array_sim, num_of_neighbors);
            if (sim > array_sim[min_index]){ // get the most similar items
                array_sim[min_index] = sim;
                array_rating[min_index] = i.second.at(user_id);
                array_ave[min_index] = ave;
            }
        }
    }
    sum = 0.0;
    for (auto const &y: item_ratings) {
        sum += y.second;
    }
    sum = sum / item_ratings.size();
    for (int n = 0; n < num_of_neighbors; n++) {
        k += array_sim[n];
        rating += array_sim[n] *( array_rating[n] - array_ave[n]);
    }
    return (rating / k) + sum;
}

/**
 * predict the rating for all items in testList
 * @returns a list of floats each containing a value between (0,5]
 */

list<double> RecommenderSystem::make_predictions() {
    int j = 0;
    list<double> predictions;
    for (auto const &i: testList) {
        // iterate through the testList and predict the rating of each (item_id,user_id)
        double prediction = IBCF(i.first, i.second);

        // round the rating
        double rounded_pred = prediction * 2;

        rounded_pred = round(rounded_pred);

        rounded_pred = rounded_pred / 2;

        // add to predictions list
        predictions.push_back(rounded_pred);


        if (++j % 100 == 0){
            // print every 100 users
            cout << j << "/" << testList.size() << endl;
        }
    }
    return predictions;
}


/**
 * print data for specific user
 */
void RecommenderSystem::printUser(int id) {

    auto iterator = userMap.find(id);
    if (iterator == userMap.end()){
        // user not in map
        cout << "can not find user";
    }
    else{
        //user in map
        cout << "id:" << id << endl;
        for (auto const &i: iterator->second) {
            cout << "\t" << i.first << "\t" << i.second << endl;
        }
    }

}

/**
 * print all users' data
 */
void RecommenderSystem::printAllUsers() {
    cout << "printing all users" << endl;
    for (auto const &i: userMap) {
        cout << "id:" << i.first << endl;
        for (auto const &j: i.second) {
            cout << "\t" << j.first << "\t" << j.second << endl;
        }
    }

}

/**
 * print the test list
 *
 */
void RecommenderSystem::printTestList() {
    cout << "printing test list" << endl;
    for (auto const &i: testList) {
        cout << i.first << "\t" << i.second << endl;
    }
}

/**
 * return the number of users
 */
unsigned long RecommenderSystem::size() {
    return userMap.size();
}
/**
 *
 * function to find and print the users that have rated the most items
 *
 */
void RecommenderSystem::topUsers() {
    int array_rating[10] = {0,0,0,0,0,0,0,0,0,0};
    int array_id[10] = {0,0,0,0,0,0,0,0,0,0};
    int j;

    cout << "Top 10 users" << endl;
    for (auto const &i: userMap) {
        int user_size = i.second.size();
        int min_index = findmin_index(array_rating,10);
        if (user_size > array_rating[min_index]){
            array_rating[min_index] = user_size;
            array_id[min_index] = i.first;
        }
    }
    sortArray(array_rating,array_id);
    for(j = 0; j < 10; j++){
        cout << j +1 << ".user id:" << array_id[j] << ' ';
        cout << "number of items rated:" << array_rating[j] << endl;
    }
}
/**
 *
 * function to find and print the most rated items
 *
 */
void RecommenderSystem::topItems() {
    int array_rating[10] = {0,0,0,0,0,0,0,0,0,0};
    int array_id[10] = {0,0,0,0,0,0,0,0,0,0};
    int j;
    cout << "Top 10 rated items" << endl;
    for (auto const &i: userMap) {
        int user_size = i.second.size();
        int min_index = findmin_index(array_rating,10);
        if (user_size > array_rating[min_index]){
            array_rating[min_index] = user_size;
            array_id[min_index] = i.first;
        }
    }
    sortArray(array_rating,array_id);
    for( j = 0; j < 10; j++){
        cout << j +1 << ".item id:" << array_id[j] << ' ';
        cout << "total number of ratings:" << array_rating[j] << endl;
    }
}

