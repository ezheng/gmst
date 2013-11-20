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
#include "setunion.h"

/******************************************************************************
 *                        Constructors and Destructors                        *
 *****************************************************************************/

SetUnion::SetUnion(unsigned int n) {
    rank.resize(n);
	parent.resize(n);
	for(unsigned int i = 0; i < n; i++) {
		parent[i] = i;
		rank[i] = 0;
	}
}

SetUnion::~SetUnion() {
}

bool SetUnion::makeSet(unsigned int x) {
	return false;
}

/******************************************************************************
 *                                  Algorithms                                *
 *****************************************************************************/
unsigned int SetUnion::find(unsigned int x) {
	if(x != parent[x]) { parent[x] = find(parent[x]); }
	return parent[x];
}

bool SetUnion::join(unsigned int x, unsigned int y) {
	unsigned int _x = find(x);  // Find x's root.
	unsigned int _y = find(y);  // Find y's root.

	if(_x == _y) { return false; }

	if(rank[_x] > rank[_y]) { parent[_y] = _x; }
	else {
		parent[_x] = _y;
		if(rank[_x] == rank[_y]) { rank[_y]++; }
	}

	return true;
}

