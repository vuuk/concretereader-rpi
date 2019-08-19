#ifndef CONCRETE_READING_H
#define CONCRETE_READING_H

//#include <QTime>
#include <regex>
#include <iostream>
#include <chrono>
#include <QPoint>
//#include <ctime>
struct concrete_reading
{
    friend std::ostream& operator<<(std::ostream&, const concrete_reading&);
    static std::chrono::time_point<std::chrono::system_clock> starter_time;
    concrete_reading() = default;
    concrete_reading(const std::string& str)
      //  val (std::stod(std::regex_replace(str, std::regex("([\\D])"), ""))),
 //       timestamp (std::chrono::duration_cast<std::chrono::milliseconds>(
     //                  std::chrono::system_clock::now() - starter_time).count())
    {
        //val = std::stod(std::regex_replace(str, std::regex("([\\D])"), ""));
        //timestamp = stamper.currentTime();
        /*
        p.setX(std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now() - starter_time).count());
                   */
        x = (std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now() - starter_time).count()) / 1000;
        y = std::stod(std::regex_replace(str, std::regex("([\\D])"), ""));
    //    p.setY(std::stod(std::regex_replace(str, std::regex("([\\D])"), "")));
       // std::cerr << "tried to create with: " << str;
    }
    static void set_start_time() { starter_time = std::chrono::system_clock::now(); }
    static int secs_since_start_time() {
        return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::system_clock::now() - starter_time).count();
    }

    qreal X() const  { return x; }
    int Y() const {return y;}
   // double val;
   // QTime timestamp;
   // long long timestamp;
   // QPoint p;
    double x;
    int y;
};
std::ostream& operator<<(std::ostream&, const concrete_reading&);
#endif // CONCRETE_READING_H
