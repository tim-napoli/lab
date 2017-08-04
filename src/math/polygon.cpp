#include <limits>
#include "lab/math/box.hpp"
#include "lab/math/triangle.hpp"
#include "lab/math/polygon.hpp"
#include "lab/math/format.hpp"

namespace lab { namespace math {

// XXX What is the coundition to update a polygon at one of its vertex update ?
//     Let T be the only polygon's triangle such that its second point 

polygon::polygon(const std::vector<glm::vec2>& points,
                 const std::vector<segment>&   edges,
                 const std::vector<triangle>&  triangles)
    : _points(points)
    , _edges(edges)
    , _triangles(triangles)
{

}

polygon::~polygon() {

}

// construction ---------------------------------------------------------------

std::vector<segment> edges_of_path(const std::vector<glm::vec2>& path) {
    std::vector<segment> edges(path.size());
    for (size_t i = 0; i < path.size(); i++) {
        edges[i] = segment(path[i], path[(i + 1) % path.size()]);
    }
    return edges;
}

bool is_valid_path(const std::vector<glm::vec2>& path) {
    std::vector<segment> edges = edges_of_path(path);

    for (size_t i = 0; i < edges.size() - 1; i++) {
        for (size_t j = i + 1; j < edges.size(); j++) {
            segment si = edges[i];
            segment sj = edges[j];
            auto inter = si.get_intersection(sj);

            if (j - i == 1) {
                if (!inter.is_set()) {
                    std::cerr << "two adjacent edges must cross at their "
                              << "common point, but no intersection found."
                              << std::endl;
                    return false;
                } else
                if (!vec2_are_equal(inter.get_value(), si.get_b()))
                {
                    std::cerr << "two adjacent edges must cross at their "
                              << "common point, but intersection is wrong:"
                              << "a = " << si << ", b = " << sj << ", inter = "
                              << inter.get_value()
                              << std::endl;
                    return false;
                }
            } else
            if (i == 0 && j == edges.size() - 1) {
                if (!inter.is_set()) {
                    std::cerr << "two adjacent edges must cross at their "
                              << "common point, but no intersection found."
                              << std::endl;
                    return false;
                } else
                if (!vec2_are_equal(inter.get_value(), si.get_a()))
                {
                    std::cerr << "two adjacent edges must cross at their "
                              << "common point, but intersection is wrong:"
                              << "a = " << si << ", b = " << sj << ", inter = "
                              << inter.get_value()
                              << std::endl;
                    return false;
                }
            } else
            if (inter.is_set()) {
                std::cerr << "two non-adjacent edges musn't cross"
                          << ", but intersection is found:"
                          << "a = " << si << ", b = " << sj << ", inter = "
                          << inter.get_value()
                          << std::endl;
                return false;
            }
        }
    }

    return true;
}

bool is_out(glm::vec2 p, const std::vector<segment>& edges,
            const box& bounds)
{
    // To know that a point is in or out a path, we will count times the segment
    // from the center of the triangle and a virtual infinite point intersect.
    // If this count is even, then the point is outside the shape.
    glm::vec2 q = glm::vec2(bounds.left() - 1.0, bounds.bottom() - 1.0);
    segment pq = segment(p, q);

    // What happen if pq intersect exactly a vertex of the path ?
    // It will be counted twice, but should count only for one.
    // A simple solution is to exclude the b point of the iterated edge.
    int count = 0;
    for (auto it = edges.begin(); it != edges.end(); it++) {
        auto inter = pq.get_intersection(*it);
        if (inter.is_set()) {
            glm::vec2 inter_point = inter.get_value();
            // We exclude the b point of the edge.
            if (!vec2_are_equal(inter_point, it->get_b())) {
                count++;
            }
        }
    }

    return count % 2 == 0;
}

bool is_hear(glm::vec2 a, glm::vec2 b, glm::vec2 c,
             const std::vector<segment>& edges,
             const box& bounds)
{
    // Three adjacent points of the path are a hear if the center of these
    // points is in the path, and if [ac] doesn't cross any other edge.
    segment ac = segment(a, c);
    for (size_t i = 0; i < edges.size(); i++) {
        segment other = edges[i];
        auto inter = ac.get_intersection(other);
        if (inter.is_set()
        &&  !vec2_are_equal(inter.get_value(), a)
        &&  !vec2_are_equal(inter.get_value(), c))
        {
            return false;
        }
    }

    glm::vec2 center = (a + b + c) / 3.0f;
    return !is_out(center, edges, bounds);
}

bool is_hear_prime(glm::vec2 a, glm::vec2 b, glm::vec2 c,
                   const std::vector<segment>& edges,
                   const box& bounds)
{
    // b must be a convex vertex of the polygon.
    // This means that the center of the triangle (a, b, c) is inside the
    // polygon.
    glm::vec2 center = (a + b + c) / 3.0f;
    if (is_out(center, edges, bounds)) {
        return false;
    }

    // Next, if no other polygon vertex is inside the triangle, it is a hear.
    triangle t = triangle (a, b, c);
    for (auto it = edges.begin(); it != edges.end(); it++) {
        glm::vec2 vertex = it->get_a();
        if (!vec2_are_equal(vertex, a)
        &&  !vec2_are_equal(vertex, b)
        &&  !vec2_are_equal(vertex, c))
        {
            if (t.contains(vertex)) {
                return false;
            }
        }
    }

    return true;
}

std::vector<triangle> triangulize(const std::vector<glm::vec2>& path,
                                  const std::vector<segment>& edges,
                                  const box& bounds)
    throw(util::exception)
{
    std::vector<glm::vec2> remaining_points = path;
    std::vector<segment>   remaining_edges  = edges;
    std::vector<triangle>  triangles;

    while (remaining_points.size() > 3) {
        bool found = false;
        for (size_t i = 0; i < remaining_points.size(); i++) {
            glm::vec2 a = remaining_points[i];
            glm::vec2 b = remaining_points[(i + 1) % remaining_points.size()];
            glm::vec2 c = remaining_points[(i + 2) % remaining_points.size()];

            if (is_hear_prime(a, b, c, remaining_edges, bounds)) {
                size_t pivot_index = (i + 1) % remaining_points.size();
                remaining_points.erase(remaining_points.begin() + pivot_index);
                remaining_edges[i].set_b(c);
                remaining_edges.erase(remaining_edges.begin() + pivot_index);

                triangles.push_back(triangle(a, b, c));
                found = true;
                break;
            }
        }

        if (!found) {
            std::cerr << "cannot found hear: " << std::endl;
            svg_header(std::cerr, 1.0, 1.0);
            svg_path(std::cerr, path);
            svg_path(std::cerr, remaining_points);
            svg_footer(std::cerr);
            throw util::exception("triangulization error");
        }
    }

    triangles.push_back(
        triangle(remaining_points[0], remaining_points[1], remaining_points[2])
    );
    return triangles;
}

box get_bounding_box(const std::vector<glm::vec2>& path) {
    float xmin = std::numeric_limits<float>::max();
    float xmax = std::numeric_limits<float>::min();
    float ymin = std::numeric_limits<float>::max();
    float ymax = std::numeric_limits<float>::min();

    for (auto it = path.begin(); it != path.end(); it++) {
        if (it->x < xmin) xmin = it->x;
        if (it->x > xmax) xmax = it->x;
        if (it->y < ymin) ymin = it->y;
        if (it->y > ymax) ymax = it->y;
    }

    return box(xmin, xmax, ymin, ymax);
}

std::vector<glm::vec2> merge_path(const std::vector<glm::vec2>& path) {
    std::vector<glm::vec2> merged;
    for (size_t i = 0; i < path.size(); i++) {
        size_t j = (i + 1) % path.size();
        if (!vec2_are_equal(path[i], path[j])) {
            merged.push_back(path[i]);
        }
    }
    return merged;
}

polygon polygon::build(const std::vector<glm::vec2>& path)
    throw(util::exception)
{
    // First, we merge path vertexes that are similars (to avoid some silly
    // precision problems).
    std::vector<glm::vec2> merged_path = merge_path(path);

    if (!is_valid_path(merged_path)) {
        throw util::exception("invalid path given");
    }

    std::vector<segment> edges = edges_of_path(merged_path);
    box bounds = get_bounding_box(merged_path);
    std::vector<triangle> triangles = triangulize(merged_path, edges, bounds);

    return polygon(path, edges, triangles);
}

// ----------------------------------------------------------------------------

}}

