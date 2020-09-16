//! \file examples/Arrangement_on_surface_2/unbounded_non_intersecting.cpp
// Constructing an arrangement of unbounded linear objects using the insertion
// function for non-intersecting curves.

#include "arr_linear.h"
#include "arr_print.h"

int main()
{
  Arrangement arr;

  // Insert a line in the (currently single) unbounded face of the arrangement,
  // then split it into two at (0,0). Assign v to be the split point.
  X_monotone_curve c1 = Line(Point(-1, 0), Point(1, 0));
  Halfedge_handle  e1 = arr.insert_in_face_interior(c1, arr.unbounded_face());
  X_monotone_curve c1_left = Ray(Point(0, 0), Point(-1, 0));
  X_monotone_curve c1_right = Ray(Point(0, 0), Point(1, 0));
  e1 = arr.split_edge(e1, c1_left, c1_right);
  Vertex_handle v = e1->target();
  CGAL_assertion(! v->is_at_open_boundary());

  // Add two more rays using the specialized insertion functions.
  arr.insert_from_right_vertex(Ray(Point(0, 0), Point(-1, 1)), v); // c2
  arr.insert_from_left_vertex(Ray(Point(0, 0), Point(1, 1)), v);   // c3

  // Insert three more interior-disjoint rays, c4, c5, and c6.
  insert_non_intersecting_curve(arr, Ray(Point(0, -1), Point(-2, -2)));
  insert_non_intersecting_curve(arr, Ray(Point(0, -1), Point(2, -2)));
  insert_non_intersecting_curve(arr, Ray(Point(0, 0), Point(0, 1)));

  print_unbounded_arrangement_size(arr);

  // Print the outer CCBs of the unbounded faces.
  int k = 1;
  for (auto it = arr.unbounded_faces_begin(); it != arr.unbounded_faces_end();
       ++it)
  {
    std::cout << "Face no. " << k++ << "(" << it->number_of_outer_ccbs()
              << "," << it->number_of_inner_ccbs() << ")" << ": ";
    Arrangement::Ccb_halfedge_const_circulator  first, curr;
    curr = first = it->outer_ccb();
    if (! curr->source()->is_at_open_boundary())
      std::cout << "(" << curr->source()->point() << ")";

    do {
      Arrangement::Halfedge_const_handle he = curr;
      if (! he->is_fictitious()) std::cout << "   [" << he->curve() << "]   ";
      else std::cout << "   [ ... ]   ";

      if (! he->target()->is_at_open_boundary())
        std::cout << "(" << he->target()->point() << ")";
    } while (++curr != first);
    std::cout << std::endl;
  }
  return 0;
}
