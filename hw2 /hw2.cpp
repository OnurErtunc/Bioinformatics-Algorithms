#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <chrono>

// Structure to store information about a suffix
struct Suffix
{
    int index;        // Original index in the string
    std::string suff; // The suffix itself
};

// Comparison function used to sort the array of suffixes
bool cmp(const Suffix &a, const Suffix &b)
{
    return a.suff < b.suff;
}

// Function to build the suffix array
std::vector<int> buildSuffixArray(const std::string &text)
{
    int n = text.length();
    std::vector<Suffix> suffixes(n);

    // Store suffixes and their indexes
    for (int i = 0; i < n; i++)
    {
        suffixes[i].index = i;
        suffixes[i].suff = text.substr(i);
    }

    // Sort the suffixes using the comparison function defined above
    std::sort(suffixes.begin(), suffixes.end(), cmp);

    // Store the indexes of sorted suffixes in the suffix array
    std::vector<int> suffixArr(n);
    for (int i = 0; i < n; i++)
    {
        suffixArr[i] = suffixes[i].index;
    }

    // Return the constructed suffix array
    return suffixArr;
}

// Function to print an array
void printArray(const std::vector<int> &array)
{
    for (int i : array)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

// ... Your existing Suffix and buildSuffixArray implementation ...

// Function to read the contents of a FASTA file, ignoring the header lines
std::string readFastaFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    std::string line, text;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '>')
                continue; // Skip headers or empty lines
            text += line;
        }
        file.close();
    }
    else
    {
        throw std::runtime_error("Could not open file: " + filePath);
    }
    return text;
}

// Function to perform binary search on the suffix array to find a pattern
bool findPatternInSuffixArray(const std::vector<int> &suffixArray, const std::string &text, const std::string &pattern)
{
    int left = 0;
    int right = suffixArray.size() - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        int res = text.compare(suffixArray[mid], pattern.length(), pattern);
        if (res == 0)
        {
            // Pattern found
            return true;
        }
        if (res < 0)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return false; // Pattern not found
}

// Helper function to compare pattern with the suffix starting at index 'idx' in text
int compareSuffix(const std::string &text, int idx, const std::string &pattern)
{
    return text.compare(idx, pattern.length(), pattern);
}

// Query 1: Check if pattern P is in text T
bool isPatternInText(const std::vector<int> &suffixArray, const std::string &text, const std::string &pattern)
{
    int l = 0, r = suffixArray.size() - 1;
    while (l <= r)
    {
        int mid = l + (r - l) / 2;
        int res = compareSuffix(text, suffixArray[mid], pattern);
        if (res == 0)
        {
            return true;
        }
        if (res < 0)
        {
            l = mid + 1;
        }
        else
        {
            r = mid - 1;
        }
    }
    return false;
}

// Query 2: Count how many times pattern P occurs in text T
int countPatternOccurrences(const std::vector<int> &suffixArray, const std::string &text, const std::string &pattern)
{
    int count = 0;
    int l = 0, r = suffixArray.size() - 1;
    // Find the first occurrence
    while (l <= r)
    {
        int mid = l + (r - l) / 2;
        int res = compareSuffix(text, suffixArray[mid], pattern);
        if (res == 0)
        {
            // Found one occurrence, now search adjacent suffixes for multiple occurrences
            count++;
            // Check for the occurrences to the left
            int left = mid - 1;
            while (left >= l && compareSuffix(text, suffixArray[left], pattern) == 0)
            {
                count++;
                left--;
            }
            // Check for the occurrences to the right
            int right = mid + 1;
            while (right <= r && compareSuffix(text, suffixArray[right], pattern) == 0)
            {
                count++;
                right++;
            }
            break;
        }
        if (res < 0)
        {
            l = mid + 1;
        }
        else
        {
            r = mid - 1;
        }
    }
    return count;
}

// Query 3: Find where in T is each occurrence of P
std::vector<int> findPatternOccurrences(const std::vector<int> &suffixArray, const std::string &text, const std::string &pattern)
{
    std::vector<int> occurrences;
    int l = 0, r = suffixArray.size() - 1;
    // Find the first occurrence
    while (l <= r)
    {
        int mid = l + (r - l) / 2;
        int res = compareSuffix(text, suffixArray[mid], pattern);
        if (res == 0)
        {
            // Found one occurrence, now search adjacent suffixes for multiple occurrences
            occurrences.push_back(suffixArray[mid]);
            // Check for the occurrences to the left
            int left = mid - 1;
            while (left >= l && compareSuffix(text, suffixArray[left], pattern) == 0)
            {
                occurrences.push_back(suffixArray[left]);
                left--;
            }
            // Check for the occurrences to the right
            int right = mid + 1;
            while (right <= r && compareSuffix(text, suffixArray[right], pattern) == 0)
            {
                occurrences.push_back(suffixArray[right]);
                right++;
            }
            std::sort(occurrences.begin(), occurrences.end());
            break;
        }
        if (res < 0)
        {
            l = mid + 1;
        }
        else
        {
            r = mid - 1;
        }
    }
    return occurrences;
}

