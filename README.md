# 🎯 Neighborhood-Based Collaborative Filtering

A recommender system implementation using Item-Based Collaborative Filtering (IBCF) algorithm.

## 🔗 Competition Link
[CMP2003 Rating Prediction Challenge](https://www.kaggle.com/competitions/cmp2003-rating-prediction)

## 👥 Authors
- Firas Elbayoumi 
- Yaser Z. K. Shoshaa 
- Briksam Kasimoglu 

## 📝 Overview
This project implements a recommender system that predicts user ratings for items using the Item-Based Collaborative Filtering algorithm. The system achieved an RMSE score of 0.89777
 on the [Kaggle leaderboard](https://www.kaggle.com/competitions/cmp2003-rating-prediction/leaderboard).

### ⭐ Key Features
- Item-Based Collaborative Filtering (IBCF) approach
- Cosine similarity for measuring item-to-item similarity
- Top-20 nearest items as neighbors
- Two nested map-based data structures for efficient queries:
  - userMap (User → (Item → Rating))  
  - userMap (reversed) (Item → (User → Rating))  
- Generation and output of rating predictions for unseen item-user pairs

## 📁 Project Structure 

```
cmp2003-rating-prediction/
├── README.md
├── data/
│   ├── train.csv
│   ├── test.csv
│   └── sample_submission.csv
├── src/
│   ├── RecommenderSystem.h
│   ├── RecommenderSystem.cpp
│   └── main.cpp
└── docs/
    ├── assignment.pdf
    └── project_report.pdf
```


## 🚀 Building and Running

1. Compile the source files:
   ```bash
   g++ -std=c++11 src/*.cpp -o recommender
   ```

2. Run the program:
   ```bash
   ./recommender
   ```
   The program will:
   1. Load training and test data from the "data/" directory.  
   2. Display top 10 users (with most ratings) and top 10 rated items.  
   3. Compute predictions for the test set via IBCF.  
   4. Output all predictions to "data/predictions.csv."

## 🗄️ Data Structures

1. userMap (in RecommenderSystem rs):  
   - Key: User ID  
   - Value: A map<Item ID, Rating>

2. userMap (in RecommenderSystem rs_rev):  
   - Key: Item ID  
   - Value: A map<User ID, Rating>

These two maps let the system efficiently look up user ratings per item, and item ratings per user, enabling tasks like finding top users, top items, and calculating item-to-item similarities.

## 🔬 Algorithm Details

1. Item-Based Collaborative Filtering (IBCF):  
   - We compute similarities between pairs of items based on how users rate these items.  
   - The final prediction for a user u and an item i is formed by:  
     $score(u,i) = \bar{r_i} + \frac{\sum_{j \in N} sim(i,j) \times (r_{u,j} - \bar{r_j})}{\sum_{j \in N} |sim(i,j)|}$
     
     where:
     - $score(u,i)$ is the predicted rating for user u on item i
     - $\bar{r_i}$ is the average rating of item i
     - $N$ is the set of neighbor items
     - $sim(i,j)$ is the similarity between items i and j
     - $r_{u,j}$ is user u's rating of item j
     - $\bar{r_j}$ is the average rating of item j

2. Similarity Calculation:  
   - Implemented by "cosine_similarity" in RecommenderSystem.cpp.  
   - Vectors correspond to users' rating deviations from each item's mean rating.  
   - Produces a value in [-1, 1], higher means more similar items.

3. Neighborhood Size:  
   - We pick the top 20 most similar items (highest cosine similarity) to make predictions and minimize RMSE.

4. RMSE Computation:  
   - Root Mean Squared Error (RMSE) is our measurement for accuracy.  
   - Achieved 0.89777
 on the Kaggle leaderboard, reaching top rank.

## 📊 Performance
- RMSE Score: 0.89777 on private test set
- Ranked #1 on [Kaggle leaderboards](https://www.kaggle.com/competitions/cmp2003-rating-prediction/leaderboard)  
- Runtime: ~11.5 minutes  
- Memory Usage: Reasonably efficient due to map-based data structures

## 📝 Note
This project is part of the CMP2003 (Data Structures and Algorithms in C++) course assignment. For detailed methodology and findings, please refer to the project report.