// ============================================================================
//
// Copyright (c) 1998 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       :
// release_date  :
//
// file          : include/CGAL/Triangulation_iterators_3
// revision      : $Revision$
//
// author(s)     : Monique Teillaud <Monique.Teillaud@sophia.inria.fr>
//
// coordinator   : Mariette Yvinec  <Mariette.Yvinec@sophia.inria.fr>
//
// ============================================================================

#ifndef CGAL_TRIANGULATION_ITERATORS_3_H
#define CGAL_TRIANGULATION_ITERATORS_3_H



#include <pair.h>
#include <iterator.h>
#include <CGAL/triangulation_assertions.h>
#include <CGAL/Triangulation_short_names_3.h>
#include <CGAL/Triangulation_ds_iterators_3.h>

template < class Gt, class Tds >
class CGAL_Triangulation_3;

template < class Gt, class Tds >
class CGAL_T_cell;

template < class Gt, class Tds >
class CGAL_Triangulation_vertex_3;

template < class Gt, class Tds>
class CGAL_Triangulation_3;

template < class Gt, class Tds>
class CGAL_Triangulation_cell_iterator_3;

template < class Gt, class Tds>
class CGAL_Triangulation_facet_iterator_3;

template < class Gt, class Tds>
class CGAL_Triangulation_edge_iterator_3;

template < class Gt, class Tds>
class CGAL_Triangulation_vertex_iterator_3;


template < class Gt, class Tds>
class CGAL_Triangulation_cell_iterator_3
  : public bidirectional_iterator<CGAL_T_cell<Gt,Tds>,ptrdiff_t>
{
public:
  typedef typename Tds::Cell CTriangulation_data_structure_3;
  typedef typename Tds::Cell_iterator Iterator_base;

  typedef CGAL_T_cell<Gt,Tds> Cell;
  typedef CGAL_Triangulation_vertex_3<Gt,Tds> Vertex;
  typedef typename Vertex::Vertex_handle Vertex_handle;
  typedef typename Cell::Cell_handle Cell_handle;
  typedef CGAL_Triangulation_3<Gt,Tds> Triangulation_3;

  typedef CGAL_Triangulation_cell_iterator_3<Gt,Tds> Cell_iterator;

  CGAL_Triangulation_cell_iterator_3()
    : _ib(), _tr(NULL), _inf(true)
  {}
        
  CGAL_Triangulation_cell_iterator_3(CGAL_Triangulation_3<Gt,Tds> *tr, bool inf)
    : _ib( &(tr->_Triangulation_data_structure_3)), _tr(tr), _inf(inf)
    { 
      if (! _inf) {
	while ( // ( _ib != _tr->_Triangulation_data_structure_3.cells_end() ) &&
	       // useless : there must be at least one finite cell
	       // since precond in _ib : dimension == 3
	       _tr->is_infinite(Cell_handle( (Cell *) &(*(_ib)) )) )
	  { ++_ib; }
      }
    }
  
  // for past-end iterator
  // does not need to find a finite cell
  CGAL_Triangulation_cell_iterator_3(CGAL_Triangulation_3<Gt,Tds> *tr)
    : _ib( &(tr->_Triangulation_data_structure_3), 1), _tr(tr), _inf(true)
  { }
       
  CGAL_Triangulation_cell_iterator_3(const Cell_iterator & cit)
    : _ib(cit._ib), _tr(cit._tr), _inf(cit._inf)
  {}
        
  Cell_iterator&
  operator=(const Cell_iterator & cit)
  { 
    _ib = cit._ib;
    _tr = cit._tr;
    _inf = cit._inf;
    return *this;
  }
  
  bool
  operator==(const Cell_iterator & cit) const
  {
    return ( _ib == cit._ib);// && ( _inf == cit._inf) );
  }

  bool
  operator!=(const Cell_iterator & cit)
  {
    return ( !(*this == cit) );
  }

  Cell_iterator &
  operator++()
  {
    if (_inf) {
      ++_ib;
    }
    else {
      do {
	++_ib; 
      } while ( ( _ib != _tr->_Triangulation_data_structure_3.cells_end() )
		&& _tr->is_infinite(Cell_handle( (Cell *) &(*(_ib)) )) );
    }
    return *this;   
  }

  Cell_iterator &
  operator--()
  {
    if (_inf) {
      --_ib;
    }
    else{
      do {
	--_ib;
      } while ( ( _ib != _tr->_Triangulation_data_structure_3.cells_end() )
		&& _tr->is_infinite(Cell_handle( (Cell *) &(*(_ib)) )) );
    }
    return *this;   
  }

  Cell_iterator
  operator++(int)
  {
    Cell_iterator tmp(*this);
    ++(*this);
    return tmp;
  }
        
  Cell_iterator
  operator--(int)
  {
    Cell_iterator tmp(*this);
    --(*this);
    return tmp;
  }
        
  inline Cell & operator*() const
  {
    return (Cell &)(*_ib);
  }

  inline Cell* operator->() const
  {
    return (Cell*)( &(*_ib) );
  }
     
private: 
  Iterator_base _ib;
  Triangulation_3 * _tr;
  bool _inf; // if _inf == true, traverses all cells
               // else only traverses finite cells
};