// Main function
int main(int argc, char *argv[])
{
    std::string textFilePath;
    std::string patternFilePath;

    // Parse command-line arguments
    //... [Previous code]

    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "-t") == 0)
        {
            textFilePath = argv[++i];
        }
        else if (std::strcmp(argv[i], "-p") == 0)
        {
            patternFilePath = argv[++i];
        }
    }

    /*
    try
    {
        // Read text and patterns from the FASTA files
        std::string text = readFastaFile(textFilePath);
        std::vector<std::string> patterns;
        std::ifstream patternsFile(patternFilePath);

        std::string line;
        while (std::getline(patternsFile, line))
        {
            if (!line.empty() && line[0] != '>')
            {
                patterns.push_back(line);
            }
        }

        // Build the suffix array for the text
        std::vector<int> suffixArray = buildSuffixArray(text);

        // For each pattern, perform the queries and output the results
        std::cout << "Suffix Array:\n";
        for (const auto &pattern : patterns)
        {
            // Query 1: Check if pattern P is in text T
            std::cout << "Time (1): " << (isPatternInText(suffixArray, text, pattern) ? "yes" : "no") << "\n";

            // Query 2: Count how many times pattern P occurs in text T
            std::cout << "Time (2): " << countPatternOccurrences(suffixArray, text, pattern) << "\n";

            // Query 3: Find where in T is each occurrence of P
            std::vector<int> occurrences = findPatternOccurrences(suffixArray, text, pattern);
            std::cout << "Time (3): ";
            for (int idx : occurrences)
            {
                std::cout << idx << " ";
            }
            std::cout << "\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    */

    try
    {
        // Read text and patterns from the FASTA files
        std::string text = readFastaFile(textFilePath);
        std::vector<std::string> patterns;
        std::ifstream patternsFile(patternFilePath);

        std::string line;
        while (std::getline(patternsFile, line))
        {
            if (!line.empty() && line[0] != '>')
            {
                patterns.push_back(line);
            }
        }

        std::vector<int> suffixArray = buildSuffixArray(text);

        // Vectors to store timings for each pattern for each query
        std::vector<long long> timesQuery1(patterns.size(), 0);
        std::vector<long long> timesQuery2(patterns.size(), 0);
        std::vector<long long> timesQuery3(patterns.size(), 0);

        // Run Query 1 for all patterns and collect timings
        for (size_t i = 0; i < patterns.size(); ++i)
        {
            auto start = std::chrono::high_resolution_clock::now();
            isPatternInText(suffixArray, text, patterns[i]);
            auto end = std::chrono::high_resolution_clock::now();
            timesQuery1[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }

        // Run Query 2 for all patterns and collect timings
        for (size_t i = 0; i < patterns.size(); ++i)
        {
            auto start = std::chrono::high_resolution_clock::now();
            countPatternOccurrences(suffixArray, text, patterns[i]);
            auto end = std::chrono::high_resolution_clock::now();
            timesQuery2[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }

        // Run Query 3 for all patterns and collect timings
        for (size_t i = 0; i < patterns.size(); ++i)
        {
            auto start = std::chrono::high_resolution_clock::now();
            findPatternOccurrences(suffixArray, text, patterns[i]);
            auto end = std::chrono::high_resolution_clock::now();
            timesQuery3[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        }

        // Output the timings for each query
        std::cout << "Suffix Array:\n";
        std::cout << "Time (1): ";
        for (const auto &time : timesQuery1)
            std::cout << time << " ";
        std::cout << "\nTime (2): ";
        for (const auto &time : timesQuery2)
            std::cout << time << " ";
        std::cout << "\nTime (3): ";
        for (const auto &time : timesQuery3)
            std::cout << time << " ";
        std::cout << std::endl;

        // Open a file stream to write the output
        std::ofstream outFile("hw2.out");
        if (!outFile.is_open())
        {
            std::cerr << "Failed to open hw2.out for writing." << std::endl;
            return 1;
        }

        outFile << "Suffix Array:\n";
        for (const auto &pattern : patterns)
        {
            // Query 1: Check if pattern P is in text T
            outFile << "Time (1): " << (isPatternInText(suffixArray, text, pattern) ? "yes" : "no") << "\n";

            // Query 2: Count how many times pattern P occurs in text T
            outFile << "Time (2): " << countPatternOccurrences(suffixArray, text, pattern) << "\n";

            // Query 3: Find where in T is each occurrence of P
            std::vector<int> occurrences = findPatternOccurrences(suffixArray, text, pattern);
            outFile << "Time (3): ";
            for (int idx : occurrences)
            {
                outFile << idx << " ";
            }
            outFile << "\n";
        }

        // Close the file stream
        outFile.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
