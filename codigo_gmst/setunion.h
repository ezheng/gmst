/**
 * \class SetUnion
 * \brief Set Union Data Structure
 *
 * This is a set union (disjoint sets) data structure, implemented with
 * Union-by-Rank and Path Compression heuristics (R.E. Tarjan's data structure).
 *
 * \author Rodrigo Franco Toso
 * \version 3.0.0
 * \date 08/12/2005
 *
 * Contact: rtoso@ic.uff.br
 * Created on: 26/11/2005
 *
 * License:
 *   Copyleft (C) 2005 by Rodrigo Franco Toso
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software Foundation,
 *   Inc. 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Changelog:
 *      [07/12/2005] Unfortunately, the author did not write this kind of
 *                   documentation.
 *      [08/12/2005] Fully documented.
 */

#ifndef SETUNION_H
#define SETUNION_H

#include <vector>

using namespace std;

class SetUnion {
	public:
    
		/**
		* \brief Constructor.
		* \param n Number of items.
		*/
		SetUnion(unsigned int n);
	
		/**
		* \brief Destructor.
		*/
		~SetUnion();
	
		/**
		* \brief Insert set x into the structure (not needed for static
		* structures, e.g. when the number of vertices does not change).
		* \param x Item to be inserted.
		* \returns A <code>bool</code> indicating success or failure.
		*/
		bool makeSet(unsigned int x);
	
		/**
		* \brief Returns the root of x.
		* \param x Item to be found.
		* \returns The root of x.
		*/
		unsigned int find(unsigned int x);
	
		/**
		* \brief Link set containing x with set containing y.
		* \param x Item 1.
		* \param y item 2.
		* \returns A <code>bool</code> indicating success or failure.
		*/
		bool join(unsigned int x, unsigned int y);

	private:
		vector< unsigned int> parent;  ///< Stores the parent of each element.
		vector< unsigned int> rank;    ///< Stores the rank of each element.
};

#endif
