#include <vector>

template <class T>
std::vector<T> concatenate_vectors(const std::vector<T> &v1, const std::vector<T> &v2){
    std::vector<T> r = v1;
    r.insert(r.end(), v2.begin(), v2.end());
    return r;
}
