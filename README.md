Textbook Similarity Analyzer

Author

Lakshya Mishra

Overview

The Textbook Similarity Analyzer is a C++ program that processes multiple textbook files to calculate and compare their word usage patterns. It identifies the most similar pairs of textbooks based on word frequency analysis, excluding common stop words.

Features
	•	Reads .txt files from a specified folder.
	•	Computes word frequencies for each file, excluding common stop words like “a,” “and,” “the.”
	•	Generates a similarity matrix to compare textbooks.
	•	Displays the top 10 most similar textbook pairs with similarity indices.

How It Works
	1.	Word Frequency Analysis: Each textbook is scanned, and word frequencies are calculated after cleaning and filtering out common stop words.
	2.	Similarity Calculation: A similarity score is computed between every pair of textbooks based on their word frequency distributions.
	3.	Report Generation: Outputs the top 10 pairs of textbooks with the highest similarity scores, providing insights into their textual alignment.

Input & Output
	•	Input: A folder containing .txt files representing textbooks.
	•	Output: Displays the top 10 similar pairs of textbooks with their similarity indices and processing progress for each file.

Requirements
	•	C++17 or later for filesystem and other modern standard libraries.
	•	A folder containing .txt files.
