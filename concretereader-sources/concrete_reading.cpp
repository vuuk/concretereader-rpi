#include "concrete_reading.h"

std::chrono::time_point<std::chrono::system_clock>
concrete_reading::starter_time = std::chrono::system_clock::now();

std::ostream& operator<<(std::ostream& out, const concrete_reading& r)
{
    //out << r.p.x() << ", " << r.p.y();
    out << r.x << ", " << r.y;
    return out;
}
