#include <iostream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

#define MAX_CHAR 256

struct SuffixTreeNode
{
    SuffixTreeNode *children[MAX_CHAR];
    SuffixTreeNode *suffixLink;
    int start;
    int *end;
    int suffixIndex;
};

typedef SuffixTreeNode Node;

char text[100];
Node *root = nullptr;
Node *lastNewNode = nullptr;
Node *activeNode = nullptr;
int count = 0;

int activeEdge = -1;
int activeLength = 0;

int remainingSuffixCount = 0;
int leafEnd = -1;
int *rootEnd = nullptr;
int *splitEnd = nullptr;
int size = -1;

// Function to create a new node in the suffix tree
Node *newNode(int start, int *end)
{
    count++;
    Node *node = new Node;
    for (int i = 0; i < MAX_CHAR; i++)
        node->children[i] = nullptr;

    node->suffixLink = root;
    node->start = start;
    node->end = end;
    node->suffixIndex = -1;
    return node;
}

// Function to calculate the length of an edge
int edgeLength(Node *n)
{
    return *(n->end) - (n->start) + 1;
}

// Function to perform walk down in the tree
int walkDown(Node *currNode)
{
    if (activeLength >= edgeLength(currNode))
    {
        activeEdge = static_cast<int>(text[activeEdge + edgeLength(currNode)]) - static_cast<int>(' ');
        activeLength -= edgeLength(currNode);
        activeNode = currNode;
        return 1;
    }
    return 0;
}

// Function to extend the suffix tree
void extendSuffixTree(int pos)
{
    leafEnd = pos;
    remainingSuffixCount++;
    lastNewNode = nullptr;

    while (remainingSuffixCount > 0)
    {

        if (activeLength == 0)
        {
            activeEdge = static_cast<int>(text[pos]) - static_cast<int>(' ');
        }

        if (activeNode->children[activeEdge] == nullptr)
        {
            activeNode->children[activeEdge] = newNode(pos, &leafEnd);

            if (lastNewNode != nullptr)
            {
                lastNewNode->suffixLink = activeNode;
                lastNewNode = nullptr;
            }
        }
        else
        {
            Node *next = activeNode->children[activeEdge];
            if (walkDown(next))
            {
                continue;
            }

            if (text[next->start + activeLength] == text[pos])
            {
                if (lastNewNode != nullptr && activeNode != root)
                {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = nullptr;
                }

                activeLength++;
                break;
            }

            splitEnd = new int;
            *splitEnd = next->start + activeLength - 1;

            Node *split = newNode(next->start, splitEnd);
            activeNode->children[activeEdge] = split;

            split->children[static_cast<int>(text[pos]) - static_cast<int>(' ')] = newNode(pos, &leafEnd);
            next->start += activeLength;
            split->children[activeEdge] = next;

            if (lastNewNode != nullptr)
            {
                lastNewNode->suffixLink = split;
            }

            lastNewNode = split;
        }

        remainingSuffixCount--;
        if (activeNode == root && activeLength > 0)
        {
            activeLength--;
            activeEdge = static_cast<int>(text[pos - remainingSuffixCount + 1]) - static_cast<int>(' ');
        }
        else if (activeNode != root)
        {
            activeNode = activeNode->suffixLink;
        }
    }
}

// Function to print characters from index i to j
void print(int i, int j)
{
    for (int k = i; k <= j; k++)
        std::cout << text[k];
}

// Function to set suffix index by DFS traversal
void setSuffixIndexByDFS(Node *n, int labelHeight)
{
    if (n == nullptr)
        return;

    if (n->start != -1)
    {
        print(n->start, *(n->end));
    }
    int leaf = 1;
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (n->children[i] != nullptr)
        {
            if (leaf == 1 && n->start != -1)
                std::cout << " [" << n->suffixIndex << "]\n";

            leaf = 0;
            setSuffixIndexByDFS(n->children[i], labelHeight + edgeLength(n->children[i]));
        }
    }
    if (leaf == 1)
    {
        n->suffixIndex = size - labelHeight;
        std::cout << " [" << n->suffixIndex << "]\n";
    }
}

// Function to free memory in post-order traversal
void freeSuffixTreeByPostOrder(Node *n)
{
    if (n == nullptr)
        return;
    for (int i = 0; i < MAX_CHAR; i++)
    {
        if (n->children[i] != nullptr)
        {
            freeSuffixTreeByPostOrder(n->children[i]);
        }
    }
    if (n->suffixIndex == -1)
        delete n->end;
    delete n;
}

// Function to build the suffix tree
void buildSuffixTree()
{
    size = strlen(text);
    rootEnd = new int;
    *rootEnd = -1;

    root = newNode(-1, rootEnd);

    activeNode = root;
    for (int i = 0; i < size; i++)
        extendSuffixTree(i);
    int labelHeight = 0;
    setSuffixIndexByDFS(root, labelHeight);

    freeSuffixTreeByPostOrder(root);
}

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
//// extras

bool isPatternInText(Node *root, const std::string &pattern)
{
    Node *currentNode = root;
    if (!currentNode)
        return false;

    int patIndex = 0; // Index for the pattern

    // Iterate over each character in the pattern and traverse the tree
    while (patIndex < pattern.size())
    {
        // Convert character to index (assuming ASCII values)
        int charIndex = pattern[patIndex];

        // If there is no edge for the current character, the pattern doesn't exist in the tree
        if (currentNode->children[charIndex] == nullptr)
        {
            return false;
        }

        // Edge exists, check along the edge for the pattern
        Node *edge = currentNode->children[charIndex];
        int edgeLen = *(edge->end) - edge->start + 1;

        // Check if the pattern matches along the edge
        for (int i = 0; i < edgeLen && patIndex < pattern.size(); ++i, ++patIndex)
        {
            if (text[edge->start + i] != pattern[patIndex])
            {
                // Character mismatch
                return false;
            }
        }

        // Pattern has been matched along the current edge, move to next node
        currentNode = edge;
    }

    // If we've reached this point, then the pattern has been fully matched
    return true;
}

