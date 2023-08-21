//= ========================================================================== =
// Name        : ProjectTwo.cpp
// Author      : Danielle Eeg
// Version     : 1.0
// Description : This program uses BST functions to manipulate course data for ABCU
//============================================================================
//

#include <iostream>
# include <stdexcept>
# include <string>
# include <vector>
# include <list>
# include <sstream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <algorithm>



using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


// define a structure to hold bid information
struct Course {
    string courseId; // unique identifier
    string name;
    vector<string> prereq;

    // default constructor
    Course() {
    }

    // create a course by courseId
    Course(string aCourseId) {
        courseId = aCourseId;
    }

    // create a course by course Id and course name
    Course(string aCourseId, string aCourseName) {
        courseId = aCourseId;
        name = aCourseName;
    }
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    void Insert(Course course);
    void printSampleSchedule();
    void printCourseInformation(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {

    // if root equal to null ptr
    if (root == nullptr) {

        // Create a new node
        root = new Node(course);

    }

    // else, root is not null
    else {
        // add Node root and bid
        addNode(root, course);
    }
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseId.compare(course.courseId) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            // recurse down the left node
            addNode(node->right, course);
        }

    }
}
/**
 * Partition the vector of courses into two parts, low and high
 *
 * @param courseIds Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<string>& courseIds, int begin, int end) {
    //set low and high equal to begin and end
    int low = begin;
    int high = end;

    // pick the middle element as pivot point
    int midpoint = low + (high - low) / 2;
    string pivot = courseIds[midpoint];

    // Add a bid variable for temporary reassignment in loop
    string tempVal;


    // initialize loop exit condition
    bool done = false;

    // while not done 
    while (!done) {

        // keep incrementing low index while courseIds[low] < courseIds[pivot]
        while (courseIds[low] < pivot) {
            low += 1;
        }

        // keep decrementing high index while courseIds[pivot] < courseIds[high]
        while (pivot < courseIds[high]) {
            high -= 1;
        }

        /* If there are zero or one elements remaining,
            all courseIds are partitioned. Return high */
        if (low >= high) {
            done = true;
            return high;
        }
        else {
            // else swap the low and high courseIds (built in vector method)
            std::swap(courseIds[low], courseIds[high]);

            // move low and high closer ++low, --high
            low += 1;
            high -= 1;
        }

    }
    return high;
}

/**
*
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param courseIds address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 *
 */
void quickSort(vector<string>& courseIds, int begin, int end) {
    //set mid equal to 0
    int mid = 0;

    /* Base case: If there are 1 or zero courseIds to sort,
     partition is already sorted otherwise if begin is greater
     than or equal to end then return
     */
    if (courseIds.size() <= 1) {
        return;
    }
    else if (begin >= end) {
        return;
    }

    /* Partition courseIds into low and high such that
     midpoint is location of last element in low */
    mid = partition(courseIds, begin, end);

    // recursively sort low partition (begin to mid)
    quickSort(courseIds, begin, mid);

    // recursively sort high partition (mid+1 to end)
    quickSort(courseIds, mid + 1, end);

}
/*
* BinarySearch is used to search a sorted vector for a value.
* In this case, it will be used to search a vector of courseIDs for a prerquisite Id
*/
int binarySearch(vector<string> searchVector, int vectorSize, string searchId) {

    // intial values of the midpoint, low, and high indices of the search vector
    int mid = 0;
    int low = 0;
    int high = vectorSize - 1;

    // keep searching as long as the high is greater than the low
    while (high >= low) {

        // set the midpoint to the average of the high and low indices
        mid = (high + low) / 2;

        // if the midpoint is less than the search ID
        if (searchVector[mid] < searchId) {

            // increase to low index to one above the midpoint
            low = mid + 1;
        }

        // if the midpoint is above the search Id...
        else if (searchVector[mid] > searchId) {

            // decrease the high search index to one below the midpoint
            high = mid - 1;
        }
        else {

            // return the midpoint when it is equal to the search ID
            return mid;
        }
    }

    // return -1 if no match is found
    return -1;
}

