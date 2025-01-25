#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <filesystem>  
using namespace std;

const vector<string> EXCLUDED_WORDS = {"a", "and", "an", "of", "in", "the"};

struct Textbook {
    string filename;
    map<string, double> wordFrequency;
    double totalWords = 0;
};

// Function to read a textbook and compute word frequencies
void processTextbook(Textbook &textbook) {
    ifstream file(textbook.filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << textbook.filename << endl;
        return;
    }

    string word;
    map<string, int> rawFrequency;
    textbook.totalWords = 0;

    // Parse and process each word in the file
    while (file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::toupper);

        // Remove non-alphanumeric characters from the word
        word.erase(remove_if(word.begin(), word.end(), [](char c) {
            return !isalnum(c);
        }), word.end());

        // Skip words listed in EXCLUDED_WORDS
        if (find(EXCLUDED_WORDS.begin(), EXCLUDED_WORDS.end(), word) == EXCLUDED_WORDS.end() && !word.empty()) {
            rawFrequency[word]++;
            textbook.totalWords++;
        }
    }

    // Calculate the frequency of each word
    for (const auto &entry : rawFrequency) {
        textbook.wordFrequency[entry.first] = static_cast<double>(entry.second) / textbook.totalWords;
    }
}

// Function to compute the similarity score between two textbooks
double calculateSimilarity(const Textbook &t1, const Textbook &t2) {
    double similarity = 0.0;
    for (const auto &entry : t1.wordFrequency) {
        auto it = t2.wordFrequency.find(entry.first);
        if (it != t2.wordFrequency.end()) {
            similarity += entry.second + it->second;
        }
    }
    return similarity;
}

// Declaration of the similarity matrix
vector<vector<double>> similarityMatrix;

// Function to construct the similarity matrix for all textbooks
void buildSimilarityMatrix(const vector<Textbook>& textbooks) {
    similarityMatrix.resize(textbooks.size(), vector<double>(textbooks.size(), 0.0));

    for (int i = 0; i < textbooks.size(); ++i) {
        for (int j = i + 1; j < textbooks.size(); ++j) {
            similarityMatrix[i][j] = calculateSimilarity(textbooks[i], textbooks[j]);
            similarityMatrix[j][i] = similarityMatrix[i][j];  
        }
    }
}

// Function to display the top 10 pairs of textbooks with the highest similarity scores
void reportTopSimilarPairs(const vector<Textbook>& textbooks) {
    vector<pair<double, pair<int, int>>> similarities;

    // Collect all similarity scores between pairs
    for (int i = 0; i < similarityMatrix.size(); ++i) {
        for (int j = i + 1; j < similarityMatrix[i].size(); ++j) {
            similarities.push_back({similarityMatrix[i][j], {i, j}});
        }
    }

    // Sort the similarity scores in descending order
    sort(similarities.rbegin(), similarities.rend());

    cout << "Top 10 Similar Pairs:\n";
    // Display the top 10 most similar textbook pairs
    for (int i = 0; i < 10 && i < similarities.size(); ++i) {
        const auto& pair = similarities[i].second;
        string file1 = filesystem::path(textbooks[pair.first].filename).stem().string();
        string file2 = filesystem::path(textbooks[pair.second].filename).stem().string();

        cout << "Textbook \"" << file1 << "\" and Textbook \""
             << file2 << "\" with similarity index: "
             << similarities[i].first << endl;
    }
}

int main() {
    vector<Textbook> textbooks;
    string folderPath;

    // Prompt user to enter the folder path containing text files
    cout << "Enter the folder path containing the textbooks (text files): ";
    getline(cin, folderPath);

    // Validate the folder path
    if (!filesystem::exists(folderPath) || !filesystem::is_directory(folderPath)) {
        cerr << "Invalid folder path!" << endl;
        return 1;
    }

    // Process all .txt files in the specified folder
    bool filesFound = false;
    for (const auto& entry : filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            Textbook tb;
            tb.filename = entry.path().string();  // Store the file's full path
            cout << "Processing file: " << tb.filename << endl;  // Notify user of progress
            processTextbook(tb);
            textbooks.push_back(tb);
            filesFound = true;
        }
    }

    // Exit if no .txt files are found
    if (!filesFound) {
        cout << "No .txt files found in the specified folder. Exiting.\n";
        return 0;
    }

    // Generate the similarity matrix
    buildSimilarityMatrix(textbooks);

    // Output the most similar pairs of textbooks
    reportTopSimilarPairs(textbooks);

    return 0;
}