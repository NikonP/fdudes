#define HASH_CONST 65536;

namespace fs = std::experimental::filesystem;
using namespace std;

unsigned long long get_hash(fs::path file_path);