/**
 * Load a CSV file containing Courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    std::cout << "Loading CSV file " << csvPath << "..." << endl;

    /*
    * Define local variables used for logic, temporary placeholders and iterations:
    */
    // create counter to count the number of items added to a course
    int columnCount = 0;

    // courseIndex for searching for prerequisite courses
    int courseIndex = 0;

    // create a counter for number of courses loaded
    int courseCount = 0;

    // initalize course variable
    string courseLine;

    //intialize prequisite variable
    string tempPrereq;

    // intializse course Id variable to ensure uppercase course IDs (for searching logic)
    string tempCourseId;

    // create a string variable to collect the course ID from the current line of the CSV
    string courseIdLine;

    // create a string variable to clear the rest of the line after a course ID is retrieved
    string deleteLine;

    // create a vector of all course IDs to ensure all prerquisites listed are courses
    vector<string> courseIdVector;

    /*
    * Start file iteration by first creating a sorted vector of all course ID
    */
    // create input file stream object
    ifstream inputFile;

    // open file of course information
    inputFile.open(csvPath);

    // Parse through the file to enter course IDs to the vector of course IDs
    while (!inputFile.eof()) {
        // get the first entry of the line only
        getline(inputFile, courseIdLine, ',');

        // add the course ID the the course ID vector
        courseIdVector.push_back(courseIdLine);
        
        // get the rest of the line (not used)
        getline(inputFile, deleteLine, '\n');

        // skip final iteration if end of file reached
        if (inputFile.eof()) break;

    }

    // sort the course ID vector for faster searchability later on
    quickSort(courseIdVector, 0, courseIdVector.size() - 1);

    // close file of course information
    inputFile.close();


    /*
    * Start iterating through the file again, this time collecting course ID, name, and prerequisites.
    * The vector of course IDs will be used to validate course prerequisites (are they in the list of classes)
    */
    // open file of course information
    inputFile.open(csvPath);
    
    // iterate over file to collect each item
    while (!inputFile.eof()) {

        // reset the column count to 0
        columnCount = 0;

        // create a temporary course object to add to binary search tree
        struct Course tempCourse;

        // get each line in CSV file, separated by newline character
        getline(inputFile, courseLine, '\n');

        // skip final iteration if end of file reached
        if (inputFile.eof()) break;

        // start a string stream with basis of courseLine for separating a line by comma characteers
        stringstream ss(courseLine);


        // while there are more objects in the course Line
        while (ss.good()) {

            // if the current position is the first column....
            if (columnCount == 0) {

                // add the first item (up to the first comma) to tempCourse's courseId
                getline(ss, tempCourseId, ',');

                // transform the course ID to all uppercase characters for consistency and comparison logic
                transform(tempCourseId.begin(), tempCourseId.end(), tempCourseId.begin(), ::toupper);

                // add the course Id to the temporary course object
                tempCourse.courseId = tempCourseId;
            }
            // if the current position is the second column....
            else if (columnCount == 1) {

                // add the second item (up to the second comma) to tempCourse's name
                getline(ss, tempCourse.name, ',');
            }

            // if the current position is in a column greater than the second....
            else {

                // add the current item to the local variable tempPrequisite
                getline(ss, tempPrereq, ',');

                // if the prerequisite variable is not empty
                if (!tempPrereq.empty()) {

                    // search for the prerequisite course in the course list
                    courseIndex = binarySearch(courseIdVector, courseIdVector.size(), tempPrereq);
                    
                    // only add the prerequisite if it exists in the course list
                    if (courseIndex != -1) {
                        // add the prerequisite to the prereq vector
                        tempCourse.prereq.push_back(tempPrereq);
                    }
                    else {
                        // output error message for a prerequisite not found in the list of courses
                        cout << tempPrereq << " was not added as a prerequisite for " << tempCourseId << " because it is not found in the course list. " << endl;
                    }
                }
            }

            // increase the column count by one
            columnCount += 1;
        }

        // send an error message if there is not a course name included in the name.
        if (tempCourse.name.empty()) {

            cout << tempCourse.courseId << " does not have a name, and was not added to the course list." <<endl;
        }
        else {
            //Insert course into binary search tree
            bst->Insert(tempCourse);
            courseCount += 1;
        }

    }
    // output the number of courses loaded from file
    cout << courseCount << " courses loaded from file." << endl;

}
/*
In-Order algorithm
@param: starting node
*/
void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node == nullptr) {
        return;
    }
    //InOrder not left
    inOrder(node->left);
    //output bidID, title, amount, fund
    std::cout << node->course.courseId << ", " << node->course.name;
    // end line
    cout << endl;

    //InOrder right
    inOrder(node->right);
}

// function for printing classes in alphanumerical order
void BinarySearchTree::printSampleSchedule() {
    inOrder(root);
}


