#ifndef MIN_H
#define MIN_H

#include <string>
#include <stdint.h>
#include <vector>

namespace Level
{
    class Min
    {
        public:
            Min(const std::string&);
            const std::vector<int16_t>& operator[] (size_t index) const;
            size_t size() const;

        private:
            std::vector<std::vector<int16_t> > mPillars;
    };
}

#endif
