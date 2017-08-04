#ifndef _lab_math_polygon_hpp_
#define _lab_math_polygon_hpp_

#include <vector>
#include <glm/glm.hpp>
#include "lab/util/exception.hpp"
#include "lab/math/segment.hpp"
#include "lab/math/triangle.hpp"

namespace lab { namespace math {

class polygon {
  private:
    std::vector<glm::vec2> _points;
    std::vector<segment>   _edges;
    std::vector<triangle>  _triangles;


    polygon(const std::vector<glm::vec2>& points,
            const std::vector<segment>&   edges,
            const std::vector<triangle>&  triangles);

  public:
    ~polygon();

    static polygon build(const std::vector<glm::vec2>& path)
        throw(util::exception);
};

}}

#endif