template < class Gt, class Tds>
class CGAL_Triangulation_vertex_iterator_3
  : public bidirectional_iterator<CGAL_Triangulation_vertex_3<Gt, Tds>,ptrdiff_t>
{
public:
  typedef typename Tds::Vertex Ve;
  typedef typename Tds::Vertex_iterator Iterator_base;

  typedef CGAL_Triangulation_vertex_3<Gt,Tds> Vertex;
  typedef typename Vertex::Vertex_handle Vertex_handle;
  typedef CGAL_Triangulation_3<Gt,Tds> Triangulation_3;

  typedef CGAL_Triangulation_vertex_iterator_3<Gt,Tds> Vertex_iterator;

  CGAL_Triangulation_vertex_iterator_3()
    : _ib(), _tr(), _inf()
  {}
        
  CGAL_Triangulation_vertex_iterator_3(CGAL_Triangulation_3<Gt,Tds> * tr, bool inf)
    : _ib( &(tr->_Triangulation_data_structure_3)), _tr(tr), _inf(inf)
    { 
      if (! _inf) {
	if ( _tr->is_infinite(Vertex_handle( (Vertex *) &(*(_ib)) )) ) {
	  ++_ib;
	}
      }
    }
        
  // for past-end iterator
  // does not need to find a finite cell
  CGAL_Triangulation_vertex_iterator_3(CGAL_Triangulation_3<Gt,Tds> * tr)
    : _ib( &(tr->_Triangulation_data_structure_3), 1), _tr(tr)
  { }
       
  CGAL_Triangulation_vertex_iterator_3(const Vertex_iterator & vi)
    : _ib(vi._ib), _tr(vi._tr), _inf(vi._inf)
  {}
        
  Vertex_iterator &
  operator=(const Vertex_iterator & vi)
  { 
    _ib = vi._ib;
    _tr = vi._tr;
    _inf = vi._inf;
    return *this;
  }
  
  bool
  operator==(const Vertex_iterator & vi) const
  {
    return ( _ib == vi._ib );
  }

  bool
  operator!=(const Vertex_iterator & vi)
  {
    return ( !(*this == vi) );
  }

  Vertex_iterator &
  operator++()
  {
    if (_inf) {
      ++_ib;
    }
    else {
      ++_ib; 
      if ( _tr->is_infinite(Vertex_handle( (Vertex *) &(*(_ib)) )) ) {
	++_ib;
      }
    }
    return *this;   
  }

  Vertex_iterator &
  operator--()
  {
    if (_inf) {
      --_ib;
    }
    else{
      --_ib;
      if ( _tr->is_infinite(Vertex_handle( (Vertex *) &(*(_ib)) )) ) {
	--_ib;
      }
    }
    return *this;   
  }

  Vertex_iterator
  operator++(int)
  {
    Vertex_iterator tmp(*this);
    ++(*this);
    return tmp;
  }
        
  Vertex_iterator
  operator--(int)
  {
    Vertex_iterator tmp(*this);
    --(*this);
    return tmp;
  }

  inline Vertex & operator*() const
  {
    return (Vertex &)(*_ib);
  }

  inline Vertex* operator->() const
  {
    return   (Vertex*)( &(*_ib) );
  }
     
private:
  Iterator_base   _ib;
  Triangulation_3 * _tr;
  bool _inf; // if _inf == true, traverses all vertices
               // else only traverses finite vertices
};


