#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cctype>
#include <algorithm>

using namespace std;

const int MAX_REVIEWS = 100;
const string FILENAME = "tiktok_reviews.txt";

// Function prototypes
int loadReviews(string trends[], int ratings[], string reviewers[]);
void saveReviews(const string trends[], const int ratings[], const string reviewers[], int count);
void viewAllReviews(const string trends[], const int ratings[], const string reviewers[], int count);
void searchReview(const string trends[], const int ratings[], const string reviewers[], int count);
int addReview(string trends[], int ratings[], string reviewers[], int count);
string toLowerCase(const string& str);
void displayMenu();

int main() {
    string trends[MAX_REVIEWS];
    int ratings[MAX_REVIEWS];
    string reviewers[MAX_REVIEWS];
    int reviewCount = 0;
    
    // Load existing reviews from file
    reviewCount = loadReviews(trends, ratings, reviewers);
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear input buffer
        
        switch (choice) {
            case 1:
                viewAllReviews(trends, ratings, reviewers, reviewCount);
                break;
            case 2:
                searchReview(trends, ratings, reviewers, reviewCount);
                break;
            case 3:
                if (reviewCount < MAX_REVIEWS) {
                    reviewCount = addReview(trends, ratings, reviewers, reviewCount);
                } else {
                    cout << "Database is full! Cannot add more reviews.\n";
                }
                break;
            case 4:
                cout << "Thank you for using the TikTok Review Database!\n";
                running = false;
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
        
        cout << endl;
    }
    
    return 0;
}

void displayMenu() {
    cout << "====================================\n";
    cout << "Welcome to the TikTok Review Database\n";
    cout << "====================================\n";
    cout << "1. View All Reviews\n";
    cout << "2. Search for a Trend/Influencer\n";
    cout << "3. Add a New Review\n";
    cout << "4. Exit\n";
    cout << "====================================\n";
}

int loadReviews(string trends[], int ratings[], string reviewers[]) {
    ifstream inFile(FILENAME);
    int count = 0;
    
    if (!inFile) {
        cout << "No existing review file found. Starting with empty database.\n";
        return 0;
    }
    
    string line;
    while (getline(inFile, line) && count < MAX_REVIEWS) {
        // Parse the line: TrendName Rating ReviewerName
        size_t lastSpace = line.rfind(' ');
        if (lastSpace == string::npos) continue;
        
        string reviewer = line.substr(lastSpace + 1);
        string remaining = line.substr(0, lastSpace);
        
        size_t secondLastSpace = remaining.rfind(' ');
        if (secondLastSpace == string::npos) continue;
        
        string ratingStr = remaining.substr(secondLastSpace + 1);
        string trend = remaining.substr(0, secondLastSpace);
        
        try {
            int rating = stoi(ratingStr);
            trends[count] = trend;
            ratings[count] = rating;
            reviewers[count] = reviewer;
            count++;
        } catch (...) {
            // Skip invalid lines
            continue;
        }
    }
    
    inFile.close();
    cout << "Loaded " << count << " reviews from file.\n";
    return count;
}

void saveReviews(const string trends[], const int ratings[], const string reviewers[], int count) {
    ofstream outFile(FILENAME);
    
    if (!outFile) {
        cout << "Error: Could not save reviews to file!\n";
        return;
    }
    
    for (int i = 0; i < count; i++) {
        outFile << trends[i] << " " << ratings[i] << " " << reviewers[i] << "\n";
    }
    
    outFile.close();
}

void viewAllReviews(const string trends[], const int ratings[], const string reviewers[], int count) {
    if (count == 0) {
        cout << "No reviews available.\n";
        return;
    }
    
    cout << "\n====================================\n";
    cout << "         ALL TIKTOK REVIEWS         \n";
    cout << "====================================\n";
    cout << left << setw(25) << "TikTok Trend" 
         << setw(10) << "Rating" 
         << "Reviewer" << "\n";
    cout << "------------------------------------\n";
    
    for (int i = 0; i < count; i++) {
        cout << left << setw(25) << trends[i] 
             << setw(10) << ratings[i] 
             << reviewers[i] << "\n";
    }
    cout << "====================================\n";
}

void searchReview(const string trends[], const int ratings[], const string reviewers[], int count) {
    if (count == 0) {
        cout << "No reviews available to search.\n";
        return;
    }
    
    string searchTerm;
    cout << "Enter trend name: ";
    getline(cin, searchTerm);
    
    string searchLower = toLowerCase(searchTerm);
    bool found = false;
    
    cout << "\n--- Search Results ---\n";
    for (int i = 0; i < count; i++) {
        string trendLower = toLowerCase(trends[i]);
        if (trendLower.find(searchLower) != string::npos) {
            cout << "Trend: " << trends[i] 
                 << ", Rating: " << ratings[i] 
                 << ", Reviewed by: " << reviewers[i] << "\n";
            found = true;
        }
    }
    
    if (!found) {
        cout << "No matching trends found.\n";
    }
}

int addReview(string trends[], int ratings[], string reviewers[], int count) {
    string trend, reviewer;
    int rating;
    
    cout << "Enter trend/influencer name: ";
    getline(cin, trend);
    
    cout << "Enter rating (1-10): ";
    cin >> rating;
    cin.ignore(); // Clear input buffer
    
    // Validate rating
    while (rating < 1 || rating > 10) {
        cout << "Invalid rating! Please enter a value between 1 and 10: ";
        cin >> rating;
        cin.ignore();
    }
    
    cout << "Enter reviewer name: ";
    getline(cin, reviewer);
    
    // Add to arrays
    trends[count] = trend;
    ratings[count] = rating;
    reviewers[count] = reviewer;
    count++;
    
    // Save to file immediately
    saveReviews(trends, ratings, reviewers, count);
    
    cout << "Review added successfully!\n";
    return count;
}

string toLowerCase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}
