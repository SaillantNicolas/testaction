#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Point_set_3/Point_set_processing_3.h>

#include <fstream>
#include <limits>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;

typedef CGAL::Point_set_3<Point> Point_set;

int main (int argc, char** argv)
{
  std::ifstream f (argc > 1 ? argv[1] : "data/example.ply");

  Point_set point_set;

  if (!f || !CGAL::read_ply_point_set (f, point_set))
    {
      std::cerr << "Can't read input file " << std::endl;
    }

  std::cerr << point_set.properties(); // Shows which properties were defined

  // Recover "label" property of type int
  Point_set::Property_map<boost::int32_t> label_prop;
  bool found = false;
  boost::tie (label_prop, found)  = point_set.property_map<boost::int32_t> ("label");
  
  if (found)
    {
      std::cerr << "Point set has an integer \"label\" property with values:" << std::endl;
      for (Point_set::iterator it = point_set.begin(); it != point_set.end(); ++ it)
        std::cerr << " * " << label_prop[*it] << std::endl;
    }
  
  return 0;
}
