#include "MainWindow.h"
#include "Scene.h"
#include <CGAL/convex_hull_3.h>

void MainWindow::on_actionConvexHull_triggered()
{
	if(onePolygonIsSelected())
	{
		// wait cursor
		QApplication::setOverrideCursor(Qt::WaitCursor);

		// get active polyhedron
		int index = getSelectedPolygonIndex();
		Polyhedron* pMesh = scene->polyhedron(index);

		// add convex hull as new polyhedron
		Polyhedron *pConvex_hull = new Polyhedron;
		CGAL::convex_hull_3(pMesh->points_begin(),pMesh->points_end(),*pConvex_hull);

		scene->addPolyhedron(pConvex_hull,
			tr("%1 (convex hull)").arg(scene->polyhedronName(index)),
			scene->polyhedronColor(index),
			scene->isPolyhedronActivated(index),
			scene->polyhedronRenderingMode(index));

		// default cursor
		QApplication::setOverrideCursor(Qt::ArrowCursor);
	}
}
