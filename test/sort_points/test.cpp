#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

struct GraphPoint {
    int barPos;
    int gridPos;
    int bpm;

    GraphPoint(int barPos, int gridPos, int bpm) :
        barPos{barPos}, gridPos{gridPos}, bpm{bpm} { }
};

int main() {
    std::vector<GraphPoint> v;

    GraphPoint point {5,2,20};
    v.push_back(point);
    v.emplace_back(2,5,50);
    v.emplace_back(2,2,80);
    v.emplace_back(4,1,70);

    std::sort(v.begin(), v.end(), [](GraphPoint &p1, GraphPoint &p2) {
         if (p1.barPos < p2.barPos) {
             return true;
         }
         else if (p1.barPos == p2.barPos) {
             if (p1.gridPos < p2.gridPos) {
                 return true;
             }
         }
         return false;
        });

    for(const auto& value: v) {
        std::cout << "(" << value.barPos << ", " << value.gridPos
                  << ", " << value.bpm << ")\n";
    }
    return 0;
}
