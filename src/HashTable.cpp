//============================================================================
// Name        : HashTable.cpp
// Author      : Brandon Thibeaux
// Class       : CS-260-H1166
// Date        : 10-3-2020
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // FIXME (1): Define structures to hold bids
	struct Node
	{
		Bid bids;
		unsigned key;
		Node* next;
		//default constructor
		Node()
		{
			key = UINT_MAX;
			next = nullptr;
		}

		//second constructor that takes a bid
		Node(Bid oneBid) : Node()
		{
			bids  = oneBid;
		}

		Node(Bid oneBid, unsigned oneKey) : Node(oneBid)
		{
			key = oneKey;
		}
	};

    unsigned int hash(int key);
    vector<Node> nodes;
    Node* head;
    unsigned tableSize = DEFAULT_SIZE; //allows us to change size later

public:
    HashTable();
    virtual ~HashTable();
    HashTable(unsigned size);
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};


/**
 * Default constructor
 */
HashTable::HashTable() {
    // FIXME (2): Initialize the structures used to hold bids
	nodes.resize(tableSize);
	head = nullptr;

}

HashTable::HashTable(unsigned size)
{
	this->tableSize = size;
	nodes.resize(tableSize);
}
/**
 * Destructor
 */
HashTable::~HashTable() {
    // FIXME (3): Implement logic to free storage when class is destroyed
	nodes.erase(nodes.begin());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // FIXME (4): Implement logic to calculate a hash value
	return key % tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // FIXME (5): Implement logic to insert a bid
	unsigned key = hash(atoi(bid.bidId.c_str()));//atoi converts ascii string to integer

	//try to retrieve node using the key
	Node* oldNode = &(nodes.at(key)); //putting the address of the node.at(key) into the pointer oldNode

	//if no entry found for key
	if(oldNode == nullptr)
	{
		//node is not found
		Node* newNode = new Node(bid,key);
		nodes.insert(nodes.begin() + key, (*newNode));

	}
	else
	{
		//node is found
		if(oldNode->key == UINT_MAX)
		{
			oldNode->key = key;
			oldNode->bids = bid;
			oldNode->next = nullptr;
		}
		else
		{
			while(oldNode->next != nullptr)
			{
				oldNode = oldNode->next;
			}
			oldNode->next = new Node(bid,key);
		}
	}

}

/**
 * Print all bids
 */
void HashTable::PrintAll() {//INOP FIX THIS
    // FIXME (6): Implement logic to print all bids
	Node* current = head;

	Node* node = &(nodes.at(0));//& takes the address of nodes.at(key) and stores it into node pointer
	//cout<<"PrintAll() has been accessed"<<endl;//Debug line

	//loops while current pointer is not null
	while (node != nullptr)
	{
		cout << node -> bids.bidId << ": " << node -> bids.title << " | "
						<< node -> bids.amount << " | " << node -> bids.fund << endl;

		node = node->next;
	}

}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // FIXME (7): Implement logic to remove a bid
	unsigned key = hash(atoi(bidId.c_str()));//atoi converts ascii string to integer
	nodes.erase(nodes.begin() + key);
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // FIXME (8): Implement logic to search for and return a bid
    unsigned key = hash(atoi(bidId.c_str()));//calculate the key for this bid

    //try and retrieve the node using the key
    Node * node = &(nodes.at(key));//& takes the address of nodes.at(key) and stores it into node pointer

    //if no entry found
    if (node == nullptr || node->key == UINT_MAX)
    {
    	return bid;
    }
    //if node found match with key
    if (node != nullptr && node->key != UINT_MAX
    		&& node->bids.bidId.compare(bidId) == 0)
    {
    	return node->bids;
    }
    //walk the linked list to find the match
    while(node != nullptr)
    {
    	if(node->key != UINT_MAX && node->bids.bidId.compare(bidId) == 0)
    	{
    		return node->bids;
    	}
    	node = node->next;
    }

    return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bidTable = new HashTable();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