template < class Gt, class Tds>
class CGAL_Triangulation_edge_iterator_3
  : public bidirectional_iterator<typename CGAL_Triangulation_3<Gt,Tds>::Edge,ptrdiff_t>
{
public:
  typedef typename Tds::Edge ETriangulation_data_structure_3;
  typedef typename Tds::Edge_iterator  Iterator_base;

  typedef typename CGAL_Triangulation_3<Gt,Tds>::Edge Edge;
  typedef typename CGAL_Triangulation_3<Gt,Tds>::Cell_handle Cell_handle;
  typedef typename CGAL_Triangulation_3<Gt,Tds>::Cell Cell;
  typedef CGAL_Triangulation_3<Gt,Tds> Triangulation_3;

  typedef CGAL_Triangulation_edge_iterator_3<Gt,Tds>      Edge_iterator;

  CGAL_Triangulation_edge_iterator_3()
    : _ib(), _tr(), _inf()
  {}
        
  CGAL_Triangulation_edge_iterator_3(CGAL_Triangulation_3<Gt,Tds> *tr, bool inf)
    : _ib( &(tr->_Triangulation_data_structure_3)), _tr(tr), _inf(inf)
  { 
    if (! _inf) {
	while ( // ( _ib != _tr->_Triangulation_data_structure_3.cells_end() ) &&
	   // useless : there must be at least one finite cell
	   // since precond in _ib : dimension == 3
	       _tr->is_infinite(CGAL_make_triple(Cell_handle( (Cell *) (*_ib).first), (*_ib).second, (*_ib).third )) )
	  { ++_ib; }
    }
  }
        
  CGAL_Triangulation_edge_iterator_3(CGAL_Triangulation_3<Gt,Tds> *tr)
    : _ib( &(tr->_Triangulation_data_structure_3), 1), _tr(tr)
  { }
       
  CGAL_Triangulation_edge_iterator_3(const Edge_iterator & ei)
    : _ib(ei._ib), _tr(ei._tr), _inf(ei._inf)
  {}
        
  Edge_iterator &
  operator=(const Edge_iterator & ei)
  { 
    _ib = ei._ib;
    _tr = ei._tr;
    _inf = ei._inf;
    return *this;
  }
  
  bool
  operator==(const Edge_iterator & ei) const
  {
    return ( _ib == ei._ib);
  }

  bool
  operator!=(const Edge_iterator & ei)
  {
    return !(*this == ei);
  }

  Edge_iterator &
  operator++()
  {
    if (_inf) {
      ++_ib;
    }
    else {
      do {
	++_ib; 
      } while ( ( _ib != _tr->_Triangulation_data_structure_3.edges_end() )
		&& _tr->is_infinite(CGAL_make_triple(Cell_handle( (Cell *) (*_ib).first), (*_ib).second, (*_ib).third )) );
    }
    return *this;   
  }

  Edge_iterator &
  operator--()
  {
    if (_inf) {
      --_ib;
    }
    else{
      do {
	--_ib;
      } while ( ( _ib != _tr->_Triangulation_data_structure_3.edges_end() )
	      && _tr->is_infinite(CGAL_make_triple(Cell_handle( (Cell *) (*_ib).first), (*_ib).second, (*_ib).third )) );
    }
    return *this;   
  }

  Edge_iterator
  operator++(int)
  {
    Edge_iterator tmp(*this);
    ++(*this);
    return tmp;
  }
        
  Edge_iterator
  operator--(int)
  {
    Edge_iterator tmp(*this);
    --(*this);
    return tmp;
  }
        
  inline Edge operator*() const
  {
    Cell_handle ch = (Cell *)( (*_ib).first );
    return CGAL_make_triple( ch, (*_ib).second, (*_ib).third );
  }
     
private:
  Iterator_base _ib ;
  Triangulation_3 * _tr;
  bool _inf; // if _inf == true, traverses all edges
               // else only traverses finite edges
};

