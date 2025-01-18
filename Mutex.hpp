#pragma once

#include <mutex>

// Deklaracja globalnego mutexu do synchronizacji dostępu do std::cout
extern std::mutex cout_mutex;
