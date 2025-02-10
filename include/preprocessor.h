#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <string>
#include <istream>

// Reads from an input stream, handling line continuation markers (e.g., "<<")
// and returns the complete preprocessed input as a string.
std::string preprocessInput(std::istream &in);

#endif