// Helper function to count leaf nodes below a given node
int countLeafNodes(Node *node)
{
    if (node == nullptr)
    {
        return 0;
    }

    // If it's a leaf node
    if (*node->end == size - 1)
    {
        return 1;
    }

    // If it's not a leaf node, count leaves for all children
    int leafCount = 0;
    for (int i = 0; i < MAX_CHAR; ++i)
    {
        leafCount += countLeafNodes(node->children[i]);
    }
    return leafCount;
}

// Main function to count occurrences of the pattern in the suffix tree
int countPatternOccurrences(Node *node, const std::string &pattern, int patIndex = 0)
{
    Node *currentNode = node;
    int patternLength = pattern.length();
    int j = 0; // Declare j outside the loop to make it accessible after the loop

    // Iterate over each character in the pattern and traverse the tree
    for (int i = patIndex; i < patternLength;)
    {
        int charIndex = pattern[i] - 'A'; // Convert the character to an index

        // If there is no edge for the current character, the pattern doesn't exist in the tree
        if (currentNode->children[charIndex] == nullptr)
        {
            return 0; // Pattern not found
        }

        // Edge exists, check along the edge for the pattern
        Node *edge = currentNode->children[charIndex];
        int edgeLen = *edge->end - edge->start + 1;

        // Check if the pattern matches along the edge
        for (j = 0; j < edgeLen && i < patternLength; ++j, ++i)
        {
            if (text[edge->start + j] != pattern[i])
            {
                return 0; // Character mismatch
            }
        }

        // Pattern has been matched along the current edge, move to next node
        currentNode = edge;

        // Correctly adjust the loop variable i after checking an edge
        // This check happens after the loop, so j's value remains from the loop
        if (j == edgeLen && i < patternLength)
        {
            // Do not increment i in the next iteration to check the next character
            // This is already handled by the loop increment
            continue;
        }
    }

    // All characters in the pattern have been matched, now count all leaf nodes below this node
    return countLeafNodes(currentNode);
}

void collectLeafIndices(Node *node, std::vector<int> &indices, int patternLength, int nodeDepth = 0)
{
    if (node == nullptr)
        return;

    // If it's a leaf node
    if (node->suffixIndex != -1)
    {
        // Calculate the start index of this occurrence by subtracting the pattern length
        // from the suffix index (adjusted for any partial edge length traversed).
        indices.push_back(node->suffixIndex - patternLength + 1 + nodeDepth);
    }

    // Recursively check all children
    for (int i = 0; i < MAX_CHAR; ++i)
    {
        if (node->children[i] != nullptr)
        {
            // The edge length to the child helps adjust the depth accurately.
            int edgeLen = *(node->children[i]->end) - node->children[i]->start + 1;
            collectLeafIndices(node->children[i], indices, patternLength, nodeDepth + edgeLen);
        }
    }
}

std::vector<int> findPatternOccurrences(Node *root, const std::string &pattern)
{
    std::vector<int> indices;
    Node *currentNode = root;

    int i = 0;
    while (i < pattern.size())
    {
        int charIndex = pattern[i] - 'A'; // Convert the character to an index

        if (currentNode->children[charIndex] == nullptr)
        {
            // If there is no edge for the current character, the pattern doesn't exist
            return indices; // Return empty vector
        }

        Node *edge = currentNode->children[charIndex];
        int edgeLen = *edge->end - edge->start + 1;

        int j;
        for (j = 0; j < edgeLen && i < pattern.size(); ++j, ++i)
        {
            if (text[edge->start + j] != pattern[i])
            {
                return indices; // Mismatch
            }
        }

        currentNode = edge; // Move to the next node
    }

    // Collect indices of all occurrences from here
    collectLeafIndices(currentNode, indices, pattern.size());
    return indices;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " -t text.fasta -p patterns.fasta" << std::endl;
        return 1;
    }

    std::string textFilePath;
    std::string patternFilePath;

    // Parse command-line arguments
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

    try
    {
        // Read text from the FASTA file
        std::string text = readFastaFile(textFilePath);
        strcpy(::text, text.c_str()); // Copy to the global text variable
        ::size = text.length();       // Set the global size variable

        // Build the suffix tree
        buildSuffixTree();

        // Read patterns from the FASTA file
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

        // Open the output file
        std::ofstream outFile("hw2_imp.out");
        if (!outFile.is_open())
        {
            throw std::runtime_error("Could not open hw2.out for writing.");
        }

        // For each pattern, find its occurrences in the text using the suffix tree
        for (const auto &pattern : patterns)
        {
            auto occurrences = findPatternOccurrences(root, pattern);
            if (!occurrences.empty())
            {
                for (const auto &idx : occurrences)
                {
                    outFile << idx << " ";
                }
                outFile << "\n";
            }
            else
            {
                outFile << "Pattern \"" << pattern << "\" not found.\n";
            }
        }

        // Cleanup and close the file
        outFile.close();
        freeSuffixTreeByPostOrder(root);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}