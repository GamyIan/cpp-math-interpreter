#include "../include/preprocessor.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

string preprocessInput(istream &in) {
    string fullInput, line;
    while (getline(in, line)) {
        // Ignoring lines with # to allow comments in code
        if (line.empty() || line[0] == '#')
        continue;
        // Remove trailing whitespace.
        line.erase(find_if(line.rbegin(), line.rend(), [](int ch) {
            return !isspace(ch);
        }).base(), line.end());
        
        // If the line ends with the continuation marker "<<", remove it and append without newline.
        if (line.size() >= 2 && line.substr(line.size() - 2) == "<<") {
            fullInput += line.substr(0, line.size() - 2);
        } else {
            // Append the line with a newline for implicit statement termination.
            fullInput += line + "\n";
        }
    }
    return fullInput;
}
