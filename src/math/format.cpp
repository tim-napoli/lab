#include "lab/math/format.hpp"

namespace lab { namespace math {

std::ostream& operator<<(std::ostream& os, glm::vec2 v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, segment s) {
    os << "(" << s.get_a() << ", " << s.get_b() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, triangle t) {
    os << "(" << t.get_a() << ", " << t.get_b()
       << ", " << t.get_c() << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, box b) {
    os << "(" << b.bottom_left() << ", " << b.top_right()  << ")";
    return os;
}

void svg_header(std::ostream& os, float width, float height) {
    os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    os << "<svg " << "xmlns=\"http://www.w3.org/2000/svg\" "
                  << "version=\"1.1\" "
                  << "width=\"" << width << "\" "
                  << "height=\"" << height << "\""
       << ">" << std::endl;
}

void svg_vec2(std::ostream& os, glm::vec2 v) {
    os << "\t<circle " << "cx=\"" << v.x << "\" "
                       << "cy=\"" << v.y << "\" "
                       << "r=\"0.01\" "
                       << "stroke-width=\"0.01\" "
       << "/>" << std::endl;
}

void svg_segment(std::ostream& os, segment s) {
    os << "\t<line " << "x1=\"" << s.get_a().x << "\" "
                     << "y1=\"" << s.get_a().y << "\" "
                     << "x2=\"" << s.get_b().x << "\" "
                     << "y2=\"" << s.get_b().y << "\" "
                     << "stroke=\"black\" "
                     << "stroke-width=\"0.01\" "
      << "/>" << std::endl;
}

void svg_triangle(std::ostream& os, triangle t) {
    os << "\t<polygon " << "points=\"" << t.get_a().x << " "
                                       << t.get_a().y << " "
                                       << t.get_b().x << " "
                                       << t.get_b().y << " "
                                       << t.get_c().x << " "
                                       << t.get_c().y << " "
                        << "\" "
                        << "fill=\"none\" "
                        << "stroke=\"black\" "
                        << "stroke-width=\"0.01\" "
      << "/>" << std::endl;
}

void svg_box(std::ostream& os, box b) {
    os << "\t<polygon " << "points=\"" << b.left()   << " "
                                       << b.top()    << " "
                                       << b.right()  << " "
                                       << b.top()    << " "
                                       << b.right()  << " "
                                       << b.bottom() << " "
                                       << b.left()   << " "
                                       << b.bottom() << " "
                        << "\" "
                        << "fill=\"none\" "
                        << "stroke=\"black\" "
                        << "stroke-width=\"0.01\" "
      << "/>" << std::endl;
}

void svg_footer(std::ostream& os) {
    os << "</svg>" << std::endl;
}

}}