template < class Gt, class Tds>
class CGAL_Triangulation_facet_iterator_3
  : public bidirectional_iterator<typename CGAL_Triangulation_3<Gt,Tds>::Facet,ptrdiff_t>
{
public:
  typedef typename Tds::Facet FTriangulation_data_structure_3;
  typedef typename Tds::Facet_iterator  Iterator_base;

  typedef typename CGAL_Triangulation_3<Gt,Tds>::Facet Facet;
  typedef typename CGAL_Triangulation_3<Gt,Tds>::Cell_handle Cell_handle;
  typedef typename CGAL_Triangulation_3<Gt,Tds>::Cell Cell;
  typedef CGAL_Triangulation_3<Gt,Tds> Triangulation_3;

  typedef CGAL_Triangulation_facet_iterator_3<Gt,Tds> Facet_iterator;

  CGAL_Triangulation_facet_iterator_3()
    : _ib(), _tr(), _inf()
  {}
        
  CGAL_Triangulation_facet_iterator_3(CGAL_Triangulation_3<Gt,Tds> *tr, bool inf)
    : _ib( &(tr->_Triangulation_data_structure_3)), _tr(tr), _inf(inf)
  {       
    if (! _inf) {
      while ( // ( _ib != _tr->_Triangulation_data_structure_3.cells_end() ) &&
	     // useless : there must be at least one finite cell
	     // since precond in _ib : dimension == 3
	     _tr->is_infinite(make_pair(Cell_handle( (Cell *) (*_ib).first), (*_ib).second )) )
	{ ++_ib; }
    }
  }
        
  CGAL_Triangulation_facet_iterator_3(CGAL_Triangulation_3<Gt,Tds> *tr)
    : _ib( &(tr->_Triangulation_data_structure_3), 1), _tr(tr), _inf(true)
  { }
       
  CGAL_Triangulation_facet_iterator_3(const Facet_iterator & fi)
    : _ib(fi._ib), _tr(fi._tr), _inf(fi._inf)
  {}
        
  Facet_iterator &
  operator=(const Facet_iterator & fi)
  { 
    _ib = fi._ib;
    _tr = fi._tr;
    _inf = fi._inf;
    return *this;
  }
  
  bool
  operator==(const Facet_iterator & fi) const
  {
    return ( _ib == fi._ib);
  }

  bool
  operator!=(const Facet_iterator & fi)
  {
    return !(*this == fi);
  }

  Facet_iterator &
  operator++()
  {
    if (_inf) {
      ++_ib;
    }
    else {
      do {
	++_ib; 
      } while ( ( _ib != _tr->_Triangulation_data_structure_3.facets_end() )
		&& _tr->is_infinite(make_pair(Cell_handle( (Cell *) (*_ib).first), (*_ib).second )) );
    }
    return *this;   
  }

  Facet_iterator &
  operator--()
  {
    if (_inf) {
      --_ib;
    }
    else{
      do {
	--_ib;
      } while ( ( _ib != _tr->_Triangulation_data_structure_3.edges_end() )
		&& _tr->is_infinite(make_pair(Cell_handle( (Cell *) (*_ib).first), (*_ib).second )) );
    }
    return *this;   
  }

  Facet_iterator
  operator++(int)
  {
    Facet_iterator tmp(*this);
    ++(*this);
    return tmp;
  }
        
  Facet_iterator
  operator--(int)
  {
    Facet_iterator tmp(*this);
    --(*this);
    return tmp;
  }
        
  inline Facet operator*() const
  {
    Cell_handle ch = (Cell *)( (*_ib).first );
    return make_pair( ch, (*_ib).second );
  }
     
private:
  Iterator_base   _ib ;
  Triangulation_3 * _tr;
  bool _inf; // if _inf == true, traverses all facets
               // else only traverses finite facets
};

#endif CGAL_TRIANGULATION_ITERATORS_3_H