/*
Function for printing a specific course based on input course ID
@param: courseId for search

*/
void BinarySearchTree::printCourseInformation(string courseId) {

    // transform input to uppercase for compariosn
    transform(courseId.begin(), courseId.end(), courseId.begin(), ::toupper);

    // set current node equal to root
    Node* curNode = root;

    // keep looping downwards until bottom reached or matching bidId found
    // 
    while (curNode != nullptr) {

        // if match found, return current bid
        if (curNode->course.courseId.compare(courseId) == 0) {
            //output course Id, course name
            std::cout << curNode->course.courseId << ", " << curNode->course.name << endl;

            //iterate over prerequisite list, printing off each prerequisite
            for (int i = 0; i < curNode->course.prereq.size(); i++) {

                // output prerequisites label
                if (i == 0){
                    cout << "Prerequisites: ";
                }

                // output comma to separate prerequisites
                else {
                    cout << ", ";
                }

                // ouput prerequisite
                cout << curNode->course.prereq[i];
            }

            // add an end line
            cout << endl;
            return;
        }

        // if bid is smaller than current node then traverse left
        if (curNode->course.courseId.compare(courseId) > 0) {
            curNode = curNode->left;
        }
        // else larger so traverse right
        else {
            curNode = curNode->right;
        }
    }

    // return statement for any input that does not have a matching course Id
    cout << "No course found with Id " << courseId << endl;
}

/*
Function to check if a file exists in the file structure
@param: filename as string to validate exisitance
*/
bool fileExists(const std::string& filename) {
    std::ifstream file(filename);

    // return true (file exists) or false (file does not exist)
    return file.good();
}

/*
Function to check if file path is a CSV path
@param: filename as string to validate as .csv
*/
bool fileCsv(const std::string& filePath) {
    // check if the file suffix indicates it is a .csv file
    if (filePath.substr(filePath.length() - 4, 4) != ".csv") {

        // return false it the file does not end with .csv
        return false;
    }

    // return true if the file does exist with a .csv
    return true;
}

/*
Function to evaluate if a string is convertable to an integer
@param: filename as string to validate as integer
Method source: https://stackoverflow.com/a/2845275
*/
inline bool isInteger(const std::string& checkString)
{

    // if string contains any characters that are non-numerical, return false
    if (checkString.empty() || ((!isdigit(checkString[0])) && (checkString[0] != '-') && (checkString[0] != '+'))) return false;

    // create a character, p, to catch any non-digit character in the input
    char* checkChar;

    // convert string to long, catching any non-integer values
    strtol(checkString.c_str(), &checkChar, 10);

    // return true if no integer values were found; false if at least one was found.
    return (*checkChar == 0);
}


int main()
{
    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    // create string for storing path
    string filePath;

    // create a string for getting courseId input
    string searchId;

    std::cout << "Welcome to the course planner:" << endl;

    // integer and string used to collect user menu input
    int choice = 0;
    string choiceStr;
    while (choice != 9) {
        // print menu:
        std::cout << endl;
        std::cout << "     1. Load Data Structure." << endl;
        std::cout << "     2. Print Course List." << endl;
        std::cout << "     3. Print Course." << endl;
        std::cout << "     9. Exit" << endl;
        std::cout << "What would you like to do? ";
        //std::cin >> choice;
        getline(std::cin, choiceStr);

        // verify that input is an integer
        if(!isInteger(choiceStr)){
            
            // invalid input message
            cout << choiceStr << " is not a valid option." << endl;
            continue;
        }
        else {

            // convert string to integer variable
            choice = stoi(choiceStr);
        }

        // verify that numerical input is an option corelating to the menu
        if ((choice > 3 && choice != 9) || choice < 1) {

            // invalid input message
            cout << choice << " is not a valid option." << endl;
            continue;
        }
        

        switch (choice) {

        case 1:
            // load courses
            // ask for file path
            cout << endl << "Please enter file path of course data. File path: ";

            // input file path
            getline(std::cin, filePath);

            //verify that the file path exists
            if (fileExists(filePath)) {

                // verify that the file is a CSV file
                if (!fileCsv(filePath)) {
                    cout << "Incompatible file type. Please enter the file path ending in .csv (comma separated variable file).\nReturning to main menu..." << endl;
                    break;
                }

                // load courses from csv
                loadCourses(filePath, bst);
                break;
            }
            else {

                // if the file path does not exist, send message
                cout << "No such file found. \n Returning to main menu..." << endl;

                break;
            }


        case 2:
            // print course schedule
            bst->printSampleSchedule();
            break;

        case 3:
            // request course ID to search
            cout << "Input course ID to search: ";
            getline(std::cin, searchId);

            // print the course information associated with that search ID
            bst->printCourseInformation(searchId);

            break;
        }

    }
}