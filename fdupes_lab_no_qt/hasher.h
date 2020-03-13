#define HASH_CONST 65536;

namespace fs = std::experimental::filesystem;
using namespace std;

typedef unsigned long long hashval;

hashval get_hash(fs::path file_path);
